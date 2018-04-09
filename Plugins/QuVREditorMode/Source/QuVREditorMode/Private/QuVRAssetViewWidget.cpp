// Fill out your copyright notice in the Description page of Project Settings.

#include "QuVRAssetViewWidget.h"
#include "Runtime/CoreUObject/Public/UObject/AssetPtr.h"
#include "Runtime/Slate/Public/Widgets/Notifications/SProgressBar.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SScrollBar.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SScrollBorder.h"
#include "Widgets/Navigation/SBreadcrumbTrail.h"
#include "Editor/UnrealEd/Public/DragAndDrop/ActorDragDropOp.h"
#include "Editor/UnrealEd/Public/DragAndDrop/AssetDragDropOp.h"
//#include "Editor/UnrealEd/Public/DragAndDrop/AssetPathDragDropOp.h"
#include "Editor/EditorWidgets/Public/EditorWidgetsModule.h"
#include "Runtime/AssetRegistry/Public/AssetRegistryModule.h"
#include "Runtime/AssetRegistry/Public/IAssetRegistry.h"
#include "SlateOptMacros.h"

#include "QuVRCatalogEntryWidget.h"
#include "QuVRCatalogNodeBase.h"
#include "QuVRUtils.h"
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

///////////////////////////////
// FQuVRAssetViewAsset
///////////////////////////////

void FQuVRAssetViewAsset::SetAssetData(const FAssetData& NewData)
{
	Data = NewData;
	OnAssetDataChanged.Broadcast();
}

void FQuVRAssetViewAsset::RefreshImage(UTexture2DDynamic* texture2D)
{
	UITexture = texture2D;
	if (EntryWidgetRef.IsValid())
	{
		EntryWidgetRef.Pin()->RefreshWidget(UITexture);
	}
}
bool FQuVRAssetViewAsset::GetTagValue(FName Tag, FString& OutString) const
{
	const FString* FoundString = CustomColumnData.Find(Tag);

	if (FoundString)
	{
		OutString = *FoundString;
		return true;
	}

	return Data.GetTagValue(Tag, OutString);
}

EAssetItemType::Type FQuVRAssetViewAsset::GetType()const
{
	return EAssetItemType::Normal;
}

bool FQuVRAssetViewAsset::IsTemporaryItem()const
{
	return false;
}

///////////////////////////////
// SQuVRAssetTileViewmodes
///////////////////////////////

FReply SQuVRAssetTileView::OnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{

	return STileView<TSharedPtr<FQuVRAssetViewAsset>>::OnKeyDown(InGeometry, InKeyEvent);
}

void SQuVRAssetTileView::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	// Regreshing an asset view is an intensive task. Do not do this while a user
	// is dragging arround content for maximum responsiveness.
	// Also prevents a re-entrancy crash caused by potentially complex thumbnail generators.
	if (!FSlateApplication::Get().IsDragDropping())
	{
		STileView<TSharedPtr<FQuVRAssetViewAsset>>::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	}

}

/************************************************************************/
/*SQuVRAssetViewWidget													*/
/************************************************************************/
SQuVRAssetViewWidget::~SQuVRAssetViewWidget()
{
	// Load the asset registry module to unregister delegates
	if (FModuleManager::Get().IsModuleLoaded("AssetRegistry"))
	{
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::GetModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
		AssetRegistryModule.Get().OnAssetAdded().RemoveAll(this);
		AssetRegistryModule.Get().OnAssetRemoved().RemoveAll(this);
		AssetRegistryModule.Get().OnAssetRenamed().RemoveAll(this);
		AssetRegistryModule.Get().OnPathAdded().RemoveAll(this);
		AssetRegistryModule.Get().OnPathRemoved().RemoveAll(this);
		AssetRegistryModule.Get().OnInMemoryAssetDeleted().RemoveAll(this);
	}
	
	// Unregister listener for asset loading and object property changes
	FCoreUObjectDelegates::OnAssetLoaded.RemoveAll(this);
	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);

	// Release all rendering resources being held onto
	if (AssetThumbnailPool.IsValid())
	{
		AssetThumbnailPool->ReleaseResources();
	}
}

void SQuVRAssetViewWidget::Construct(const FArguments& InArgs)
{
	bRefreshSource = false;
	bIsWorking = false;
	SelectionMode = ESelectionMode::Single;
	AssetThumbnailPool = MakeShareable(new FAssetThumbnailPool(1024, true));
	bAllowDragging = true;
	TileViewThumbnailResolution = 256;
	TileViewThumbnailSize = 128;
	TileViewThumbnailPadding = 4;
	TileViewNameHeight = 36;
	FillScale = 1.25f;
	CurrentFactory = GEditor->FindActorFactoryByClass(UClass::StaticClass());
	// Get desktop metrics
	FDisplayMetrics DisplayMetrics;
	FSlateApplication::Get().GetDisplayMetrics(DisplayMetrics);

	const FVector2D DisplaySize(
		DisplayMetrics.PrimaryDisplayWorkAreaRect.Right - DisplayMetrics.PrimaryDisplayWorkAreaRect.Left,
		DisplayMetrics.PrimaryDisplayWorkAreaRect.Bottom - DisplayMetrics.PrimaryDisplayWorkAreaRect.Top);

	const float ThumbnailScaleRangeScalar = (DisplaySize.Y / 1080);

	MinThumbnailScale = 0.2f * ThumbnailScaleRangeScalar;
	MaxThumbnailScale = 2.0f * ThumbnailScaleRangeScalar;
	MinThumbnailScale = FMath::Clamp<float>(MinThumbnailScale, 0.5f, 1.0f);
	MaxThumbnailScale = FMath::Clamp<float>(MaxThumbnailScale, 1.0f, 1.65f);
	CurrentNode = nullptr;

	FEditorWidgetsModule& EditorWidgetsModule = FModuleManager::LoadModuleChecked<FEditorWidgetsModule>("EditorWidgets");
	TSharedRef<SWidget> AssetDiscoveryIndicator = EditorWidgetsModule.CreateAssetDiscoveryIndicator(EAssetDiscoveryIndicatorScaleMode::Scale_Vertical);


	TSharedRef<SVerticalBox> VerticalBox = SNew(SVerticalBox);

	ChildSlot
		[
			VerticalBox
		];

	// Assets area
	VerticalBox->AddSlot()
		.FillHeight(1.f)
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 0, 0, 0)
		[
			SNew(SBox)
			.HeightOverride(2)
			[
				SNew(SProgressBar)
				.Percent(this, &SQuVRAssetViewWidget::GetIsWorkingProgressBarState)
			.Style(FEditorStyle::Get(), "WorkingBar")
			.BorderPadding(FVector2D(0, 0))
			]
		]

	+ SVerticalBox::Slot()
		.FillHeight(1.f)
		.Padding(0, 0, 0, 0)
		[
			SNew(SOverlay)

				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					// Container for the view types
					SAssignNew(ViewContainer, SBorder)
					.Padding(0)
					.BorderImage(FEditorStyle::GetBrush("NoBorder"))
				]

			+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Bottom)
				.Padding(FMargin(24, 0, 24, 0))
				[
					// Asset discovery indicator
					AssetDiscoveryIndicator
				]
		]
		];

	CreateCurrentView(ViewContainer);

	// Load the asset registry module to listen for updates
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	AssetRegistryModule.Get().OnAssetRemoved().AddSP(this, &SQuVRAssetViewWidget::OnAssetRemoved);
	AssetRegistryModule.Get().OnInMemoryAssetDeleted().AddSP(this, &SQuVRAssetViewWidget::OnInMemoryAssetDeleted);
}

void SQuVRAssetViewWidget::OnAssetRemoved(const FAssetData& AssetData)
{
	// Void
}

void SQuVRAssetViewWidget::OnInMemoryAssetDeleted(UObject *Object)
{
	if (CurrentNode.IsValid())
	{
		for (auto asset : CurrentNode->AssetList)
		{
			FString assetName = UQuVRUtils::GetObjectName(UQuVRUtils::GetAssetPath(asset->PackageUrl));
			if (assetName == Object->GetName())
			{
				bRefreshSource = true;
				break;
			};
		}
	}
}

void SQuVRAssetViewWidget::CreateCurrentView(	/** Creates a new tile view */TSharedPtr<SBorder>& inViewContainer)
{
	TileView.Reset();
	TSharedRef<SWidget> NewView = SNullWidget::NullWidget;
	TileView = CreateTileView();
	NewView = CreateShadowOverlay(TileView.ToSharedRef());
	inViewContainer->SetContent(NewView);
}

TSharedRef<SQuVRAssetTileView> SQuVRAssetViewWidget::CreateTileView()
{
return SNew(SQuVRAssetTileView)
			.SelectionMode(SelectionMode)
			.ListItemsSource(&FilteredAssetItems)
  		.OnGenerateTile(this, &SQuVRAssetViewWidget::MakeTileViewWidget)
  		.OnItemScrolledIntoView(this, &SQuVRAssetViewWidget::ItemScrolledIntoView)
  		.OnContextMenuOpening(this, &SQuVRAssetViewWidget::OnGetContextMenuContent)
  		.OnMouseButtonDoubleClick(this, &SQuVRAssetViewWidget::OnListMouseButtonDoubleClick)
		.OnMouseButtonClick(this, &SQuVRAssetViewWidget::OnListMouseButtonClick)
  		.OnSelectionChanged(this, &SQuVRAssetViewWidget::AssetSelectionChanged)
 		.ItemHeight(this, &SQuVRAssetViewWidget::GetTileViewItemHeight)
 		.ItemWidth(this, &SQuVRAssetViewWidget::GetTileViewItemWidth);

}

TSharedRef<SWidget> SQuVRAssetViewWidget::CreateShadowOverlay(TSharedRef<STableViewBase> Table)
{
	return SNew(SScrollBorder, Table)
		[
			Table
		];
}

/** @return The state of the is working progress bar */
TOptional< float > SQuVRAssetViewWidget::GetIsWorkingProgressBarState() const
{
	return bIsWorking ? TOptional< float >() : 0.0f;
}

/** Handler for when any asset item widget gets destroyed */
void SQuVRAssetViewWidget::AssetItemWidgetDestroyed(const TSharedPtr<FQuVRAssetViewAsset>& Item)
{

}

void SQuVRAssetViewWidget::UpdateThumbnails()
{

}

void SQuVRAssetViewWidget::CalculateFillScale(const FGeometry& AllottedGeometry)
{

	float ItemWidth = GetTileViewItemBaseWidth();

	// Scrollbars are 16, but we add 1 to deal with half pixels.
	const float ScrollbarWidth = 16 + 1;
	float TotalWidth = AllottedGeometry.Size.X - (ScrollbarWidth / AllottedGeometry.Scale);
	float Coverage = TotalWidth / ItemWidth;
	int32 Items = (int)(TotalWidth / ItemWidth);

	// If there isn't enough room to support even a single item, don't apply a fill scale.
	if (Items > 0)
	{
		float GapSpace = ItemWidth * (Coverage - Items);
		float ExpandAmount = GapSpace / (float)Items;
		FillScale = (ItemWidth + ExpandAmount) / ItemWidth;
		FillScale = FMath::Max(1.0f, FillScale);
	}
	else
	{
		FillScale = 1.0f;
	}


}


FReply SQuVRAssetViewWidget::OnDraggingAssetItem(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (bAllowDragging)
	{
		TArray<FAssetData> AssetDataList = GetSelectedAssets();
		if (AssetDataList.Num())
		{
			// We have some items selected, start a drag-drop
			TArray<FAssetData> InAssetData;

			for (int32 AssetIdx = 0; AssetIdx < AssetDataList.Num(); ++AssetIdx)
			{
				const FAssetData& AssetData = AssetDataList[AssetIdx];

				if (!AssetData.IsValid() || AssetData.AssetClass == UObjectRedirector::StaticClass()->GetFName())
				{
					// Skip invalid assets and redirectors
					continue;
				}

				// If dragging a class, send though an FAssetData whose name is null and class is this class' name
				InAssetData.Add(AssetData);
			}
			
			if (InAssetData.Num() > 0)
			{
				TArray<TSharedPtr<FQuVRAssetViewAsset>> SelectedItems = GetSelectedItems();
				FindActorFactoryByType(SelectedItems[0]->ObjectType);

				UActorFactory* FactoryToUse = nullptr;
				FEditorDelegates::OnAssetDragStarted.Broadcast(InAssetData, FactoryToUse);
				if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
				{
					return FReply::Handled().BeginDragDrop(FAssetDragDropOp::New(InAssetData, CurrentFactory));
				}
				else
				{
					return FReply::Handled();
				}
			}
		}
		else
		{
			// are we dragging some folders?
			TArray<FString> SelectedFolders = GetSelectedFolders();
			if (SelectedFolders.Num() > 0 && !SourcesData.HasCollections())
			{
				return FReply::Handled().BeginDragDrop(FAssetDragDropOp::New(SelectedFolders));
			}
		}
	}

	return FReply::Unhandled();
}
void SQuVRAssetViewWidget::ClearSourceItems()
{
	AssetItems.Empty();
	AssetItems.Reset();
	FilteredAssetItems.Empty();
	FilteredAssetItems.Reset();
	RefreshList();
}

void SQuVRAssetViewWidget::RefreshSourceItems(const TSharedRef<FQuVRCatalogNode> node)
{
	ClearSourceItems();
	CurrentNode = node;
	for (auto asset : node->AssetList)
	{
		FString filepath = UQuVRUtils::GetAssetPath(asset->PackageUrl);
		const FAssetData& AssetData = FAssetData(LoadObject<UObject>(nullptr, *filepath));	
		FQuVRAssetViewAsset* viewAsset = new FQuVRAssetViewAsset(
												AssetData,
												asset->ObjectType,
												asset->PackageUrl,
												asset->DisplayName,
												asset->Texture2Dimage);
		asset->ImageDownloadDone.Clear();
		asset->ImageDownloadDone.AddRaw(viewAsset, &FQuVRAssetViewAsset::RefreshImage);
		FilteredAssetItems.Add(MakeShareable(viewAsset));
		asset->Initialise();
	}

	RefreshList();
}

TSharedRef<ITableRow> SQuVRAssetViewWidget::MakeTileViewWidget(TSharedPtr<FQuVRAssetViewAsset> AssetItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (!ensure(AssetItem.IsValid()))
	{
		return SNew(STableRow<TSharedPtr<FQuVRAssetViewAsset>>, OwnerTable);
	}

	TSharedPtr< STableRow<TSharedPtr<FQuVRAssetViewAsset>> > TableRowWidget;
	SAssignNew(TableRowWidget, STableRow<TSharedPtr<FQuVRAssetViewAsset>>, OwnerTable)
		.Style(FEditorStyle::Get(), "ContentBrowser.AssetListView.TableRow")
		.Cursor(bAllowDragging ? EMouseCursor::GrabHand : EMouseCursor::Default)
		.OnDragDetected(this, &SQuVRAssetViewWidget::OnDraggingAssetItem);

	TSharedRef<SQuVRCatlogEntryWidget> Item =
		SNew(SQuVRCatlogEntryWidget)
		.AssetInfo(*AssetItem)
		.ItemWidth(this, &SQuVRAssetViewWidget::GetTileViewItemWidth)
		.ThumbnailPadding(TileViewThumbnailPadding);

	Item->FileDownloadDone.AddSP(this, &SQuVRAssetViewWidget::UpdataAssetData);
	AssetItem->EntryWidgetRef = Item;
	TableRowWidget->SetContent(Item);

	return TableRowWidget.ToSharedRef();
}

void SQuVRAssetViewWidget::UpdataAssetData(FString name)
{
	for (int32 AssetIdx = 0; AssetIdx < FilteredAssetItems.Num(); ++AssetIdx)
	{
		FString filepath = UQuVRUtils::GetAssetPath(FilteredAssetItems[AssetIdx]->PackageUrl);
		FString filename = UQuVRUtils::GetObjectName(filepath);
		if (filename == name)
		{
			const FAssetData& AssetData = FAssetData(LoadObject<UObject>(nullptr, *filepath));
			FilteredAssetItems[AssetIdx]->Data = AssetData;
			break;
		}
	}

	RefreshList();
}

void SQuVRAssetViewWidget::RefreshList()
{
	TileView->RequestListRefresh();
}

void SQuVRAssetViewWidget::OnAssetAdded(const FAssetData& AssetData)
{
	AssetItems.Add(AssetData);
}

TArray<TSharedPtr<FQuVRAssetViewAsset>> SQuVRAssetViewWidget::GetSelectedItems() const
{
	return TileView->GetSelectedItems();

}

TArray<FString> SQuVRAssetViewWidget::GetSelectedFolders() const
{
	TArray<TSharedPtr<FQuVRAssetViewAsset>> SelectedItems = GetSelectedItems();
	TArray<FString> SelectedFolders;
	for (auto ItemIt = SelectedItems.CreateConstIterator(); ItemIt; ++ItemIt)
	{
		const TSharedPtr<FQuVRAssetViewAsset>& Item = *ItemIt;
		if (Item.IsValid() && Item->GetType() == EAssetItemType::Folder)
		{
	//		SelectedFolders.Add(StaticCastSharedPtr<FAssetViewFolder>(Item)->FolderPath);
		}
	}

	return SelectedFolders;
}


void SQuVRAssetViewWidget::FindActorFactoryByType(const int32 ObjectType)
{
	switch (ObjectType)
	{
		/************************************************************************/
		/*		SK_ 骨骼模型 0       SM_  静态模型 1        M_  材质球 2  			*/
		/*		QuVRTianQiu_ 天空球 5											*/
		/************************************************************************/
	case 0:
		CurrentFactory = GEditor->FindActorFactoryByClass(UQuVRAssetFactoryAnimation::StaticClass());
		break;
	case 1:
	case 5:
		CurrentFactory = GEditor->FindActorFactoryByClass(UQuVRAssetFactoryModel::StaticClass());
		break;
	case 2:
		CurrentFactory = GEditor->FindActorFactoryByClass(UClass::StaticClass());
		break;
	}
}


/** Returns all the asset data objects in items currently selected in the view */
TArray<FAssetData> SQuVRAssetViewWidget::GetSelectedAssets() const
{
	TArray<TSharedPtr<FQuVRAssetViewAsset>> SelectedItems = GetSelectedItems();
	TArray<FAssetData> SelectedAssets;
	for (auto ItemIt = SelectedItems.CreateConstIterator(); ItemIt; ++ItemIt)
	{
		const TSharedPtr<FQuVRAssetViewAsset>& Item = *ItemIt;
		// Only report non-temporary & non-folder items
		if (Item.IsValid() && !Item->IsTemporaryItem() && Item->GetType() != EAssetItemType::Folder)
		{
			SelectedAssets.Add(StaticCastSharedPtr<FQuVRAssetViewAsset>(Item)->Data);
		}
	}

	return SelectedAssets;
}


void SQuVRAssetViewWidget::ItemScrolledIntoView(TSharedPtr<FQuVRAssetViewAsset> AssetItem, const TSharedPtr<ITableRow>& Widget)
{

}

void SQuVRAssetViewWidget::OnListMouseButtonDoubleClick(TSharedPtr<FQuVRAssetViewAsset> AssetItem)
{
	if (AssetItem.IsValid())
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, AssetItem->DisplayName);*/
	}
};

void SQuVRAssetViewWidget::OnListMouseButtonClick(TSharedPtr<FQuVRAssetViewAsset> AssetItem)
{
	if (AssetItem.IsValid())
	{
		if (AssetItem->EntryWidgetRef.IsValid())
		{
			AssetItem->EntryWidgetRef.Pin()->CheckDownloadAsset();
		}
	
/*	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, AssetItem->DisplayName);*/

	}
};

void SQuVRAssetViewWidget::AssetSelectionChanged(TSharedPtr<FQuVRAssetViewAsset > AssetItem, ESelectInfo::Type SelectInfo)
{
	if (AssetItem.IsValid())
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, AssetItem->DisplayName);*/
	}
}

TSharedPtr<SWidget> SQuVRAssetViewWidget::OnGetContextMenuContent()
{

	return NULL;
}

float SQuVRAssetViewWidget::GetTileViewItemHeight() const
{
	return TileViewNameHeight + GetTileViewItemBaseHeight() * FillScale;
}

float SQuVRAssetViewWidget::GetTileViewItemBaseHeight() const
{
	return (TileViewThumbnailSize + TileViewThumbnailPadding * 2) * FMath::Lerp(MinThumbnailScale, MaxThumbnailScale, 0.2f);
}

float SQuVRAssetViewWidget::GetTileViewItemWidth() const
{
	return GetTileViewItemBaseWidth() * FillScale;
}

float SQuVRAssetViewWidget::GetTileViewItemBaseWidth() const //-V524
{
	return (TileViewThumbnailSize + TileViewThumbnailPadding * 2) * FMath::Lerp(MinThumbnailScale, MaxThumbnailScale, 0.20f);
}

void SQuVRAssetViewWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	// void
	CalculateFillScale(AllottedGeometry);

	if (bRefreshSource)
	{
		RefreshSourceItems(CurrentNode.ToSharedRef());
		bRefreshSource = false;
	}

}

FReply SQuVRAssetViewWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{

	return FReply::Handled();
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

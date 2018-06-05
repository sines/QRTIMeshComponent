// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "QuVRCatalogEntryWidget.h"
// Core System
#include "Widgets/SWidget.h"
#include "Widgets/SUserWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Runtime/Slate/Public/SlateOptMacros.h"
#include "Runtime/Slate/Public/Widgets/Text/SInlineEditableTextBlock.h"
#include "Runtime/AssetRegistry/Public/AssetData.h"
#include "Editor/UnrealEd/Public/Editor.h"
#include "Editor/UnrealEd/Public/DragAndDrop/AssetDragDropOp.h"
#include "Editor/UnrealEd/Classes/ActorFactories/ActorFactoryBasicShape.h"
#include "Editor/PlacementMode/Public/IPlacementModeModule.h"
#include "Editor/UnrealEd/Classes/ActorFactories/ActorFactoryDirectionalLight.h"
#include "Editor/UnrealEd/Classes/ActorFactories/ActorFactoryAnimationAsset.h"
#include "Editor/UnrealEd/Public/EditorClassUtils.h"
#include "Developer/AssetTools/Public/IAssetTypeActions.h"
#include "Developer/AssetTools/Public/IAssetTools.h"
#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "Runtime/Core/Public/Internationalization/BreakIterator.h"
#include "Core.h"
#include "SlateBasics.h"
// include
#include "QuVRFileDownloader.h"
#include "QuVRUtils.h"
#include "QuVRAssetFactoryModel.h"
#include "QuVRCatalogStyleSettings.h"
#include "QuVRAssetDownloader.h"
#include "QuVRImageDownloaderManager.h"
#include "QuVRAssetDownloaderManager.h"
#include "Runtime/Online/HTTP/Public/HttpManager.h"
#if !UE_BUILD_SHIPPING

const FString DownloadTopImagePath= FPaths::GamePluginsDir() + FString(TEXT("QuVREditorMode\\Resources\\DownLoad.png"));
const FString DownloadBackImagePath = FPaths::GamePluginsDir() + FString(TEXT("QuVREditorMode\\Resources\\refresh.png"));
bool ImageValid = true;

//////////////////////////////////////////////////////////////////////////
// Async Task

class FDownloadAssetAsyncTask : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<FDownloadAssetAsyncTask>;
	friend class FAsyncTask<FDownloadAssetAsyncTask>;

	TSharedPtr<SQuVRCatlogEntryWidget> AssetAsyncTask;

	FDownloadAssetAsyncTask(TSharedPtr<SQuVRCatlogEntryWidget> InCatlogEntryWidget)
		: AssetAsyncTask(InCatlogEntryWidget)
	{
	}

	void DoWork()
	{
		if (AssetAsyncTask.IsValid())
		{
			AssetAsyncTask->StartDownloadAsset();
		}
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FDownloadAssetAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};


SQuVRCatlogEntryWidget::~SQuVRCatlogEntryWidget()
{
	if (Texture2Dimage)
	{
		Texture2Dimage->RemoveFromRoot();
	}
	
	if (AssetInfo.EntryWidget.IsValid())
	{
		AssetInfo.EntryWidget.Reset();
	}
}

void SQuVRCatlogEntryWidget::Construct(const FArguments& InArgs)
{
	// Init value
	AsyncImageDownloader = nullptr;
	AsyncTaskDownloadFile = nullptr;
	Texture2Dimage = nullptr;
	bIsPressed = false;
	bDraggedOver = false;
	PressedImage = new FSlateBrush();
	PlaceableItem = NULL;
	ProgressRate = 0;
	downloadProgressBar = NULL;
	downloadTopImage = NULL;
	ItemWidth = 100;
	ThumbnailPadding = 5;

	// init Value
	AssetInfo = InArgs._AssetInfo;
	ItemWidth = InArgs._ItemWidth;
	ThumbnailPadding = InArgs._ThumbnailPadding;

	buttonstyle = new FButtonStyle();

	// init image
	fSlateDownload = FSlateIcon(FQuVRCatalogStyleSettings::Get().GetStyleSetName(), "QuVREditorMode.DownLoad", "QuVREditorMode.DownLoad");
	fSlateReference = FSlateIcon(FQuVRCatalogStyleSettings::Get().GetStyleSetName(), "QuVREditorMode.Refresh", "QuVREditorMode.Refresh");

	// init tool tip
	TSharedPtr<IToolTip> AssetEntryToolTip;
	InitPlaceableItem();
	if (PlaceableItem.IsValid())
	{
		const bool bIsClass = PlaceableItem->AssetData.GetClass() == UClass::StaticClass();
		const bool bIsActor = bIsClass ? CastChecked<UClass>(PlaceableItem->AssetData.GetAsset())->IsChildOf(AActor::StaticClass()) : false;

		AActor* DefaultActor = nullptr;
		if (PlaceableItem->Factory != nullptr)
		{
			DefaultActor = PlaceableItem->Factory->GetDefaultActor(PlaceableItem->AssetData);
		}
		else if (bIsActor)
		{
			DefaultActor = CastChecked<AActor>(CastChecked<UClass>(PlaceableItem->AssetData.GetAsset())->ClassDefaultObject);
		}

		if (DefaultActor != nullptr)
		{
			/************************************************************************/
			/*show class tip
			/*AssetEntryToolTip = FEditorClassUtils::GetTooltip(DefaultActor->GetClass())
			/************************************************************************/
			FString filepath = UQuVRUtils::GetAssetPath(AssetInfo.PackageUrl);
			FString filename = UQuVRUtils::GetObjectName(filepath);
			AssetEntryToolTip = FSlateApplicationBase::Get().MakeToolTip(FText::FromString(filename));
		}
		if (!AssetEntryToolTip.IsValid())
		{
			AssetEntryToolTip = FSlateApplicationBase::Get().MakeToolTip(PlaceableItem->DisplayName);
		}
	}

	ChildSlot
		[
			SNew(SBorder)
			.BorderImage(this, &SQuVRCatlogEntryWidget::GetBorderImage)
			.Padding(4)
			[
				SNew(SVerticalBox)
				// Thumbnail
				+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Center)
				[
					SNew(SBox)
					.Padding(ThumbnailPadding - 4.0f)
					.WidthOverride(this, &SQuVRCatlogEntryWidget::GetThumbnailBoxSize)
					.HeightOverride(this, &SQuVRCatlogEntryWidget::GetThumbnailBoxSize)
					[
						// Drop shadow border
						SNew(SBorder)
						.Padding(4.f)
						.BorderImage(this, &SQuVRCatlogEntryWidget::GetSlateBrushState).ToolTip(AssetEntryToolTip).Padding(0)
						[
							SNew(SOverlay)
 							+ SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Bottom)
 							[		
								SAssignNew(downloadTopImage,SImage).Image(this, &SQuVRCatlogEntryWidget::GetSlateBrushTop).Visibility(this, &SQuVRCatlogEntryWidget::GetIsDownloadeVisible)
 							]
							+ SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Center)
							[
								SAssignNew(downloadProgressBar,SProgressBar).Visibility(this, &SQuVRCatlogEntryWidget::GetIsProgressVisible).Percent(this, &SQuVRCatlogEntryWidget::GetProgressBarState)
							]
						]
					]
				]

				+ SVerticalBox::Slot()
					.Padding(FMargin(1.0f, 0))
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.AutoHeight()
					.FillHeight(1.0f)
					[
						SNew(SInlineEditableTextBlock)
						.Text(FText::FromString(AssetInfo.DisplayName))
						.IsSelected(false)
						.IsReadOnly(true)
						.Justification(ETextJustify::Center)
						.LineBreakPolicy(FBreakIterator::CreateCamelCaseBreakIterator())
					]
			]
		];

	// init AsyncTask
	DownloadImage(AssetInfo.ImageUrl);
	// Refresh Texture
	if (AssetInfo.UITexture)
	{
		RefreshWidget(AssetInfo.UITexture);
	}
}

const FSlateBrush* SQuVRCatlogEntryWidget::GetBorderImage() const
{
	return bDraggedOver ? FEditorStyle::GetBrush("Menu.Background") : FEditorStyle::GetBrush("NoBorder");
}

void SQuVRCatlogEntryWidget::CheckDownloadAsset()
{
	CreateDownloadAsset(AssetInfo.PackageUrl);
	if(AsyncTaskDownloadFile.IsValid())
	{
		if (false == AsyncTaskDownloadFile->GetIsDownloadAsset())
		{
			if (FileDownLoadState::Start == AsyncTaskDownloadFile->GetDownLoadState())
			{
				InitPlaceableItem();
				StartDownloadAsset();
			}

		}
	}
}

void SQuVRCatlogEntryWidget::InitPlaceableItem()
{
	if (AssetInfo.Data.IsValid()&&false == PlaceableItem.IsValid())
	{
		if (UQuVRUtils::CheckFileExists(AssetInfo.PackageUrl))
		{
			static TOptional<FLinearColor> BasicShapeColorOverride;

			if (!BasicShapeColorOverride.IsSet())
			{
				FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
				TSharedPtr<IAssetTypeActions> AssetTypeActions;
				AssetTypeActions = AssetToolsModule.Get().GetAssetTypeActionsForClass(UClass::StaticClass()).Pin();
				if (AssetTypeActions.IsValid())
				{
					BasicShapeColorOverride = TOptional<FLinearColor>(AssetTypeActions->GetTypeColor());
				}
			}

			//UQuVRAssetFactoryStaticMeshModel
	
			FString filepath = UQuVRUtils::GetAssetPath(AssetInfo.PackageUrl);
			int32 SortOrder = 0;
			switch (AssetInfo.ObjectType)
			{
				/************************************************************************/
				/*		SK_ 骨骼模型 0       SM_  静态模型 1        M_  材质球 2  			*/
				/*		QuVRTianQiu_ 天空球 5											*/
				/************************************************************************/
			case 0:
				AssetInfo.Data = FAssetData(LoadObject<UAnimSequence>(nullptr, *filepath));
				PlaceableItem = MakeShareable( new FPlaceableItem(*UQuVRAssetFactoryAnimation::StaticClass(), AssetInfo.Data, NAME_None, BasicShapeColorOverride, SortOrder += 10));
				break;
			case 1:
			case 5:
				AssetInfo.Data = FAssetData(LoadObject<UStaticMesh>(nullptr, *filepath));
				PlaceableItem = MakeShareable(new FPlaceableItem(*UQuVRAssetFactoryModel::StaticClass(), AssetInfo.Data, NAME_None, BasicShapeColorOverride, SortOrder += 10));
				break;
			case 2:
				AssetInfo.Data = FAssetData(LoadObject<UMaterialInterface>(nullptr, *filepath));
				UMaterialInterface* MA = LoadObject<UMaterialInterface>(nullptr, *filepath);
				PlaceableItem = MakeShareable(new FPlaceableItem(*UClass::StaticClass(), AssetInfo.Data, NAME_None, BasicShapeColorOverride, SortOrder += 10));
				break;
			}			
		};
	};
}

void SQuVRCatlogEntryWidget::RefreshWidget(UTexture2DDynamic* texture2D)
{
	if (texture2D)
	{
		Texture2Dimage = texture2D;
		Texture2Dimage->AddToRoot();
		FSlateBrush* AssetImage = new FSlateBrush();
		AssetImage->ImageSize.X = texture2D->GetSurfaceWidth();
		AssetImage->ImageSize.Y = texture2D->GetSurfaceHeight();
		AssetImage->DrawAs = ESlateBrushDrawType::Image;
		AssetImage->SetResourceObject(Texture2Dimage);
		buttonstyle->SetNormal(*AssetImage);
		buttonstyle->SetPressed(*AssetImage);
		buttonstyle->SetHovered(*AssetImage);
		PressedImage = AssetImage;
	}
	AsyncTaskDownloadFile = UQuVRAssetDownloaderManager::GetInstance()->GetAssetDownloader(AssetInfo.PackageUrl);
	if (AsyncTaskDownloadFile.IsValid())
	{
		AsyncTaskDownloadFile->OnDownloadAssetDone.Clear();
		AsyncTaskDownloadFile->OnDownloadAssetDone.AddSP(this, &SQuVRCatlogEntryWidget::OnDownloadDone);
	}
}
FReply SQuVRCatlogEntryWidget::StartDownloadAsset()
{
	if (AsyncTaskDownloadFile.IsValid())
	{
		if (AsyncTaskDownloadFile->GetDownLoadState()==FileDownLoadState::Start)
		{
			AsyncTaskDownloadFile->DownloadAssetFile(AssetInfo.PackageUrl);
		}
	}
	return FReply::Handled();
}

FReply SQuVRCatlogEntryWidget::CreateDownloadAsset(FString URL)
{
	if (false == AsyncTaskDownloadFile.IsValid())
	{
		AsyncTaskDownloadFile = UQuVRAssetDownloaderManager::GetInstance()->CreateAssetDownloader(URL);
		AsyncTaskDownloadFile->OnDownloadAssetDone.Clear();
		AsyncTaskDownloadFile->OnDownloadAssetDone.AddSP(this, &SQuVRCatlogEntryWidget::OnDownloadDone);
	}
	return FReply::Handled();
}


FReply SQuVRCatlogEntryWidget::DownloadImage(FString URL)
{
	if (URL.Len() > 8)
	{
		AsyncImageDownloader = UQuVRImageDownloaderManager::GetInstance()->CreateImageDownloader(URL);
		AsyncImageDownloader->OnDownloadImageDone.AddSP(this, &SQuVRCatlogEntryWidget::RefreshWidget);

		if (AsyncImageDownloader.IsValid())
		{
			if (AsyncImageDownloader->GetDownLoadState()==FileDownLoadState::Start)
			{
				AsyncImageDownloader->DownloadImage(URL);
			}
		}
	}
	return FReply::Handled();
}

const FSlateBrush* SQuVRCatlogEntryWidget::GetSlateBrushState() const 
{
	if (AsyncImageDownloader.IsValid())
	{
		if (false == AsyncImageDownloader->GetIsDownloadImage())
		{
			return fSlateReference.GetIcon();
		}
	}
	return PressedImage;
}

const FSlateBrush* SQuVRCatlogEntryWidget::GetSlateBrushTop()const
{
	return fSlateDownload.GetIcon();
}

EVisibility SQuVRCatlogEntryWidget::GetIsProgressVisible() const
{
	if (AsyncTaskDownloadFile.IsValid())
	{
		if (AsyncTaskDownloadFile->GetDownLoadState() == FileDownLoadState::InProgress)
		{
			return	EVisibility::Visible;
		}
	}

	return EVisibility::Hidden;
}

EVisibility SQuVRCatlogEntryWidget::GetIsDownloadeVisible() const
{
	FString URL = AssetInfo.PackageUrl;
	if (5 < URL.Len())
	{
		if (UQuVRUtils::CheckFileExists(URL))
		{
		return	EVisibility::Hidden;
		}
	}
	return EVisibility::Visible;
}

TOptional< float > SQuVRCatlogEntryWidget::GetProgressBarState() const
{
	if (AsyncTaskDownloadFile.IsValid())
	{
		return AsyncTaskDownloadFile->GetAssetProgressRate();
	}
	return ProgressRate;

}

void SQuVRCatlogEntryWidget::OnDownloadDone(int32 code)
{
	FString URL = AssetInfo.PackageUrl;
	FString filepath = UQuVRUtils::GetAssetPath(AssetInfo.PackageUrl);
	FString filename = UQuVRUtils::GetObjectName(filepath);
	if (5 < URL.Len())
	{
		if (UQuVRUtils::CheckFileExists(URL))
		{	
			FileDownloadDone.Broadcast(filename);
		}
	}
	if (AsyncTaskDownloadFile.IsValid())
	{
		AsyncTaskDownloadFile->ConditionalBeginDestroy();
	}
	AsyncTaskDownloadFile = nullptr;
}

FOptionalSize SQuVRCatlogEntryWidget::GetThumbnailBoxSize() const
{
	return FOptionalSize(ItemWidth.Get());
}


TSharedRef<SWidget> MakeCatalogEntryWidget(FQuVRAssetViewAsset& item)
{
	return SNew(SQuVRCatlogEntryWidget).AssetInfo(item);
}

#undef LOCTEXT_NAMESPACE

#endif // #if !UE_BUILD_SHIPPING

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

#if !UE_BUILD_SHIPPING

const FString DownloadTopImagePath= FPaths::GamePluginsDir() + FString(TEXT("QuVREditorMode\\Resources\\DownLoad.png"));
const FString DownloadBackImagePath = FPaths::GamePluginsDir() + FString(TEXT("QuVREditorMode\\Resources\\refresh.png"));
bool ImageValid = true;

float InverseLerp(float A, float B, float Value)
{
	if (FMath::IsNearlyEqual(A, B))
	{
		if (Value < A)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return ((Value - A) / (B - A));
	}
}
SQuVRCatlogEntryWidget::~SQuVRCatlogEntryWidget()
{
	AssetInfo.EntryWidgetRef.Reset();
	AssetInfo.EntryWidgetRef = nullptr;
}

void SQuVRCatlogEntryWidget::Construct(const FArguments& InArgs)
{
	// const value
	Texture2Dimage = nullptr;
	bIsPressed = false;
	bIsDownloadImage = false;
	bIsDownloadAsset = false;
	bDraggedOver = false;
	PressedImage = new FSlateBrush();
	PlaceableItem = NULL;
	AsyncTaskDownloadFile = NULL;
	ProgressRate = 0;
	downloadProgressBar = NULL;
	downloadTopImage = NULL;
	ItemWidth = 100;
	ThumbnailPadding = 5;
	DownloadFileState = EntryDownLoadState::Start;

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
	if (false == bIsDownloadAsset)
	{
		if (EntryDownLoadState::Start == DownloadFileState)
		{
			InitPlaceableItem();
			DownloadAsset();
		}

	}
}

void SQuVRCatlogEntryWidget::InitPlaceableItem()
{
	if (AssetInfo.Data.IsValid()&&false == PlaceableItem.IsValid())
	{
		if (UQuVRUtils::CheckFileExists(AssetInfo.PackageUrl))
		{
			bIsDownloadAsset = true;
			DownloadFileState = EntryDownLoadState::Finish;
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
		FSlateBrush* AssetImage = new FSlateBrush();
		AssetImage->ImageSize.X = texture2D->GetSurfaceWidth();
		AssetImage->ImageSize.Y = texture2D->GetSurfaceHeight();
		AssetImage->DrawAs = ESlateBrushDrawType::Image;
		AssetImage->SetResourceObject(Texture2Dimage);
		buttonstyle->SetNormal(*AssetImage);
		buttonstyle->SetPressed(*AssetImage);
		buttonstyle->SetHovered(*AssetImage);
		PressedImage = AssetImage;
		bIsDownloadImage = true;
	}
}

FReply SQuVRCatlogEntryWidget::DownloadAsset()
{
	if (false == AssetInfo.Data.IsValid())
	{
		FString URL = AssetInfo.PackageUrl;
		if (5 <URL.Len())
		{
			if (!UQuVRUtils::CheckFileExists(URL))
			{
				if (AsyncTaskDownloadFile.IsValid())
				{
					AsyncTaskDownloadFile->StartDownloadZipFile(URL);
				}
				else
				{
					
					AsyncTaskDownloadFile = UQuVRFileDownloader::DownloadZipLoader(URL);
					AsyncTaskDownloadFile->OnDownloadFileDone.AddSP(this, &SQuVRCatlogEntryWidget::OnDownloadDone);
					AsyncTaskDownloadFile->OnUlpdataProegress.AddSP(this, &SQuVRCatlogEntryWidget::OnDownloadProegress);

				}
			}
		}
	}
	return FReply::Handled();
}

const FSlateBrush* SQuVRCatlogEntryWidget::GetSlateBrushState() const
{
	if (false == bIsDownloadImage)
	{
		return fSlateReference.GetIcon();
	}
	return PressedImage;
}

const FSlateBrush* SQuVRCatlogEntryWidget::GetSlateBrushTop()const
{
	return fSlateDownload.GetIcon();
}

EVisibility SQuVRCatlogEntryWidget::GetIsProgressVisible() const
{
	if (DownloadFileState == EntryDownLoadState::InProgress)
	{
		return	EVisibility::Visible;
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

void SQuVRCatlogEntryWidget::OnDownloadProegress(int32 ReceivedDataInBytes, int32 TotalDataInBytes, const TArray<uint8>& BinaryData)
{
	DownloadFileState = EntryDownLoadState::InProgress;
	ProgressRate = InverseLerp(0.0f, (float)TotalDataInBytes, (float)ReceivedDataInBytes);
}
TOptional< float > SQuVRCatlogEntryWidget::GetProgressBarState() const
{
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
			bIsDownloadAsset = true;
			DownloadFileState = EntryDownLoadState::Finish;
			FileDownloadDone.Broadcast(filename);
		}
	}

	AsyncTaskDownloadFile->ConditionalBeginDestroy();
	AsyncTaskDownloadFile = NULL;
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

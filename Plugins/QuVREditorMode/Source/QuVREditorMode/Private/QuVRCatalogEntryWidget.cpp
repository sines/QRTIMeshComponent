// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "QuVRCatalogEntryWidget.h"
// Core System
#include "Widgets/SWidget.h"
#include "Widgets/SUserWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Runtime/Slate/Public/SlateOptMacros.h"
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
#include "Core.h"
#include "SlateBasics.h"
// Custom include
#include "QuVRFileDownloader.h"
#include "QuVRUtils.h"
#include "QuVRAssetFactoryModel.h"

#if !UE_BUILD_SHIPPING


void SQuVRCatlogEntryWidget::Construct(const FArguments& InDelcaration)
{
	bIsPressed = false;
	IsDownload = false;
	NormalImage = new FSlateBrush();
	HoverImage = new FSlateBrush();
	PressedImage = new FSlateBrush();
	PlaceableItem = NULL;
	AsyncTaskDownloadFile = NULL;
	button = NULL;

	AssetInfo = InDelcaration._AssetInfo;
	buttonstyle = new FButtonStyle();
	if (AssetInfo.IsValid())
	{
		AssetInfo->ImageDownloadDone.AddSP(this, &SQuVRCatlogEntryWidget::RefreshWidget);
	}

	RefreshWidget();
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
			AssetEntryToolTip = FEditorClassUtils::GetTooltip(DefaultActor->GetClass());
		}
		if (!AssetEntryToolTip.IsValid())
		{
			AssetEntryToolTip = FSlateApplicationBase::Get().MakeToolTip(PlaceableItem->DisplayName);
		}
	}

	ChildSlot
		[
		SNew(SBorder).BorderImage(this, &SQuVRCatlogEntryWidget::GetSlateBrushState).ToolTip(AssetEntryToolTip)
		[
			SNew(SOverlay)
			+SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
			[
				SNew(SImage).ColorAndOpacity(this, &SQuVRCatlogEntryWidget::GetSlateColorState)
			]
			+ SOverlay::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
			[
				SNew(STextBlock).Text(this, &SQuVRCatlogEntryWidget::GetIsDownloade).ColorAndOpacity(FSlateColor(FLinearColor(1.0f,0.3f,0.3f,1.0f)))
			]
			
		]
/*			
		SAssignNew(button,SButton)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.ButtonStyle(buttonstyle)
		.OnClicked(this, &SQuVRCatlogEntryWidget::OnDownloadAsset)
*/
		   
		];

}

void SQuVRCatlogEntryWidget::InitPlaceableItem()
{
	if (AssetInfo.IsValid()&& false == PlaceableItem.IsValid())
	{
		if (UQuVRUtils::CheckFileExists(AssetInfo->PackageUrl))
		{
			IsDownload = true;
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
	
			FString filepath = UQuVRUtils::GetAssetPath(AssetInfo->PackageUrl);
			int32 SortOrder = 0;
			switch (AssetInfo->ObjectType)
			{
				/************************************************************************/
				/*		SK_ 骨骼模型 0       SM_  静态模型 1        M_  材质球 2  			*/
				/*		QuVRTianQiu_ 天空球 5											*/
				/************************************************************************/
			case 0:
				PlaceableItem = MakeShareable( new FPlaceableItem(*UQuVRAssetFactoryAnimation::StaticClass(), FAssetData(LoadObject<UAnimSequence>(nullptr, *filepath)), NAME_None, BasicShapeColorOverride, SortOrder += 10));
				break;
			case 1:
			case 5:
				PlaceableItem = MakeShareable(new FPlaceableItem(*UQuVRAssetFactoryModel::StaticClass(), FAssetData(LoadObject<UStaticMesh>(nullptr, *filepath)), NAME_None, BasicShapeColorOverride, SortOrder += 10));
				break;
			case 2:
				UMaterialInterface* MA = LoadObject<UMaterialInterface>(nullptr, *filepath);
				PlaceableItem = MakeShareable(new FPlaceableItem(*UClass::StaticClass(), FAssetData(LoadObject<UMaterialInterface>(nullptr, *filepath)), NAME_None, BasicShapeColorOverride, SortOrder += 10));
				break;
			}			
		};
	};
}

void SQuVRCatlogEntryWidget::RefreshWidget()
{
	if (AssetInfo.IsValid())
	{
		if (AssetInfo->Texture2Dimage)
		{
			FSlateBrush* AssetImage = new FSlateBrush();
			Texture2Dimage = AssetInfo->Texture2Dimage;
			AssetImage->ImageSize.X = AssetInfo->Texture2Dimage->GetSurfaceWidth();
			AssetImage->ImageSize.Y = AssetInfo->Texture2Dimage->GetSurfaceHeight();
			AssetImage->DrawAs = ESlateBrushDrawType::Image;
			AssetImage->SetResourceObject(Texture2Dimage);
			buttonstyle->SetNormal(*AssetImage);
			buttonstyle->SetPressed(*AssetImage);
			buttonstyle->SetHovered(*AssetImage);
			
			NormalImage = AssetImage;
			PressedImage = AssetImage;
			HoverImage = AssetImage;
		}
	}
}

FReply SQuVRCatlogEntryWidget::OnDownloadAsset()
{
	if (AssetInfo.IsValid())
	{
		FString URL = AssetInfo->PackageUrl;
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
					AsyncTaskDownloadFile->OnDownloadFileDone.AddSP(this, &SQuVRCatlogEntryWidget::DownloadDone);

				}
			}
		}
	}
	return FReply::Handled();
}


FReply SQuVRCatlogEntryWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsPressed = true;
		OnDownloadAsset();
		InitPlaceableItem();
		return FReply::Handled().DetectDrag(SharedThis(this), MouseEvent.GetEffectingButton());
	}

	return FReply::Unhandled();
}

FReply SQuVRCatlogEntryWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsPressed = false;
	}

	return FReply::Unhandled();
}

// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString("++++++++++OnDragDetected++++++++++"));

FReply SQuVRCatlogEntryWidget::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (AssetInfo.IsValid())
	{
		FString URL = AssetInfo->PackageUrl;
		if (5 < URL.Len())
		{
			if (!UQuVRUtils::CheckFileExists(URL))
			{
				return FReply::Handled();
			}
		}
	}
	bIsPressed = false;
	
	// if PlaceableItem Null
	if (false == PlaceableItem.IsValid())
	{
		return FReply::Handled();
	}

	/* Placement Actor */
	TArray<FAssetData> DraggedAssetDatas;
	DraggedAssetDatas.Add(PlaceableItem->AssetData);
	FEditorDelegates::OnAssetDragStarted.Broadcast(DraggedAssetDatas, PlaceableItem->Factory);
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		// BeginDragDrop
		return FReply::Handled().BeginDragDrop(FAssetDragDropOp::New(DraggedAssetDatas, PlaceableItem->Factory));
	}
	else
	{
		return FReply::Handled();
	}
}

FReply SQuVRCatlogEntryWidget::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

const FSlateBrush* SQuVRCatlogEntryWidget::GetSlateBrushState() const
{
	if (bIsPressed)
	{
		return PressedImage;
	}else if (IsHovered())
	{
		return HoverImage;
	}
	else
	{
		return NormalImage;
	}
}

FSlateColor SQuVRCatlogEntryWidget::GetSlateColorState() const
{
	if (IsDownload)
	{
		if (IsHovered())
		{
			return FSlateColor(FLinearColor(0.85f, 0.850f, 0.026f, 0.20f));
		}
		else
		{
			return FSlateColor(FLinearColor(1.00f, 1.00f, 1.00f, 0.05f));
		}
	}
	else
	{
		return FSlateColor(FLinearColor(0.3f, 0.30f, 0.30f, 0.30f));
	}
}

void SQuVRCatlogEntryWidget::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bIsHovered = true;
//	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, AssetInfo->DisplayName);
}

void SQuVRCatlogEntryWidget::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	bIsHovered = false;
//	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, AssetInfo->DisplayName);
}

FText SQuVRCatlogEntryWidget::GetIsDownloade() const
{
	if (IsDownload)
	{
		return FText::FromString(TEXT(""));
	}
	else
	{
		return FText::FromString(TEXT("Please Download"));
	}
}

void SQuVRCatlogEntryWidget::DownloadDone(int32 code)
{
	FString URL = AssetInfo->PackageUrl;
	if (5 < URL.Len())
	{
		if (UQuVRUtils::CheckFileExists(URL))
		{
			IsDownload = true;
		}
	}

	AsyncTaskDownloadFile->ConditionalBeginDestroy();
	AsyncTaskDownloadFile = NULL;
}

TSharedRef<SWidget> MakeCatalogEntryWidget(TWeakObjectPtr<UQuVRCatalogAssetInfo> item)
{
	return SNew(SQuVRCatlogEntryWidget).AssetInfo(item);
}

#undef LOCTEXT_NAMESPACE

#endif // #if !UE_BUILD_SHIPPING

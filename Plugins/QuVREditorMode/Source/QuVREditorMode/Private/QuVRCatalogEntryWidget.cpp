// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "QuVRCatalogEntryWidget.h"
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
#include "Developer/AssetTools/Public/IAssetTypeActions.h"
#include "Developer/AssetTools/Public/IAssetTools.h"
#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "Core.h"
#include "SlateBasics.h"
#include "QuVRFileDownloader.h"
#include "QuVRUtils.h"
#include "QuVRAssetFactoryModel.h"

#if !UE_BUILD_SHIPPING


void SQuVRCatlogEntryWidget::Construct(const FArguments& InDelcaration)
{
	bIsPressed = false;
	NormalImage = new FSlateBrush();
	HoverImage = new FSlateBrush();
	PressedImage = new FSlateBrush();

	AssetInfo = InDelcaration._AssetInfo;
	buttonstyle = new FButtonStyle();
	if (AssetInfo.IsValid())
	{
		AssetInfo->ImageDownloadDone.AddSP(this, &SQuVRCatlogEntryWidget::RefreshWidget);
	}

	RefreshWidget();
	ChildSlot
		[
			// SNew(SImage).Image(NormalImage)
			SNew(SBorder).BorderImage(this, &SQuVRCatlogEntryWidget::GetSlateBrushState)

			/*
			   SAssignNew(button,SButton)
			   .HAlign(HAlign_Fill)
			   .VAlign(VAlign_Fill)
			   .ButtonStyle(buttonstyle)
			   .OnClicked(this, &SQuVRCatlogEntryWidget::OnDownloadAsset)
		   */
		];

	InitPlaceableItem();
	

}

void SQuVRCatlogEntryWidget::InitPlaceableItem()
{
	if (AssetInfo.IsValid()&& NULL == PlaceableItem)
	{
		if (UQuVRUtils::CheckFileExists(AssetInfo->PackageUrl))
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
			FString filepath = UQuVRUtils::GetAssetPath(AssetInfo->PackageUrl);
			int32 SortOrder = 0;
			PlaceableItem = new FPlaceableItem(*UQuVRAssetFactoryModel::StaticClass(), FAssetData(LoadObject<UStaticMesh>(nullptr, *filepath)), NAME_None, BasicShapeColorOverride, SortOrder += 10);
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
				if (AsyncTaskDownloadImage)
				{
					AsyncTaskDownloadImage->StartDownloadZipFile(URL);
				}
				else
				{
					AsyncTaskDownloadImage = UQuVRFileDownloader::DownloadZipLoader(URL);
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
	bIsPressed = false;
	
	// if PlaceableItem Null
	if (NULL ==PlaceableItem)
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

TSharedRef<SWidget> MakeCatalogEntryWidget(TWeakObjectPtr<UQuVRCatalogAssetInfo> item)
{
	return SNew(SQuVRCatlogEntryWidget).AssetInfo(item);
}

#undef LOCTEXT_NAMESPACE

#endif // #if !UE_BUILD_SHIPPING

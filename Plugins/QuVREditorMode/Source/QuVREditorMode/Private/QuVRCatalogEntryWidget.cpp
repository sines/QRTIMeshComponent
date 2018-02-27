// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "QuVRCatalogEntryWidget.h"
#include "Widgets/SWidget.h"
#include "Widgets/SUserWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Runtime/Slate/Public/SlateOptMacros.h"
#include "Core.h"
#include "SlateBasics.h"
#include "QuVRFileDownloader.h"

#if !UE_BUILD_SHIPPING

//#define LOCTEXT_NAMESPACE "QuVRCatlogEntryWidget"


void SQuVRCatlogEntryWidget::Construct(const FArguments& InDelcaration)
{		
	AssetInfo = InDelcaration._AssetInfo;
	brush = new FSlateBrush();
	buttonstyle = new FButtonStyle();
	if (AssetInfo.IsValid())
	{
		AssetInfo->ImageDownloadDone.AddSP(this, &SQuVRCatlogEntryWidget::RefreshWidget);
	}

	RefreshWidget();
	ChildSlot
		[
//			SNew(SImage).Image(brush)

			SAssignNew(button,SButton)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.ButtonStyle(buttonstyle)
			.OnClicked(this, &SQuVRCatlogEntryWidget::OnDownloadAsset)
		];
};
void SQuVRCatlogEntryWidget::RefreshWidget()
{
	if (AssetInfo.IsValid())
	{
		if (AssetInfo->Texture2Dimage)
		{
			Texture2Dimage = AssetInfo->Texture2Dimage;
			brush->ImageSize.X = AssetInfo->Texture2Dimage->GetSurfaceWidth();
			brush->ImageSize.Y = AssetInfo->Texture2Dimage->GetSurfaceHeight();
			brush->DrawAs = ESlateBrushDrawType::Image;
			brush->SetResourceObject(Texture2Dimage);
			buttonstyle->SetNormal(*brush);
			buttonstyle->SetPressed(*brush);
			buttonstyle->SetHovered(*brush);

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
	return FReply::Handled();
}

TSharedRef<SWidget> MakeCatalogEntryWidget(TWeakObjectPtr<UQuVRCatalogAssetInfo> item)
{
	return SNew(SQuVRCatlogEntryWidget).AssetInfo(item);
}

#undef LOCTEXT_NAMESPACE

#endif // #if !UE_BUILD_SHIPPING

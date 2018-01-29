// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "QuVRCatalogBtDownloader.h"
#include "Widgets/SWidget.h"
#include "Widgets/SUserWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Runtime/Slate/Public/SlateOptMacros.h"
#include "Core.h"
#include "SlateBasics.h"

#if !UE_BUILD_SHIPPING

#define LOCTEXT_NAMESPACE "CatlogBtDownloader"


void SQuVRCatlogBtDownloader::Construct(const FArguments& InDelcaration)
{
		const FVector2D HeadingShadowOffset(2, 2);

		FSlateFontInfo LargeLayoutFont(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 12);
		FSlateFontInfo SmallLayoutFont(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 10);
		// Create doc link widget if there is a class to link to
		TSharedRef<SWidget> DocWidget = SNew(SSpacer);
		UClass* DocClass = nullptr;
		if (DocClass != NULL)
		{
		//	DocWidget = FEditorClassUtils::GetDocumentationLinkWidget(DocClass);
		//	DocWidget->SetCursor(EMouseCursor::Default);
		}

#if 1
		ChildSlot
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("Menu.Background"))
				.Cursor(EMouseCursor::GrabHand)
				[
					SNew(SHorizontalBox)

					+ SHorizontalBox::Slot()
					.Padding(0)
					.AutoWidth()
					[
						// Drop shadow border
						SNew(SBorder)
						.Padding(4)
						.BorderImage(FCoreStyle::Get().GetBrush("Menu.Background"))
						[
							SNew(SBox)
							.WidthOverride(35)
							.HeightOverride(35)
							[
								SNew(SButton)
// 				SNew(SPlacementAssetThumbnail, InAsset)
// 				.ClassThumbnailBrushOverride(Item->ClassThumbnailBrushOverride)
// 			 .AlwaysUseGenericThumbnail(Item->bAlwaysUseGenericThumbnail)
// 			.AssetTypeColorOverride(Item->AssetTypeColorOverride)
							]
						]
					]

					+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.Padding(2, 0, 4, 0)
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.Padding(0, 0, 0, 1)
							.AutoHeight()
							[
								SNew(STextBlock)
							//	.Text(Item->DisplayName)
							]
						]

					+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.AutoWidth()
						[
							DocWidget
						]
				]
			];
#endif
#if 0

		this->ChildSlot
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)

			[


				SNew(SOverlay)
				+SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
				[
					SNew(SSpacer)
				]
				+ SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
					[
						SNew(SBorder).BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
						[
							SNew(SBox).HeightOverride(64.0f).WidthOverride(64.0f)
							[
								SNew(SButton).Text(LOCTEXT("CatlogBtDownloader-Download", "обть"))
							]
						]
					]


			];

#endif
};




TSharedRef<SWidget> MakeCatalogBtDownload()
{
	return SNew(SQuVRCatlogBtDownloader);}

#undef LOCTEXT_NAMESPACE

#endif // #if !UE_BUILD_SHIPPING

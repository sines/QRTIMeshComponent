// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "QuVRCatalogEntryWidget.h"
#include "Widgets/SWidget.h"
#include "Widgets/SUserWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Runtime/Slate/Public/SlateOptMacros.h"
#include "Core.h"
#include "SlateBasics.h"

#if !UE_BUILD_SHIPPING

#define LOCTEXT_NAMESPACE "SQuVRCatlogEntryWidget"


void SQuVRCatlogEntryWidget::Construct(const FArguments& InDelcaration)
{		
	ChildSlot
		[
			SNew(SButton).HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.OnClicked(this, &SQuVRCatlogEntryWidget::OnDownloadAsset)
		];


#if false
		/** HorizontalScrollbar  Begin**/
	TSharedPtr<SScrollBox> HorizontalScrollbar =
		SNew(SScrollBox)
		.Orientation(Orient_Horizontal);
		HorizontalScrollbar->Slot().HAlign(HAlign_Center);
		HorizontalScrollbar->Slot().VAlign(VAlign_Center);		
		HorizontalScrollbar->AddSlot().HAlign(HAlign_Center).VAlign(VAlign_Center).Padding(0)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(SBox).HAlign(HAlign_Center).VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Justification(ETextJustify::Center)
					.TextStyle(FEditorStyle::Get(), "LargeText")
					.Text(FText::FromString(TEXT("TestButton")))
				]
			]
		];
		/** HorizontalScrollbar  End**/
		// Create doc link widget if there is a class to link to
		TSharedRef<SWidget> DocWidget = SNew(SSpacer);
		UClass* DocClass = nullptr;
		if (DocClass != NULL)
		{
		}


/**
 Add ChildSlot
*/
		ChildSlot
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("Menu.Background"))
				.Cursor(EMouseCursor::GrabHand)
				[

					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.Padding(0)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.AutoHeight()
					[
						// Drop shadow border
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(SSpacer).Size(FVector2D(1, 1))
						]
						+SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SBorder)
							.Padding(4)
							.BorderImage(FCoreStyle::Get().GetBrush("Menu.Background"))
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							[
								SNew(SBox)
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								.WidthOverride(128)
								.HeightOverride(128)
								[
									SNew(SButton)
									.HAlign(HAlign_Fill)
									.VAlign(VAlign_Fill)
								]
							]
						]
						+ SHorizontalBox::Slot()
							[
								SNew(SSpacer).Size(FVector2D(1.0f, 1.0f))
							]
					]
					+ SVerticalBox::Slot()
						.VAlign(VAlign_Fill)
						.HAlign(HAlign_Fill)
						.AutoHeight()
						[
							SNew(SSpacer)
						]
					+ SVerticalBox::Slot()
						.VAlign(VAlign_Fill)
						.HAlign(HAlign_Fill)
						.AutoHeight()
						.Padding(2, 0, 4, 0)
						[
							HorizontalScrollbar.ToSharedRef()
						]
				]
			];
#endif
};


FReply SQuVRCatlogEntryWidget::OnDownloadAsset()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString("DownloadAsset"));

	return FReply::Handled();
}

TSharedRef<SWidget> MakeCatalogEntryWidget()
{
	return SNew(SQuVRCatlogEntryWidget);}

#undef LOCTEXT_NAMESPACE

#endif // #if !UE_BUILD_SHIPPING

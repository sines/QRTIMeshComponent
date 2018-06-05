// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "QuVRCatalogWidget.h"

#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SScrollBar.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SScissorRectBox.h"
#include "Widgets/Navigation/SBreadcrumbTrail.h"
#include "Editor/ContentBrowser/Private/SAssetView.h"
#include "LevelEditor.h"
#include "EditorStyleSet.h"
#include "QuVRCatalogGategoryWidget.h"
#include "QuVRCatalogEntryWidget.h"
#include "QuVRCatalogNodeButton.h"
#include "QuVRCatalogDataManager.h"
#include "QuVRCatalogPlaneWidget.h"
#include "QuVRAssetViewWidget.h"

void SQuVRCatalogWidget::CreateGroupGroupTabRoot(TSharedPtr<FQuVRCatalogNode > node)
{
	HTB->ClearChildren();
	HTB->AddSlot().Padding(0)
	.AutoWidth()
	[
		SNew(SQuVRCatalogPlaneWidget).TreeItem(node).RootContent(HTB).RootWidget(SharedThis(this))
	];
	AddAssetView();
	ClearAssetList();
}

void SQuVRCatalogWidget::ClearAssetList()
{
	viewWidget->ClearSourceItems();
}

void SQuVRCatalogWidget::ClearPageTool()
{
	viewWidget->ClearPage();
}

/* Group Tab Asset List*/
void SQuVRCatalogWidget::CreateCatalogGroupTabAssetList(TSharedPtr<FQuVRCatalogNode > node,bool InHold)
{
	ClearAssetList();

	bool NFlip = true;
	TArray< FName > ObjectPaths;
	ObjectPaths.Reset();

	for (auto asset : node->AssetList)
	{
		FString filepath = UQuVRUtils::GetAssetPath(asset->PackageUrl);
		ObjectPaths.Add(*filepath);
		NFlip = !NFlip;
	}

	viewWidget->RefreshSourceItems(node.ToSharedRef(), InHold);
}


void SQuVRCatalogWidget::Construct(const FArguments& InArgs)
{
	
	bNeedsUpdate = true;
	SAssignNew(HTB, SHorizontalBox);
	TSharedRef<SScrollBar> ScrollBar = SNew(SScrollBar).Thickness(FVector2D(1, 1));

 // add ChildSlot Layout
	ChildSlot
	[
		
		SNew(SVerticalBox)
		#if 0
		// Create ResGategory
		+SVerticalBox::Slot().AutoHeight()
		[
			SNew(SBorder)
			[
				MakeGategoryWidget()
			]
		]
		#endif
		// Create Download
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(SBorder)
			[
				SNew(SBorder)
				[
					SNew(SBox).HeightOverride(8.0f).WidthOverride(2.0f)
				]
			]
		]
 		+ SVerticalBox::Slot()
 		[
 			HTB.ToSharedRef()
 		]
	];

	UQuVRCatalogDataManager::GetInstance()->SetWidget(SharedThis(this));
	UQuVRCatalogDataManager::GetInstance()->GetCatalogListACountFromUrl();
	bNeedsUpdate = true;
}

void SQuVRCatalogWidget::AddAssetView()
{
	HTB->AddSlot()
		[
			SAssignNew(viewWidget, SQuVRAssetViewWidget)
		];
	bNeedsUpdate = true;
}

FReply SQuVRCatalogWidget::HandleBreadcrumbTrailAddButtonClicked()
{
	// (FString(TEXT("AX")))
	FString abc(TEXT("²âÊÔ°´Å¥"));
	BreadcrumbTrail->PushCrumb(FText::FromString(abc), 0);

	return FReply::Handled();
}

TSharedRef<SWidget> SQuVRCatalogWidget::CreateGroupTabManufacturer(const FString& CatalogName)
{
	return 
		SNew(SBorder)
		.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
		.Padding(0.0f)
		[
			SNew(SGridPanel)
			.FillColumn(0, 0.5f)
			.FillColumn(1, 0.5f)

			// SBorder
			+ SGridPanel::Slot(0, 0)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("SBorder")))
			]

			+ SGridPanel::Slot(1, 0)
				.Padding(0.0f, 5.0f)
			[
				SNew(SBorder)
				[
					SNew(SSpacer)
					.Size(FVector2D(100.0f, 50.0f))
				]
			]

			// SBreadcrumbTrailLabel
			+ SGridPanel::Slot(0, 1)
				[
					SNew(STextBlock)
					.Text(FText::FromString("SBreadcrumbTrail"))
				]

			+ SGridPanel::Slot(1, 1)
				.Padding(0.0f, 5.0f)
				[
					SNew(SHorizontalBox)

					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SAssignNew(BreadcrumbTrail, SBreadcrumbTrail<int32>)

					]

				+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SButton)
						.Text(FText::FromString("Add"))
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
					.OnClicked(this, &SQuVRCatalogWidget::HandleBreadcrumbTrailAddButtonClicked)
					]
				]
			]
		];
}



void SQuVRCatalogWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (bNeedsUpdate)
	{
		// void
		bNeedsUpdate = false;
	}
}

void SQuVRCatalogEntry::Construct(const FArguments& InArgs, TWeakObjectPtr<UQuVRCatalogAssetInfo> InItem)
{
//	CatalogEntryWidget = SNew(SQuVRCatlogEntryWidget).AssetInfo(InItem);
#if false
	ChildSlot
		[
			MakeCatalogEntryWidget(InItem)
		];
#endif

#if true
		/** HorizontalScrollbar  Begin**/
	TSharedPtr<SScrollBox> HorizontalScrollbar =
		SNew(SScrollBox)
		.Orientation(Orient_Vertical);
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
					.Text(FText::FromString(InItem->DisplayName))
					.HighlightColor(FLinearColor::Red)
				]
			]
		];
		/** HorizontalScrollbar  End**/


	/**
	Add ChildSlot
	*/
ChildSlot
[
	SNew(SBorder).Padding(1)
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
				SNew(SSpacer).Size(FVector2D(2.0f, 2.0f))
			]
				+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBorder)
						.Padding(2)
						.BorderImage(FCoreStyle::Get().GetBrush("Menu.Background"))
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SBox)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.WidthOverride(112)
							.HeightOverride(112)
						]
					]
				+ SHorizontalBox::Slot()
					[
						SNew(SSpacer).Size(FVector2D(2.0f, 2.0f))
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

}
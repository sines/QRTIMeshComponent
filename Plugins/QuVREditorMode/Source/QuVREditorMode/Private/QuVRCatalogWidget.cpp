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
#include "LevelEditor.h"
#include "EditorStyleSet.h"
#include "QuVRCatalogGategoryWidget.h"
#include "QuVRCatalogEntryWidget.h"
#include "QuVRCatalogNodeButton.h"
#include "QuVRCatalogDataManager.h"
#include "QuVRCatalogPlaneWidget.h"

void SQuVRCatalogWidget::CreateGroupGroupTabRoot(TSharedPtr<FQuVRCatalogNode > node)
{
	HTB->ClearChildren();
	HTB->AddSlot().Padding(0)
	.AutoWidth()
	[
		SNew(SQuVRCatalogPlaneWidget).TreeItem(node).RootContent(HTB).RootWidget(SharedThis(this))
	];
	AddListViewLR();
	ClearAssetList();
}

void SQuVRCatalogWidget::ClearAssetList()
{
	ListViewFilteredLeftItems.Empty();
	ListViewFilteredRightItems.Empty();
	RequestRefresh();
}

/* Group Tab Asset List*/
void SQuVRCatalogWidget::CreateCatalogGroupTabAssetList(TSharedPtr<FQuVRCatalogNode > node)
{
	ClearAssetList();
	bool NFlip = true;
	for (auto asset : node->AssetList)
	{
		if (NFlip)
		{
			ListViewFilteredLeftItems.AddUnique(asset);
		}
		else
		{
			ListViewFilteredRightItems.AddUnique(asset);
		}
		NFlip = !NFlip;
	}
	RequestRefresh();
}


void SQuVRCatalogWidget::Construct(const FArguments& InArgs)
{
	
	bNeedsUpdate = true;
	SAssignNew(HTB, SHorizontalBox);
	TSharedRef<SScrollBar> ScrollBar = SNew(SScrollBar).Thickness(FVector2D(1, 1));

#if 1 // add ChildSlot Layout
	ChildSlot
	[
		
		SNew(SVerticalBox)
		// Create ResGategory
		+SVerticalBox::Slot().AutoHeight()
		[
			SNew(SBorder)
			[
				MakeGategoryWidget()
			]
		]
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
#endif
	UQuVRCatalogDataManager::GetInstance()->SetWidget(SharedThis(this));
	bNeedsUpdate = true;
}

void SQuVRCatalogWidget::AddListViewLR()
{
	TSharedRef<SScrollBar> ScrollBar = SNew(SScrollBar).Thickness(FVector2D(1, 1));
	HTB->AddSlot()
		[
			SNew(SBorder)
			.Padding(FMargin(3))
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.DarkGroupBorder"))
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
		[
			SNew(SBox)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SAssignNew(ListViewLeft, SListView<TWeakObjectPtr<UQuVRCatalogAssetInfo>>)
					.ListItemsSource(&ListViewFilteredLeftItems)
					.OnGenerateRow(this, &SQuVRCatalogWidget::OnGenerateWidgetForItem)
					.ExternalScrollbar(ScrollBar)
				]

				+ SHorizontalBox::Slot()
					[

						SAssignNew(ListViewRight, SListView<TWeakObjectPtr<UQuVRCatalogAssetInfo>>)
						.ListItemsSource(&ListViewFilteredRightItems)
					.OnGenerateRow(this, &SQuVRCatalogWidget::OnGenerateWidgetForItem)
					.ExternalScrollbar(ScrollBar)
					]

				+ SHorizontalBox::Slot().AutoWidth()
					[
						ScrollBar
					]
				]
			]
		]
	];
	bNeedsUpdate = true;
}

void SQuVRCatalogWidget::CreateWidgetElement()
{

}

/** Request that both Tree and List refresh themselves on the next tick */
FReply SQuVRCatalogWidget::RequestRefresh()
{
	if (ListViewRight.IsValid())
	{
		ListViewRight->RequestListRefresh();
	}
	if (ListViewLeft.IsValid())
	{
		ListViewLeft->RequestListRefresh();
	}
	return FReply::Handled();
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
		bNeedsUpdate = false;
		RequestRefresh();
	}
}

TSharedRef<ITableRow> SQuVRCatalogWidget::OnGenerateWidgetForItem(TWeakObjectPtr<UQuVRCatalogAssetInfo> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (InItem.IsValid())
	{
		return SNew(STableRow<TWeakObjectPtr<UQuVRCatalogAssetInfo>>, OwnerTable)
			[
				SNew(SQuVRCatalogEntry, InItem)
			];
	}
	else
	{
		return SNew(STableRow<TWeakObjectPtr<UQuVRCatalogAssetInfo>>, OwnerTable);
	}

}

void SQuVRCatalogEntry::Construct(const FArguments& InArgs, TWeakObjectPtr<class UQuVRCatalogAssetInfo> InItem)
{
/*

	ChildSlot
		[
			MakeCatalogEntryWidget()
		];
*/

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
				]
			]
		];
		/** HorizontalScrollbar  End**/


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
			+ SHorizontalBox::Slot()
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
						.WidthOverride(112)
						.HeightOverride(112)
						[
							MakeCatalogEntryWidget(InItem)
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

}
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
#include "QuVRAssetDownNet.h"
#include "QuVRCatalogPlaneWidget.h"

static TArray<TSharedPtr<FCatalogItem>> GFilteredLeftItems;
static TArray<TSharedPtr<FCatalogItem>> GFilteredRightItems;

void SQuVRCatalogWidget::CreateGroupGroupTabRoot(TSharedPtr<FQuVRCatalogNode > node)
{
	HTB->ClearChildren();
	HTB->AddSlot().Padding(0)
	.AutoWidth()
	[
		SNew(SQuVRCatalogPlaneWidget).TreeItem(node).RootContent(HTB).RootWidget(SharedThis(this))
	];
	AddListViewLR();
}

void SQuVRCatalogWidget::ClearAssetList()
{
	GFilteredLeftItems.Empty();
	GFilteredLeftItems.Reset();
	GFilteredRightItems.Empty();
	GFilteredRightItems.Reset();
	RequestRefresh();
}

/* Group Tab Asset List*/
void SQuVRCatalogWidget::CreateCatalogGroupTabAssetList(TArray <TSharedPtr<FQuVRCatalogNode>> nodeList)
{
	GFilteredLeftItems.Empty();
	GFilteredLeftItems.Reset();
	GFilteredRightItems.Empty();
	GFilteredRightItems.Reset();

	bool NFlip = true;
	for (auto node : nodeList)
	{
		TSharedPtr<FCatalogItem> CatalogItem = MakeShareable(new FCatalogItem());
		CatalogItem->DisplayName = FText::FromString(node->NodeData.DisplayName);
		if (NFlip)
		{
			GFilteredLeftItems.Add(CatalogItem);
		}
		else
		{
			GFilteredRightItems.Add(CatalogItem);
		}
		NFlip = !NFlip;
	}
	RequestRefresh();
}


void SQuVRCatalogWidget::Construct(const FArguments& InArgs)
{
	
	bNeedsUpdate = false;
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
	UQuVRAssetDownNet::GetInstance()->SetWidget(SharedThis(this));
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

			SAssignNew(ListViewLeft, SListView<TSharedPtr<FCatalogItem>>)
			.ListItemsSource(&GFilteredLeftItems)
			.OnGenerateRow(this, &SQuVRCatalogWidget::OnGenerateWidgetForItem)
			.ExternalScrollbar(ScrollBar)

		]

	+ SHorizontalBox::Slot()
		[

			SAssignNew(ListViewRight, SListView<TSharedPtr<FCatalogItem>>)
			.ListItemsSource(&GFilteredRightItems)
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

TSharedRef<ITableRow> SQuVRCatalogWidget::OnGenerateWidgetForItem(TSharedPtr<FCatalogItem> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FCatalogItem>>, OwnerTable)
			[
				SNew(SQuVRCatalogEntry, InItem.ToSharedRef())
			];
}

void SQuVRCatalogEntry::Construct(const FArguments& InArgs, const TSharedPtr<const FCatalogItem>& InItem)
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
					.Text(InItem->DisplayName)
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
					MakeCatalogEntryWidget()
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
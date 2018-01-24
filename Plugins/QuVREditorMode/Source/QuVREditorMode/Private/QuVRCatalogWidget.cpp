// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "QuVRCatalogWidget.h"

#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SScrollBar.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SListView.h"

#include "LevelEditor.h"
#include "EditorStyleSet.h"

static TArray<TSharedPtr<FCatalogItem>> GFilteredItems;

void SQuVRCatalogWidget::Construct(const FArguments& InArgs)
{
	
	bNeedsUpdate = false;
	ActiveTabName = TEXT("Models");
	GenerateItemsByCatalog(ActiveTabName.ToString());

	// Construct the tab1
	TSharedRef<SVerticalBox> Tab1 = SNew(SVerticalBox);
	
	// Construct the tab2
	TSharedRef<SVerticalBox> Tab2 = SNew(SVerticalBox);

	// Construct the tab3
	TSharedRef<SVerticalBox> Tab3 = SNew(SVerticalBox);
	TSharedRef<SScrollBar> ScrollBar = SNew(SScrollBar).Thickness(FVector2D(5, 5));

	// Add each catalog item
	Tab1->AddSlot().AutoHeight()
	[
		CreateCatalogGroupTab(TEXT("Models"))
	];

	Tab1->AddSlot().AutoHeight()
	[
		CreateCatalogGroupTab(TEXT("Materials"))
	];

	Tab1->AddSlot().AutoHeight()
	[
		CreateCatalogGroupTab(TEXT("MEIWU"))
	];
	
	for (int i=0;i<100;  ++i)
	{
		Tab2->AddSlot().AutoHeight()
		[
			CreateCatalogGroupTab(TEXT("Tab2Iteam1") + FString::FromInt(i))
		];

	}

	Tab3->AddSlot().AutoHeight()
	[
		CreateCatalogGroupTab(TEXT("T2Models1"))
	];
	Tab3->AddSlot().AutoHeight()
	[
		CreateCatalogGroupTab(TEXT("T2Models2"))
	];

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(0)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				Tab1
			]
			+ SHorizontalBox::Slot()
			[
				Tab2
			]
			+ SHorizontalBox::Slot()
			[
				Tab3
			]
#if 0


			+ SHorizontalBox::Slot()
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

								SAssignNew(ListView, SListView<TSharedPtr<FCatalogItem>>)
								.ListItemsSource(&GFilteredItems)
								.OnGenerateRow(this, &SQuVRCatalogWidget::OnGenerateWidgetForItem)
								.ExternalScrollbar(ScrollBar)

							]

							+SHorizontalBox::Slot().AutoWidth()
							[
								ScrollBar
							]
						]	
					]
				]
			
			]
		]
	];

	
}

void SQuVRCatalogWidget::GenerateItemsByCatalog(const FString& CatalogName)
{
	GFilteredItems.Reset();

	if (CatalogName.Equals(TEXT("Models")))
	{
		TSharedPtr<FCatalogItem> CatalogItem = MakeShareable(new FCatalogItem());
		CatalogItem->DisplayName = FText::FromString(TEXT("Model A"));
		GFilteredItems.Add(CatalogItem);

		TSharedPtr<FCatalogItem> CatalogItem1 = MakeShareable(new FCatalogItem());
		CatalogItem1->DisplayName = FText::FromString(TEXT("Model B"));
		GFilteredItems.Add(CatalogItem1);

		TSharedPtr<FCatalogItem> CatalogItem2 = MakeShareable(new FCatalogItem());
		CatalogItem2->DisplayName = FText::FromString(TEXT("Model C"));
		GFilteredItems.Add(CatalogItem2);
	}
	else if (CatalogName.Equals(TEXT("Materials")))
	{
		TSharedPtr<FCatalogItem> CatalogItem = MakeShareable(new FCatalogItem());
		CatalogItem->DisplayName = FText::FromString(TEXT("Material A"));
		GFilteredItems.Add(CatalogItem);

		TSharedPtr<FCatalogItem> CatalogItem1 = MakeShareable(new FCatalogItem());
		CatalogItem1->DisplayName = FText::FromString(TEXT("Material B"));
		GFilteredItems.Add(CatalogItem1);

		TSharedPtr<FCatalogItem> CatalogItem2 = MakeShareable(new FCatalogItem());
		CatalogItem2->DisplayName = FText::FromString(TEXT("Material C"));
		GFilteredItems.Add(CatalogItem2);
	}
	else if (CatalogName.Equals(TEXT("MEIWU")))
	{
		TSharedPtr<FCatalogItem> CatalogItem = MakeShareable(new FCatalogItem());
		CatalogItem->DisplayName = FText::FromString(TEXT("MEIWU A"));
		GFilteredItems.Add(CatalogItem);

		TSharedPtr<FCatalogItem> CatalogItem1 = MakeShareable(new FCatalogItem());
		CatalogItem1->DisplayName = FText::FromString(TEXT("MEIWU B"));
		GFilteredItems.Add(CatalogItem1);

		TSharedPtr<FCatalogItem> CatalogItem2 = MakeShareable(new FCatalogItem());
		CatalogItem2->DisplayName = FText::FromString(TEXT("MEIWU C"));
		GFilteredItems.Add(CatalogItem2);
	}

}

TSharedRef<SWidget> SQuVRCatalogWidget::CreateCatalogGroupTab(const FString& CatalogName)
{
	TSharedRef<SScrollBar> ScrollBar = SNew(SScrollBar).Thickness(FVector2D(5, 5));
	return SNew(SCheckBox)
			.OnCheckStateChanged(this, &SQuVRCatalogWidget::OnCatalogTabChanged, FName(*CatalogName))
			.IsChecked(this, &SQuVRCatalogWidget::GetCatalogTabCheckedState, FName(*CatalogName))
			.Style(FEditorStyle::Get(), "PlacementBrowser.Tab")
			[

				SNew(SOverlay)
				+ SOverlay::Slot()
				.VAlign(VAlign_Center)
				[
					SNew(SSpacer)
					.Size(FVector2D(1, 30))
				]
				+ SOverlay::Slot()
				.Padding(FMargin(6, 0, 15, 0))
				.VAlign(VAlign_Center)
				[
					
					SNew(STextBlock)
					.TextStyle(FEditorStyle::Get(), "PlacementBrowser.Tab.Text")
					.Text(FText::FromString(CatalogName))
				]
				+ SOverlay::Slot()
					.VAlign(VAlign_Fill)
					.HAlign(HAlign_Left)
					[
						SNew(SImage)
						.Image(this, &SQuVRCatalogWidget::CatalogGroupBorderImage, FName(*CatalogName))
					]
			];

}

void SQuVRCatalogWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (bNeedsUpdate)
	{
		bNeedsUpdate = false;
		ListView->RequestListRefresh();
	}

}


void SQuVRCatalogWidget::OnCatalogTabChanged(ECheckBoxState NewState, FName CategoryName)
{
	if (NewState == ECheckBoxState::Checked)
	{
		ActiveTabName = CategoryName;
		GenerateItemsByCatalog(CategoryName.ToString());

		bNeedsUpdate = true;
	}
}

ECheckBoxState SQuVRCatalogWidget::GetCatalogTabCheckedState(FName CategoryName) const
{
	return ActiveTabName == CategoryName ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}


TSharedRef<ITableRow> SQuVRCatalogWidget::OnGenerateWidgetForItem(TSharedPtr<FCatalogItem> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FCatalogItem>>, OwnerTable)
			[
				SNew(SQuVRCatalogEntry, InItem.ToSharedRef())
			];
}

const FSlateBrush* SQuVRCatalogWidget::CatalogGroupBorderImage(FName CategoryName) const
{
	if (ActiveTabName == CategoryName)
	{
		static FName QuVRActiveTabBarBrush("PlacementBrowser.ActiveTabBar");
		return FEditorStyle::GetBrush(QuVRActiveTabBarBrush);
	}
	else
	{
		return nullptr;
	}
}

void SQuVRCatalogEntry::Construct(const FArguments& InArgs, const TSharedPtr<const FCatalogItem>& InItem)
{
	ChildSlot
	[
		SNew(STextBlock).Text(InItem->DisplayName)
	];
}
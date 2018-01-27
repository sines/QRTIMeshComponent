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
#include "QuVRCatalogResGategory.h"
#include "QuVRCatalogBtDownloader.h"

static TArray<TSharedPtr<FCatalogItem>> GFilteredItems;

void SQuVRCatalogWidget::Construct(const FArguments& InArgs)
{
	
	bNeedsUpdate = false;

	TSharedRef<SVerticalBox> VerticalBoxPrimary = SNew(SVerticalBox);

	TSharedRef<SVerticalBox> VerticalBoxSection = SNew(SVerticalBox);
	
	CreateGroupTabData(VerticalBoxPrimary, VerticalBoxSection);
	TSharedRef<SScrollBar> ScrollBar = SNew(SScrollBar).Thickness(FVector2D(5, 5));

#if true
	// add ChildSlot Layout
	ChildSlot
	[
		
		SNew(SVerticalBox)
		// Create ResGategory
		+SVerticalBox::Slot().AutoHeight()
		[
			SNew(SBorder)
			[
				SNew(SScissorRectBox)
				[
					MakeResGategory()
				]
			]
		]
		// Create Download
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(SBorder)
			[
				SNew(SScissorRectBox)
				[
					MakeCatalogBtDownload()
				]
			]
		]
		+ SVerticalBox::Slot()
		.Padding(0)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SScrollBox)
				+SScrollBox::Slot()
				[
					VerticalBoxPrimary
				]
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SScrollBox)
				+ SScrollBox::Slot()
				[
					VerticalBoxSection
				]
			]


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

								SAssignNew(ListViewLeft, SListView<TSharedPtr<FCatalogItem>>)
								.ListItemsSource(&GFilteredItems)
								.OnGenerateRow(this, &SQuVRCatalogWidget::OnGenerateWidgetForItem)
								.ExternalScrollbar(ScrollBar)

							]

							+SHorizontalBox::Slot()
							[

								SAssignNew(ListViewRight, SListView<TSharedPtr<FCatalogItem>>)
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
#endif
	RebuildData();
}


void SQuVRCatalogWidget::CreateWidgetElement()
{

}

void SQuVRCatalogWidget::CreateGroupTabData(TSharedRef<SVerticalBox> InPrimary, TSharedRef<SVerticalBox> InSection)
{
	InPrimary->AddSlot().AutoHeight()
	[
		CreateGroupGroupTabPrimary(FString(TEXT("TabPrimary")))
	];

	InSection->AddSlot().AutoHeight()
	[
		CreateCatalogGroupTabSection(FString(TEXT("TabSection")))
	];
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

FReply SQuVRCatalogWidget::RebuildData()
{
	GFilteredItems.Empty();
	GFilteredItems.Reset();

	TSharedPtr<FCatalogItem> CatalogItem = MakeShareable(new FCatalogItem());
	CatalogItem->DisplayName = FText::FromString(TEXT("Model A"));
	GFilteredItems.Add(CatalogItem);
	RequestRefresh();
	
	return FReply::Handled();
}

void SQuVRCatalogWidget::GenerateItemsByCatalog(const FString& CatalogName)
{
	GFilteredItems.Reset();
	TSharedPtr<FCatalogItem> CatalogItem = MakeShareable(new FCatalogItem());
	CatalogItem->DisplayName = FText::FromString(TEXT("Model A"));
	GFilteredItems.Add(CatalogItem);

#if 0
	if (CatalogName.Equals(TEXT("Models")))
	{
		TSharedPtr<FCatalogItem> CatalogItem = MakeShareable(new FCatalogItem());
		CatalogItem->DisplayName = FText::FromString(TEXT("Model A"));
		CatalogItem->widget = MakeCatalogBtDownload();
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
#endif
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

TSharedRef<SWidget> SQuVRCatalogWidget::CreateGroupGroupTabPrimary(const FString& CatalogName)
{
	return SNew(SCheckBox)
		.OnCheckStateChanged(this, &SQuVRCatalogWidget::OnCatalogTabChangedPrimary, FName(*CatalogName))
		.IsChecked(this, &SQuVRCatalogWidget::GetCatalogTabCheckedStatePrimary, FName(*CatalogName))
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


TSharedRef<SWidget> SQuVRCatalogWidget::CreateCatalogGroupTabSection(const FString& CatalogName)
{
	return SNew(SCheckBox)
			.OnCheckStateChanged(this, &SQuVRCatalogWidget::OnCatalogTabChangedSection, FName(*CatalogName))
			.IsChecked(this, &SQuVRCatalogWidget::GetCatalogTabCheckedStateSection, FName(*CatalogName))
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
		RequestRefresh();
	}
}

ECheckBoxState SQuVRCatalogWidget::GetCatalogTabCheckedStatePrimary(FName CategoryName) const
{
	return ActiveTabName == CategoryName ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SQuVRCatalogWidget::OnCatalogTabChangedPrimary(ECheckBoxState NewState, FName CategoryName)
{
	if (NewState == ECheckBoxState::Checked)
	{
		ActiveTabName = CategoryName;
//		GenerateItemsByCatalog(CategoryName.ToString());

		bNeedsUpdate = true;
	}
}


void SQuVRCatalogWidget::OnCatalogTabChangedSection(ECheckBoxState NewState, FName CategoryName)
{
	if (NewState == ECheckBoxState::Checked)
	{
		SectionTabName = CategoryName;
	//	GenerateItemsByCatalog(CategoryName.ToString());
		bNeedsUpdate = true;
	}
}

ECheckBoxState SQuVRCatalogWidget::GetCatalogTabCheckedStateSection(FName CategoryName) const
{
	return SectionTabName == CategoryName ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
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
	//

	ChildSlot
	[
		SNew(SBox)
		[		
			SNew(SQuVRCatlogBtDownloader)
		]

	//	SNew(STextBlock).Text(InItem->DisplayName)
	];
}
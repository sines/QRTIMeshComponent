// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Widgets/SWidget.h"
#include "Widgets/SUserWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/STextComboBox.h"
#include "Widgets/Navigation/SBreadcrumbTrail.h"


/**
* Structure defining a catalog item in the placement mode panel
*/
struct FCatalogItem
{
	/** Default constructor */
	FCatalogItem(){}

	/** This item's display name */
	FText DisplayName;

};

/**
* Catalog widget, display all models and materials data
*/
class SQuVRCatalogWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SQuVRCatalogWidget) {}
	SLATE_END_ARGS();
	
	void Construct(const FArguments& InArgs);
	void test();

private:
	/** Flag to indicate that we need to update the filtered items array */
	bool bNeedsUpdate;

	/* List_View */
	/** List view that shows placeable items */
	TSharedPtr<SListView<TSharedPtr<FCatalogItem>>> ListViewLeft;

	/** List view that shows placeable items */
	TSharedPtr<SListView<TSharedPtr<FCatalogItem>>> ListViewRight;

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	
	/*Group Tab Data*/
	void CreateGroupTabData(TSharedRef<SVerticalBox> InPrimary, TSharedRef<SVerticalBox> InSection);
	FReply RebuildData();
	FReply RequestRefresh();

	// Create GridPanel
	TSharedPtr<SBreadcrumbTrail<int32> > BreadcrumbTrail;
	TSharedRef<SWidget> CreateGroupTabManufacturer(const FString& CatalogName);
	FReply HandleBreadcrumbTrailAddButtonClicked();
	// Create PrimaryTab
	TSharedPtr<SVerticalBox> VerticalBoxPrimary;
	FName ActiveTabName;
	TSharedRef<SWidget> CreateGroupGroupTabPrimary(const FString& CatalogName);
	void OnCatalogTabChangedPrimary(ECheckBoxState NewState, FName CategoryName);
	ECheckBoxState GetCatalogTabCheckedStatePrimary(FName CategoryName) const;

	// Create SelectSection
	TSharedPtr<SVerticalBox> VerticalBoxSection;
	FName SectionTabName;
	TSharedRef<SWidget> CreateCatalogGroupTabSection(const FString& CatalogName);
	void OnCatalogTabChangedSection(ECheckBoxState NewState, FName CategoryName);
	ECheckBoxState GetCatalogTabCheckedStateSection(FName CategoryName) const;


	TSharedRef<ITableRow> OnGenerateWidgetForItem(TSharedPtr<FCatalogItem> InItem, const TSharedRef<STableViewBase>& OwnerTable);

	void GenerateItemsByCatalog(const FString& CatalogName);

	void CreateWidgetElement();
	/** Gets the border image for the tab, this is the 'active' orange bar. */
	const FSlateBrush* CatalogGroupBorderImage(FName CategoryName) const;
};

/**
* Catalog entry widget, displaying one catalog item (A Model or a Texture)
*/
class SQuVRCatalogEntry : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SQuVRCatalogEntry){ }
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedPtr<const FCatalogItem>& InItem);

	TSharedPtr<const FCatalogItem> Item;
};
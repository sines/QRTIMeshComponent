// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Widgets/SWidget.h"
#include "Widgets/SUserWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/STextComboBox.h"
#include "Widgets/Navigation/SBreadcrumbTrail.h"

#if !UE_BUILD_SHIPPING

class FQuVRCatalogNode;
/**
* Structure defining a catalog item in the placement mode panel
*/

/*
struct FCatalogItem
{
	/ ** Default constructor * /
	FCatalogItem()
	{
		DisplayName =FText::FromString(TEXT("0000"));
		image = nullptr;
	}

	/ ** This item's display name * /
	FText DisplayName;
	FSlateBrush* image;

};
*/

/**
* Catalog widget, display all models and materials data
*/
class SQuVRCatalogWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SQuVRCatalogWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

public:
	void CreateCatalogGroupTabAssetList(TSharedPtr<FQuVRCatalogNode > node);
	void ClearAssetList();
	void CreateGroupGroupTabRoot(TSharedPtr<FQuVRCatalogNode > node);
private:
	/** Flag to indicate that we need to update the filtered items array */
	bool bNeedsUpdate;

	/* List_View */
	/** List view that shows placeable items */\
	TArray<TWeakObjectPtr<class UQuVRcatalogAssetInfo>> ListViewFilteredLeftItems;
	TSharedPtr<SListView<TWeakObjectPtr<class UQuVRcatalogAssetInfo>>> ListViewLeft;

	/** List view that shows placeable items */
	TArray<TWeakObjectPtr<class UQuVRcatalogAssetInfo>> ListViewFilteredRightItems;
	TSharedPtr<SListView<TWeakObjectPtr<class UQuVRcatalogAssetInfo>>> ListViewRight;

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	
	/*Group Tab Data*/
	FReply RequestRefresh();
	// Create GridPanel
	TSharedPtr<SBreadcrumbTrail<int32> > BreadcrumbTrail;
	TSharedRef<SWidget> CreateGroupTabManufacturer(const FString& CatalogName);
	FReply HandleBreadcrumbTrailAddButtonClicked();

	TSharedRef<ITableRow> OnGenerateWidgetForItem(TWeakObjectPtr<class UQuVRcatalogAssetInfo> InItem, const TSharedRef<STableViewBase>& OwnerTable);

	void CreateWidgetElement();

	//////////////////////////////////////////////////////////////////////////
	TSharedPtr<SHorizontalBox> HTB;
	void AddListViewLR();
};

/**
* Catalog entry widget, displaying one catalog item (A Model or a Texture)
*/
class SQuVRCatalogEntry : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SQuVRCatalogEntry){ }
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TWeakObjectPtr<class UQuVRcatalogAssetInfo> InItem);
};

#endif
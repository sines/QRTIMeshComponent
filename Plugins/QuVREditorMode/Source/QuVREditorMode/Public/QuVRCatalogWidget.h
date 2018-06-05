// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Widgets/SWidget.h"
#include "Widgets/SUserWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/STextComboBox.h"
#include "Widgets/Navigation/SBreadcrumbTrail.h"
#include "QuVRAssetViewWidget.h"

#if !UE_BUILD_SHIPPING

class FQuVRCatalogNode;
struct FQuVRCatalogAssetBase;

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
	void CreateCatalogGroupTabAssetList(TSharedPtr<FQuVRCatalogNode > node, bool InHold=false);
	void ClearAssetList();
	void ClearPageTool();
	void CreateGroupGroupTabRoot(TSharedPtr<FQuVRCatalogNode > node);
private:
	/** Flag to indicate that we need to update the filtered items array */
	bool bNeedsUpdate;
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	
	// Create GridPanel
	TSharedPtr<SBreadcrumbTrail<int32> > BreadcrumbTrail;
	TSharedRef<SWidget> CreateGroupTabManufacturer(const FString& CatalogName);
	FReply HandleBreadcrumbTrailAddButtonClicked();

	//////////////////////////////////////////////////////////////////////////
	TSharedPtr<SHorizontalBox> HTB;
	void AddAssetView();

	TSharedPtr<SQuVRAssetViewWidget> viewWidget;
	
	//////////////////////////////////////////////////////////////////////////

};

/**
* Catalog entry widget, displaying one catalog item (A Model or a Texture)
*/
class SQuVRCatalogEntry : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SQuVRCatalogEntry){ }
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TWeakObjectPtr<class UQuVRCatalogAssetInfo> InItem);

	TSharedPtr<class SQuVRCatlogEntryWidget>CatalogEntryWidget;
	
};

#endif
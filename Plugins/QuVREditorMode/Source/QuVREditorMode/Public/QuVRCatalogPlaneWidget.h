// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Widgets/Input/SButton.h"
#if !UE_BUILD_SHIPPING

class FQuVRCatalogNode;
class SVerticalBox;

class SQuVRCatalogPlaneWidget
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SQuVRCatalogPlaneWidget):
		_Content()
		,_TreeItem()
		,_RootContent()
		,_ParentWidget()
		,_RootWidget()
	{ }

	SLATE_DEFAULT_SLOT(FArguments, Content)
	/** Data for the collection this item represents */
	SLATE_ARGUMENT(TSharedPtr<class FQuVRCatalogNode>, TreeItem)
	
	/** SHOW ParentWidget */
	SLATE_ARGUMENT(TSharedPtr<class SQuVRCatalogPlaneWidget>, ParentWidget)

	/**  RootContent */
	SLATE_ARGUMENT(TSharedPtr<class SHorizontalBox>, RootContent)

	/**  RootWidget */
	SLATE_ARGUMENT(TSharedPtr<class SQuVRCatalogWidget>, RootWidget)

	SLATE_END_ARGS()

	/**
	* Construct the widget
	*
	* @param InArgs   Declaration from which to construct the widget.
	*/
	void Construct(const FArguments& InDelcaration);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;


public:
	void AddGroupTabPlane(TSharedPtr<FQuVRCatalogNode> node);
	void AddGroupTabAssetList(TSharedPtr<FQuVRCatalogNode> node);

private:

	TSharedRef<SWidget> CreateGroupGroupTab(TSharedRef<FQuVRCatalogNode> node);
	void OnCatalogTabChanged(ECheckBoxState NewState, TSharedRef<FQuVRCatalogNode> node);
	ECheckBoxState GetCatalogTabCheckedState(FName CategoryName) const;
	/** Gets the border image for the tab, this is the 'active' orange bar. */
	const FSlateBrush* CatalogGroupBorderImage(FName CategoryName) const;
protected:

	TSharedPtr<class SHorizontalBox> RootContent;
	TSharedPtr<class SQuVRCatalogWidget> RootWidget;
	/*Parent Widget*/
	TSharedPtr<class SQuVRCatalogPlaneWidget> ParentWidget;

	/*Parent Widget*/
	TSharedPtr<class SQuVRCatalogPlaneWidget> ChildWidget;

	/** The data for this item */
	TSharedPtr<class FQuVRCatalogNode> TreeItem;
	// Create PrimaryTab
	TSharedPtr<SVerticalBox> VerticalPlane;

	FName ActiveTabName;
	/** Flag to indicate that we need to update the filtered items array */
	bool bNeedsUpdate;

private:
	void RemoveChildWidget(TSharedRef<SQuVRCatalogPlaneWidget> widget);
};

TSharedRef<SWidget> MakeCatalogPlaneWidget();


#endif // #if !UE_BUILD_SHIPPING

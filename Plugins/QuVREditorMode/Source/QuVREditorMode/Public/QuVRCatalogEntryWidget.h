// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Widgets/Input/SButton.h"
#if !UE_BUILD_SHIPPING

struct FCatalogItem;

class SQuVRCatlogEntryWidget
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SQuVRCatlogEntryWidget) :_Item() { }

	/** Data for the collection this item represents */
	SLATE_ARGUMENT(TSharedPtr<const FCatalogItem>, Item)
	SLATE_END_ARGS()

		/**
		* Construct the widget
		*
		* @param InArgs   Declaration from which to construct the widget.
		*/
	void Construct(const FArguments& InDelcaration);

	// Void DownLoad Asset.zip
	FReply OnDownloadAsset();
	const FSlateBrush* SQuVRCatlogEntryWidget::CatalogGroupBorderImage() const;
	// 
public:
	TSharedPtr<class SButton> button;

	TSharedPtr<const FCatalogItem> Item;
};

TSharedRef<SWidget> MakeCatalogEntryWidget(TSharedPtr<const FCatalogItem> item);


#endif // #if !UE_BUILD_SHIPPING

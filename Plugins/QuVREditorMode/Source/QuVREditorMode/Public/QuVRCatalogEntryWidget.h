// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Widgets/Input/SButton.h"
#if !UE_BUILD_SHIPPING

DECLARE_MULTICAST_DELEGATE(EntryWidgetDone);

struct FCatalogItem;

class SQuVRCatlogEntryWidget
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SQuVRCatlogEntryWidget) :_AssetInfo() { }

	/** Data for the collection this item represents */
	SLATE_ARGUMENT(TSharedPtr<UQuVRcatalogAssetInfo>, AssetInfo)
	SLATE_END_ARGS()

		/**
		* Construct the widget
		*
		* @param InArgs   Declaration from which to construct the widget.
		*/
	void Construct(const FArguments& InDelcaration);

	// Void DownLoad Asset.zip
	FReply OnDownloadAsset();

	void RefreshWidget();
	// 
protected:
	TSharedPtr<class SButton> button;

	TSharedPtr<class UQuVRcatalogAssetInfo> AssetInfo;
	UTexture2DDynamic* Texture2Dimage;
	FSlateBrush* brush;
	FButtonStyle* buttonstyle;
};

TSharedRef<SWidget> MakeCatalogEntryWidget(TSharedPtr<UQuVRcatalogAssetInfo> AssetInfo);


#endif // #if !UE_BUILD_SHIPPING

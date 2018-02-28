// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Widgets/Input/SButton.h"
#if !UE_BUILD_SHIPPING

DECLARE_MULTICAST_DELEGATE(EntryWidgetDone);
struct FPlaceableItem;
struct FCatalogItem;

class SQuVRCatlogEntryWidget
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SQuVRCatlogEntryWidget) :_AssetInfo() { }

	/** Data for the collection this item represents */
	SLATE_ARGUMENT(TWeakObjectPtr<UQuVRCatalogAssetInfo>, AssetInfo)
	SLATE_END_ARGS()

		/**
		* Construct the widget
		*
		* @param InArgs   Declaration from which to construct the widget.
		*/
	void Construct(const FArguments& InDelcaration);

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	// Void DownLoad Asset.zip
	FReply OnDownloadAsset();

	void RefreshWidget();

private:
	const FSlateBrush* GetSlateBrushState() const;
	// 
protected:
	TSharedPtr<class SButton> button;

	TWeakObjectPtr<class UQuVRCatalogAssetInfo> AssetInfo;
	UTexture2DDynamic* Texture2Dimage;
	FButtonStyle* buttonstyle;
	class UQuVRFileDownloader* AsyncTaskDownloadImage;

private:
	bool bIsPressed;
	
	FSlateBrush* NormalImage;
	FSlateBrush* HoverImage;
	FSlateBrush* PressedImage;

	FPlaceableItem* PlaceableItem;
};

TSharedRef<SWidget> MakeCatalogEntryWidget(TWeakObjectPtr<UQuVRCatalogAssetInfo> AssetInfo);


#endif // #if !UE_BUILD_SHIPPING

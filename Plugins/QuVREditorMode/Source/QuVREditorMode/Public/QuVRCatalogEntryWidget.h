// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Widgets/Input/SButton.h"
#if !UE_BUILD_SHIPPING

DECLARE_MULTICAST_DELEGATE(EntryWidgetDone);
struct FPlaceableItem;
struct FCatalogItem;
struct FQuVRAssetViewAsset;

enum class EntryDownLoadState : uint8
{
	Unknown,
	Start,
	InProgress,
	Finish,
};

class SQuVRCatlogEntryWidget
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SQuVRCatlogEntryWidget) :_AssetInfo() { }

	/** Data for the collection this item represents */
	SLATE_ARGUMENT(FQuVRAssetViewAsset, AssetInfo)
		SLATE_ARGUMENT(float, ThumbnailPadding)
		SLATE_ATTRIBUTE(float, ItemWidth)
	SLATE_END_ARGS()

		/**
		* Construct the widget
		*
		* @param InArgs   Declaration from which to construct the widget.
		*/
	virtual ~SQuVRCatlogEntryWidget();
	void Construct(const FArguments& InDelcaration);

	void CheckDownloadAsset();
	// Void DownLoad Asset.zip
	FReply DownloadAsset();

	/** Show UI TEXTURE */
	FQuVRDownloadImageC2Delegate ImageDownloadDone;
	FQuVRDownloadFileC2Delegate FileDownloadDone;
	void RefreshWidget(UTexture2DDynamic* texture2D);

	/** Get the border image to display */
	const FSlateBrush* GetBorderImage() const;

private:
	const FSlateBrush* GetSlateBrushState() const;
	const FSlateBrush* GetSlateBrushTop()const;
	EVisibility GetIsDownloadeVisible() const;
	EVisibility GetIsProgressVisible() const;
	TOptional< float > GetProgressBarState() const;
	void InitPlaceableItem();
	void OnDownloadProegress(int32 ReceivedDataInBytes, int32 TotalDataInBytes, const TArray<uint8>& BinaryData);
	void OnDownloadDone(int32 code);
protected:
	TSharedPtr<class SImage> downloadTopImage;
	TSharedPtr<class SProgressBar> downloadProgressBar;

	EntryDownLoadState DownloadFileState;
	FQuVRAssetViewAsset AssetInfo;
	UTexture2DDynamic* Texture2Dimage;
	
	UTexture2D* UT2DBack;
	UTexture2D* UT2DTop;
	FSlateBrush* fSlbrushTopImage;
	FButtonStyle* buttonstyle;
	TWeakObjectPtr<class UQuVRFileDownloader> AsyncTaskDownloadFile;
	float ProgressRate;
	bool IsDownload;
private:
	bool bIsPressed;
	bool bDraggedOver;

	FSlateBrush* NormalImage;
	FSlateBrush* HoverImage;
	FSlateBrush* PressedImage;
	TSharedPtr<FPlaceableItem> PlaceableItem;

protected:
	/** Returns the size of the thumbnail box widget */
	FOptionalSize GetThumbnailBoxSize() const;
private:
	/** The width of the item. Used to enforce a square thumbnail. */
	TAttribute<float> ItemWidth;

	/** The padding allotted for the thumbnail */
	float ThumbnailPadding;
};

TSharedRef<SWidget> MakeCatalogEntryWidget(FQuVRAssetViewAsset& AssetInfo);


#endif // #if !UE_BUILD_SHIPPING

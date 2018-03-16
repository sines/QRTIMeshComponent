// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Widgets/Input/SButton.h"
#if !UE_BUILD_SHIPPING

DECLARE_MULTICAST_DELEGATE(EntryWidgetDone);
struct FPlaceableItem;
struct FCatalogItem;


struct FQuVRCatalogAssetBase
{
	FString Id;
	/*
	ObjectType =
	SK_ 骨骼模型 0
	SM_  静态模型 1
	M_  材质球 2
	T_   贴图 3
	P_   特效 4
	*/
	int32 ObjectType;
	FString ObjectTypeDesc;
	FString Name;
	FString DisplayName;
	FString Description;
	FString AssetRelativePath;
	FString MainCategoryID;
	FString SubCategoryID;
	FString MainCategory;
	FString SubCategory;
	FString ImageUrl;
	UTexture2DDynamic* Texture2Dimage;
	FString	PackageUrl;
	int32 Size;

	FQuVRCatalogAssetBase()
	{
		Id = "-1";
		ObjectType = -1;
		ObjectTypeDesc = "NULL";
		Name = "-1";
		DisplayName = "NULL";
		Description = "NULL";
		AssetRelativePath = "0";
		MainCategoryID = "0";
		SubCategoryID = "0";
		MainCategory = "0";
		SubCategory = "0";
		ImageUrl = "0";
		PackageUrl = "0";
		Size = 0;
		Texture2Dimage = NULL;
	}

	FQuVRCatalogAssetInfoImageDownloadDone ImageDownloadDone;
};


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
//	virtual ~SQuVRCatlogEntryWidget();
	void Construct(const FArguments& InDelcaration);

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent);

	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent);

	// Void DownLoad Asset.zip
	FReply OnDownloadAsset();

	void RefreshWidget();

private:
	const FSlateBrush* GetSlateBrushState() const;

	FSlateColor GetSlateColorState() const;
	FText GetIsDownloade() const;
	void InitPlaceableItem();

	void DownloadDone(int32 code);
	// 
protected:
	TSharedPtr<class SButton> button;

	TWeakObjectPtr<class UQuVRCatalogAssetInfo> AssetInfo;
	UTexture2DDynamic* Texture2Dimage;
	FButtonStyle* buttonstyle;
	TWeakObjectPtr<class UQuVRFileDownloader> AsyncTaskDownloadFile;

	bool IsDownload;
private:
	bool bIsPressed;
	
	FSlateBrush* NormalImage;
	FSlateBrush* HoverImage;
	FSlateBrush* PressedImage;

	TSharedPtr<FPlaceableItem> PlaceableItem;
};

TSharedRef<SWidget> MakeCatalogEntryWidget(TWeakObjectPtr<class UQuVRCatalogAssetInfo> AssetInfo);


#endif // #if !UE_BUILD_SHIPPING

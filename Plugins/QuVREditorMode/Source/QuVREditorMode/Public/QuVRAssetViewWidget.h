// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

#include "ITypedTableView.h"
#include "Editor/ContentBrowser/Private/AssetViewTypes.h"
#include "Editor/ContentBrowser/Private/SourcesData.h"
#include "Editor/ContentBrowser/Private/AssetViewWidgets.h"
#include "QuVRFileDownloader.h"

/**
 * 
 */
class FAssetData;

struct FQuVRAssetViewAsset : public FAssetViewItem
{
	int32 ObjectType;
	FString	PackageUrl;
	FString	ImageUrl;
	FString DisplayName;
	bool IsAssetExists;
	/** The asset registry data associated with this item */
	FAssetData Data;
	/** Map of values for custom columns */
	TMap<FName, FString> CustomColumnData;
	/** Show UI TEXTURE */
	UTexture2DDynamic* UITexture;

	TSharedPtr<class SQuVRCatlogEntryWidget> EntryWidget= nullptr;

public:
	~FQuVRAssetViewAsset()
	{
		EntryWidget = nullptr;
		UITexture = nullptr;
	}

	FQuVRAssetViewAsset(FQuVRAssetViewAsset& AssetData)
	{
		Data = AssetData.Data;
		ObjectType = AssetData.ObjectType;
		PackageUrl = AssetData.PackageUrl;
		ImageUrl = AssetData.ImageUrl;
		DisplayName = AssetData.DisplayName;
		CustomColumnData = AssetData.CustomColumnData;
		UITexture = AssetData.UITexture;
		EntryWidget = AssetData.EntryWidget;
		IsAssetExists = AssetData.IsAssetExists;
	}

	explicit FQuVRAssetViewAsset() :ObjectType(-1), 
									PackageUrl(TEXT("0")),
									ImageUrl(TEXT("0")),
									DisplayName(TEXT("0")), 
									Data(FAssetData()), 
									UITexture(nullptr),
									IsAssetExists(false),
									EntryWidget(nullptr)
	{
	};

	explicit FQuVRAssetViewAsset(const FAssetData& InAssetData, int32 InObjtype=-1, FString InPackageURL = FString(TEXT("0")), FString InImageURL = FString(TEXT("0")), FString InName=FString(TEXT("0")), UTexture2DDynamic* InTexture = nullptr)
		:ObjectType(InObjtype),
		PackageUrl(InPackageURL),
		ImageUrl(InImageURL),
		Data(InAssetData),
		DisplayName(InName),
		UITexture(InTexture), 
		IsAssetExists(false),
		EntryWidget(nullptr)
	{
	}

	void CreateEntryWidget();
	void RefreshImage(UTexture2DDynamic* texture2D);
	void SetAssetData(const FAssetData& NewData);

	bool GetTagValue(FName Tag, FString& OutString) const;

	// FAssetViewItem interface
	virtual EAssetItemType::Type GetType() const override;

	virtual bool IsTemporaryItem() const override;

};

/** The tile view mode of the asset view */
class SQuVRAssetTileView : public STileView<TSharedPtr<FQuVRAssetViewAsset>>
{
public:
	virtual bool SupportsKeyboardFocus() const override { return true; }
	virtual FReply OnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
};


class QUVREDITORMODE_API SQuVRAssetViewWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SQuVRAssetViewWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	~SQuVRAssetViewWidget();

	// SWidget inherited
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	void UpdateThumbnails();

	/** Creates a new tile view */
	TSharedRef<class SQuVRAssetTileView> CreateTileView();
	
	TSharedRef<SWidget> CreateShadowOverlay(TSharedRef<STableViewBase> Table);
	/** Clears the reference to the current view and creates a new one, based on CurrentViewType */
	void CreateCurrentView(	/** Creates a new tile view */TSharedPtr<SBorder>& inViewContainer);

	/** Handler for tile view widget creation */
	TSharedRef<ITableRow> MakeTileViewWidget(TSharedPtr<FQuVRAssetViewAsset> AssetItem, const TSharedRef<STableViewBase>& OwnerTable);

	/** Handler for when an item has scrolled into view after having been requested to do so */
	void ItemScrolledIntoView(TSharedPtr<FQuVRAssetViewAsset> AssetItem, const TSharedPtr<ITableRow>& Widget);

	/** Handler for double clicking an item */
	void OnListMouseButtonDoubleClick(TSharedPtr<FQuVRAssetViewAsset> AssetItem);

	/** Handler for double clicking an item */
	void OnListMouseButtonClick(TSharedPtr<FQuVRAssetViewAsset> AssetItem);

	/** Handler for tree view selection changes */
	void AssetSelectionChanged(TSharedPtr<FQuVRAssetViewAsset > AssetItem, ESelectInfo::Type SelectInfo);

	/** Regenerates the FilteredAssetItems list from the AssetItems list */
	void ClearSourceItems();
	void ClearPage();
	void RefreshSourceItems(const TSharedRef<class FQuVRCatalogNode> node, bool InHold=false);
	void RefreshList();
	void UpdataAssetData(FString name);
	/** Handler for when an asset was created or added to the asset registry */
	void OnAssetAdded(const FAssetData& AssetData);
	/** @return The state of the is working progress bar */
	TOptional< float > GetIsWorkingProgressBarState() const;

	/** Returns all the folders currently selected in the view */
	TArray<FString> GetSelectedFolders() const;

	/** Returns all the asset data objects in items currently selected in the view */
	TArray<FAssetData> GetSelectedAssets() const;

	/** Returns all the items currently selected in the view */
	TArray<TSharedPtr<FQuVRAssetViewAsset>> GetSelectedItems() const;

	// 
	/** Handler for context menus */
	TSharedPtr<SWidget> OnGetContextMenuContent();

	/** Handle dragging an asset */
	FReply OnDraggingAssetItem(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	void FindActorFactoryByType(const int32 ObjectType);
	UActorFactory* CurrentFactory;
	/** Whether the asset view is currently working on something and should display a cue to the user */
	bool bIsWorking;

	/** Pool for maintaining and rendering thumbnails */
	TSharedPtr<class FAssetThumbnailPool> AssetThumbnailPool;

	/** A map of FAssetViewAsset to the thumbnail that represents it. Only items that are currently visible or within half of the FilteredAssetItems array index distance described by NumOffscreenThumbnails are in this list */
	TMap< TSharedPtr<FAssetViewAsset>, TSharedPtr<class FAssetThumbnail> > RelevantThumbnails;

	/** The current selection mode used by the asset view */
	TArray<FAssetData> AssetItems;
	TArray<TSharedPtr<FQuVRAssetViewAsset>> FilteredAssetItems;
	ESelectionMode::Type SelectionMode;
	
	/** The current base source filter for the view */
	FSourcesData SourcesData;

	/** Whether to allow dragging of items */
	bool bAllowDragging;
	bool bRefreshSource;

	int32 TileViewThumbnailResolution;
	int32 TileViewThumbnailSize;
	int32 TileViewThumbnailPadding;
	int32 TileViewNameHeight;
	float FillScale;
	
	/** The max and min thumbnail scales as a fraction of the rendered size */
	float MinThumbnailScale;
	float MaxThumbnailScale;

	TSharedPtr<SBorder> ViewContainer;
	/** The asset view widget */
	TSharedPtr<SQuVRAssetTileView> TileView;
	/** Calculates a new filler scale used to adjust the thumbnails to fill empty space. */
	void CalculateFillScale(const FGeometry& AllottedGeometry);
	float GetTileViewItemBaseWidth() const;
	float GetTileViewItemHeight() const;
	float GetTileViewItemBaseHeight() const;
	float GetTileViewItemWidth() const;

	/** Handler for when any asset item widget gets destroyed */
	void AssetItemWidgetDestroyed(const TSharedPtr<FQuVRAssetViewAsset>& Item);

	/** Handler for when an asset was deleted or removed from the asset registry */
	void OnAssetRemoved(const FAssetData& AssetData);

	/** Event for when assets are deleted */
	void OnInMemoryAssetDeleted(UObject *Object);

	/** The text to highlight on the assets */
	TAttribute< FText > HighlightedText;

	/** Called to get a custom asset item tooltip (If necessary) */
	FOnGetCustomAssetToolTip OnGetCustomAssetToolTip;

	/** Called when a custom asset item is about to show a tooltip */
	FOnVisualizeAssetToolTip OnVisualizeAssetToolTip;

	/** Called when a custom asset item's tooltip is closing */
	FOnAssetToolTipClosing OnAssetToolTipClosing;

	/*Current Node*/
	TSharedPtr<FQuVRCatalogNode>	CurrentNode;

	TSharedPtr<class SQuVRCatalogPageNavi> CatalogPageNavi;
	
};

// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Widgets/Input/SButton.h"
#if !UE_BUILD_SHIPPING

/*class FQuVRCatalogNode;*/


class SQuVRCatalogNodeButton
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SQuVRCatalogNodeButton)
		: _TreeItem()
		,_Content()
		, _IsChecked(ECheckBoxState::Unchecked)
		,_OnCheckStateChanged()
		,_BkImage(FCoreStyle::Get().GetDefaultBrush())
		,_ParentWidget()
	{ }

	/** Data for the collection this item represents */
	SLATE_ARGUMENT(TSharedPtr<class FQuVRCatalogNode>, TreeItem)

	/** Data for the collection this item represents */
	SLATE_ARGUMENT(TSharedPtr<SCheckBox>, SectionScheckBox)
	
	/** SHOW IMAGE */
	SLATE_ATTRIBUTE(const FSlateBrush*, BkImage)

	/** SHOW ParentWidget */
	SLATE_ARGUMENT(TSharedPtr<class SQuVRCatalogWidget>, ParentWidget)

	/** Called when the checked state has changed */
	SLATE_EVENT(FOnCheckStateChanged, OnCheckStateChanged)

	/** Whether the check box is currently in a checked state */
	SLATE_ATTRIBUTE(ECheckBoxState, IsChecked)

	/** Content to be placed next to the check box, or for a toggle button, the content to be placed inside the button */
	SLATE_DEFAULT_SLOT(FArguments, Content)
	
	SLATE_END_ARGS()

	void Construct(const FArguments& InDelcaration);

	FReply OnSectionButtonChanged();

protected:

void OnSectionButtonChanged(ECheckBoxState NewState);
/** The list of active system messages */
TSharedPtr<SNotificationList> NotificationListPtr;

/** Delegate called when the check box changes state */
FOnCheckStateChanged OnCheckStateChanged;

/** Are we checked */
TAttribute<ECheckBoxState> IsSectionButtonChecked;

/** The FName of the image resource to show */
TAttribute< const FSlateBrush* > BkImage;
/** The data for this item */
TSharedPtr<class FQuVRCatalogNode> TreeItem;

/*Parent Widget*/
TSharedPtr<class SQuVRCatalogWidget> ParentWidget;
};

TSharedRef<SWidget> MakeCatalogSectionButton(const TSharedRef<class FQuVRCatalogNode> node);


#endif // #if !UE_BUILD_SHIPPING

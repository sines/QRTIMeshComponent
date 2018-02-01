// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Widgets/Input/SButton.h"
#if !UE_BUILD_SHIPPING

extern struct FQuVRCatalogNode;

/** Delegate that is executed when the check box state changes */
DECLARE_DELEGATE_OneParam(FOnCheckStateChanged, ECheckBoxState);

class SQuVRCatalogSectionButton
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SQuVRCatalogSectionButton)
		: _TreeItem()
		,_Content()
		,_OnCheckStateChanged()
		,_IsChecked(ECheckBoxState::Unchecked)
		,_SectionScheckBox(SNew(SCheckBox))
	{ }

	/** Data for the collection this item represents */
	SLATE_ARGUMENT(FQuVRCatalogNode, TreeItem)

	/** Data for the collection this item represents */
	SLATE_ARGUMENT(TSharedPtr<SCheckBox>, SectionScheckBox)

	/** Called when the checked state has changed */
	SLATE_EVENT(FOnCheckStateChanged, OnCheckStateChanged)
	
	/** Whether the check box is currently in a checked state */
	SLATE_ATTRIBUTE(ECheckBoxState, IsChecked)
	
	/** Content to be placed next to the check box, or for a toggle button, the content to be placed inside the button */
	SLATE_DEFAULT_SLOT(FArguments, Content)
	
	SLATE_END_ARGS()

	void Construct(const FArguments& InDelcaration);

	void OnSectionButtonChanged(ECheckBoxState NewState, FName CategoryName);	

protected:
/** Delegate called when the check box changes state */
FOnCheckStateChanged OnCheckStateChanged;

/** Are we checked */
TAttribute<ECheckBoxState> IsCheckboxChecked;

TSharedPtr<SCheckBox> SectionScheckBox;
private:
/** The data for this item */
FQuVRCatalogNode TreeItem;

};

TSharedRef<SWidget> MakeCatalogSectionButton(const FQuVRCatalogNode& node);


#endif // #if !UE_BUILD_SHIPPING

// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "QuVRCatalogSectionButton.h"
#include "Widgets/SWidget.h"
#include "Widgets/SUserWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Runtime/Slate/Public/SlateOptMacros.h"
#include "Core.h"
#include "SlateBasics.h"

#if !UE_BUILD_SHIPPING

#define LOCTEXT_NAMESPACE "SQuVRCatalogSectionButton"


void SQuVRCatalogSectionButton::Construct(const SQuVRCatalogSectionButton::FArguments& InDelcaration)
{

	TreeItem = InDelcaration._TreeItem;
	OnCheckStateChanged = InDelcaration._OnCheckStateChanged;
	IsCheckboxChecked = InDelcaration._IsChecked;
	SectionScheckBox = InDelcaration._SectionScheckBox;
	if (SectionScheckBox.IsValid())
	{
	// Check boxes use a separate check button to the side of the user's content (often, a text label or icon.)
		this->ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SectionScheckBox.ToSharedRef()
			]

		+ SOverlay::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				InDelcaration._Content.Widget
			]
		];
	}
};

void SQuVRCatalogSectionButton::OnSectionButtonChanged(ECheckBoxState NewState, FName CategoryName)
{
	const ECheckBoxState State = NewState;

	// If the current check box state is checked OR undetermined we set the check box to checked.
	if (State == ECheckBoxState::Checked || State == ECheckBoxState::Undetermined)
	{
		if (!IsCheckboxChecked.IsBound())
		{
			// When we are not bound, just toggle the current state.
			IsCheckboxChecked.Set(ECheckBoxState::Unchecked);
		}

		// The state of the check box changed.  Execute the delegate to notify users
		OnCheckStateChanged.ExecuteIfBound(ECheckBoxState::Unchecked);
	}
	else if (State == ECheckBoxState::Unchecked)
	{
		if (!IsCheckboxChecked.IsBound())
		{
			// When we are not bound, just toggle the current state.
			IsCheckboxChecked.Set(ECheckBoxState::Checked);
		}

		// The state of the check box changed.  Execute the delegate to notify users
		OnCheckStateChanged.ExecuteIfBound(ECheckBoxState::Checked);
	}
}


TSharedRef<SWidget> MakeCatalogSectionButton(const FQuVRCatalogNode& node)
{
	return SNew(SQuVRCatalogSectionButton).TreeItem(node);}

#undef LOCTEXT_NAMESPACE

#endif // #if !UE_BUILD_SHIPPING

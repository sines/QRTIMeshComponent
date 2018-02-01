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


void SQuVRCatalogSectionButton::Construct(const FArguments& InDelcaration)
{		

	ChildSlot
		[
			SNew(SButton).HAlign(HAlign_Fill)

			.VAlign(VAlign_Fill)
		];
};




TSharedRef<SWidget> MakeCatalogSectionButton()
{
	return SNew(SQuVRCatalogSectionButton);}

#undef LOCTEXT_NAMESPACE

#endif // #if !UE_BUILD_SHIPPING
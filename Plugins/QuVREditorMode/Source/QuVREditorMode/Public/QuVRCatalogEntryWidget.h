// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Widgets/Input/SButton.h"
#if !UE_BUILD_SHIPPING


class SQuVRCatlogEntryWidget
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SQuVRCatlogEntryWidget){ }
	SLATE_END_ARGS()

		/**
		* Construct the widget
		*
		* @param InArgs   Declaration from which to construct the widget.
		*/
	void Construct(const FArguments& InDelcaration);
};

TSharedRef<SWidget> MakeCatalogEntryWidget();


#endif // #if !UE_BUILD_SHIPPING

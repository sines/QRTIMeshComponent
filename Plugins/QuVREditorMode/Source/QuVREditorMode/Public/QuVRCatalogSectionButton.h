// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Widgets/Input/SButton.h"
#if !UE_BUILD_SHIPPING


class SQuVRCatalogSectionButton
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SQuVRCatalogSectionButton){ }
	SLATE_END_ARGS()

		/**
		* Construct the widget
		*
		* @param InArgs   Declaration from which to construct the widget.
		*/
	void Construct(const FArguments& InDelcaration);
};

TSharedRef<SWidget> MakeCatalogSectionButton();


#endif // #if !UE_BUILD_SHIPPING

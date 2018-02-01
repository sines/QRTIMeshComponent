// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Widgets/Input/SButton.h"
#if !UE_BUILD_SHIPPING


class SQuVRCatalogAssetButton
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SQuVRCatalogAssetButton){ }
	SLATE_END_ARGS()

		/**
		* Construct the widget
		*
		* @param InArgs   Declaration from which to construct the widget.
		*/
	void Construct(const FArguments& InDelcaration);
};

TSharedRef<SWidget> MakeCatalogAssetButton();


#endif // #if !UE_BUILD_SHIPPING

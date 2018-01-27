// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "QuVRCatalogBtDownloader.h"
#include "Widgets/SWidget.h"
#include "Widgets/SUserWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Runtime/Slate/Public/SlateOptMacros.h"
#include "Core.h"
#include "SlateBasics.h"

#if !UE_BUILD_SHIPPING

#define LOCTEXT_NAMESPACE "CatlogBtDownloader"


void SQuVRCatlogBtDownloader::Construct(const FArguments& InDelcaration)
{
		const FVector2D HeadingShadowOffset(2, 2);

		FSlateFontInfo LargeLayoutFont(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 12);
		FSlateFontInfo SmallLayoutFont(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 10);
		this->ChildSlot
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBox)
				[
					SNew(SButton).Text(LOCTEXT("CatlogResGategory-All", "≤‚ ‘"))
				]
			];
};




TSharedRef<SWidget> MakeCatalogBtDownload()
{
	return SNew(SQuVRCatlogBtDownloader);}

#undef LOCTEXT_NAMESPACE

#endif // #if !UE_BUILD_SHIPPING

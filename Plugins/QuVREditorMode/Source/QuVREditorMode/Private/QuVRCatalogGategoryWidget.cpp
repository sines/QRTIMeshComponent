// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "QuVRCatalogGategoryWidget.h"
#include "Widgets/SWidget.h"
#include "Widgets/SUserWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Runtime/Slate/Public/SlateOptMacros.h"
#include "Core.h"
#include "SlateBasics.h"

#if !UE_BUILD_SHIPPING

#define LOCTEXT_NAMESPACE "CatlogResGategory"


class QuVRCatalogGategoryWidget
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(QuVRCatalogGategoryWidget)
	{ }
	SLATE_END_ARGS();
	
	/**
	 * Construct the widget
	 *
	 * @param InArgs   Declaration from which to construct the widget.
	 */
	BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
	void Construct(const FArguments& InDelcaration)
	{
		const FVector2D HeadingShadowOffset(2,2);

		FSlateFontInfo LargeLayoutFont( FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 12 );
		FSlateFontInfo SmallLayoutFont( FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 10 );
		this->ChildSlot
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SScrollBox).Orientation(Orient_Horizontal)
 			+ SScrollBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
 			[
 				SNew(SSpacer)
 				.Size(FVector2D(12, 64))
 			]
			+ SScrollBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)
				// Default settings example
				+ SVerticalBox::Slot() .Padding(2)
				[
					SNew(STextBlock) .ShadowOffset(HeadingShadowOffset) .Font( LargeLayoutFont ) .Text( LOCTEXT("CatlogResGategory-DefaultSettingsLabel", "Catalog Type:") )
				]
				+ SVerticalBox::Slot() .Padding(2,2)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.Padding(1)
					.AutoWidth()
					[
						SNew(SBox).HAlign(HAlign_Fill).VAlign(VAlign_Fill).HeightOverride(16)
						[
							SNew(SButton).Text(LOCTEXT("CatlogResGategory-All", "All-Asset")).VAlign(VAlign_Fill).HAlign(HAlign_Fill)
							.OnClicked(this, &QuVRCatalogGategoryWidget::RefreshAllButtonClicked)
						]

					]
					
					// Add test button
					+SHorizontalBox::Slot()
					.Padding(2)
					.AutoWidth()
					[
						SNew(SButton).Text( LOCTEXT("CatlogResGategory-Jtl4.0", "JTL4.0") )
						.OnClicked(this, &QuVRCatalogGategoryWidget::TestButtonClicked)
					]
/*

					+SHorizontalBox::Slot()
					.Padding(2)
					.AutoWidth()
					[
						SNew(SButton) .Text( LOCTEXT("CatlogResGategory-HD", "HDPJ") )
						.OnClicked(this,&QuVRCatalogGategoryWidget::TestButtonClicked)
					]
				*/
				]
			]
		];
	}

	END_SLATE_FUNCTION_BUILD_OPTIMIZATION
//  Attribute
	FReply RefreshAllButtonClicked()
	{
		UQuVRCatalogDataManager::GetInstance()->GetAllCatalogNodeListFromUrl();
		return FReply::Handled();
	}

	FReply TestButtonClicked()
	{
		FString CurrentPath = FPaths::GameSavedDir();
		FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
		AssetToolsModule.Get().ImportAssets(CurrentPath);
		return FReply::Handled();
	}
	//////////////////////////////////////////////////////////////////////////
	FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
	{
		return FReply::Handled();
	}
};




TSharedRef<SWidget> MakeGategoryWidget()
{
	return
		SNew(QuVRCatalogGategoryWidget);
}

#undef LOCTEXT_NAMESPACE

#endif // #if !UE_BUILD_SHIPPING

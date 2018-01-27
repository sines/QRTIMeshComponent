// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "QuVRCatalogResGategory.h"
#include "Widgets/SWidget.h"
#include "Widgets/SUserWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Runtime/Slate/Public/SlateOptMacros.h"
#include "Core.h"
#include "SlateBasics.h"

#if !UE_BUILD_SHIPPING

#define LOCTEXT_NAMESPACE "CatlogResGategory"


class SQuVRCatlogResGategory
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SQuVRCatlogResGategory)
	{ }
	SLATE_END_ARGS()
	
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
			]
			+ SScrollBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)
				// Default settings example
				+ SVerticalBox::Slot() .Padding(5)
				[
					SNew(STextBlock) .ShadowOffset(HeadingShadowOffset) .Font( LargeLayoutFont ) .Text( LOCTEXT("CatlogResGategory-DefaultSettingsLabel", "分类选择:") )
				]
				+ SVerticalBox::Slot() .Padding(5,5)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SButton) .Text( LOCTEXT("CatlogResGategory-All", "全部") )
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SButton) .Text( LOCTEXT("CatlogResGategory-Jtl4.0", "金螳螂家4.0") )
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SButton) .Text( LOCTEXT("CatlogResGategory-HD", "恒大御湖山庄") )
					]
			
				]
			]
		];
	}

	END_SLATE_FUNCTION_BUILD_OPTIMIZATION
	

//  Attribute

};




TSharedRef<SWidget> MakeResGategory()
{
// 	extern TOptional<FSlateRenderTransform> GetTestRenderTransform();
// 	extern FVector2D GetTestRenderTransformPivot();
	return
		SNew(SQuVRCatlogResGategory);
//		.RenderTransform_Static(&GetTestRenderTransform)
//		.RenderTransformPivot_Static(&GetTestRenderTransformPivot);
}

#undef LOCTEXT_NAMESPACE

#endif // #if !UE_BUILD_SHIPPING

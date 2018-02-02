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
				.Size(FVector2D(2, 2))
			]
			+ SScrollBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)
				// Default settings example
				+ SVerticalBox::Slot() .Padding(5)
				[
					SNew(STextBlock) .ShadowOffset(HeadingShadowOffset) .Font( LargeLayoutFont ) .Text( LOCTEXT("CatlogResGategory-DefaultSettingsLabel", "Catalog Type:") )
				]
				+ SVerticalBox::Slot() .Padding(5,5)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.Padding(2)
					.AutoWidth()
					[
						SNew(SButton).Text(LOCTEXT("CatlogResGategory-All", "All"))
						.OnClicked(this,&QuVRCatalogGategoryWidget::RefreshAllButtonClicked)
					]
					+SHorizontalBox::Slot()
					.Padding(2)
					.AutoWidth()
					[
						SNew(SButton).Text( LOCTEXT("CatlogResGategory-Jtl4.0", "JTL4.0") )
					]
					+SHorizontalBox::Slot()
					.Padding(2)
					.AutoWidth()
					[
						SNew(SButton) .Text( LOCTEXT("CatlogResGategory-HD", "HDPJ") )
					]
			
				]
			]
		];
	}

	END_SLATE_FUNCTION_BUILD_OPTIMIZATION
//  Attribute
	FReply RefreshAllButtonClicked()
	{
	
		UQuVRAssetDownNet::GetInstance()->GetAllTypeDataFromUrl();
		
//		UQuVRAssetDownNet::GetInstance()->GetNodeList();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString("RefreshAllData"));
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
// 	extern TOptional<FSlateRenderTransform> GetTestRenderTransform();
// 	extern FVector2D GetTestRenderTransformPivot();
	return
		SNew(QuVRCatalogGategoryWidget);
//		.RenderTransform_Static(&GetTestRenderTransform)
//		.RenderTransformPivot_Static(&GetTestRenderTransformPivot);
}

#undef LOCTEXT_NAMESPACE

#endif // #if !UE_BUILD_SHIPPING

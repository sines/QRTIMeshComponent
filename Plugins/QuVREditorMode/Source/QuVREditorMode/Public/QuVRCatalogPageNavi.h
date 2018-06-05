// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SSpinBox.h"
#if !UE_BUILD_SHIPPING

/*class FQuVRCatalogNode;*/

DECLARE_MULTICAST_DELEGATE(FQuVRPageNaviC2Delegate);

class SQuVRCatalogPageNavi
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SQuVRCatalogPageNavi)
		:_Content()
	{ }

	/** Content to be placed next to the check box, or for a toggle button, the content to be placed inside the button */
	SLATE_DEFAULT_SLOT(FArguments, Content)
	
	SLATE_END_ARGS()

	void Construct(const FArguments& InDelcaration);

	FReply OnSectionButtonChanged();
	void CalcPageIndex(TSharedPtr<FQuVRCatalogNode> node);
	void ClearPage();
	void ActivationButton(bool IsEnable);
private:
	/** Delegate to get labels root text from settings */
	FText GetPageTotalPageText() const;
	FText GetPageCurrentPageText() const;
	TOptional<int32>  GetPageTotalPageInt32() const;
	void OnWriteValue(int32 NewValue);
	FReply OnPreButtonClicked();
	FReply OnNextButtonClicked();
	FReply OnJumpButtonClicked();
protected:
	/** The data for this item */
	TSharedPtr<FQuVRCatalogNode> CurrentNode;
	TSharedPtr<SButton> PreButton;
	TSharedPtr<SButton> NextButton;
	TSharedPtr<SButton> JumpButton;
	TSharedPtr<SSpinBox<int32>> JumpSpinBox;
	int32 CurrentIndex;
	int32 Total;
	int32 JumpIndex;

public:
	FQuVRPageNaviC2Delegate OnPreButtonClickedCall;
	FQuVRPageNaviC2Delegate OnNextButtonClickedCall;
};

TSharedRef<SWidget> MakeCatalogPageNavi();


#endif // #if !UE_BUILD_SHIPPING

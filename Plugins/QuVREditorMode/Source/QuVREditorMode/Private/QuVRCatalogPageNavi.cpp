// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "QuVRCatalogPageNavi.h"
#include "Widgets/SWidget.h"
#include "Widgets/SUserWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Runtime/Slate/Public/SlateOptMacros.h"
#include "Core.h"
#include "SlateBasics.h"
#include "SNotificationList.h"
#include "QuVRCatalogDataManager.h"


#if !UE_BUILD_SHIPPING

#define LOCTEXT_NAMESPACE "SQuVRCatalogPageNavi"
#define PAGE_LIST_NUM 10
void SQuVRCatalogPageNavi::Construct(const SQuVRCatalogPageNavi::FArguments& InDelcaration)
{
	ClearPage();
	PreButton = nullptr;
	NextButton = nullptr;
	JumpButton = nullptr;
	JumpSpinBox = nullptr;
// Create ChildSlot
this->ChildSlot
	[
		SNew(SBorder)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
			[
				SNew(SSpacer)
			]
			+ SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Fill).VAlign(VAlign_Center)
			[
				SAssignNew(PreButton,SButton).Text(LOCTEXT("SQuVRCatalogPageNavi:PrePage", "PrePage")).OnClicked(this, &SQuVRCatalogPageNavi::OnPreButtonClicked)
			]
			+ SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Fill).VAlign(VAlign_Center)
			[
				SNew(SEditableTextBox).IsReadOnly(true).Text(this, &SQuVRCatalogPageNavi::GetPageCurrentPageText)
			]
			+ SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Fill).VAlign(VAlign_Center)
			[
				SNew(STextBlock).Text(LOCTEXT("SQuVRCatalogPageNavi:Separator", "/"))
			]
			+ SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Fill).VAlign(VAlign_Center)
			[
				SNew(SEditableTextBox).IsReadOnly(true)
				.Text(this, &SQuVRCatalogPageNavi::GetPageTotalPageText)
			]
			+ SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Fill).VAlign(VAlign_Center)
			[
				SAssignNew(NextButton, SButton).Text(LOCTEXT("SQuVRCatalogPageNavi:NextPage", "NextPage")).OnClicked(this, &SQuVRCatalogPageNavi::OnNextButtonClicked)
			]
			+ SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
				[
					SNew(SSpacer).Size(FVector2D(20.0f,2.0f))
				]
			+ SHorizontalBox::Slot().FillWidth(0.1f).HAlign(HAlign_Fill).VAlign(VAlign_Fill)
			[
				SAssignNew(JumpSpinBox,SSpinBox<int32>)
					.MinValue(1)
					.MaxValue_Raw(this, &SQuVRCatalogPageNavi::GetPageTotalPageInt32)
					.OnValueChanged(this, &SQuVRCatalogPageNavi::OnWriteValue)
			]
			+ SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Fill).VAlign(VAlign_Center)
				[
					SAssignNew(JumpButton, SButton).Text(LOCTEXT("SQuVRCatalogPageNavi:Jump", "Jump")).OnClicked(this, &SQuVRCatalogPageNavi::OnJumpButtonClicked)
				]

			+ SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
			[
				SNew(SSpacer).Size(FVector2D(20.0f, 2.0f))
			]
		]
	];
};

TOptional<int32> SQuVRCatalogPageNavi::GetPageTotalPageInt32() const
{
	return Total;
}

FText SQuVRCatalogPageNavi::GetPageTotalPageText() const
{
	return FText::FromString(FString::FromInt(Total));
}

FText SQuVRCatalogPageNavi::GetPageCurrentPageText() const
{
	return FText::FromString(FString::FromInt(CurrentIndex));
}

FReply SQuVRCatalogPageNavi::OnSectionButtonChanged()
{
		// The state of the check box changed.  Execute the delegate to notify users
	return FReply::Handled();
}

FReply SQuVRCatalogPageNavi::OnPreButtonClicked()
{
	if (CurrentIndex == 0 && Total ==0)
	{
		return FReply::Handled();
	}

	int tempIndex = CurrentIndex;
	int index = CurrentIndex - 1;
	CurrentIndex = FMath::Clamp<int32>(index, 1, Total+1);
	if (OnPreButtonClickedCall.IsBound())
	{
		OnPreButtonClickedCall.Broadcast();
	}
	if (CurrentNode.IsValid() && tempIndex != CurrentIndex)
	{
		UQuVRCatalogDataManager::GetInstance()->HoldCatalogNodeAssetFromUrl(CurrentNode, (CurrentIndex - 1)*PAGE_LIST_NUM);
		ActivationButton(false);
	}
	return FReply::Handled();
}

void SQuVRCatalogPageNavi::ClearPage()
{
	CurrentNode = nullptr;
	CurrentIndex = 0;
	Total = 0;
	JumpIndex = 0;
	if (JumpSpinBox.IsValid())
	{
		JumpSpinBox->SetValue(1.0);
	}
}
void SQuVRCatalogPageNavi::OnWriteValue(int32 NewValue)
{
	JumpIndex = NewValue;
}
void SQuVRCatalogPageNavi::CalcPageIndex(TSharedPtr<FQuVRCatalogNode> node)
{
	if (node.IsValid())
	{
		CurrentNode = node;
		if (node->NodeData.ChildNum > 0)
		{
			float num = node->NodeData.ChildNum;
			CurrentIndex = 1;
			float nt = num / PAGE_LIST_NUM;
			Total = FPlatformMath::CeilToInt(nt);
		}
	}

}
void SQuVRCatalogPageNavi::ActivationButton(bool IsEnable)
{
	if (PreButton.IsValid())
	{
		PreButton->SetEnabled(IsEnable);
	}
	if (NextButton.IsValid())
	{
		NextButton->SetEnabled(IsEnable);
	}
	if (JumpButton.IsValid())
	{
		JumpButton->SetEnabled(IsEnable);
	}
}

FReply SQuVRCatalogPageNavi::OnNextButtonClicked()
{
	if (CurrentIndex == 0 && Total == 0)
	{
		return FReply::Handled();
	}

	int tempIndex = CurrentIndex;
	int index = CurrentIndex + 1;
	CurrentIndex = FMath::Clamp<int32>(index, 1, Total);
	if (OnNextButtonClickedCall.IsBound())
	{
		OnNextButtonClickedCall.Broadcast();

	}
	if (CurrentNode.IsValid() && tempIndex != CurrentIndex)
	{
		UQuVRCatalogDataManager::GetInstance()->HoldCatalogNodeAssetFromUrl(CurrentNode, (CurrentIndex - 1)*PAGE_LIST_NUM);
		ActivationButton(false);
	}
	return FReply::Handled();
}

FReply SQuVRCatalogPageNavi::OnJumpButtonClicked()
{
	if (Total>0)
	{
		if (FMath::IsWithinInclusive(JumpIndex, 1, Total))
		{
			if (CurrentNode.IsValid() && JumpIndex != CurrentIndex)
			{
				CurrentIndex = JumpIndex;
				UQuVRCatalogDataManager::GetInstance()->HoldCatalogNodeAssetFromUrl(CurrentNode, (JumpIndex-1)*PAGE_LIST_NUM);
				ActivationButton(false);
			}
		}
	}

	return FReply::Handled();
}

TSharedRef<SWidget> MakeCatalogPageNavi()
{
	return SNew(SQuVRCatalogPageNavi);}

#undef LOCTEXT_NAMESPACE

#endif // #if !UE_BUILD_SHIPPING

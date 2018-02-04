// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "QuVRCatalogPlaneWidget.h"
#include "Widgets/SWidget.h"
#include "Widgets/SUserWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Runtime/Slate/Public/SlateOptMacros.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "Core.h"
#include "SlateBasics.h"

#include "QuVRAssetDownNet.h"

#if !UE_BUILD_SHIPPING

#define LOCTEXT_NAMESPACE "SQuVRCatalogPlaneWidget"


void SQuVRCatalogPlaneWidget::Construct(const FArguments& InDelcaration)
{		
	TreeItem = InDelcaration._TreeItem;
	ParentWidget = InDelcaration._ParentWidget;
	RootContent= InDelcaration._RootContent;
	RootWidget = InDelcaration._RootWidget;

	SAssignNew(VerticalPlane, SVerticalBox);
	for (auto node : TreeItem->ChildList)
	{
		VerticalPlane->AddSlot().AutoHeight()
		[
			CreateGroupGroupTab(node.ToSharedRef())
		];
	}
	//////////////////////////////////////////////////////////////////////////
	ChildSlot
		[
			SNew(SScrollBox).ScrollBarThickness(FVector2D(1, 1))
			+ SScrollBox::Slot()
			.Padding(0)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot().AutoHeight()
				[
					VerticalPlane.ToSharedRef()
				]
			]
			+ SScrollBox::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				InDelcaration._Content.Widget
			]
		];
};

TSharedRef<SWidget> SQuVRCatalogPlaneWidget::CreateGroupGroupTab(TSharedRef<FQuVRCatalogNode> node)
{
	return	SNew(SQuVRCatalogNodeButton)
		.TreeItem(node)
		.OnCheckStateChanged(this, &SQuVRCatalogPlaneWidget::OnCatalogTabChanged, node)
		.IsChecked(this, &SQuVRCatalogPlaneWidget::GetCatalogTabCheckedState, FName(*node->NodeData.DisplayName))
		.BkImage(this, &SQuVRCatalogPlaneWidget::CatalogGroupBorderImage, FName(*node->NodeData.DisplayName))
		.ParentWidget(SharedThis(this));
}

void SQuVRCatalogPlaneWidget::OnCatalogTabChanged(ECheckBoxState NewState, TSharedRef<FQuVRCatalogNode> node)
{
	if (NewState == ECheckBoxState::Checked)
	{
		ActiveTabName = FName(*node->NodeData.DisplayName);
		bNeedsUpdate = true;
	}
}
void SQuVRCatalogPlaneWidget::RemoveChildWidget(TSharedRef<SQuVRCatalogPlaneWidget> widget)
{

	if (widget->ChildWidget.IsValid())
	{
		RemoveChildWidget(widget->ChildWidget.ToSharedRef());
	}
	RootContent->RemoveSlot(widget);
}
void SQuVRCatalogPlaneWidget::AddGroupTabPlane(TSharedPtr<FQuVRCatalogNode > node)
{

	RootWidget->ClearAssetList();

	if (ChildWidget.IsValid())
	{
		RemoveChildWidget(ChildWidget.ToSharedRef());
	}
	if (node.IsValid()&&0<node->ChildList.Num())
	{
		ChildWidget = SNew(SQuVRCatalogPlaneWidget).TreeItem(node).ParentWidget(SharedThis(this)).RootContent(RootContent).RootWidget(RootWidget);
		RootContent->InsertSlot(RootContent->NumSlots() - 1).Padding(0)
			.AutoWidth()
			[
				ChildWidget.ToSharedRef()
			];
	}
}

void SQuVRCatalogPlaneWidget::AddGroupTabAssetList(TSharedPtr<FQuVRCatalogNode> node)
{
	if (RootWidget.IsValid())
	{
		if (node.IsValid())
		{
			RootWidget->CreateCatalogGroupTabAssetList(node->ChildList);
		}
	}

}
void SQuVRCatalogPlaneWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (bNeedsUpdate)
	{
		bNeedsUpdate = false;
	}
};

ECheckBoxState SQuVRCatalogPlaneWidget::GetCatalogTabCheckedState(FName CategoryName) const
{
	return ActiveTabName == CategoryName ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

const FSlateBrush* SQuVRCatalogPlaneWidget::CatalogGroupBorderImage(FName CategoryName) const
{
	if (ActiveTabName == CategoryName)
	{
		static FName QuVRActiveTabBarBrush("PlacementBrowser.ActiveTabBar");
		return FEditorStyle::GetBrush(QuVRActiveTabBarBrush);
	}
	else
	{
		return nullptr;
	}
}


TSharedRef<SWidget> MakeCatalogAssetButton()
{
	return SNew(SQuVRCatalogPlaneWidget);}

#undef LOCTEXT_NAMESPACE

#endif // #if !UE_BUILD_SHIPPING

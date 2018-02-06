// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "QuVRCatalogNodeButton.h"
#include "Widgets/SWidget.h"
#include "Widgets/SUserWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Runtime/Slate/Public/SlateOptMacros.h"
#include "Core.h"
#include "SlateBasics.h"
#include "SNotificationList.h"
#include "QuVRCatalogPlaneWidget.h"
#include "QuVRAssetDownNet.h"

#if !UE_BUILD_SHIPPING

#define LOCTEXT_NAMESPACE "SQuVRCatalogNodeButton"

#define PrimaryListPanel 1
#define SectionListPanel 2

void SQuVRCatalogNodeButton::Construct(const SQuVRCatalogNodeButton::FArguments& InDelcaration)
{
	TreeItem = InDelcaration._TreeItem;
	IsSectionButtonChecked = InDelcaration._IsChecked;
	OnCheckStateChanged = InDelcaration._OnCheckStateChanged;
	BkImage = InDelcaration._BkImage;
	ParentWidget = InDelcaration._ParentWidget;
#if 1 // Create ChildSlot
this->ChildSlot
	[
	SNew(SCheckBox)
		.OnCheckStateChanged(this, &SQuVRCatalogNodeButton::OnSectionButtonChanged)
		.IsChecked(IsSectionButtonChecked)
		.Style(FEditorStyle::Get(), "PlacementBrowser.Tab")
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
		.VAlign(VAlign_Center)
		[
			SNew(SSpacer)
			.Size(FVector2D(1, 30))
		]
	+ SOverlay::Slot()
		.Padding(FMargin(6, 0, 30, 0))
		.VAlign(VAlign_Center)
		[

			SNew(STextBlock)
			.TextStyle(FEditorStyle::Get(), "PlacementBrowser.Tab.Text")
			.Text(FText::FromString(TreeItem->NodeData.DisplayName))
		]
	+ SOverlay::Slot()
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Left)
		[
			SNew(SImage)
			.Image(BkImage)
		]
	// Bottom-right corner text for notification list position
	+ SOverlay::Slot()
		.Padding(15.f)
		.VAlign(VAlign_Bottom)
		.HAlign(HAlign_Right)
		[
			SAssignNew(NotificationListPtr, SNotificationList)
			.Visibility(EVisibility::HitTestInvisible)
		]
		]
	];
#endif

};

void SQuVRCatalogNodeButton::OnSectionButtonChanged(ECheckBoxState NewState)
{
	OnCheckStateChanged.ExecuteIfBound(NewState);
#if 0 // SHOW DEBUG INFO
	// The state of the check box changed.  Execute the delegate to notify users
	FNotificationInfo Info(FText::FromString(TreeItem->NodeData.DisplayName+FString::FromInt(TreeItem->NodeData.ZOrder)));//LOCTEXT("TestNotification01", "OnMouseButtonDown"));
	NotificationListPtr->AddNotification(Info);
#endif

	int32 ZOrder = TreeItem->NodeData.ZOrder;

	switch (ZOrder)
	{
	case PrimaryListPanel:
		ParentWidget->AddGroupTabPlane(TreeItem);
		break;
	case SectionListPanel:
		UQuVRAssetDownNet::GetInstance()->GetCatalogNodeAssetFromUrl(TreeItem);
		break;
	default:
		ParentWidget->AddGroupTabPlane(TreeItem);
		break;
	}
}

FReply SQuVRCatalogNodeButton::OnSectionButtonChanged()
{
		// The state of the check box changed.  Execute the delegate to notify users
	FNotificationInfo Info(FText::FromString(TreeItem->NodeData.DisplayName));//LOCTEXT("TestNotification01", "OnMouseButtonDown"));
	NotificationListPtr->AddNotification(Info);
	return FReply::Handled();
}


TSharedRef<SWidget> MakeCatalogSectionButton(const TSharedRef<class FQuVRCatalogNode> node)
{
	return SNew(SQuVRCatalogNodeButton).TreeItem(node);}

#undef LOCTEXT_NAMESPACE

#endif // #if !UE_BUILD_SHIPPING

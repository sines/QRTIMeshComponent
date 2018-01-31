// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "QuVREditorModeEdModeToolkit.h"
#include "QuVREditorModeEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"
#include "QuVRCatalogWidget.h"

#define LOCTEXT_NAMESPACE "FQuVREditorModeEdModeToolkit"

FQuVREditorModeEdModeToolkit::FQuVREditorModeEdModeToolkit()
{
	SAssignNew(ToolkitWidget, SQuVRCatalogWidget);
}

void FQuVREditorModeEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	FModeToolkit::Init(InitToolkitHost);
}

FName FQuVREditorModeEdModeToolkit::GetToolkitFName() const
{
	return FName("QuVREditorModeEdMode");
}

FText FQuVREditorModeEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("QuVREditorModeEdModeToolkit", "DisplayName", "QuVREditorModeEdMode Tool");
}

class FEdMode* FQuVREditorModeEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FQuVREditorModeEdMode::EM_QuVREditorModeEdModeId);
}

#undef LOCTEXT_NAMESPACE

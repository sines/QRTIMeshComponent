// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "QuVREditorModeEdMode.h"
#include "QuVREditorModeEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FQuVREditorModeEdMode::EM_QuVREditorModeEdModeId = TEXT("EM_QuVREditorModeEdMode");

FQuVREditorModeEdMode::FQuVREditorModeEdMode()
{

}

FQuVREditorModeEdMode::~FQuVREditorModeEdMode()
{

}

void FQuVREditorModeEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FQuVREditorModeEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FQuVREditorModeEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FQuVREditorModeEdMode::UsesToolkits() const
{
	return true;
}





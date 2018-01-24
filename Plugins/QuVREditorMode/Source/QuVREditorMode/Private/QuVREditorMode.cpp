// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "QuVREditorMode.h"
#include "QuVREditorModeEdMode.h"
#include "IPlacementModeModule.h"
#include "ActorFactories/ActorFactory.h"
#include "ActorFactories/ActorFactoryDirectionalLight.h"


#define LOCTEXT_NAMESPACE "FQuVREditorModeModule"

void FQuVREditorModeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FQuVREditorModeEdMode>(FQuVREditorModeEdMode::EM_QuVREditorModeEdModeId, LOCTEXT("QuVREditorModeEdModeName", "QuVREditorModeEdMode"), FSlateIcon(), true);

	// Add a new light for the mode
	/*IPlacementModeModule& PlacementModeModule = IPlacementModeModule::Get();

	FPlacementCategoryInfo Info(
		LOCTEXT("QuVRCategoryName", "QuVR"),
		"QuVR",
		TEXT("QuVR Tools"),
		25
	);

	PlacementModeModule.RegisterPlacementCategory(Info);

	int32 SortOrder = 0;
	PlacementModeModule.RegisterPlaceableItem("QuVR", 
		MakeShareable(new FPlaceableItem(*UActorFactoryDirectionalLight::StaticClass(), SortOrder += 10)));
	*/
}

void FQuVREditorModeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FQuVREditorModeEdMode::EM_QuVREditorModeEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuVREditorModeModule, QuVREditorMode)
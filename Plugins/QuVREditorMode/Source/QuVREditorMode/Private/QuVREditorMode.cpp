// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "QuVREditorMode.h"
#include "QuVREditorModeEdMode.h"
#include "IPlacementModeModule.h"
#include "ActorFactories/ActorFactory.h"
#include "ActorFactories/ActorFactoryDirectionalLight.h"
#include "QuVRAssetFactoryStaticMesh.h"


#define LOCTEXT_NAMESPACE "FQuVREditorModeModule"

void FQuVREditorModeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FQuVREditorModeEdMode>(FQuVREditorModeEdMode::EM_QuVREditorModeEdModeId, LOCTEXT("QuVREditorModeEdModeName", "QuVREditorModeEdMode"), FSlateIcon(), true);


	handle = FModuleManager::Get().OnModulesChanged().AddRaw(this, &FQuVREditorModeModule::OnModulesChanged);

if (IPlacementModeModule::IsAvailable())
	{
		OnModulesChanged("PlacementMode", EModuleChangeReason::ModuleLoaded);
	}

}

void FQuVREditorModeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FModuleManager::Get().OnModulesChanged().Remove(handle);
	FEditorModeRegistry::Get().UnregisterMode(FQuVREditorModeEdMode::EM_QuVREditorModeEdModeId);
}

void FQuVREditorModeModule::OnModulesChanged(FName Module, EModuleChangeReason Reason)
{
	if (Module == TEXT("PlacementMode") && Reason == EModuleChangeReason::ModuleLoaded)
	{
		RegisterNewPlaceMode();
	}
}


void FQuVREditorModeModule::RegisterNewPlaceMode()
{
	// Add a new light for the mode
	IPlacementModeModule& PlacementModeModule = IPlacementModeModule::Get();

	FPlacementCategoryInfo Info(LOCTEXT("QuVRCategoryName", "QuVRPanoKit"), "QuVRPanoKit", TEXT("QuVR Pano Tools"), 100);

	PlacementModeModule.RegisterPlacementCategory(Info);

	int32 SortOrder = 100;
	PlacementModeModule.RegisterPlaceableItem(Info.UniqueHandle,
		MakeShareable(new FPlaceableItem(*UQuVRAssetFactoryStaticMesh::StaticClass(), SortOrder += 10)));
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuVREditorModeModule, QuVREditorMode)
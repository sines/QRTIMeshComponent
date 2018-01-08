// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#include "RuntimeQuVRTransformAxisPlugin.h"
#include "RuntimeQuVRTransformAxisActor.h"

#if WITH_EDITOR
#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#endif

#define LOCTEXT_NAMESPACE "FRuntimeQuVRTransformAxisModule"


namespace QuVR
{
	static FAutoConsoleCommand EditMode(TEXT("QuVR.EditMode"),TEXT("Toggles EditMode on desktop"), FConsoleCommandDelegate::CreateStatic(&FRuntimeQuVRTransformAxisPlugin::EditMode));
	static FAutoConsoleCommand GameMode(TEXT("QuVR.GameMode"), TEXT("Toggles GameMode on desktop"), FConsoleCommandDelegate::CreateStatic(&FRuntimeQuVRTransformAxisPlugin::GameMode));
}


void FRuntimeQuVRTransformAxisPlugin::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FRuntimeQuVRTransformAxisPlugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}
void FRuntimeQuVRTransformAxisPlugin::GameMode()
{
	UWorld* World = GEngine->GetWorld();
	if (!World)
	{
		World = GWorld;
	}
	if (World)
	{
		TArray<AActor*> AxisActors;
		UGameplayStatics::GetAllActorsOfClass(World, ARuntimeQuVRTransformAxisActor::StaticClass(), AxisActors);
		for (auto axisActor : AxisActors)
		{
			if (axisActor && !axisActor->IsPendingKill())
			{
				// Game mode
			}
		}
	}

}

void FRuntimeQuVRTransformAxisPlugin::EditMode()
{
	UWorld* World = GEngine->GetWorld();
	if (!World)
	{
		World = GWorld;
	}
	if (World)
	{

		TArray<AActor*> AxisActors;
		UGameplayStatics::GetAllActorsOfClass(World, ARuntimeQuVRTransformAxisActor::StaticClass(), AxisActors);
		if (AxisActors.Num() < 1)
		{
			World->SpawnActor<ARuntimeQuVRTransformAxisActor>();
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRuntimeQuVRTransformAxisPlugin, RuntimeQuVRTransformAxis)

DEFINE_LOG_CATEGORY(RuntimeQuVRTransformAxisPluginLog);
// Fill out your copyright notice in the Description page of Project Settings.

#include "QuVRAssetFactory.h"
#include "Engine/StaticMeshActor.h"

/************************************************************************/
/*       UUQuVRAssetFactory                                             */
/************************************************************************/
#define LOCTEXT_NAMESPACE "QuVRAssetFactory"

UQuVRAssetFactory::UQuVRAssetFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayName = LOCTEXT("QuVRAssetFactory", "QuVR IES Spot Light");
	NewActorClass = AStaticMeshActor::StaticClass();
	SpawnPositionOffset = FVector(50, 0, 0);
	bUseSurfaceOrientation = true;
}

AActor* UQuVRAssetFactory::SpawnActor(UObject* Asset, ULevel* InLevel, const FTransform& Transform, EObjectFlags ObjectFlags, const FName Name)
{
	AActor* NewActor = nullptr;
	{
		// Spawn a temporary actor for dragging around
		NewActor = Super::SpawnActor(Asset, InLevel, Transform, ObjectFlags, Name);
	}

	return NewActor;
}

void UQuVRAssetFactory::PostSpawnActor(UObject* Asset, AActor* NewActor)
{
	Super::PostSpawnActor(Asset, NewActor);

}


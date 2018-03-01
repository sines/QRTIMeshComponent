// Fill out your copyright notice in the Description page of Project Settings.
#include "QuVRAssetFactoryModel.h"
#include "QuVRAssetFactoryStaticMesh.h"
#include "Engine/StaticMeshActor.h"

/************************************************************************/
/*       UUQuVRAssetFactoryModel                                             */
/************************************************************************/
#define LOCTEXT_NAMESPACE "QuVRAssetFactoryModel"

UQuVRAssetFactoryModel::UQuVRAssetFactoryModel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayName = LOCTEXT("QuVRAssetFactoryModel", "QuVRAssetFactoryModels");
	NewActorClass = AStaticMeshActor::StaticClass();
	bUseSurfaceOrientation = true;
}

bool UQuVRAssetFactoryModel::CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg)
{
	if (AssetData.IsValid() && AssetData.GetClass()->IsChildOf(UStaticMesh::StaticClass()))
	{
		return true;
	}

	return false;
}

void UQuVRAssetFactoryModel::PostSpawnActor(UObject* Asset, AActor* NewActor)
{
	Super::PostSpawnActor(Asset, NewActor);
	// Change properties
	UStaticMesh* StaticMesh = CastChecked<UStaticMesh>(Asset);

	AStaticMeshActor* StaticMeshActor = CastChecked<AStaticMeshActor>(NewActor);
	UStaticMeshComponent* StaticMeshComponent = StaticMeshActor->GetStaticMeshComponent();

	if (StaticMeshComponent)
	{
		StaticMeshComponent->UnregisterComponent();

		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->StaticMeshDerivedDataKey = StaticMesh->RenderData->DerivedDataKey;
		// Init Component
		StaticMeshComponent->RegisterComponent();
	}
}

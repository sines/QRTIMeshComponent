// Fill out your copyright notice in the Description page of Project Settings.

#include "QuVRAssetFactoryStaticMesh.h"
#include "Engine/StaticMeshActor.h"

/************************************************************************/
/*       UUQuVRAssetFactoryStaticMesh                                             */
/************************************************************************/
#define LOCTEXT_NAMESPACE "QuVRAssetFactoryStaticMesh"

UQuVRAssetFactoryStaticMesh::UQuVRAssetFactoryStaticMesh(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayName = LOCTEXT("QuVRAssetFactoryStaticMesh", "StaticMesh");
	NewActorClass = AStaticMeshActor::StaticClass();
	bUseSurfaceOrientation = true;
}



void UQuVRAssetFactoryStaticMesh::PostSpawnActor(UObject* Asset, AActor* NewActor)
{
	Super::PostSpawnActor(Asset, NewActor);

	UStaticMesh* StaticMesh = CastChecked<UStaticMesh>(Asset);

	UE_LOG(LogActorFactory, Log, TEXT("Actor Factory created %s"), *StaticMesh->GetName());

	// Change properties
	AStaticMeshActor* StaticMeshActor = CastChecked<AStaticMeshActor>(NewActor);
	UStaticMeshComponent* StaticMeshComponent = StaticMeshActor->GetStaticMeshComponent();
	check(StaticMeshComponent);

	StaticMeshComponent->UnregisterComponent();

	StaticMeshComponent->SetStaticMesh(StaticMesh);
	StaticMeshComponent->StaticMeshDerivedDataKey = StaticMesh->RenderData->DerivedDataKey;
	// Init Component
	StaticMeshComponent->RegisterComponent();

}

bool UQuVRAssetFactoryStaticMesh::CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg)
{
	if (!AssetData.IsValid() || !AssetData.GetClass()->IsChildOf(UStaticMesh::StaticClass()))
	{
		OutErrorMsg = NSLOCTEXT("CanCreateActor", "NoStaticMesh", "A valid static mesh must be specified.");
		return false;
	}
	return false;
}

void UQuVRAssetFactoryStaticMesh::PostCreateBlueprint(UObject* Asset, AActor* CDO)
{
	if (Asset != NULL && CDO != NULL)
	{
		UStaticMesh* StaticMesh = CastChecked<UStaticMesh>(Asset);
		AStaticMeshActor* StaticMeshActor = CastChecked<AStaticMeshActor>(CDO);
		UStaticMeshComponent* StaticMeshComponent = StaticMeshActor->GetStaticMeshComponent();

		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->StaticMeshDerivedDataKey = StaticMesh->RenderData->DerivedDataKey;
	}
};

UObject* UQuVRAssetFactoryStaticMesh::GetAssetFromActorInstance(AActor* Instance)
{
	check(Instance->IsA(NewActorClass));
	AStaticMeshActor* SMA = CastChecked<AStaticMeshActor>(Instance);

	check(SMA->GetStaticMeshComponent());
	return SMA->GetStaticMeshComponent()->GetStaticMesh();
};


// Fill out your copyright notice in the Description page of Project Settings.

#include "QuVRAssetFactoryModel.h"
#include "Engine/StaticMeshActor.h"

/************************************************************************/
/*       UUQuVRAssetFactoryModel                                             */
/************************************************************************/
#define LOCTEXT_NAMESPACE "QuVRAssetFactoryModel"

UQuVRAssetFactoryModel::UQuVRAssetFactoryModel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayName = LOCTEXT("QuVRAssetFactoryModelDisplayName", "Models");
	NewActorClass = AStaticMeshActor::StaticClass();
	bUseSurfaceOrientation = true;
}

bool UQuVRAssetFactoryModel::CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg)
{
	if (AssetData.IsValid())
	{
		return true;
	}

	return false;
}


void UQuVRAssetFactoryModel::PostSpawnActor(UObject* Asset, AActor* NewActor)
{
	Super::PostSpawnActor(Asset, NewActor);

}


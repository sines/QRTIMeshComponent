// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ActorFactories/ActorFactory.h"
#include "ActorFactories/ActorFactoryStaticMesh.h"
#include "QuVRAssetFactoryStaticMesh.generated.h"

/************************************************************************/
/*       UUQuVRAssetFactoryStaticMesh                                             */
/************************************************************************/
class AActor;
struct FAssetData;

UCLASS(MinimalAPI, config = Editor)
class UQuVRAssetFactoryStaticMesh : public UActorFactory
{
	GENERATED_UCLASS_BODY()

	/** The Net Download Asset Path */
	UPROPERTY()
	FString CatalogAssetPath;
	
	//~ Begin UActorFactory Interface
	virtual bool CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg) override;
	virtual void PostSpawnActor(UObject* Asset, AActor* NewActor) override;
	virtual void PostCreateBlueprint(UObject* Asset, AActor* CDO) override;
	virtual UObject* GetAssetFromActorInstance(AActor* ActorInstance) override;
	//~ End UActorFactory Interface
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ActorFactories/ActorFactory.h"
#include "QuVRAssetFactory.generated.h"

/************************************************************************/
/*       UUQuVRAssetFactory                                             */
/************************************************************************/

//UCLASS(MinimalAPI, config = Editor, collapsecategories, hidecategories = Object)
UCLASS()
class QUVREDITORMODE_API UQuVRAssetFactory : public UActorFactory
{
	GENERATED_UCLASS_BODY()
public:
	/** The Net Download Asset Path */
	UPROPERTY()
		FString CatalogAssetPath;

protected:
	//~ Begin UActorFactory Interface
	virtual void PostSpawnActor(UObject* Asset, AActor* NewActor) override;
	virtual AActor* SpawnActor(UObject* Asset, ULevel* InLevel, const FTransform& Ftransform, EObjectFlags ObjectFlags, const FName Name) override;
	//~ End UActorFactory Interface
};

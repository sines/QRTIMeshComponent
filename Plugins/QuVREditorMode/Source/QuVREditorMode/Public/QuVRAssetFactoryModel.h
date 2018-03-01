// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ActorFactories/ActorFactory.h"
#include "QuVRAssetFactoryStaticMesh.h"
#include "QuVRAssetFactoryModel.generated.h"

/************************************************************************/
/*       UUQuVRAssetFactoryStaticMeshModel                                             */
/************************************************************************/
class AActor;
struct FAssetData;

UCLASS(MinimalAPI, config = Editor, collapsecategories, hidecategories = Object)
class UQuVRAssetFactoryModel : public UQuVRAssetFactoryStaticMesh
{
	GENERATED_UCLASS_BODY()

	/*-----------------------------------------------------------------------------
	UActorFactory override function
	-----------------------------------------------------------------------------*/
	//~ Begin UActorFactory Interface
	virtual void PostSpawnActor(UObject* Asset, AActor* NewActor) override;
	virtual bool CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg) override;
	//~ End UActorFactory Interface
};

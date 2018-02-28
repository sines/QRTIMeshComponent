// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ActorFactories/ActorFactory.h"
#include "ActorFactories/ActorFactoryStaticMesh.h"
#include "QuVRAssetFactoryModel.generated.h"

/************************************************************************/
/*       UUQuVRAssetFactoryModel                                             */
/************************************************************************/

//UCLASS(MinimalAPI, config = Editor, collapsecategories, hidecategories = Object)
UCLASS(MinimalAPI, config = Editor)
class UQuVRAssetFactoryModel : public UActorFactoryStaticMesh
{
	GENERATED_UCLASS_BODY()
public:
	/** The Net Download Asset Path */
	UPROPERTY()
		FString CatalogAssetPath;

protected:
	/*-----------------------------------------------------------------------------
	UActorFactoryStaticMesh override function
	-----------------------------------------------------------------------------*/
	virtual bool CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg) override;

	/*-----------------------------------------------------------------------------
	UActorFactory override function
	-----------------------------------------------------------------------------*/
	//~ Begin UActorFactory Interface
	virtual void PostSpawnActor(UObject* Asset, AActor* NewActor) override;
	//~ End UActorFactory Interface
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ActorFactories/ActorFactory.h"
#include "Editor/UnrealEd/Classes/ActorFactories/ActorFactoryAnimationAsset.h"
#include "Editor/UnrealEd/Classes/ActorFactories/ActorFactorySkeletalMesh.h"
#include "QuVRAssetFactoryAnimation.generated.h"

/**
 *  UActorFactoryAnimationAsset UActorFactorySkeletalMesh
 */
class AActor;
class FAssetData;
class USkeletalMesh;

UCLASS(MinimalAPI, config = Editor, collapsecategories, hidecategories = Object)
class UQuVRAssetFactoryAnimation : public UActorFactory
{
	 GENERATED_UCLASS_BODY()
protected:
	//~ Begin UActorFactory Interface
	virtual void PostSpawnActor(UObject* Asset, AActor* NewActor) override;
	virtual void PostCreateBlueprint(UObject* Asset, AActor* CDO) override;
	virtual bool CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg) override;
	//~ End UActorFactory Interface

	virtual FQuat AlignObjectToSurfaceNormal(const FVector& InSurfaceNormal, const FQuat& ActorRotation) const override;
	virtual USkeletalMesh* GetSkeletalMeshFromAsset(UObject* Asset) const;
	
};

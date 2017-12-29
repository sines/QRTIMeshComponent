// Copyright 2016 Chris Conway (Koderz). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RuntimeQuVRAssetContainer.generated.h"

// Forward declarations
class UMaterialInterface;
class USoundBase;
class UStaticMesh;


/**
 *  Asset container for Render.
 */
UCLASS()
class RUNTIMEQUVRTRANSFORMAXIS_API URuntimeQuVRAssetContainer : public UDataAsset
{
	GENERATED_BODY()
	
public:
	static const FString AssetContainerPath;
	//
	// Sound
	//

	UPROPERTY(EditAnywhere, Category = Sound)
		USoundBase* GizmoHandleSelectedSound;

	UPROPERTY(EditAnywhere, Category = Sound)
		USoundBase* GizmoHandleDropSound;

	UPROPERTY(EditAnywhere, Category = Sound)
		USoundBase* SelectionChangeSound;

	UPROPERTY(EditAnywhere, Category = Sound)
		USoundBase* SelectionDropSound;

	UPROPERTY(EditAnywhere, Category = Sound)
		USoundBase* SelectionStartDragSound;

	UPROPERTY(EditAnywhere, Category = Sound)
		USoundBase* GridSnapSound;

	UPROPERTY(EditAnywhere, Category = Sound)
		USoundBase* ActorSnapSound;

	UPROPERTY(EditAnywhere, Category = Sound)
		USoundBase* UndoSound;

	UPROPERTY(EditAnywhere, Category = Sound)
		USoundBase* RedoSound;

	//
	// Meshes
	//

	UPROPERTY(EditAnywhere, Category = Mesh)
		UStaticMesh* GridMesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
		UStaticMesh* TranslationHandleMesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
		UStaticMesh* UniformScaleHandleMesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
		UStaticMesh* ScaleHandleMesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
		UStaticMesh* PlaneTranslationHandleMesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
		UStaticMesh* RotationHandleMesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
		UStaticMesh* RotationHandleSelectedMesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
		UStaticMesh* StartRotationIndicatorMesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
		UStaticMesh* CurrentRotationIndicatorMesh;

	UPROPERTY(EditAnywhere, Category = Mesh)
		UStaticMesh* FreeRotationHandleMesh;

	//
	// Materials
	//

	UPROPERTY(EditAnywhere, Category = Material)
		UMaterialInterface* GridMaterial;

	UPROPERTY(EditAnywhere, Category = Material)
		UMaterialInterface* TransformGizmoMaterial;

	UPROPERTY(EditAnywhere, Category = Material)
		UMaterialInterface* TranslucentTransformGizmoMaterial;
	
public:
	/** Static function to load the asset container */
	static const class URuntimeQuVRAssetContainer& LoadAssetContainer();
};

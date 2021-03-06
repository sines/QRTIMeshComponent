// Copyright 2016 Chris Conway (Koderz). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "UObject/ObjectMacros.h"
#include "Math/Axis.h"
#include "RuntimeQuVRTransformType.h"
#include "RuntimeQuVRGizmoHandleGroup.generated.h"

class UMaterialInterface;
class UStaticMesh;
class UActorComponent;

/** Placement of a handle in pivot space */
USTRUCT()
struct RUNTIMEQUVRTRANSFORMAXIS_API FQuVRTransformGizmoHandlePlacement
{
	GENERATED_BODY()

		/* Handle direction in X, Y and Z */
		RuntimeQuVRtransformType::EQuVRTransformGizmoHandleDirection Axes[3];


	/** Finds the center handle count and facing axis index.  The center handle count is simply the number
	of axes where the handle would be centered on the bounds along that axis.  The facing axis index is
	the index (0-2) of the axis where the handle would be facing, or INDEX_NONE for corners or edges.
	The center axis index is valid for edges, and defines the axis perpendicular to that edge direction,
	or INDEX_NONE if it's not an edge */
	void GetCenterHandleCountAndFacingAxisIndex(int32& OutCenterHandleCount, int32& OutFacingAxisIndex, int32& OutCenterAxisIndex) const;
};


USTRUCT()
struct RUNTIMEQUVRTRANSFORMAXIS_API FQuVRGizmoHandle
{
	GENERATED_BODY()

	/** Static mesh for this handle */
	class URuntimeQuVRHandleMeshComponent* HandleMesh;

	/** Scalar that will advance toward 1.0 over time as we hover over the gizmo handle */
	float HoverAlpha;

	// Handle Type
	EAxisList::Type HandleType;
};



/**
* Base class for gizmo handles
*/
UCLASS(ABSTRACT)
class RUNTIMEQUVRTRANSFORMAXIS_API URuntimeQuVRGizmoHandleGroup : public USceneComponent
{
	GENERATED_BODY()

public:

	/** Default constructor that sets up CDO properties */
	URuntimeQuVRGizmoHandleGroup();

	/** Given the unique index, makes a handle */
	FQuVRTransformGizmoHandlePlacement MakeHandlePlacementForIndex(const int32 HandleIndex) const;

	/** Makes a unique index for a handle */
	int32 MakeHandleIndex(const FQuVRTransformGizmoHandlePlacement HandlePlacement) const;

	/** Makes up a name string for a handle */
	FString MakeHandleName(const FQuVRTransformGizmoHandlePlacement HandlePlacement, EAxisList::Type& out_type) const;

	/** Static: Given an axis (0-2) and a facing direction, returns the vector normal for that axis */
	static FVector GetAxisVector(const int32 AxisIndex, const RuntimeQuVRtransformType::EQuVRTransformGizmoHandleDirection HandleDirection);

	/** Updates the Gizmo handles, needs to be implemented by derived classes */
	virtual void UpdateGizmoHandleGroup(const FTransform& LocalToWorld, const FBox& LocalBounds, const FVector ViewLocation, const bool bAllHandlesVisible, class UActorComponent* DraggingHandle, const TArray< UActorComponent* >& HoveringOverHandles,
		float AnimationAlpha, float GizmoScale, const float GizmoHoverScale, const float GizmoHoverAnimationDuration, bool& bOutIsHoveringOrDraggingThisHandleGroup);

	/** Default setting the visibility and collision for all the handles in this group */
	void UpdateVisibilityAndCollision(const RuntimeQuVRtransformType::EQuVRMode GizmoType, const RuntimeQuVRtransformType::EQuVRCoordSystem GizmoCoordinateSpace, const bool bAllHandlesVisible, const bool bAllowRotationAndScaleHandles, UActorComponent* DraggingHandle);

	virtual void UpdateAxisToDragActorTranslational(FVector& pos);
	virtual void UpdateAxisToDragActorRotation(FRotator& rotator);
	virtual void UpdateAxisToDragActorScale(FVector& scale);
	virtual void UpdateDragActorToAxis();

	/** Record Actor */
	class AActor* GetDragActor() { return DragActor; };
	virtual void StartTracking(class AActor* actor);
	virtual void EndTracking();
	virtual void SetIsDrag(bool& inIsDrag);
//	class UViewportDragOperationComponent* GetDragOperationComponent();

	/** Finds the index of DraggedMesh in HandleMeshes */
	virtual int32 GetDraggedHandleIndex(class UStaticMeshComponent* DraggedMesh);

	/** Sets the Gizmo material */
	void SetGizmoMaterial(UMaterialInterface* Material);

	/** Sets the translucent Gizmo material */
	void SetTranslucentGizmoMaterial(UMaterialInterface* Material);

	/** Gets all the handles */
	TArray< FQuVRGizmoHandle >& GetHandles();

	/** Gets the GizmoType for this Gizmo handle */
	virtual RuntimeQuVRtransformType::EQuVRMode GetHandleType() const;

	/** Returns true if this type of handle is allowed with world space gizmos */
	virtual bool SupportsWorldCoordinateSpace() const
	{
		return true;
	}

	/** Sets if this handlegroup will be visible with the universal gizmo */
	void SetShowOnUniversalGizmo(const bool bShowHandleUniversal);

	/** Gets if this handlegroup will be visible with the universal gizmo */
	bool GetShowOnUniversalGizmo() const;

	/** Sets the owning transform gizmo for this handle group*/
	void SetQuVROwningTransformGizmo(class ARuntimeQuVRTransformAxisActor* TransformGizmo);
	
	virtual RuntimeQuVRtransformType::EQuVRGizmoHandleHoveredTypes GetHandleHoveredType()const;
protected:
	/** Updates the colors of the dynamic material instances for the handle passed using its axis index */
	void UpdateHandleColor(const int32 AxisIndex, FQuVRGizmoHandle& Handle, class UActorComponent* DraggingHandle, const TArray< UActorComponent* >& HoveringOverHandles);

	/** Helper function to create gizmo handle meshes */
	class URuntimeQuVRHandleMeshComponent* CreateMeshHandle(class UStaticMesh* HandleMesh, const FString& ComponentName);

	/** Creates handle meshcomponent and adds it to the Handles list */
	class URuntimeQuVRHandleMeshComponent* CreateAndAddMeshHandle(class UStaticMesh* HandleMesh, const FString& ComponentName, const FQuVRTransformGizmoHandlePlacement& HandlePlacement, const EAxisList::Type& type = EAxisList::None);

	/** Adds the HandleMeshComponent to the Handles list */
	void AddMeshToHandles(class URuntimeQuVRHandleMeshComponent* HandleMeshComponent, const FQuVRTransformGizmoHandlePlacement& HandlePlacement, const EAxisList::Type& type=EAxisList::None);

	/** Gets the handleplacement axes */
	FQuVRTransformGizmoHandlePlacement GetHandlePlacement(const int32 X, const int32 Y, const int32 Z) const;

	/** Gizmo material (opaque) */
	UPROPERTY()
		UMaterialInterface* GizmoMaterial;

	/** Gizmo material (translucent) */
	UPROPERTY()
		UMaterialInterface* TranslucentGizmoMaterial;

	/** All the StaticMeshes for this handle type */
	UPROPERTY()
		TArray< FQuVRGizmoHandle > Handles;

	/** The actor transform gizmo owning this handlegroup */
 	UPROPERTY()
 		class ARuntimeQuVRTransformAxisActor* QuVROwningTransformGizmoActor;

 	UPROPERTY()
 	class AActor* DragActor;

	UPROPERTY()
		bool bIsDrag;

private:

	/** Updates the hover animation for the HoveringOverHandles */
	void UpdateHoverAnimation(class UActorComponent* DraggingHandle, const TArray< UActorComponent* >& HoveringOverHandles, const float GizmoHoverAnimationDuration, bool& bOutIsHoveringOrDraggingThisHandleGroup);

	/** If this handlegroup will be visible with the universal gizmo */
	bool bShowOnUniversalGizmo;

	FTransform DragActorTransform;
protected:
	RuntimeQuVRtransformType::EQuVRGizmoHandleHoveredTypes eQuVRHandleHoveredType;

public:
	/** The gizmo component we're dragging right now */
	class UPrimitiveComponent*	DraggingTransformGizmoComponent;

	/** Gizmo component that we're hovering over, or nullptr if not hovering over any */
	class UPrimitiveComponent*	HoveringOverTransformGizmoComponent;

};

/**
* Base class for gizmo handles on axis
*/

UCLASS(ABSTRACT)
class RUNTIMEQUVRTRANSFORMAXIS_API URuntimeQuVRAxisGizmoHandleGroup : public URuntimeQuVRGizmoHandleGroup
{
	GENERATED_BODY()

protected:

	/** Creates mesh for every axis */
	void CreateHandles(UStaticMesh* HandleMesh, const FString& HandleComponentName);

	/**
	* Calculates the transforms of meshes of this handlegroup
	* @param HandleToCenter - The offset from the center of the gizmo in roomspace
	*/
	void UpdateHandlesRelativeTransformOnAxis(const FTransform& HandleToCenter, const float AnimationAlpha, const float GizmoScale, const float GizmoHoverScale,
		const FVector& ViewLocation, class UActorComponent* DraggingHandle, const TArray< UActorComponent* >& HoveringOverHandles);

public:
	virtual class URuntimeQuVRHandleMeshComponent* GetHandleMesh(const EAxisList::Type type);

	UFUNCTION()
	virtual	void OnHoverAxisX(class UPrimitiveComponent* OtherComp);
	UFUNCTION()
	virtual	void OnReleaseAxisX(class UPrimitiveComponent* OtherComp);

	UFUNCTION()
	virtual	void OnHoverAxisY(class UPrimitiveComponent* OtherComp);
	UFUNCTION()
	virtual	void OnReleaseAxisY(class UPrimitiveComponent* OtherComp);

	UFUNCTION()
	virtual	void OnHoverAxisZ(class UPrimitiveComponent* OtherComp);
	UFUNCTION()
	virtual	void OnReleaseAxisZ(class UPrimitiveComponent* OtherComp);

	virtual void OnBindingAxis();
};

/**
* Axis Gizmo handle for translating
*/
UCLASS()
class  URuntimeQuVRPivotTranslationGizmoHandleGroup : public URuntimeQuVRAxisGizmoHandleGroup
{
	GENERATED_BODY()

public:

	/** Default constructor that sets up CDO properties */
	URuntimeQuVRPivotTranslationGizmoHandleGroup();

	/** Updates the gizmo handles */
	virtual void UpdateGizmoHandleGroup(const FTransform& LocalToWorld, const FBox& LocalBounds, const FVector ViewLocation, const bool bAllHandlesVisible, class UActorComponent* DraggingHandle, const TArray< UActorComponent* >& HoveringOverHandles,
		float AnimationAlpha, float GizmoScale, const float GizmoHoverScale, const float GizmoHoverAnimationDuration, bool& bOutIsHoveringOrDraggingThisHandleGroup) override;

	/** Gets the GizmoType for this Gizmo handle */
	virtual RuntimeQuVRtransformType::EQuVRMode GetHandleType() const override;
};


/**
* Axis Gizmo handle for plane translation
*/
UCLASS()
class URuntimeQuVRPivotPlaneTranslationGizmoHandleGroup : public URuntimeQuVRAxisGizmoHandleGroup
{
	GENERATED_BODY()

public:

	/** Default constructor that sets up CDO properties */
	URuntimeQuVRPivotPlaneTranslationGizmoHandleGroup();

	/** Updates the gizmo handles */
	virtual void UpdateGizmoHandleGroup(const FTransform& LocalToWorld, const FBox& LocalBounds, const FVector ViewLocation, const bool bAllHandlesVisible, class UActorComponent* DraggingHandle, const TArray< UActorComponent* >& HoveringOverHandles,
		float AnimationAlpha, float GizmoScale, const float GizmoHoverScale, const float GizmoHoverAnimationDuration, bool& bOutIsHoveringOrDraggingThisHandleGroup) override;

	/** Gets the GizmoType for this Gizmo handle */
	virtual RuntimeQuVRtransformType::EQuVRMode  GetHandleType() const override;
};


/**
* Gizmo handle for stretching/scaling
*/
UCLASS()
class RUNTIMEQUVRTRANSFORMAXIS_API URuntimeQuVRStretchGizmoHandleGroup : public URuntimeQuVRGizmoHandleGroup
{
	GENERATED_BODY()

public:

	/** Default constructor that sets up CDO properties */
	URuntimeQuVRStretchGizmoHandleGroup();

	/** Updates the Gizmo handles */
	virtual void UpdateGizmoHandleGroup(const FTransform& LocalToWorld, const FBox& LocalBounds, const FVector ViewLocation, const bool bAllHandlesVisible, class UActorComponent* DraggingHandle, const TArray< UActorComponent* >& HoveringOverHandles,
		float AnimationAlpha, float GizmoScale, const float GizmoHoverScale, const float GizmoHoverAnimationDuration, bool& bOutIsHoveringOrDraggingThisHandleGroup) override;

	/** Gets the GizmoType for this Gizmo handle */
	virtual RuntimeQuVRtransformType::EQuVRMode  GetHandleType() const override;

	/** Returns true if this type of handle is allowed with world space gizmos */
	virtual bool SupportsWorldCoordinateSpace() const override;
};


/**
* Axis Gizmo handle for scaling
*/

UCLASS()
class RUNTIMEQUVRTRANSFORMAXIS_API URuntimeQuVRPivotScaleGizmoHandleGroup : public URuntimeQuVRAxisGizmoHandleGroup
{
	GENERATED_BODY()

public:

	/** Default constructor that sets up CDO properties */
	URuntimeQuVRPivotScaleGizmoHandleGroup();

	/** Updates the Gizmo handles */
	virtual void UpdateGizmoHandleGroup(const FTransform& LocalToWorld, const FBox& LocalBounds, const FVector ViewLocation, const bool bAllHandlesVisible, class UActorComponent* DraggingHandle, const TArray< UActorComponent* >& HoveringOverHandles,
		float AnimationAlpha, float GizmoScale, const float GizmoHoverScale, const float GizmoHoverAnimationDuration, bool& bOutIsHoveringOrDraggingThisHandleGroup) override;

	/** Gets the GizmoType for this Gizmo handle */
	virtual RuntimeQuVRtransformType::EQuVRMode  GetHandleType() const override;

	/** Sets if the pivot point is used as location for the handle */
	void SetUsePivotPointAsLocation(const bool bInUsePivotAsLocation);
private:

	/** If the pivot point is used for the uniform scaling handle */
	bool bUsePivotAsLocation;
};


/**
* Axis Gizmo handle for rotation
*/
UCLASS()
class RUNTIMEQUVRTRANSFORMAXIS_API URuntimeQuVRPivotRotationGizmoHandleGroup : public URuntimeQuVRAxisGizmoHandleGroup
{
	GENERATED_BODY()

public:

	/** Default constructor that sets up CDO properties */
	URuntimeQuVRPivotRotationGizmoHandleGroup();

	/** Updates the Gizmo handles */
	virtual void UpdateGizmoHandleGroup(const FTransform& LocalToWorld, const FBox& LocalBounds, const FVector ViewLocation, const bool bAllHandlesVisible, class UActorComponent* DraggingHandle, const TArray< UActorComponent* >& HoveringOverHandles,
		float AnimationAlpha, float GizmoScale, const float GizmoHoverScale, const float GizmoHoverAnimationDuration, bool& bOutIsHoveringOrDraggingThisHandleGroup) override;

	/** Gets the GizmoType for this Gizmo handle */
	virtual RuntimeQuVRtransformType::EQuVRMode  GetHandleType() const override;

	class UPrimitiveComponent* GetDraggingGizmoComponent() {return DraggingGizmoComponent;}

	UFUNCTION()
		virtual	void OnHoverAxisX(class UPrimitiveComponent* OtherComp);
	UFUNCTION()
		virtual	void OnReleaseAxisX(class UPrimitiveComponent* OtherComp);

	UFUNCTION()
		virtual	void OnHoverAxisY(class UPrimitiveComponent* OtherComp);
	UFUNCTION()
		virtual	void OnReleaseAxisY(class UPrimitiveComponent* OtherComp);

	UFUNCTION()
		virtual	void OnHoverAxisZ(class UPrimitiveComponent* OtherComp);
	UFUNCTION()
		virtual	void OnReleaseAxisZ(class UPrimitiveComponent* OtherComp);

	virtual void StartTracking(class AActor* actor);
	virtual void EndTracking();
	bool GetAllHandlesVisible();
private:

	/** Updates the root of an indicator to rotate the indicator itself */
	void UpdateIndicator(USceneComponent* IndicatorRoot, const FVector& Direction, const uint32 FacingAxisIndex);

	/** Make the components visible when dragging rotation */
	void ShowRotationVisuals(const bool bInShow);

	void SetupIndicator(USceneComponent* RootComponent, URuntimeQuVRHandleMeshComponent* IndicatorMeshComponent, UStaticMesh* Mesh);

	void SetIndicatorColor(UStaticMeshComponent* InMeshComponent, const FLinearColor& InHandleColor);

	/** Root component of all the mesh components that are used to visualize the rotation when dragging */
	UPROPERTY()
		USceneComponent* RootFullRotationHandleComponent;

	/** When dragging a rotation handle the full rotation circle appears */
	UPROPERTY()
		class URuntimeQuVRHandleMeshComponent* FullRotationHandleMeshComponent;

	/** The mesh that indicated the start rotation */
	UPROPERTY()
		class URuntimeQuVRHandleMeshComponent* StartRotationIndicatorMeshComponent;

	/** The root component of the start rotation indicator */
	UPROPERTY()
		USceneComponent* RootStartRotationIdicatorComponent;

	/** The mesh that indicated the delta rotation */
	UPROPERTY()
		class URuntimeQuVRHandleMeshComponent* DeltaRotationIndicatorMeshComponent;

	/** The root component of the delta rotation indicator */
	UPROPERTY()
		USceneComponent* RootDeltaRotationIndicatorComponent;

	/** Gizmo component that we're hovering over, or nullptr if not hovering over any */
	class UPrimitiveComponent*	DraggingGizmoComponent;

	/** The rotation when starting to drag the gizmo */
	TOptional<FQuat> StartDragRotation;
};
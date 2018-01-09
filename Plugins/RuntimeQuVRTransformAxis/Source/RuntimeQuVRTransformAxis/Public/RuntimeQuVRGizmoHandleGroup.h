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
	void UpdateVisibilityAndCollision(const RuntimeQuVRtransformType::EQuVRGizmoHandleTypes GizmoType, const RuntimeQuVRtransformType::EQuVRCoordSystem GizmoCoordinateSpace, const bool bAllHandlesVisible, const bool bAllowRotationAndScaleHandles, UActorComponent* DraggingHandle);

	void UpdateDragActorTranslate(FVector& pos);

	class AActor* GetDragActor() { return DragActor; };
	void StartTracking(class AActor* actor);
	void EndTracking();
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
	virtual RuntimeQuVRtransformType::EQuVRGizmoHandleTypes GetHandleType() const;

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

private:

	/** Updates the hover animation for the HoveringOverHandles */
	void UpdateHoverAnimation(class UActorComponent* DraggingHandle, const TArray< UActorComponent* >& HoveringOverHandles, const float GizmoHoverAnimationDuration, bool& bOutIsHoveringOrDraggingThisHandleGroup);

	/** If this handlegroup will be visible with the universal gizmo */
	bool bShowOnUniversalGizmo;

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
	virtual RuntimeQuVRtransformType::EQuVRGizmoHandleTypes GetHandleType() const override;


private:
	class URuntimeQuVRHandleMeshComponent* GetHandleMesh(const EAxisList::Type type);

	UFUNCTION()
	void OnHover_AxisX(class UPrimitiveComponent* OtherComp);
	UFUNCTION()
	void OnRelease_AxisX(class UPrimitiveComponent* OtherComp);

	UFUNCTION()
	void OnHover_AxisY(class UPrimitiveComponent* OtherComp);
	UFUNCTION()
	void OnRelease_AxisY(class UPrimitiveComponent* OtherComp);

	UFUNCTION()
	void OnHover_AxisZ(class UPrimitiveComponent* OtherComp);
	UFUNCTION()
	void OnRelease_AxisZ(class UPrimitiveComponent* OtherComp);
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
	virtual RuntimeQuVRtransformType::EQuVRGizmoHandleTypes  GetHandleType() const override;
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
	virtual RuntimeQuVRtransformType::EQuVRGizmoHandleTypes  GetHandleType() const override;

	/** Returns true if this type of handle is allowed with world space gizmos */
	virtual bool SupportsWorldCoordinateSpace() const override;
};
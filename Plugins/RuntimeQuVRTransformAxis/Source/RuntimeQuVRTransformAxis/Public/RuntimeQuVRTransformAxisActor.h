// Copyright 2017 Louis. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RuntimeQuVRGizmoHandleGroup.h"
#include "RuntimeQuVRTransformAxisActor.generated.h"

class FRuntimeQuVRTransformAlgorithm;
class URuntimeQuVRWorldExtension;
class ULocalPlayer;
/**
* Displays measurements along the bounds of selected objects
*/
USTRUCT()
struct FQuVRTransformGizmoMeasurement
{
	GENERATED_BODY()

		/** The text that displays the actual measurement and units */
		UPROPERTY()
		class UTextRenderComponent* MeasurementText;
};


UCLASS()
class RUNTIMEQUVRTRANSFORMAXIS_API ARuntimeQuVRTransformAxisActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARuntimeQuVRTransformAxisActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed();
	/**
	* Begin tracking at the specified location for the specified viewport.
	*/
	void StartTracking(class URuntimeQuVRAxisGizmoHandleGroup* InHandleGroup);

	/**
	* Called when a mouse button has been released.  If there are no other
	* mouse buttons being held down, the internal information is reset.
	*/
	void EndTracking();

	/**
	* Adds delta movement into the tracker.
	*/
	void  AddDelta(const int32 InDelta, bool InNudge);

	void Init();

	// Input  LeftButton Pressed Event
	void LeftButtonPressed();

	// Input LeftButton Released Event
	void LeftButtonReleased();
	
	// Input  RightButton Pressed Event
	void RightButtonPressed();

	// HideGizemo Event
	void HideGizemo();
public:	
	// construction
	void OnConstruction(const FTransform& Transform) override;

	void CrateHandleGroups();
	float GetAnimationAlpha();
	void OnNewObjectsSelected();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/** Gets the owner */
	class URuntimeQuVRWorldInteraction* GetOwnerWorldInteraction() const;

	/*UPdataAxis*/
	void UpdateGizmoAxis();

	/** Gets the current gizmo handle type */
	RuntimeQuVRtransformType::EQuVRMode GetGizmoType() const;
private:
	// Get GWorld
	UWorld* QuVRWorld;
	// Current LocalPlayer
	ULocalPlayer* QuVRLocalPlayer;

	//////////////////////////////////////////////////////////////////////////
	// RuntimeQuVRtransformAlgorithm instance
	FRuntimeQuVRTransformAlgorithm*  QuVRTransformAlgorithm;

	/** Current gizmo type */
	RuntimeQuVRtransformType::EQuVRMode GizmoType;

	/** Translation handle group component */
	UPROPERTY()
	class URuntimeQuVRPivotTranslationGizmoHandleGroup* TranslationGizmoHandleGroup;

	/** Plane translation handle group component */
	UPROPERTY()
	class  URuntimeQuVRPivotPlaneTranslationGizmoHandleGroup* PlaneTranslationGizmoHandleGroup;

	UPROPERTY()
		class URuntimeQuVRPivotRotationGizmoHandleGroup* RotationGizmoHandleGroup;

	UPROPERTY()
		class URuntimeQuVRPivotScaleGizmoHandleGroup* ScaleGizmoHandleGroup;

	/** Stretch handle group component */
	UPROPERTY()
	class URuntimeQuVRStretchGizmoHandleGroup* StretchGizmoHandleGroup;



	/** Scene component root of this actor */
	UPROPERTY()
	USceneComponent* SceneComponent;

	/** All gizmo components */
	UPROPERTY()
	TArray< class URuntimeQuVRGizmoHandleGroup* > AllHandleGroups;

	/** Owning object */
	UPROPERTY()
		class URuntimeQuVRWorldInteraction* WorldInteraction;

	/** Real time that the gizmo was last attached to a selected set of objects.  This is used for animation transitions */
	FTimespan SelectedAtTime;

	UFUNCTION()
		void OnClicked_AxisX(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	
	/** Left Mouse  Hit object */
	void HitObject();

	/** TranslationGizmoHandleGroup :: calculate Transform */
	void GizmoTranslation(const FVector& InLocation, FVector& OutDrag, FRotator& OutRotation, FVector& OutScale);
	
	/** TranslationGizmoHandleGroup :: calculate Rotation */
	void GizmoRotation(const FVector& InLocation, FVector& OutDrag, FRotator& OutRotation, FVector& OutScale);

	/** update all group draw */
	void UpdateGizmoGroupDraw();

	/** UpdateTranslation */
	void UpdateTranslation();

	/** UpdateRotation */
	void UpdateRotation();

	/** UpdateScale */
	void UpdateScale();
private:
	bool bIsHover;
	bool bIsDrag;
	bool bIsMouseButtonDown;

	FVector vDragV3D;

};

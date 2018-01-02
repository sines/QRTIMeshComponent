// Copyright 2017 Louis. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RuntimeQuVRGizmoHandleGroup.h"
#include "RuntimeQuVRTransformAxisActor.generated.h"

class FRuntimeQuVRTransformAlgorithm;

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
	void StartTracking();

	/**
	* Called when a mouse button has been released.  If there are no other
	* mouse buttons being held down, the internal information is reset.
	*/
	void EndTracking();

	/**
	* Adds delta movement into the tracker.
	*/
	void  AddDelta(const int32 InDelta, bool InNudge);

	// Input  Pressed Event
	void ButtonPressed();

	// Input Released Event
	void ButtonReleased();

public:	
	// construction
	void OnConstruction(const FTransform& Transform) override;

	void CrateHandleGroups();
	float GetAnimationAlpha();
	void OnNewObjectsSelected();
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	// Get GWorld
	UWorld* QuVRWorld;
	// Current LocalPlayer
	ULocalPlayer* QuVRLocalPlayer;

	//////////////////////////////////////////////////////////////////////////
	// RuntimeQuVRtransformAlgorithm instance
	FRuntimeQuVRTransformAlgorithm*  QuVRTransformAlgorithm;

	/** Current gizmo type */
	RuntimeQuVRtransformType::EQuVRGizmoHandleTypes GizmoType;

	/** Translation handle group component */
	UPROPERTY()
	class URuntimeQuVRPivotTranslationGizmoHandleGroup* TranslationGizmoHandleGroup;

	/** Plane translation handle group component */
	UPROPERTY()
	class  URuntimeQuVRPivotPlaneTranslationGizmoHandleGroup* PlaneTranslationGizmoHandleGroup;

	/** Stretch handle group component */
	UPROPERTY()
	class URuntimeQuVRStretchGizmoHandleGroup* StretchGizmoHandleGroup;

	/** Scene component root of this actor */
	UPROPERTY()
	USceneComponent* SceneComponent;

	/** All gizmo components */
	UPROPERTY()
	TArray< class URuntimeQuVRGizmoHandleGroup* > AllHandleGroups;

	/** Real time that the gizmo was last attached to a selected set of objects.  This is used for animation transitions */
	FTimespan SelectedAtTime;

	UFUNCTION()
		void OnHover_AxisX(class UPrimitiveComponent* OtherComp);
};

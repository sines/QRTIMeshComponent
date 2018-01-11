// Copyright 2018 Louis. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RuntimeQuVRTransformType.h"

#include "RuntimeQuVRWorldInteraction.generated.h"

/**
 * 
 */
UCLASS()
class RUNTIMEQUVRTRANSFORMAXIS_API URuntimeQuVRWorldInteraction : public UObject
{
	GENERATED_BODY()
public:	
	URuntimeQuVRWorldInteraction();
	
	/** Default destructor */
	virtual ~URuntimeQuVRWorldInteraction();

	/** Initialize extension */
	virtual void Init();

	/** Shut down extension when world is destroyed */
	virtual void Shutdown() {};

	/** Give base class the chance to tick */
	virtual void Tick(float DeltaSeconds) {};


	/** Gets the current Gizmo handle type */
	RuntimeQuVRtransformType::EQuVRGizmoHandleTypes GetCurrentGizmoType() const;

	/** Sets the current gizmo handle type */
	void SetGizmoHandleType(const RuntimeQuVRtransformType::EQuVRGizmoHandleTypes GizmoHandleType);

	/** Gets the color from color type */
	FLinearColor GetColor(const RuntimeQuVRtransformType::EQuVRColors Color, const float Multiplier = 1.f) const;

	/** Sets which transform gizmo coordinate space is used */
	void SetTransformGizmoCoordinateSpace(const RuntimeQuVRtransformType::EQuVRCoordSystem NewCoordSystem);

	/** Returns which transform gizmo coordinate space we're using, world or local */
	RuntimeQuVRtransformType::EQuVRCoordSystem GetTransformGizmoCoordinateSpace();

private:
	/** The current gizmo type */ //@todo ViewportInteraction: Currently this is only used for universal gizmo.
	TOptional<RuntimeQuVRtransformType::EQuVRGizmoHandleTypes> CurrentGizmoType;

	// All the colors for this mode
	TArray<FLinearColor> Colors;

	/** Candidate actors for aligning in the scene */
	TArray<const AActor*> CandidateActors;

	/** App time that we entered this */
	FTimespan AppTimeEntered;

	RuntimeQuVRtransformType::EQuVRCoordSystem CurrentCoordSystem;

};

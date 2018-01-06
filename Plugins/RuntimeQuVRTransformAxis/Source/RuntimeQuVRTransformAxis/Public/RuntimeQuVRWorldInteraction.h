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
	void SetGizmoHandleType(const RuntimeQuVRtransformType::EQuVRGizmoHandleTypes InGizmoHandleType);

	/** The current gizmo type */ //@todo ViewportInteraction: Currently this is only used for universal gizmo.
	TOptional<RuntimeQuVRtransformType::EQuVRGizmoHandleTypes> GizmoType;


private:

	// All the colors for this mode
	TArray<FLinearColor> Colors;

	/** Candidate actors for aligning in the scene */
	TArray<const AActor*> CandidateActors;

	/** App time that we entered this */
	FTimespan AppTimeEntered;
};

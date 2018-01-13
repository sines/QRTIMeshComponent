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

	/**
	* Begin tracking at the specified location for the specified viewport.
	*/
	void StartTracking( const int32 InX, const int32 InY);

	/**
	* Called when a mouse button has been released.  If there are no other
	* mouse buttons being held down, the internal information is reset.
	*/
	bool EndTracking();

	/**
	* Returns the current delta.
	*/
	const FVector GetMouseDelta() const;

	/**
	* Adds delta movement into the tracker.
	*/
	void AddMouseDelta(const float InDeltaX, const float InDeltaY);

	/**
	* Subtracts the specified value from End and EndSnapped.
	*/
	void ReduceBy(const FVector& In);
	
	/** Gets the current Gizmo handle type */
	RuntimeQuVRtransformType::EQuVRMode GetCurrentGizmoType() const;

	/** Sets the current gizmo handle type */
	void SetGizmoHandleType(const RuntimeQuVRtransformType::EQuVRMode GizmoHandleType);

	/** Gets the color from color type */
	FLinearColor GetColor(const RuntimeQuVRtransformType::EQuVRColors Color, const float Multiplier = 1.f) const;

	/** Sets which transform gizmo coordinate space is used */
	void SetTransformGizmoCoordinateSpace(const RuntimeQuVRtransformType::EQuVRCoordSystem NewCoordSystem);

	/** Returns which transform gizmo coordinate space we're using, world or local */
	RuntimeQuVRtransformType::EQuVRCoordSystem GetTransformGizmoCoordinateSpace();
public:
	/**
	* Used to store the required cursor visibility states and override cursor appearance
	*/
	struct FQuVRRequiredCursorState
	{
		/** Should the software cursor be visible */
		bool	bSoftwareCursorVisible;

		/** Should the hardware be visible */
		bool	bHardwareCursorVisible;

		/** Should the software cursor position be reset to pre-drag */
		bool	bDontResetCursor;

		/** Should we override the cursor appearance with the value in RequiredCursor */
		bool	bOverrideAppearance;

		/** What the cursor should look like */
		EMouseCursor::Type RequiredCursor;
	};
	FQuVRRequiredCursorState QuVRCursorState;
private:
	/** The current gizmo type */ //@todo ViewportInteraction: Currently this is only used for universal gizmo.
	TOptional<RuntimeQuVRtransformType::EQuVRMode> CurrentGizmoType;

	// All the colors for this mode
	TArray<FLinearColor> Colors;

	/** Candidate actors for aligning in the scene */
	TArray<const AActor*> CandidateActors;

	/** App time that we entered this */
	FTimespan AppTimeEntered;

	RuntimeQuVRtransformType::EQuVRCoordSystem CurrentCoordSystem;

	/** The unsnapped start position of the current mouse drag. */
	FVector Start;

	/** The unsnapped end position of the current mouse drag. */
	FVector End;
	FVector2D MouseDelta;

	float MouseSensitivty;
};

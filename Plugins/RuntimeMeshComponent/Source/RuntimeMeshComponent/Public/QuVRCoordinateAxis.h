// // Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

class UMaterialInterface;
class FSceneView;


/**
* Contains information about a mouse cursor position within a viewport, transformed into the correct coordinate
* system for the viewport.
*/
struct FQuViewportCursorLocation
{
public:
	FQuViewportCursorLocation(const FSceneView* View, int32 X, int32 Y);

	const FVector&		GetOrigin()			const { return Origin; }
	const FVector&		GetDirection()		const { return Direction; }
	const FIntPoint&	GetCursorPos()		const { return CursorPos; }

private:
	FVector	Origin;
	FVector	Direction;
	FIntPoint CursorPos;
};

/*FQuVRCoordinateAxis */
class RUNTIMEMESHCOMPONENT_API FQuVRCoordinateAxis :public FGCObject
{
public:
	
	enum EQuVRMode
	{
		WM_None = -1,
		WM_Translate,
		WM_TranslateRotateZ,
		WM_2D,
		WM_Rotate,
		WM_Scale,
		WM_Max,
	};


	enum
	{
		AXIS_ARROW_SEGMENTS = 16
	};


	/** Coordinate system identifiers. */
	enum EQuVRCoordSystem
	{
		COORD_None = -1,
		COORD_World,
		COORD_Local,
		COORD_Max,
	};

private:
	//////////////////////////////////////////////////////////////////////////
	// STRUCT DATA
	//
	//////////////////////////////////////////////////////////////////////////
	struct FQuVRAbsoluteMovementParams
	{
		/** The normal of the plane to project onto */
		FVector PlaneNormal;
		/** A vector that represent any displacement we want to mute (remove an axis if we're doing axis movement)*/
		FVector NormalToRemove;
		/** The current position of the widget */
		FVector Position;

		//Coordinate System Axes
		FVector XAxis;
		FVector YAxis;
		FVector ZAxis;

		//true if camera movement is locked to the object
		float bMovementLockedToCamera;

		//Direction in world space to the current mouse location
		FVector PixelDir;
		//Direction in world space of the middle of the camera
		FVector CameraDir;
		FVector EyePos;

		//whether to snap the requested positionto the grid
		bool bPositionSnapping;
	};

	struct FQuVRThickArcParams
	{
		FQuVRThickArcParams(const FVector& InPosition, UMaterialInterface* InMaterial, const float InInnerRadius, const float InOuterRadius)
			: Position(InPosition)
			, Material(InMaterial)
			, InnerRadius(InInnerRadius)
			, OuterRadius(InOuterRadius)
		{
		}

		/** The current position of the widget */
		FVector Position;

		//Material to use to render
		UMaterialInterface* Material;

		//Radii
		float InnerRadius;
		float OuterRadius;
	};
public:
	FQuVRCoordinateAxis();


public:
	// FUNCTION
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	/**
	* Converts mouse movement on the screen to widget axis movement/rotation.
	*/
	void ConvertMouseMovementToAxisMovement(bool bInUsedDragModifier, FVector& InDiff, FVector& OutDrag, FRotator& OutRotation, FVector& OutScale);

	/**
	* Absolute Translation conversion from mouse movement on the screen to widget axis movement/rotation.
	*/
	void AbsoluteTranslationConvertMouseMovementToAxisMovement(FSceneView* InView, const FVector& InLocation, const FVector2D& InMousePosition, FVector& OutDrag, FRotator& OutRotation, FVector& OutScale);

	/**
	* Grab the initial offset again first time input is captured
	*/
	void ResetInitialTranslationOffset(void)
	{
		bAbsoluteTranslationInitialOffsetCached = false;
	}

	/** Only some modes support Absolute Translation Movement.  Check current mode */
	static bool AllowsAbsoluteTranslationMovement(EQuVRMode WidgetMode);

	/**
	* Sets the default visibility of the widget, if it is not overridden by an active editor mode tool.
	*
	* @param	bInVisibility	true for visible
	*/
	void SetDefaultVisibility(bool bInDefaultVisibility)
	{
		bDefaultVisibility = bInDefaultVisibility;
	}

	/**
	* Sets the axis currently being moused over.  Typically called by FMouseDeltaTracker or FLevelEditorViewportClient.
	*
	* @param	InCurrentAxis	The new axis value.
	*/
	void SetCurrentAxis(EAxisList::Type InCurrentAxis)
	{
		CurrentAxis = InCurrentAxis;
	}

	/**
	* @return	The axis currently being moused over.
	*/
	EAxisList::Type GetCurrentAxis() const
	{
		return CurrentAxis;
	}

	/**
	* @return	The widget origin in viewport space.
	*/
	FVector2D GetOrigin() const
	{
		return Origin;
	}

	/**
	* @return	The mouse drag start position in viewport space.
	*/
	void SetDragStartPosition(const FVector2D& Position)
	{
		DragStartPos = Position;
	}

	/**
	* Returns whether we are actively dragging
	*/
	bool IsDragging(void) const
	{
		return bDragging;
	}

	/**
	* Sets if we are currently engaging the widget in dragging
	*/
	void SetDragging(const bool InDragging)
	{
		bDragging = InDragging;
	}

	/**
	* Sets if we are currently engaging the widget in dragging
	*/
	void SetSnapEnabled(const bool InSnapEnabled)
	{
		bSnapEnabled = InSnapEnabled;
	}

	/**
	* FGCObject interface: Serializes the widget reference so they dont get garbage collected.
	*
	* @param Ar	FArchive to serialize with
	*/
//	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;


	/** @return true if the widget is disabled */
	bool IsWidgetDisabled() const;

	/** Updates the delta rotation on the widget */
	void UpdateDeltaRotation();

	/** Resets the total delta rotation back to zero */
	void ResetDeltaRotation() { TotalDeltaRotation = 0; }

	/** @return the rotation speed of the widget */
	static float GetRotationSpeed() { return (2.f*(float)PI) / 360.f; }


private:

	/**
	* Snaps a point value to the positional grid
	*
	* @param Point	The point to snap.  This value will be modified to account for snapping
	* @param GridBase	Base grid offset
	*/
	void QuVRSnapPointToGrid(FVector& Point, const FVector& GridBase);


	/**
	* Snaps a rotator to the rotational grid
	*
	* @param Rotation	The rotator to snap.  This value will be modified to account for snapping
	*/
	 void QuVRSnapRotatorToGrid(FRotator& Rotation);

	//////////////////////////////////////////////////////////////////////////
	// FUNCTION
	//
	//////////////////////////////////////////////////////////////////////////

	/**
	* Returns the Delta from the current position that the absolute movement system wants the object to be at
	* @param InParams - Structure containing all the information needed for absolute movement
	* @return - The requested delta from the current position
	*/
	FVector GetAbsoluteTranslationDelta(const FQuVRAbsoluteMovementParams& InParams);
	/**
	* Returns the offset from the initial selection point
	*/
	FVector GetAbsoluteTranslationInitialOffset(const FVector& InNewPosition, const FVector& InCurrentPosition);

	/**
	* Returns true if we're in Local Space editing mode or editing BSP (which uses the World axes anyway
	*/
	bool IsRotationLocalSpace() const;

	/**
	* Returns how far we have just rotated
	*/
	float GetDeltaRotation() const;

	/**
	* If actively dragging, draws a ring representing the potential rotation of the selected objects, snap ticks, and "delta" markers
	* If not actively dragging, draws a quarter ring representing the closest quadrant to the camera
	* @param View - Information about the scene/camera/etc
	* @param PDI - Drawing interface
	* @param InAxis - Enumeration of axis to rotate about
	* @param InLocation - The Origin of the widget
	* @param Axis0 - The Axis that describes a 0 degree rotation
	* @param Axis1 - The Axis that describes a 90 degree rotation
	* @param InDirectionToWidget - Direction from camera to the widget
	* @param InColor - The color associated with the axis of rotation
	* @param InScale - Multiplier to maintain a constant screen size for rendering the widget
	* @param OutAxisDir - Viewport-space direction of rotation arc chord is placed here
	*/

	/**
	* Caches off HUD text to display after 3d rendering is complete
	* @param View - Information about the scene/camera/etc
	* @param PDI - Drawing interface
	* @param InLocation - The Origin of the widget
	* @param Axis0 - The Axis that describes a 0 degree rotation
	* @param Axis1 - The Axis that describes a 90 degree rotation
	* @param AngleOfAngle - angle we've rotated so far (in degrees)
	* @param InScale - Multiplier to maintain a constant screen size for rendering the widget
	*/
	void CacheRotationHUDText(const FVector& InLocation, const FVector& Axis0, const FVector& Axis1, const float AngleOfChange, const float InScale);

	/**
	* Gets the axis to use when converting mouse movement, accounting for Ortho views.
	*
	* @param InDiff Difference vector to determine dominant axis.
	* @param ViewportType Type of viewport for ortho checks.
	*
	* @return Index of the dominant axis.
	*/
	uint32 GetDominantAxisIndex(const FVector& InDiff) const;

	/** The axis currently being moused over */
	EAxisList::Type CurrentAxis;

	/** Viewport space origin location of the widget */
	FVector2D Origin;
	/** Viewport space direction vectors of the axes on the widget */
	FVector2D XAxisDir, YAxisDir, ZAxisDir;
	/** Drag start position in viewport space */
	FVector2D DragStartPos;

	/** Materials and colors to be used when drawing the items for each axis */
	UMaterialInterface* TransparentPlaneMaterialXY;
	UMaterialInterface* GridMaterial;

	UMaterialInstanceDynamic* AxisMaterialX;
	UMaterialInstanceDynamic* AxisMaterialY;
	UMaterialInstanceDynamic* AxisMaterialZ;
	UMaterialInstanceDynamic* CurrentAxisMaterial;
	UMaterialInstanceDynamic* OpaquePlaneMaterialXY;

	FLinearColor AxisColorX, AxisColorY, AxisColorZ;
	FColor PlaneColorXY, ScreenSpaceColor, CurrentColor;

	/************************************************************************/
	/*Axis 	WM_None,WM_Translate,WM_TranslateRotateZ,						*/
	/************************************************************************/
	EQuVRMode  MoveModeType;

	/**
	* An extra matrix to apply to the widget before drawing it (allows for local/custom coordinate systems).
	*/
	FMatrix CustomCoordSystem;

	/** The space of the custom coord system */
	EQuVRCoordSystem CustomCoordSystemSpace;

	//location in the viewport to render the hud string
	FVector2D HUDInfoPos;
	//string to be displayed on top of the viewport
	FString HUDString;

	/** Whether Absolute Translation cache position has been captured */
	bool bAbsoluteTranslationInitialOffsetCached;
	/** The initial offset where the widget was first clicked */
	FVector InitialTranslationOffset;
	/** The initial position of the widget before it was clicked */
	FVector InitialTranslationPosition;
	/** Whether or not the widget is actively dragging */
	bool bDragging;
	/** Whether or not snapping is enabled for all actors */
	bool bSnapEnabled;
	/** Default visibility for the widget if an Editor Mode Tool doesn't override it */
	bool bDefaultVisibility;
	/** Whether we are drawing the full ring in rotation mode (ortho viewports only) */
	bool bIsOrthoDrawingFullRing;

	/** Total delta rotation applied since the widget was dragged */
	float TotalDeltaRotation;

	/** Current delta rotation applied to the rotation widget */
	float CurrentDeltaRotation;

	//////////////////////////////////////////////////////////////////////////
	public:
		FPlane plane;

};
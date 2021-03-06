#include "RuntimeQuVRTransformAlgorithm.h"
#include "GCObject.h"
#include "CanvasTypes.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "EngineGlobals.h"
#include "Engine.h"
#include "SceneView.h"

using namespace RuntimeQuVRtransformType;

#define CAMERA_LOCK_DAMPING_FACTOR 0.1f
#define MAX_CAMERA_MOVEMENT_SPEED 512.0f

// CONST FLOAT
static const float AXIS_LENGTH = 35.0f;
static const float TRANSLATE_ROTATE_AXIS_CIRCLE_RADIUS = 20.0f;
static const float TWOD_AXIS_CIRCLE_RADIUS = 10.0f;
static const float INNER_AXIS_CIRCLE_RADIUS = 48.0f;
static const float OUTER_AXIS_CIRCLE_RADIUS = 56.0f;
static const float ROTATION_TEXT_RADIUS = 75.0f;
static const float AXIS_GRUDSIZE = 5.0f;
static const int32 AXIS_CIRCLE_SIDES = 24;

uint8 LargeInnerAlpha = 0x3f;
uint8 SmallInnerAlpha = 0x0f;
uint8 LargeOuterAlpha = 0x7f;
uint8 SmallOuterAlpha = 0x0f;

/**
* For axis movement, get the "best" planar normal and axis mask
* @param InAxis - Axis of movement
* @param InDirToPixel -
* @param OutPlaneNormal - Normal of the plane to project the mouse onto
* @param OutMask - Used to mask out the component of the planar movement we want
*/
void QuVRGetAxisPlaneNormalAndMask(const FMatrix& InCoordSystem, const FVector& InAxis, const FVector& InDirToPixel, FVector& OutPlaneNormal, FVector& NormalToRemove)
{
	FVector XAxis = InCoordSystem.TransformVector(FVector(1, 0, 0));
	FVector YAxis = InCoordSystem.TransformVector(FVector(0, 1, 0));
	FVector ZAxis = InCoordSystem.TransformVector(FVector(0, 0, 1));

	float XDot = FMath::Abs(InDirToPixel | XAxis);
	float YDot = FMath::Abs(InDirToPixel | YAxis);
	float ZDot = FMath::Abs(InDirToPixel | ZAxis);

	if ((InAxis | XAxis) > .1f)
	{
		OutPlaneNormal = (YDot > ZDot) ? YAxis : ZAxis;
		NormalToRemove = (YDot > ZDot) ? ZAxis : YAxis;
	}
	else if ((InAxis | YAxis) > .1f)
	{
		OutPlaneNormal = (XDot > ZDot) ? XAxis : ZAxis;
		NormalToRemove = (XDot > ZDot) ? ZAxis : XAxis;
	}
	else
	{
		OutPlaneNormal = (XDot > YDot) ? XAxis : YAxis;
		NormalToRemove = (XDot > YDot) ? YAxis : XAxis;
	}
}

/**
* For planar movement, get the "best" planar normal and axis mask
* @param InAxis - Axis of movement
* @param OutPlaneNormal - Normal of the plane to project the mouse onto
* @param OutMask - Used to mask out the component of the planar movement we want
*/
void QuVRGetPlaneNormalAndMask(const FVector& InAxis, FVector& OutPlaneNormal, FVector& NormalToRemove)
{
	OutPlaneNormal = InAxis;
	NormalToRemove = InAxis;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//	FViewportCursorLocation
//	Contains information about a mouse cursor position within a viewport, transformed into the correct
//	coordinate system for the viewport.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
FQuViewportCursorLocation::FQuViewportCursorLocation(const FSceneView* View, int32 X, int32 Y)
	: Origin(ForceInit), Direction(ForceInit), CursorPos(X, Y)
{

	FVector4 ScreenPos = View->PixelToScreen(X, Y, 0);

	const FMatrix InvViewMatrix = View->ViewMatrices.GetInvViewMatrix();
	const FMatrix InvProjMatrix = View->ViewMatrices.GetInvProjectionMatrix();

	const float ScreenX = ScreenPos.X;
	const float ScreenY = ScreenPos.Y;
	
	if (View->IsPerspectiveProjection())
	{
		Origin = View->ViewMatrices.GetViewOrigin();
		Direction = InvViewMatrix.TransformVector(FVector(InvProjMatrix.TransformFVector4(FVector4(ScreenX * GNearClippingPlane, ScreenY * GNearClippingPlane, 0.0f, GNearClippingPlane)))).GetSafeNormal();
	}
	else
	{
		Origin = InvViewMatrix.TransformFVector4(InvProjMatrix.TransformFVector4(FVector4(ScreenX, ScreenY, 0.5f, 1.0f)));
		Direction = InvViewMatrix.TransformVector(FVector(0, 0, 1)).GetSafeNormal();
	}
}


FRuntimeQuVRTransformAlgorithm::FRuntimeQuVRTransformAlgorithm()
{

	TotalDeltaRotation = 0;
	CurrentDeltaRotation = 0;

	AxisColorX = FLinearColor(0.594f,0.0197f,0.0f);
	AxisColorY = FLinearColor(0.1349f,0.3959f,0.0f);
	AxisColorZ = FLinearColor(0.0251f,0.207f,0.85f);
	PlaneColorXY = FColor::Yellow;
	ScreenSpaceColor = FColor(196, 196, 196);
	CurrentColor = FColor::Yellow;

	UMaterial* AxisMaterialBase = GEngine->ArrowMaterial;

	AxisMaterialX = UMaterialInstanceDynamic::Create(AxisMaterialBase, NULL);
	AxisMaterialX->SetVectorParameterValue("GizmoColor", AxisColorX);

	AxisMaterialY = UMaterialInstanceDynamic::Create(AxisMaterialBase, NULL);
	AxisMaterialY->SetVectorParameterValue("GizmoColor", AxisColorY);

	AxisMaterialZ = UMaterialInstanceDynamic::Create(AxisMaterialBase, NULL);
	AxisMaterialZ->SetVectorParameterValue("GizmoColor", AxisColorZ);

	CurrentAxisMaterial = UMaterialInstanceDynamic::Create(AxisMaterialBase, NULL);
	CurrentAxisMaterial->SetVectorParameterValue("GizmoColor", CurrentColor);



	OpaquePlaneMaterialXY = UMaterialInstanceDynamic::Create(AxisMaterialBase, NULL);
	OpaquePlaneMaterialXY->SetVectorParameterValue("GizmoColor", FLinearColor::White);

	TransparentPlaneMaterialXY = (UMaterial*)StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Engine/EditorMaterials/WidgetVertexColorMaterial.WidgetVertexColorMaterial"), NULL, LOAD_None, NULL);

	GridMaterial = (UMaterial*)StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Engine/EditorMaterials/WidgetGridVertexColorMaterial_Ma.WidgetGridVertexColorMaterial_Ma"), NULL, LOAD_None, NULL);
	if (!GridMaterial)
	{
		GridMaterial = TransparentPlaneMaterialXY;
	}

	CurrentAxis = EAxisList::X;

	CustomCoordSystem = FMatrix::Identity;
	CustomCoordSystemSpace = EQuVRCoordSystem::QuVR_COORD_World;

	bAbsoluteTranslationInitialOffsetCached = false;
	InitialTranslationOffset = FVector::ZeroVector;
	InitialTranslationPosition = FVector(0, 0, 0);

	bDragging = false;
	bSnapEnabled = true;
	bDefaultVisibility = true;
	bIsOrthoDrawingFullRing = false;

	Origin = FVector2D::ZeroVector;
	XAxisDir = FVector2D::ZeroVector;
	YAxisDir = FVector2D::ZeroVector;
	ZAxisDir = FVector2D::ZeroVector;
	DragStartPos = FVector2D::ZeroVector;
	ModeType = RuntimeQuVRtransformType::EQuVRMode::QuVR_WM_Translate;
}

/**
* Serializes the widget references so they don't get garbage collected.
*
* @param Ar	FArchive to serialize with
*/
void FRuntimeQuVRTransformAlgorithm::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(AxisMaterialX);
	Collector.AddReferencedObject(AxisMaterialY);
	Collector.AddReferencedObject(AxisMaterialZ);
	Collector.AddReferencedObject(OpaquePlaneMaterialXY);
	Collector.AddReferencedObject(TransparentPlaneMaterialXY);
	Collector.AddReferencedObject(GridMaterial);
	Collector.AddReferencedObject(CurrentAxisMaterial);
}

void FRuntimeQuVRTransformAlgorithm::QuVRSnapPointToGrid(FVector& Point, const FVector& GridBase)
{
	if (bSnapEnabled)
	{
		Point = (Point - GridBase).GridSnap(AXIS_GRUDSIZE) + GridBase;
	}
}

void FRuntimeQuVRTransformAlgorithm::QuVRSnapRotatorToGrid(FRotator& Rotation)
{
	if (bSnapEnabled)
	{
		Rotation = Rotation.GridSnap(FRotator(0.1, 0.1, 0.1));
	}
}

/**
* Converts mouse movement on the screen to widget axis movement/rotation.
*/

void FRuntimeQuVRTransformAlgorithm::ConvertMouseMovementToAxisMovement(const FVector2D& InOrigin, bool bInUsedDragModifier, FVector& InDiff, FVector& OutDrag, FRotator& OutRotation, FVector& OutScale)
{
	OutDrag = FVector::ZeroVector;
	OutRotation = FRotator::ZeroRotator;
	OutScale = FVector::ZeroVector;
	Origin = InOrigin;
	// Get input Delta as 2D vector, adjusted for inverted screen space y axis
	const FVector2D DragDir = FVector2D(InDiff.X, -InDiff.Y);
	
	// Get offset of the drag start position from the widget origin
	const FVector2D DirectionToMousePos = FVector2D(DragStartPos - Origin).GetSafeNormal();
	// For rotations which display as a full ring, calculate the tangent direction representing a clockwise movement

	FVector2D TangentDir;
	if (bInUsedDragModifier)
	{
		// If a drag modifier has been used, this implies we are not actually touching the widget, so don't attempt to
		// calculate the tangent dir based on the relative offset of the cursor from the widget location.
		TangentDir = FVector2D(1, 1).GetSafeNormal();
	}
	else
	{
		// Treat the tangent dir as perpendicular to the relative offset of the cursor from the widget location.
		TangentDir = FVector2D(-DirectionToMousePos.Y, DirectionToMousePos.X);
	}
	switch (ModeType)
	{
	case EQuVRMode::QuVR_WM_Translate:
		{
			// Get drag delta in widget axis space
			OutDrag = FVector(
				(CurrentAxis & EAxisList::X) ? FVector2D::DotProduct(XAxisDir, DragDir) : 0.0f,
				(CurrentAxis & EAxisList::Y) ? FVector2D::DotProduct(YAxisDir, DragDir) : 0.0f,
				(CurrentAxis & EAxisList::Z) ? FVector2D::DotProduct(ZAxisDir, DragDir) : 0.0f
			);

			// Snap to grid in widget axis space
			//<---------------------------------->
			QuVRSnapPointToGrid(OutDrag, FVector(AXIS_GRUDSIZE));

			// Convert to effective screen space delta, and replace input delta, adjusted for inverted screen space Y axis
			const FVector2D EffectiveDelta = OutDrag.X * XAxisDir + OutDrag.Y * YAxisDir + OutDrag.Z * ZAxisDir;
			InDiff = FVector(EffectiveDelta.X, -EffectiveDelta.Y, 0.0f);

			// Transform drag delta into world space
			OutDrag = CustomCoordSystem.TransformPosition(OutDrag);
		}
		break;
	case EQuVRMode::QuVR_WM_Rotate:
		{
			FRotator Rotation;
			FVector2D EffectiveDelta;

			if (CurrentAxis == EAxisList::X)
			{
					// Get screen direction representing positive rotation
					const FVector2D AxisDir = bIsOrthoDrawingFullRing ? TangentDir : XAxisDir;

					// Get rotation in widget local space
					Rotation = FRotator(0, 0, FVector2D::DotProduct(AxisDir, DragDir));
					// FSnappingUtils::SnapRotatorToGrid(Rotation);

					// Record delta rotation (used by the widget to render the accumulated delta)
					CurrentDeltaRotation = Rotation.Roll;

					// Use to calculate the new input delta
					EffectiveDelta = AxisDir * Rotation.Roll;
				}
				else if (CurrentAxis == EAxisList::Y)
				{
					// TODO: Determine why -TangentDir is necessary here, and fix whatever is causing it
					const FVector2D AxisDir = bIsOrthoDrawingFullRing ? -TangentDir : YAxisDir;

					Rotation = FRotator(FVector2D::DotProduct(AxisDir, DragDir), 0, 0);
					QuVRSnapRotatorToGrid(Rotation);

					CurrentDeltaRotation = Rotation.Pitch;
					EffectiveDelta = AxisDir * Rotation.Pitch;
				}
				else if (CurrentAxis == EAxisList::Z)
				{
					const FVector2D AxisDir = bIsOrthoDrawingFullRing ? TangentDir : ZAxisDir;

					Rotation = FRotator(0, 0.1/*FVector2D::DotProduct(AxisDir, DragDir)*/, 0);
					QuVRSnapRotatorToGrid(Rotation);

					CurrentDeltaRotation = Rotation.Yaw;
					EffectiveDelta = AxisDir * Rotation.Yaw;
				}

				// Adjust the input delta according to how much rotation was actually applied
				InDiff = FVector(EffectiveDelta.X, -EffectiveDelta.Y, 0.0f);

				// Need to get the delta rotation in the current coordinate space of the widget
				OutRotation = (CustomCoordSystem.Inverse() * FRotationMatrix(Rotation) * CustomCoordSystem).Rotator();
			}
		break;
	case EQuVRMode::QuVR_WM_Scale:
	{
		FVector2D AxisDir = FVector2D::ZeroVector;

		if (CurrentAxis & EAxisList::X)
		{
			AxisDir += XAxisDir;
		}

		if (CurrentAxis & EAxisList::Y)
		{
			AxisDir += YAxisDir;
		}

		if (CurrentAxis & EAxisList::Z)
		{
			AxisDir += ZAxisDir;
		}

		AxisDir.Normalize();
		const float ScaleDelta = FVector2D::DotProduct(AxisDir, DragDir);

		OutScale = FVector(
			(CurrentAxis & EAxisList::X) ? ScaleDelta : 0.0f,
			(CurrentAxis & EAxisList::Y) ? ScaleDelta : 0.0f,
			(CurrentAxis & EAxisList::Z) ? ScaleDelta : 0.0f
		);

		// Snap to grid in widget axis space
		const FVector GridSize = FVector(5);
		// FSnappingUtils::SnapScale(OutScale, GridSize);

		// Convert to effective screen space delta, and replace input delta, adjusted for inverted screen space Y axis
		const float ScaleMax = OutScale.GetMax();
		const float ScaleMin = OutScale.GetMin();
		const float ScaleApplied = (ScaleMax > -ScaleMin) ? ScaleMax : ScaleMin;
		const FVector2D EffectiveDelta = AxisDir * ScaleApplied;
		InDiff = FVector(EffectiveDelta.X, -EffectiveDelta.Y, 0.0f);
	}
		break;
	case EQuVRMode::QuVR_WM_TranslateRotateZ:
	{
		if (CurrentAxis == EAxisList::ZRotation)
		{

			const FVector2D AxisDir = bIsOrthoDrawingFullRing ? TangentDir : ZAxisDir;
			FRotator Rotation = FRotator(0, FVector2D::DotProduct(AxisDir, DragDir), 0);
			QuVRSnapRotatorToGrid(Rotation);
			CurrentDeltaRotation = Rotation.Yaw;

			const FVector2D EffectiveDelta = AxisDir * Rotation.Yaw;
			InDiff = FVector(EffectiveDelta.X, -EffectiveDelta.Y, 0.0f);

			OutRotation = (CustomCoordSystem.Inverse() * FRotationMatrix(Rotation) * CustomCoordSystem).Rotator();
		}
		else
		{
			// Get drag delta in widget axis space
			OutDrag = FVector(
				(CurrentAxis & EAxisList::X) ? FVector2D::DotProduct(XAxisDir, DragDir) : 0.0f,
				(CurrentAxis & EAxisList::Y) ? FVector2D::DotProduct(YAxisDir, DragDir) : 0.0f,
				(CurrentAxis & EAxisList::Z) ? FVector2D::DotProduct(ZAxisDir, DragDir) : 0.0f
			);

			// Snap to grid in widget axis space
			QuVRSnapPointToGrid(OutDrag, FVector(AXIS_GRUDSIZE));

			// Convert to effective screen space delta, and replace input delta, adjusted for inverted screen space Y axis
			const FVector2D EffectiveDelta = OutDrag.X * XAxisDir + OutDrag.Y * YAxisDir + OutDrag.Z * ZAxisDir;
			InDiff = FVector(EffectiveDelta.X, -EffectiveDelta.Y, 0.0f);

			// Transform drag delta into world space
			OutDrag = CustomCoordSystem.TransformPosition(OutDrag);
		}
	}
	break;

	case EQuVRMode::QuVR_WM_2D:
	{
		if (CurrentAxis == EAxisList::Rotate2D)
		{
			// TODO: Determine why -TangentDir is necessary here, and fix whatever is causing it
			const FVector2D AxisDir = bIsOrthoDrawingFullRing ? -TangentDir : YAxisDir;

			FRotator Rotation = FRotator(FVector2D::DotProduct(AxisDir, DragDir), 0, 0);
			QuVRSnapRotatorToGrid(Rotation);

			CurrentDeltaRotation = Rotation.Pitch;
			FVector2D EffectiveDelta = AxisDir * Rotation.Pitch;


			// Adjust the input delta according to how much rotation was actually applied
			InDiff = FVector(EffectiveDelta.X, -EffectiveDelta.Y, 0.0f);

			// Need to get the delta rotation in the current coordinate space of the widget
			OutRotation = (CustomCoordSystem.Inverse() * FRotationMatrix(Rotation) * CustomCoordSystem).Rotator();
		}
		else
		{
			// Get drag delta in widget axis space
			OutDrag = FVector(
				(CurrentAxis & EAxisList::X) ? FVector2D::DotProduct(XAxisDir, DragDir) : 0.0f,
				(CurrentAxis & EAxisList::Y) ? FVector2D::DotProduct(YAxisDir, DragDir) : 0.0f,
				(CurrentAxis & EAxisList::Z) ? FVector2D::DotProduct(ZAxisDir, DragDir) : 0.0f
			);

			// Snap to grid in widget axis space
			QuVRSnapPointToGrid(OutDrag, FVector(AXIS_GRUDSIZE));

			// Convert to effective screen space delta, and replace input delta, adjusted for inverted screen space Y axis
			const FVector2D EffectiveDelta = OutDrag.X * XAxisDir + OutDrag.Y * YAxisDir + OutDrag.Z * ZAxisDir;
			InDiff = FVector(EffectiveDelta.X, -EffectiveDelta.Y, 0.0f);

			// Transform drag delta into world space
			OutDrag = CustomCoordSystem.TransformPosition(OutDrag);
		}
	}
	break;

	default:
		break;
	}
}


void FRuntimeQuVRTransformAlgorithm::AbsoluteTranslationConvertMouseMovementToAxisMovement(FSceneView* InView, const FVector& InLocation, const FVector2D& InMousePosition, FVector& OutDrag, FRotator& OutRotation, FVector& OutScale)
{
	// Compute a world space ray from the screen space mouse coordinates
	FQuViewportCursorLocation MouseViewportRay(InView, InMousePosition.X, InMousePosition.Y);

	FQuVRAbsoluteMovementParams Params;
	Params.EyePos = MouseViewportRay.GetOrigin();
	Params.PixelDir = MouseViewportRay.GetDirection();
	Params.CameraDir = InView->GetViewDirection();
	Params.Position = InLocation;
	//dampen by 
	Params.bMovementLockedToCamera = false;
	Params.bPositionSnapping = true;

	FMatrix InputCoordSystem =FMatrix::Identity;

	Params.XAxis = InputCoordSystem.TransformVector(FVector(1, 0, 0));
	Params.YAxis = InputCoordSystem.TransformVector(FVector(0, 1, 0));
	Params.ZAxis = InputCoordSystem.TransformVector(FVector(0, 0, 1));

	switch (ModeType)
	{
	case EQuVRMode::QuVR_WM_Translate:
	{
		switch (CurrentAxis)
		{
		case EAxisList::X:  QuVRGetAxisPlaneNormalAndMask(InputCoordSystem, Params.XAxis, Params.CameraDir, Params.PlaneNormal, Params.NormalToRemove); break;
		case EAxisList::Y:  QuVRGetAxisPlaneNormalAndMask(InputCoordSystem, Params.YAxis, Params.CameraDir, Params.PlaneNormal, Params.NormalToRemove); break;
		case EAxisList::Z:  QuVRGetAxisPlaneNormalAndMask(InputCoordSystem, Params.ZAxis, Params.CameraDir, Params.PlaneNormal, Params.NormalToRemove); break;
		case EAxisList::XY: QuVRGetPlaneNormalAndMask(Params.ZAxis, Params.PlaneNormal, Params.NormalToRemove); break;
		case EAxisList::XZ: QuVRGetPlaneNormalAndMask(Params.YAxis, Params.PlaneNormal, Params.NormalToRemove); break;
		case EAxisList::YZ: QuVRGetPlaneNormalAndMask(Params.XAxis, Params.PlaneNormal, Params.NormalToRemove); break;
		case EAxisList::Screen:
			Params.XAxis = InView->ViewMatrices.GetViewMatrix().GetColumn(0);
			Params.YAxis = InView->ViewMatrices.GetViewMatrix().GetColumn(1);
			Params.ZAxis = InView->ViewMatrices.GetViewMatrix().GetColumn(2);
			QuVRGetPlaneNormalAndMask(Params.ZAxis, Params.PlaneNormal, Params.NormalToRemove); break;
			break;
		}

		OutDrag = GetAbsoluteTranslationDelta(Params);

		break;
	}

	case EQuVRMode::QuVR_WM_2D:
	{
		switch (CurrentAxis)
		{
		case EAxisList::X:
		{
			QuVRGetAxisPlaneNormalAndMask(InputCoordSystem, Params.XAxis, Params.CameraDir, Params.PlaneNormal, Params.NormalToRemove);
			OutDrag = GetAbsoluteTranslationDelta(Params);
			break;
		}
		case EAxisList::Z:
		{
			QuVRGetAxisPlaneNormalAndMask(InputCoordSystem, Params.ZAxis, Params.CameraDir, Params.PlaneNormal, Params.NormalToRemove);
			OutDrag = GetAbsoluteTranslationDelta(Params);
			break;
		}
		case EAxisList::XZ:
		{
			QuVRGetPlaneNormalAndMask(Params.YAxis, Params.PlaneNormal, Params.NormalToRemove);
			OutDrag = GetAbsoluteTranslationDelta(Params);
			break;
		}

		//Rotate about the y-axis
		case EAxisList::Rotate2D:

		{
			//no position snapping, we'll handle the rotation snapping elsewhere
			Params.bPositionSnapping = false;

			QuVRGetPlaneNormalAndMask(Params.YAxis, Params.PlaneNormal, Params.NormalToRemove);
			//No DAMPING
			Params.bMovementLockedToCamera = false;
			//this is the one movement type where we want to always use the widget origin and 
			//NOT the "first click" origin
			FVector XZPlaneProjectedPosition = GetAbsoluteTranslationDelta(Params) + InitialTranslationOffset;

			//remove the component along the normal we want to mute
			float MovementAlongMutedAxis = XZPlaneProjectedPosition | Params.NormalToRemove;
			XZPlaneProjectedPosition = XZPlaneProjectedPosition - (Params.NormalToRemove*MovementAlongMutedAxis);

			if (!XZPlaneProjectedPosition.Normalize())
			{
				XZPlaneProjectedPosition = Params.YAxis;
			}

			//NOW, find the rotation around the PlaneNormal to make the xaxis point at InDrag
			OutRotation = FRotator::ZeroRotator;

			float PitchDegrees = -FMath::Atan2(-XZPlaneProjectedPosition.Z, XZPlaneProjectedPosition.X) * 180.f / PI;
			OutRotation.Pitch = PitchDegrees - (0.2f);

			if (bSnapEnabled)
			{
				QuVRSnapRotatorToGrid(OutRotation);
			}

			break;
		}
		}

		break;
	}

	case EQuVRMode::QuVR_WM_TranslateRotateZ:
	{
		FVector LineToUse;
		switch (CurrentAxis)
		{
		case EAxisList::X:
		{
			QuVRGetAxisPlaneNormalAndMask(InputCoordSystem, Params.XAxis, Params.CameraDir, Params.PlaneNormal, Params.NormalToRemove);
			OutDrag = GetAbsoluteTranslationDelta(Params);
			break;
		}
		case EAxisList::Y:
		{
			QuVRGetAxisPlaneNormalAndMask(InputCoordSystem, Params.YAxis, Params.CameraDir, Params.PlaneNormal, Params.NormalToRemove);
			OutDrag = GetAbsoluteTranslationDelta(Params);
			break;
		}
		case EAxisList::Z:
		{
			QuVRGetAxisPlaneNormalAndMask(InputCoordSystem, Params.ZAxis, Params.CameraDir, Params.PlaneNormal, Params.NormalToRemove);
			OutDrag = GetAbsoluteTranslationDelta(Params);
			break;
		}
		case EAxisList::XY:
		{
			QuVRGetPlaneNormalAndMask(Params.ZAxis, Params.PlaneNormal, Params.NormalToRemove);
			OutDrag = GetAbsoluteTranslationDelta(Params);
			break;
		}
		//Rotate about the z-axis
		case EAxisList::ZRotation:
		{
			//no position snapping, we'll handle the rotation snapping elsewhere
			Params.bPositionSnapping = false;

			//find new point on the 
			QuVRGetPlaneNormalAndMask(Params.ZAxis, Params.PlaneNormal, Params.NormalToRemove);
			//No DAMPING
			Params.bMovementLockedToCamera = false;
			//this is the one movement type where we want to always use the widget origin and 
			//NOT the "first click" origin
			FVector XYPlaneProjectedPosition = GetAbsoluteTranslationDelta(Params) + InitialTranslationOffset;

			//remove the component along the normal we want to mute
			float MovementAlongMutedAxis = XYPlaneProjectedPosition | Params.NormalToRemove;
			XYPlaneProjectedPosition = XYPlaneProjectedPosition - (Params.NormalToRemove*MovementAlongMutedAxis);

			if (!XYPlaneProjectedPosition.Normalize())
			{
				XYPlaneProjectedPosition = Params.XAxis;
			}

			//NOW, find the rotation around the PlaneNormal to make the xaxis point at InDrag
			OutRotation = FRotator::ZeroRotator;

			OutRotation.Yaw = XYPlaneProjectedPosition.Rotation().Yaw - (0.25f);

			if (bSnapEnabled)
			{
				QuVRSnapRotatorToGrid(OutRotation);
			}

			break;
		}
		default:
			break;
		}
	}

	case EQuVRMode::QuVR_WM_Rotate:
	case EQuVRMode::QuVR_WM_Scale:
		break;
	}
}

FVector FRuntimeQuVRTransformAlgorithm::GetAbsoluteTranslationDelta(const FQuVRAbsoluteMovementParams& InParams)
{

	FPlane MovementPlane(InParams.Position, InParams.PlaneNormal);
	FVector ProposedEndofEyeVector = InParams.EyePos + (InParams.PixelDir * (InParams.Position - InParams.EyePos).Size());

	//default to not moving
	FVector RequestedPosition = InParams.Position;

	float DotProductWithPlaneNormal = InParams.PixelDir | InParams.PlaneNormal;
	//check to make sure we're not co-planar
	if (FMath::Abs(DotProductWithPlaneNormal) > DELTA)
	{
		//Get closest point on plane
		RequestedPosition = FMath::LinePlaneIntersection(InParams.EyePos, ProposedEndofEyeVector, MovementPlane);
	}

	//drag is a delta position, so just update the different between the previous position and the new position
	FVector DeltaPosition = RequestedPosition - InParams.Position;

	//Retrieve the initial offset, passing in the current requested position and the current position
	FVector InitialOffset = GetAbsoluteTranslationInitialOffset(RequestedPosition, InParams.Position);

	// Log
	//subtract off the initial offset (where the widget was clicked) to prevent popping
	DeltaPosition -= InitialOffset;


	//remove the component along the normal we want to mute
	float MovementAlongMutedAxis = DeltaPosition | InParams.NormalToRemove;
	FVector OutDrag = DeltaPosition - (InParams.NormalToRemove*MovementAlongMutedAxis);
	if (InParams.bMovementLockedToCamera)
	{
		//DAMPEN ABSOLUTE MOVEMENT when the camera is locked to the object
		OutDrag *= CAMERA_LOCK_DAMPING_FACTOR;
		OutDrag.X = FMath::Clamp(OutDrag.X, -MAX_CAMERA_MOVEMENT_SPEED, MAX_CAMERA_MOVEMENT_SPEED);
		OutDrag.Y = FMath::Clamp(OutDrag.Y, -MAX_CAMERA_MOVEMENT_SPEED, MAX_CAMERA_MOVEMENT_SPEED);
		OutDrag.Z = FMath::Clamp(OutDrag.Z, -MAX_CAMERA_MOVEMENT_SPEED, MAX_CAMERA_MOVEMENT_SPEED);
	}

	//the they requested position snapping and we're not moving with the camera
	if (InParams.bPositionSnapping && !InParams.bMovementLockedToCamera && bSnapEnabled)
	{
		FVector MovementAlongAxis = FVector(OutDrag | InParams.XAxis, OutDrag | InParams.YAxis, OutDrag | InParams.ZAxis);
		//translation (either xy plane or z)
		QuVRSnapPointToGrid(MovementAlongAxis, FVector(AXIS_GRUDSIZE));
		OutDrag = MovementAlongAxis.X*InParams.XAxis + MovementAlongAxis.Y*InParams.YAxis + MovementAlongAxis.Z*InParams.ZAxis;
	}
	//get the distance from the original position to the new proposed position 
	FVector DeltaFromStart = InParams.Position + OutDrag - InitialTranslationPosition;

	//Get the vector from the eye to the proposed new position (to make sure it's not behind the camera
	FVector EyeToNewPosition = (InParams.Position + OutDrag) - InParams.EyePos;
	float BehindTheCameraDotProduct = EyeToNewPosition | InParams.CameraDir;

	//Don't let the requested position go behind the camera
	if (BehindTheCameraDotProduct <= 0)
	{
		OutDrag = OutDrag.ZeroVector;
	}

	plane = MovementPlane;
	return OutDrag;
}


/**
* Returns the offset from the initial selection point
*/
FVector FRuntimeQuVRTransformAlgorithm::GetAbsoluteTranslationInitialOffset(const FVector& InNewPosition, const FVector& InCurrentPosition)
{
	if (!bAbsoluteTranslationInitialOffsetCached)
	{
		bAbsoluteTranslationInitialOffsetCached = true;
		InitialTranslationOffset = InNewPosition - InCurrentPosition;
		InitialTranslationPosition = InCurrentPosition;
	}
	return InitialTranslationOffset;
}



/**
* Returns true if we're in Local Space editing mode
*/
bool FRuntimeQuVRTransformAlgorithm::IsRotationLocalSpace() const
{
	return (CustomCoordSystemSpace == EQuVRCoordSystem::QuVR_COORD_Local);
}

void FRuntimeQuVRTransformAlgorithm::UpdateDeltaRotation()
{
	TotalDeltaRotation += CurrentDeltaRotation;
	if ((TotalDeltaRotation <= -360.f) || (TotalDeltaRotation >= 360.f))
	{
		TotalDeltaRotation = FRotator::ClampAxis(TotalDeltaRotation);
	}
}

void FRuntimeQuVRTransformAlgorithm::GetRotationArc(const FSceneView* View, EQuVRGizmoHandleHoveredTypes InAxis,const FVector& InLocation, const FVector& InDirectionToWidget, const float InScale)
{
	//get the axes 
	FVector XAxis = CustomCoordSystem.TransformVector(FVector(1, 0, 0));
	FVector YAxis = CustomCoordSystem.TransformVector(FVector(0, 1, 0));
	FVector ZAxis = CustomCoordSystem.TransformVector(FVector(0, 0, 1));

	FVector& Axis0 = ZAxis;
	FVector& Axis1 = YAxis;
	switch (InAxis)
	{
	case EQuVRGizmoHandleHoveredTypes::QUVR_X:
		Axis0 = ZAxis;
		Axis1 = YAxis;
		break;
	case EQuVRGizmoHandleHoveredTypes::QUVR_Y:
		Axis0 = XAxis;
		Axis1 = ZAxis;
		break;
	case EQuVRGizmoHandleHoveredTypes::QUVR_Z:
		Axis0 = XAxis;
		Axis1 = YAxis;
		break;
	}

	bool bIsPerspective = (View->ViewMatrices.GetProjectionMatrix().M[3][3] < 1.0f);
	bool bIsOrtho = !bIsPerspective;

	//if we're in an ortho viewport and the ring is perpendicular to the camera (both Axis0 & Axis1 are perpendicular)
	bIsOrthoDrawingFullRing |= bIsOrtho && (FMath::Abs(Axis0 | InDirectionToWidget) < KINDA_SMALL_NUMBER) && (FMath::Abs(Axis1 | InDirectionToWidget) < KINDA_SMALL_NUMBER);

	if (bDragging || (bIsOrthoDrawingFullRing))
	{
	}
	else
	{
		//Reverse the axes based on camera view
		bool bMirrorAxis0 = ((Axis0 | InDirectionToWidget) <= 0.0f);
		bool bMirrorAxis1 = ((Axis1 | InDirectionToWidget) <= 0.0f);

		FVector RenderAxis0 = bMirrorAxis0 ? Axis0 : -Axis0;
		FVector RenderAxis1 = bMirrorAxis1 ? Axis1 : -Axis1;
		float Direction = (bMirrorAxis0 ^ bMirrorAxis1) ? -1.0f : 1.0f;

		FVector2D Axis0ScreenLocation;
		if (!View->ScreenToPixel(View->WorldToScreen(InLocation + RenderAxis0 * 64.0f), Axis0ScreenLocation))
		{
			Axis0ScreenLocation.X = Axis0ScreenLocation.Y = 0;
		}

		FVector2D Axis1ScreenLocation;
		if (!View->ScreenToPixel(View->WorldToScreen(InLocation + RenderAxis1 * 64.0f), Axis1ScreenLocation))
		{
			Axis1ScreenLocation.X = Axis1ScreenLocation.Y = 0;
		}
		switch (InAxis)
		{
		case EQuVRGizmoHandleHoveredTypes::QUVR_X:
			XAxisDir = ((Axis1ScreenLocation - Axis0ScreenLocation) * Direction).GetSafeNormal();
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString("Begin++++++++++++OnHover_QUVR_X ") + XAxisDir.ToString());
			break;
		case EQuVRGizmoHandleHoveredTypes::QUVR_Y:
			YAxisDir = ((Axis1ScreenLocation - Axis0ScreenLocation) * Direction).GetSafeNormal();
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString("Begin++++++++++++OnHover_QUVR_Y ") + YAxisDir.ToString());
			break;
		case EQuVRGizmoHandleHoveredTypes::QUVR_Z:
			ZAxisDir = ((Axis1ScreenLocation - Axis0ScreenLocation) * Direction).GetSafeNormal();
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString("Begin++++++++++++OnHover_QUVR_Y ") + YAxisDir.ToString());
			break;
		}
	}
}

/**
* Returns the angle in degrees representation of how far we have just rotated
*/
float FRuntimeQuVRTransformAlgorithm::GetDeltaRotation() const
{
	return TotalDeltaRotation;
}

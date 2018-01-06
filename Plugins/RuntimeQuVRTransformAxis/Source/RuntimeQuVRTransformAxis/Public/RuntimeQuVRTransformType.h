// Copyright 2016 Chris Conway (Koderz). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
/**
 * 
 */
namespace RuntimeQuVRtransformType
{
	/** Methods of dragging objects around in VR */
	enum class EQuVRMode
	{
		QuVR_WM_None = -1,
		QuVR_WM_Translate,
		QuVR_WM_TranslateRotateZ,
		QuVR_WM_2D,
		QuVR_WM_Rotate,
		QuVR_WM_Scale,
		QuVR_WM_Max,
	};

	/** Coordinate system identifiers. */
	enum class EQuVRCoordSystem
	{
		QuVR_COORD_None = -1,
		QuVR_COORD_World,
		QuVR_COORD_Local,
		QuVR_COORD_Max,
	};


	/* Directions that a transform handle can face along any given axis */
	enum class EQuVRTransformGizmoHandleDirection
	{
		QUVR_Negative,
		QUVR_Center,
		QUVR_Positive,
	};

	enum class EQuVRGizmoHandleTypes : uint8
	{
		QUVR_All = 0,
		QUVR_Translate = 1,
		QUVR_Rotate = 2,
		QUVR_Scale = 3
	};

	enum class EQuVRGizmoHandleHoveredTypes
	{
		QUVR_VOID,
		QUVR_X,
		QUVR_Y,
		QUVR_Z
	};

	enum class EQuVRColors
	{
		DefaultColor,
		Forward,
		Right,
		Up,
		GizmoHover,
		GizmoDragging,
		TotalCount
	};
}
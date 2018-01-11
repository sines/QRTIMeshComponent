// Copyright 2018 Louis. All Rights Reserved.

#include "RuntimeQuVRWorldInteraction.h"
#include "RuntimeQuVRTransformType.h"

using namespace RuntimeQuVRtransformType;

URuntimeQuVRWorldInteraction::URuntimeQuVRWorldInteraction():Super(),
								AppTimeEntered(FTimespan::Zero()),
								CurrentCoordSystem(EQuVRCoordSystem::QuVR_COORD_World)
{
	Init();
}

URuntimeQuVRWorldInteraction::~URuntimeQuVRWorldInteraction()
{

}

void URuntimeQuVRWorldInteraction::Init()
{
	Colors.SetNumZeroed((int32)EQuVRColors::TotalCount);
	{
		Colors[(int32)EQuVRColors::DefaultColor] = FLinearColor(0.7f, 0.7f, 0.7f, 1.0f);
		Colors[(int32)EQuVRColors::Forward] = FLinearColor(0.594f, 0.0197f, 0.0f, 1.0f);
		Colors[(int32)EQuVRColors::Right] = FLinearColor(0.1349f, 0.3959f, 0.0f, 1.0f);
		Colors[(int32)EQuVRColors::Up] = FLinearColor(0.0251f, 0.207f, 0.85f, 1.0f);
		Colors[(int32)EQuVRColors::GizmoHover] = FLinearColor::Yellow;
		Colors[(int32)EQuVRColors::GizmoDragging] = FLinearColor::Yellow;
	}

	AppTimeEntered = FTimespan::FromSeconds(FApp::GetCurrentTime());
}

FLinearColor URuntimeQuVRWorldInteraction::GetColor(const EQuVRColors Color, const float Multiplier /*= 1.f*/) const
{
	return Colors[(int32)Color] * Multiplier;
}


EQuVRCoordSystem URuntimeQuVRWorldInteraction::GetTransformGizmoCoordinateSpace()
{
	return CurrentCoordSystem;
}

void URuntimeQuVRWorldInteraction::SetTransformGizmoCoordinateSpace(const EQuVRCoordSystem NewCoordSystem)
{
	CurrentCoordSystem = NewCoordSystem;
}
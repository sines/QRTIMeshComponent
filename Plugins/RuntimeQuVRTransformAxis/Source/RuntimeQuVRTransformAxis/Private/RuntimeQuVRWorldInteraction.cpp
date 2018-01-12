// Copyright 2018 Louis. All Rights Reserved.

#include "RuntimeQuVRWorldInteraction.h"
#include "RuntimeQuVRTransformType.h"
#include "InputCoreTypes.h"

using namespace RuntimeQuVRtransformType;

URuntimeQuVRWorldInteraction::URuntimeQuVRWorldInteraction():Super(),
								AppTimeEntered(FTimespan::Zero()),
								MouseDelta(FVector2D::ZeroVector),
								MouseSensitivty(0.5),
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

void URuntimeQuVRWorldInteraction::StartTracking(const int32 InX, const int32 InY)
{
	Start = FVector(InX, InY, 0);
	End = Start;
}

bool URuntimeQuVRWorldInteraction::EndTracking()
{
	Start =  End =  FVector::ZeroVector;
	MouseDelta = FVector2D::ZeroVector;
	return true;
}

const FVector URuntimeQuVRWorldInteraction::GetMouseDelta() const
{
	const FVector Delta(End - Start);
	return Delta;
}

void URuntimeQuVRWorldInteraction::AddMouseDelta(const float InDeltaX, const float InDeltaY)
{
	FVector Wk = FVector(InDeltaX, InDeltaY, 0);
	Wk *= MouseSensitivty;
 	End += Wk;
}


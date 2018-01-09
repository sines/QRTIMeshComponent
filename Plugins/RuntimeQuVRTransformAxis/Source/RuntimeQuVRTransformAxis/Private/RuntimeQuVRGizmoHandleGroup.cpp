// Copyright 2018 Louis. All Rights Reserved.

#include "RuntimeQuVRGizmoHandleGroup.h"
#include "RuntimeQuVRHandleMeshComponent.h"
#include "RuntimeQuVRAssetContainer.h"
#include "RuntimeQuVRWorldInteraction.h"
#include "RuntimeQuVRTransformAxisActor.h"

#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/WorldSettings.h"

using namespace RuntimeQuVRtransformType;


namespace QuVR //@todo QuVR: Duplicates of TransformGizmo
{
	// @todo vreditor tweak: Tweak out console variables
	static FAutoConsoleVariable QuVRPivotGizmoMinDistanceForScaling(TEXT("QuVR.PivotGizmoMinDistanceForScaling"), 0.0f, TEXT("How far away the camera needs to be from an object before we'll start scaling it based on distance"));
	static FAutoConsoleVariable QuVRPivotGizmoDistanceScaleFactor(TEXT("QuVR.PivotGizmoDistanceScaleFactor"), 0.0035f, TEXT("How much the gizmo handles should increase in size with distance from the camera, to make it easier to select"));
	static FAutoConsoleVariable QuVRPivotGizmoTranslationPivotOffsetX(TEXT("QuVR.PivotGizmoTranslationPivotOffsetX"), 30.0f, TEXT("How much the translation cylinder is offsetted from the pivot"));
	static FAutoConsoleVariable QuVRPivotGizmoScalePivotOffsetX(TEXT("QuVR.PivotGizmoScalePivotOffsetX"), 120.0f, TEXT("How much the non-uniform scale is offsetted from the pivot"));
	static FAutoConsoleVariable QuVRPivotGizmoPlaneTranslationPivotOffsetYZ(TEXT("QuVR.PivotGizmoPlaneTranslationPivotOffsetYZ"), 40.0f, TEXT("How much the plane translation is offsetted from the pivot"));
	static FAutoConsoleVariable QuVRPivotGizmoTranslationScaleMultiply(TEXT("QuVR.PivotGizmoTranslationScaleMultiply"), 2.0f, TEXT("Multiplies translation handles scale"));
	static FAutoConsoleVariable QuVRPivotGizmoTranslationHoverScaleMultiply(TEXT("QuVR.PivotGizmoTranslationHoverScaleMultiply"), 0.75f, TEXT("Multiplies translation handles hover scale"));
	static FAutoConsoleVariable QuVRPivotGizmoAimAtShrinkSize(TEXT("QuVR.PivotGizmoAimAtShrinkSize"), 0.3f, TEXT("The minimum size when not aiming at the gizmo (0 to 1)"));
	static FAutoConsoleVariable QuVRPivotGizmoAimAtAnimationSpeed(TEXT("QuVR.PivotGizmoAimAtAnimationSpeed"), 0.15f, TEXT("The speed to animate to the gizmo full size when aiming at it"));
	static FAutoConsoleVariable QuVRGizmoSelectionAnimationDuration(TEXT("QuVR.GizmoSelectionAnimationDuration"), 0.15f, TEXT("How long to animate the gizmo after objects are selected"));
	static FAutoConsoleVariable QuVRGizmoSelectionAnimationCurvePower(TEXT("QuVR.GizmoSelectionAnimationCurvePower"), 2.0f, TEXT("Controls the animation curve for the gizmo after objects are selected"));
	static FAutoConsoleVariable QuVRGizmoShowMeasurementText(TEXT("QuVR.GizmoShowMeasurementText"), 0, TEXT("When enabled, gizmo measurements will always be visible.  Otherwise, only when hovering over a scale/stretch gizmo handle"));

}


URuntimeQuVRGizmoHandleGroup::URuntimeQuVRGizmoHandleGroup()
	: Super(),
	GizmoMaterial(nullptr),
	TranslucentGizmoMaterial(nullptr),
	Handles(),
	QuVROwningTransformGizmoActor(nullptr),
	bShowOnUniversalGizmo(true),
	DragActor(nullptr),
	DraggingTransformGizmoComponent(nullptr),
	HoveringOverTransformGizmoComponent(nullptr)
{
	eQuVRHandleHoveredType = EQuVRGizmoHandleHoveredTypes::QUVR_VOID;
//	DragOperationComponent = CreateDefaultSubobject<UViewportDragOperationComponent>(TEXT("DragOperation"));
}

FQuVRTransformGizmoHandlePlacement URuntimeQuVRGizmoHandleGroup::MakeHandlePlacementForIndex(const int32 HandleIndex) const
{
	FQuVRTransformGizmoHandlePlacement HandlePlacement;
	HandlePlacement.Axes[0] = (EQuVRTransformGizmoHandleDirection)(HandleIndex / 9);
	HandlePlacement.Axes[1] = (EQuVRTransformGizmoHandleDirection)((HandleIndex % 9) / 3);
	HandlePlacement.Axes[2] = (EQuVRTransformGizmoHandleDirection)((HandleIndex % 9) % 3);
	//	GWarn->Logf( TEXT( "%i = HandlePlacment[ %i %i %i ]" ), HandleIndex, (int32)HandlePlacement.Axes[ 0 ], (int32)HandlePlacement.Axes[ 1 ], (int32)HandlePlacement.Axes[ 2 ] );
	return HandlePlacement;
}

int32 URuntimeQuVRGizmoHandleGroup::MakeHandleIndex(const FQuVRTransformGizmoHandlePlacement HandlePlacement) const
{
	const int32 HandleIndex = (int32)HandlePlacement.Axes[0] * 9 + (int32)HandlePlacement.Axes[1] * 3 + (int32)HandlePlacement.Axes[2];
	//	GWarn->Logf( TEXT( "HandlePlacment[ %i %i %i ] = %i" ), (int32)HandlePlacement.Axes[ 0 ], (int32)HandlePlacement.Axes[ 1 ], (int32)HandlePlacement.Axes[ 2 ], HandleIndex );
	return HandleIndex;
}

FString URuntimeQuVRGizmoHandleGroup::MakeHandleName(const FQuVRTransformGizmoHandlePlacement HandlePlacement, EAxisList::Type& out_type) const
{
	FString HandleName;
	int32 CenteredAxisCount = 0;
	for (int32 AxisIndex = 0; AxisIndex < 3; ++AxisIndex)
	{
		const EQuVRTransformGizmoHandleDirection HandleDirection = HandlePlacement.Axes[AxisIndex];

		if (HandleDirection == EQuVRTransformGizmoHandleDirection::QUVR_Center)
		{
			++CenteredAxisCount;
		}
		else
		{
			switch (AxisIndex)
			{
			case 0:
				HandleName += (HandleDirection == EQuVRTransformGizmoHandleDirection::QUVR_Negative) ? TEXT("Back") : TEXT("Front");
				out_type = EAxisList::X;
				break;

			case 1:
				HandleName += (HandleDirection == EQuVRTransformGizmoHandleDirection::QUVR_Negative) ? TEXT("Left") : TEXT("Right");
				out_type = EAxisList::Y;
				break;

			case 2:
				HandleName += (HandleDirection == EQuVRTransformGizmoHandleDirection::QUVR_Negative) ? TEXT("Bottom") : TEXT("Top");
				out_type = EAxisList::Z;
				break;
			}
		}
	}

	if (CenteredAxisCount == 2)
	{
		HandleName += TEXT("Center");
	}
	else if (CenteredAxisCount == 3)
	{
		HandleName = TEXT("Origin");
	}

	return HandleName;
}

FVector URuntimeQuVRGizmoHandleGroup::GetAxisVector(const int32 AxisIndex, const EQuVRTransformGizmoHandleDirection HandleDirection)
{
	FVector AxisVector;

	if (HandleDirection == EQuVRTransformGizmoHandleDirection::QUVR_Center)
	{
		AxisVector = FVector::ZeroVector;
	}
	else
	{
		const bool bIsFacingPositiveDirection = HandleDirection == EQuVRTransformGizmoHandleDirection::QUVR_Positive;
		switch (AxisIndex)
		{
		case 0:
			AxisVector = (bIsFacingPositiveDirection ? FVector::ForwardVector : -FVector::ForwardVector);
			break;

		case 1:
			AxisVector = (bIsFacingPositiveDirection ? FVector::RightVector : -FVector::RightVector);
			break;

		case 2:
			AxisVector = (bIsFacingPositiveDirection ? FVector::UpVector : -FVector::UpVector);
			break;
		}
	}

	return AxisVector;
}

void URuntimeQuVRGizmoHandleGroup::UpdateGizmoHandleGroup(const FTransform& LocalToWorld, const FBox& LocalBounds, const FVector ViewLocation, const bool bAllHandlesVisible, class UActorComponent* DraggingHandle,
	const TArray< UActorComponent* >& HoveringOverHandles, float AnimationAlpha, float GizmoScale, const float GizmoHoverScale, const float GizmoHoverAnimationDuration, bool& bOutIsHoveringOrDraggingThisHandleGroup)
{
	UpdateHoverAnimation(DraggingHandle, HoveringOverHandles, GizmoHoverAnimationDuration, bOutIsHoveringOrDraggingThisHandleGroup);
}

void URuntimeQuVRGizmoHandleGroup::StartTracking(class AActor* actor)
{
	if (actor)
	{
		if (GetOwner() != actor)
		{
			DragActor = actor;
			GetOwner()->SetActorLocation(DragActor->GetActorLocation());
		}
	}
};

void URuntimeQuVRGizmoHandleGroup::EndTracking()
{
	if (DragActor)
	{
		DragActor = NULL;
	}
};


void URuntimeQuVRGizmoHandleGroup::UpdateDragActorTranslate(FVector& pos)
{
	if (DragActor)
	{
		DragActor->SetActorLocation(DragActor->GetActorLocation()+pos);
	}
}


int32 URuntimeQuVRGizmoHandleGroup::GetDraggedHandleIndex(class UStaticMeshComponent* DraggedMesh)
{
	for (int32 HandleIndex = 0; HandleIndex < Handles.Num(); ++HandleIndex)
	{
		if (Handles[HandleIndex].HandleMesh == DraggedMesh)
		{
			return HandleIndex;
		}
	}
	return INDEX_NONE;
}

void URuntimeQuVRGizmoHandleGroup::SetGizmoMaterial(UMaterialInterface* Material)
{
	GizmoMaterial = Material;
}

void URuntimeQuVRGizmoHandleGroup::SetTranslucentGizmoMaterial(UMaterialInterface* Material)
{
	TranslucentGizmoMaterial = Material;
}

TArray<FQuVRGizmoHandle>& URuntimeQuVRGizmoHandleGroup::GetHandles()
{
	return Handles;
}

EQuVRGizmoHandleTypes URuntimeQuVRGizmoHandleGroup::GetHandleType() const
{
	return EQuVRGizmoHandleTypes::QUVR_All;
}

void URuntimeQuVRGizmoHandleGroup::SetShowOnUniversalGizmo(const bool bInShowOnUniversal)
{
	bShowOnUniversalGizmo = bInShowOnUniversal;
}

bool URuntimeQuVRGizmoHandleGroup::GetShowOnUniversalGizmo() const
{
	return bShowOnUniversalGizmo;
}

EQuVRGizmoHandleHoveredTypes URuntimeQuVRGizmoHandleGroup::GetHandleHoveredType()const
{
	return eQuVRHandleHoveredType;
}

void URuntimeQuVRGizmoHandleGroup::SetQuVROwningTransformGizmo(class ARuntimeQuVRTransformAxisActor* TransformGizmo)
{
	QuVROwningTransformGizmoActor = TransformGizmo;
}

void URuntimeQuVRGizmoHandleGroup::UpdateHandleColor(const int32 AxisIndex, FQuVRGizmoHandle& Handle, class UActorComponent* DraggingHandle, const TArray< UActorComponent* >& HoveringOverHandles)
{
	UStaticMeshComponent* HandleMesh = Handle.HandleMesh;
	if (!HandleMesh->GetMaterial(0)->IsA(UMaterialInstanceDynamic::StaticClass()))
	{
		UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(GizmoMaterial, this);
		HandleMesh->SetMaterial(0, MID);
	}
	if (!HandleMesh->GetMaterial(1)->IsA(UMaterialInstanceDynamic::StaticClass()))
	{
		UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(TranslucentGizmoMaterial, this);
		HandleMesh->SetMaterial(1, MID);
	}
	UMaterialInstanceDynamic* MID0 = CastChecked<UMaterialInstanceDynamic>(HandleMesh->GetMaterial(0));
	UMaterialInstanceDynamic* MID1 = CastChecked<UMaterialInstanceDynamic>(HandleMesh->GetMaterial(1));

	FLinearColor AxisColorX = FLinearColor(0.594f, 0.0197f, 0.0f);
	FLinearColor AxisColorY = FLinearColor(0.1349f, 0.3959f, 0.0f);
	FLinearColor AxisColorZ = FLinearColor(0.0251f, 0.207f, 0.85f);

	ARuntimeQuVRTransformAxisActor* GizmoActor = CastChecked<ARuntimeQuVRTransformAxisActor>(GetOwner());


	if (GizmoActor)
	{
		URuntimeQuVRWorldInteraction* WorldInteraction = GizmoActor->GetOwnerWorldInteraction();
		if (WorldInteraction)
		{
			FLinearColor HandleColor = WorldInteraction->GetColor(RuntimeQuVRtransformType::EQuVRColors::DefaultColor);
			if (HandleMesh == DraggingHandle)
			{
				HandleColor = WorldInteraction->GetColor(RuntimeQuVRtransformType::EQuVRColors::GizmoDragging);
			}
			else if (AxisIndex != INDEX_NONE)
			{
				switch (AxisIndex)
				{
				case 0:
					HandleColor = WorldInteraction->GetColor(RuntimeQuVRtransformType::EQuVRColors::Forward);
					break;

				case 1:
					HandleColor = WorldInteraction->GetColor(RuntimeQuVRtransformType::EQuVRColors::Right);
					break;

				case 2:
					HandleColor = WorldInteraction->GetColor(RuntimeQuVRtransformType::EQuVRColors::Up);
					break;
				}

				if (HoveringOverHandles.Contains(HandleMesh))
				{
					 HandleColor = FLinearColor::LerpUsingHSV(HandleColor, WorldInteraction->GetColor(RuntimeQuVRtransformType::EQuVRColors::GizmoHover), Handle.HoverAlpha);
				}
			}

			static FName StaticHandleColorParameter("Color");
			MID0->SetVectorParameterValue(StaticHandleColorParameter, HandleColor);
			MID1->SetVectorParameterValue(StaticHandleColorParameter, HandleColor);
		}
	}

}

void URuntimeQuVRGizmoHandleGroup::UpdateVisibilityAndCollision(const EQuVRGizmoHandleTypes GizmoType, const EQuVRCoordSystem GizmoCoordinateSpace, const bool bAllHandlesVisible, const bool bAllowRotationAndScaleHandles, UActorComponent* DraggingHandle)
{
	const bool bIsTypeSupported =
		((GizmoType == EQuVRGizmoHandleTypes::QUVR_All && GetShowOnUniversalGizmo()) || GetHandleType() == GizmoType) &&
		(bAllowRotationAndScaleHandles || (GetHandleType() != EQuVRGizmoHandleTypes::QUVR_Rotate && GetHandleType() != EQuVRGizmoHandleTypes::QUVR_Scale));

	const bool bSupportsCurrentCoordinateSpace = SupportsWorldCoordinateSpace() || GizmoCoordinateSpace != EQuVRCoordSystem::QuVR_COORD_World;

	for (FQuVRGizmoHandle& Handle : GetHandles())
	{
		if (Handle.HandleMesh != nullptr)
		{
			const bool bShowIt = (bIsTypeSupported && bSupportsCurrentCoordinateSpace && bAllHandlesVisible) || (DraggingHandle != nullptr && DraggingHandle == Handle.HandleMesh);

			Handle.HandleMesh->SetVisibility(bShowIt);

			// Never allow ray queries to impact hidden handles
		//	Handle.HandleMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			Handle.HandleMesh->SetCollisionEnabled(bShowIt ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		}
	}
}
/*

UViewportDragOperationComponent* URuntimeQuVRGizmoHandleGroup::GetDragOperationComponent()
{
	return DragOperationComponent;
}
*/

class URuntimeQuVRHandleMeshComponent* URuntimeQuVRGizmoHandleGroup::CreateMeshHandle(class UStaticMesh* HandleMesh, const FString& ComponentName)
{
	const bool bAllowGizmoLighting = false;	// @todo vreditor: Not sure if we want this for gizmos or not yet.  Needs feedback.  Also they're translucent right now.

	URuntimeQuVRHandleMeshComponent* HandleComponent = CreateDefaultSubobject<URuntimeQuVRHandleMeshComponent>(*ComponentName);
	check(HandleComponent != nullptr);

	HandleComponent->SetStaticMesh(HandleMesh);
	HandleComponent->SetMobility(EComponentMobility::Movable);
	HandleComponent->SetupAttachment(this);

	HandleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HandleComponent->SetSimulatePhysics(false);
	HandleComponent->SetCollisionResponseToChannel(COLLISION_GIZMO, ECollisionResponse::ECR_Block);
	HandleComponent->SetCollisionObjectType(COLLISION_GIZMO);

	HandleComponent->bGenerateOverlapEvents = true;
	HandleComponent->bCastDynamicShadow = bAllowGizmoLighting;
	HandleComponent->bCastStaticShadow = false;
	HandleComponent->bAffectDistanceFieldLighting = bAllowGizmoLighting;
	HandleComponent->bAffectDynamicIndirectLighting = bAllowGizmoLighting;
	//HandleComponent->bUseEditorCompositing = true;

	return HandleComponent;
}

URuntimeQuVRHandleMeshComponent* URuntimeQuVRGizmoHandleGroup::CreateAndAddMeshHandle(UStaticMesh* HandleMesh, const FString& ComponentName, const FQuVRTransformGizmoHandlePlacement& HandlePlacement, const EAxisList::Type& type)
{
	URuntimeQuVRHandleMeshComponent* HandleComponent = CreateMeshHandle(HandleMesh, ComponentName);
	AddMeshToHandles(HandleComponent, HandlePlacement, type);
	return HandleComponent;
}

void URuntimeQuVRGizmoHandleGroup::AddMeshToHandles(URuntimeQuVRHandleMeshComponent* HandleMeshComponent, const FQuVRTransformGizmoHandlePlacement& HandlePlacement, const EAxisList::Type& type)
{

	int32 HandleIndex = MakeHandleIndex(HandlePlacement);
	if (Handles.Num() < (HandleIndex + 1))
	{
		Handles.SetNumZeroed(HandleIndex + 1);
	}
	Handles[HandleIndex].HandleMesh = HandleMeshComponent;
	Handles[HandleIndex].HandleType = type;
}

FQuVRTransformGizmoHandlePlacement URuntimeQuVRGizmoHandleGroup::GetHandlePlacement(const int32 X, const int32 Y, const int32 Z) const
{
	FQuVRTransformGizmoHandlePlacement HandlePlacement;
	HandlePlacement.Axes[0] = (EQuVRTransformGizmoHandleDirection)X;
	HandlePlacement.Axes[1] = (EQuVRTransformGizmoHandleDirection)Y;
	HandlePlacement.Axes[2] = (EQuVRTransformGizmoHandleDirection)Z;

	return HandlePlacement;
}

void URuntimeQuVRGizmoHandleGroup::UpdateHoverAnimation(UActorComponent* DraggingHandle, const TArray< UActorComponent* >& HoveringOverHandles, const float GizmoHoverAnimationDuration, bool& bOutIsHoveringOrDraggingThisHandleGroup)
{
	for (FQuVRGizmoHandle& Handle : Handles)
	{
		const bool bIsHoveringOverHandle = HoveringOverHandles.Contains(Handle.HandleMesh) || (DraggingHandle != nullptr && DraggingHandle == Handle.HandleMesh);

		if (bIsHoveringOverHandle)
		{
			Handle.HoverAlpha += GetWorld()->GetDeltaSeconds() / GizmoHoverAnimationDuration;
			bOutIsHoveringOrDraggingThisHandleGroup = true;
		}
		else
		{
			Handle.HoverAlpha -= GetWorld()->GetDeltaSeconds() / GizmoHoverAnimationDuration;
		}
		Handle.HoverAlpha = FMath::Clamp(Handle.HoverAlpha, 0.0f, 1.0f);
	}
}

void URuntimeQuVRPivotTranslationGizmoHandleGroup::OnHover_AxisX(class UPrimitiveComponent* OtherComp)
{
	eQuVRHandleHoveredType = EQuVRGizmoHandleHoveredTypes::QUVR_X;
	HoveringOverTransformGizmoComponent = OtherComp;
	//	FPlatformMisc::MessageBoxExt(EAppMsgType::Ok, TEXT("OnHover_AxisX"),TEXT("OnHover_AxisX"));
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString("Begin++++++++++++OnHover_AxisX "));
}

void URuntimeQuVRPivotTranslationGizmoHandleGroup::OnHover_AxisY(class UPrimitiveComponent* OtherComp)
{
	eQuVRHandleHoveredType = EQuVRGizmoHandleHoveredTypes::QUVR_Y;
	HoveringOverTransformGizmoComponent = OtherComp;
}

void URuntimeQuVRPivotTranslationGizmoHandleGroup::OnHover_AxisZ(class UPrimitiveComponent* OtherComp)
{
	eQuVRHandleHoveredType = EQuVRGizmoHandleHoveredTypes::QUVR_Z;
	HoveringOverTransformGizmoComponent = OtherComp;
}

void URuntimeQuVRPivotTranslationGizmoHandleGroup::OnRelease_AxisX(class UPrimitiveComponent* OtherComp)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString("Begin++++++++++++OnRelease_AxisX "));
	eQuVRHandleHoveredType = EQuVRGizmoHandleHoveredTypes::QUVR_VOID;
	HoveringOverTransformGizmoComponent = NULL;
}

void URuntimeQuVRPivotTranslationGizmoHandleGroup::OnRelease_AxisY(class UPrimitiveComponent* OtherComp)
{
	eQuVRHandleHoveredType = EQuVRGizmoHandleHoveredTypes::QUVR_VOID;
	HoveringOverTransformGizmoComponent = NULL;
}
void URuntimeQuVRPivotTranslationGizmoHandleGroup::OnRelease_AxisZ(class UPrimitiveComponent* OtherComp)
{
	eQuVRHandleHoveredType = EQuVRGizmoHandleHoveredTypes::QUVR_VOID;
	HoveringOverTransformGizmoComponent = NULL;
}

void URuntimeQuVRAxisGizmoHandleGroup::CreateHandles(UStaticMesh* HandleMesh, const FString& HandleComponentName)
{
	for (int32 X = 0; X < 3; ++X)
	{
		for (int32 Y = 0; Y < 3; ++Y)
		{
			for (int32 Z = 0; Z < 3; ++Z)
			{
				FQuVRTransformGizmoHandlePlacement HandlePlacement = GetHandlePlacement(X, Y, Z);

				int32 CenterHandleCount, FacingAxisIndex, CenterAxisIndex;
				HandlePlacement.GetCenterHandleCountAndFacingAxisIndex( /* Out */ CenterHandleCount, /* Out */ FacingAxisIndex, /* Out */ CenterAxisIndex);

				// Don't allow translation/stretching/rotation from the origin
				if (CenterHandleCount < 3)
				{
					EAxisList::Type type = EAxisList::None;
					const FString HandleName = MakeHandleName(HandlePlacement, type);

					// Only happens on the center of an axis. And we only bother drawing one for the "positive" direction.
					if (CenterHandleCount == 2 && HandlePlacement.Axes[FacingAxisIndex] == EQuVRTransformGizmoHandleDirection::QUVR_Positive)
					{
						// Plane translation handle
						FString ComponentName = HandleName + HandleName;
						CreateAndAddMeshHandle(HandleMesh, ComponentName, HandlePlacement, type);
					}
				}
			}
		}
	}
}

void URuntimeQuVRAxisGizmoHandleGroup::UpdateHandlesRelativeTransformOnAxis(const FTransform& HandleToCenter, const float AnimationAlpha, const float GizmoScale, const float GizmoHoverScale,
	const FVector& ViewLocation, class UActorComponent* DraggingHandle, const TArray< UActorComponent* >& HoveringOverHandles)
{
	const float WorldScaleFactor = GetWorld()->GetWorldSettings()->WorldToMeters / 100.0f;

	for (int32 HandleIndex = 0; HandleIndex < Handles.Num(); ++HandleIndex)
	{
		FQuVRGizmoHandle& Handle = Handles[HandleIndex];
		UStaticMeshComponent* GizmoHandleMeshComponent = Handle.HandleMesh;
		if (GizmoHandleMeshComponent != nullptr)	// Can be null if no handle for this specific placement
		{
			const FQuVRTransformGizmoHandlePlacement HandlePlacement = MakeHandlePlacementForIndex(HandleIndex);

			int32 CenterHandleCount, FacingAxisIndex, CenterAxisIndex;
			HandlePlacement.GetCenterHandleCountAndFacingAxisIndex( /* Out */ CenterHandleCount, /* Out */ FacingAxisIndex, /* Out */ CenterAxisIndex);

			if (DraggingHandle == nullptr)
			{
				FVector GizmoSpaceFacingAxisVector = GetAxisVector(FacingAxisIndex, HandlePlacement.Axes[FacingAxisIndex]);

				// Check on which side we are relative to the gizmo
				const FVector GizmoSpaceViewLocation = GetOwner()->GetTransform().InverseTransformPosition(ViewLocation);
				if (GizmoSpaceViewLocation[FacingAxisIndex] < 0)
				{
					GizmoSpaceFacingAxisVector[FacingAxisIndex] *= 1.0f;
				//	GizmoSpaceFacingAxisVector[FacingAxisIndex] *= -1.0f;
				}

				const FTransform GizmoOriginToFacingAxisRotation(GizmoSpaceFacingAxisVector.ToOrientationQuat());

				FTransform HandleToGizmoOrigin = HandleToCenter * GizmoOriginToFacingAxisRotation;

				// Check on the axis if the offset is on the other side of the object compared to the view. Switch the offset to the side of the view if it does
				FVector GizmoSpaceFacingAxisOffset = HandleToGizmoOrigin.GetLocation();
				for (int32 AxisIndex = 0; AxisIndex < 3; ++AxisIndex)
				{
					if (AxisIndex != FacingAxisIndex &&
						((GizmoSpaceFacingAxisOffset[AxisIndex] > 0 && GizmoSpaceViewLocation[AxisIndex] < 0) ||
						(GizmoSpaceFacingAxisOffset[AxisIndex] < 0 && GizmoSpaceViewLocation[AxisIndex] > 0)))
					{
						GizmoSpaceFacingAxisOffset[AxisIndex] *= -1.0f;
					}
				}

				GizmoSpaceFacingAxisOffset *= AnimationAlpha;

				HandleToGizmoOrigin.SetLocation(GizmoSpaceFacingAxisOffset * GizmoScale);

				// Set the final transform
				GizmoHandleMeshComponent->SetRelativeTransform(HandleToGizmoOrigin);

				float GizmoHandleScale = GizmoScale;

				// Make the handle bigger while hovered (but don't affect the offset -- we want it to scale about it's origin)
				GizmoHandleScale *= FMath::Lerp(1.0f, GizmoHoverScale, Handle.HoverAlpha);

				GizmoHandleScale *= AnimationAlpha;
				GizmoHandleMeshComponent->SetRelativeScale3D(FVector(GizmoHandleScale));
			}

			// Update material
			UpdateHandleColor(FacingAxisIndex, Handle, DraggingHandle, HoveringOverHandles);
		}
	}
}

/**
* FQuVRTransformGizmoHandlePlacement GetCenterHandleCountAndFacingAxisIndex
*/
void FQuVRTransformGizmoHandlePlacement::GetCenterHandleCountAndFacingAxisIndex(int32& OutCenterHandleCount, int32& OutFacingAxisIndex, int32& OutCenterAxisIndex) const
{
	OutCenterHandleCount = 0;
	OutFacingAxisIndex = INDEX_NONE;
	OutCenterAxisIndex = INDEX_NONE;
	for (int32 AxisIndex = 0; AxisIndex < 3; ++AxisIndex)
	{
		if (Axes[AxisIndex] == EQuVRTransformGizmoHandleDirection::QUVR_Center)
		{
			++OutCenterHandleCount;
			OutCenterAxisIndex = AxisIndex;
		}
		else
		{
			OutFacingAxisIndex = AxisIndex;
		}
	}

	if (OutCenterHandleCount < 2)
	{
		OutFacingAxisIndex = INDEX_NONE;
	}

	if (OutCenterHandleCount != 1)
	{
		OutCenterAxisIndex = INDEX_NONE;
	}
}



/************************************************************************/
/* Translation                                                          */
/************************************************************************/
URuntimeQuVRPivotTranslationGizmoHandleGroup::URuntimeQuVRPivotTranslationGizmoHandleGroup() :
	Super()
{
	const URuntimeQuVRAssetContainer& AssetContainer = URuntimeQuVRAssetContainer::LoadAssetContainer();
	if (!IsValid(&AssetContainer))
	{
		return;
	}
	CreateHandles(AssetContainer.TranslationHandleMesh, FString("PivotTranslationHandle"));

	//////////////////////////////////////////////////////////////////////////
	// Bind Axis
	URuntimeQuVRHandleMeshComponent* mesh = GetHandleMesh(EAxisList::X);
	check(mesh);
	mesh->OnBeginCursorOver.AddDynamic(this, &URuntimeQuVRPivotTranslationGizmoHandleGroup::OnHover_AxisX);
	mesh->OnEndCursorOver.AddDynamic(this, &URuntimeQuVRPivotTranslationGizmoHandleGroup::OnRelease_AxisX);
	mesh = GetHandleMesh(EAxisList::Y);
	check(mesh);
	mesh->OnBeginCursorOver.AddDynamic(this, &URuntimeQuVRPivotTranslationGizmoHandleGroup::OnHover_AxisY);
	mesh->OnEndCursorOver.AddDynamic(this, &URuntimeQuVRPivotTranslationGizmoHandleGroup::OnRelease_AxisY);
	mesh = GetHandleMesh(EAxisList::Z);
	check(mesh);
	mesh->OnBeginCursorOver.AddDynamic(this, &URuntimeQuVRPivotTranslationGizmoHandleGroup::OnHover_AxisZ);
	mesh->OnEndCursorOver.AddDynamic(this, &URuntimeQuVRPivotTranslationGizmoHandleGroup::OnRelease_AxisZ);
}


void URuntimeQuVRPivotTranslationGizmoHandleGroup::UpdateGizmoHandleGroup(const FTransform& LocalToWorld, const FBox& LocalBounds, const FVector ViewLocation, const bool bAllHandlesVisible, class UActorComponent* DraggingHandle,
	const TArray< UActorComponent* >& HoveringOverHandles, float AnimationAlpha, float GizmoScale, const float GizmoHoverScale, const float GizmoHoverAnimationDuration, bool& bOutIsHoveringOrDraggingThisHandleGroup)
{
	// Call parent implementation (updates hover animation)
	Super::UpdateGizmoHandleGroup(LocalToWorld, LocalBounds, ViewLocation, bAllHandlesVisible, DraggingHandle, HoveringOverHandles, AnimationAlpha,
		GizmoScale, GizmoHoverScale, GizmoHoverAnimationDuration, bOutIsHoveringOrDraggingThisHandleGroup);

	const float MultipliedGizmoScale = GizmoScale * QuVR::QuVRPivotGizmoTranslationScaleMultiply->GetFloat();
	const float MultipliedGizmoHoverScale = GizmoHoverScale * QuVR::QuVRPivotGizmoTranslationHoverScaleMultiply->GetFloat();

	UpdateHandlesRelativeTransformOnAxis(FTransform(FVector(QuVR::QuVRPivotGizmoTranslationPivotOffsetX->GetFloat(), 0, 0)), AnimationAlpha, MultipliedGizmoScale, MultipliedGizmoHoverScale, ViewLocation, DraggingHandle, HoveringOverHandles);
}

EQuVRGizmoHandleTypes URuntimeQuVRPivotTranslationGizmoHandleGroup::GetHandleType() const
{
	return EQuVRGizmoHandleTypes::QUVR_Translate;
}

class URuntimeQuVRHandleMeshComponent* URuntimeQuVRPivotTranslationGizmoHandleGroup::GetHandleMesh(const EAxisList::Type type)
{
	for (int32 HandleIndex = 0; HandleIndex < Handles.Num(); ++HandleIndex)
	{
		FQuVRGizmoHandle& Handle = Handles[HandleIndex];
		if (Handle.HandleType == type)
		{
			return Handle.HandleMesh;
		}
		
	}
	return NULL;
}


/************************************************************************/
/* Plane Translation	                                                */
/************************************************************************/
URuntimeQuVRPivotPlaneTranslationGizmoHandleGroup::URuntimeQuVRPivotPlaneTranslationGizmoHandleGroup() :
	Super()
{
	const URuntimeQuVRAssetContainer& AssetContainer = URuntimeQuVRAssetContainer::LoadAssetContainer();
	if (!IsValid(&AssetContainer))
	{
		return;
	}
	CreateHandles(AssetContainer.PlaneTranslationHandleMesh, FString("PlaneTranslationHandle"));
}

void URuntimeQuVRPivotPlaneTranslationGizmoHandleGroup::UpdateGizmoHandleGroup(const FTransform& LocalToWorld, const FBox& LocalBounds, const FVector ViewLocation, const bool bAllHandlesVisible, class UActorComponent* DraggingHandle,
	const TArray< UActorComponent* >& HoveringOverHandles, float AnimationAlpha, float GizmoScale, const float GizmoHoverScale, const float GizmoHoverAnimationDuration, bool& bOutIsHoveringOrDraggingThisHandleGroup)
{
	// Call parent implementation (updates hover animation)
	Super::UpdateGizmoHandleGroup(LocalToWorld, LocalBounds, ViewLocation, bAllHandlesVisible, DraggingHandle, HoveringOverHandles, AnimationAlpha,
		GizmoScale, GizmoHoverScale, GizmoHoverAnimationDuration, bOutIsHoveringOrDraggingThisHandleGroup);

	UpdateHandlesRelativeTransformOnAxis(FTransform(FVector(0, QuVR::QuVRPivotGizmoPlaneTranslationPivotOffsetYZ->GetFloat(), QuVR::QuVRPivotGizmoPlaneTranslationPivotOffsetYZ->GetFloat())),
		AnimationAlpha, GizmoScale, GizmoHoverScale, ViewLocation, DraggingHandle, HoveringOverHandles);
}

EQuVRGizmoHandleTypes URuntimeQuVRPivotPlaneTranslationGizmoHandleGroup::GetHandleType() const
{
	return EQuVRGizmoHandleTypes::QUVR_Translate;
}

/************************************************************************/
/* URuntimeQuVRStretchGizmoHandleGroup									*/
/************************************************************************/

URuntimeQuVRStretchGizmoHandleGroup::URuntimeQuVRStretchGizmoHandleGroup()
	: Super()
{
	UStaticMesh* StretchingHandleMesh = nullptr;
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> ObjectFinder(TEXT("/Engine/VREditor/TransformGizmo/PlaneTranslationHandle"));
		StretchingHandleMesh = ObjectFinder.Object;
		check(StretchingHandleMesh != nullptr);
	}

	UStaticMesh* BoundingBoxCornerMesh = nullptr;
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> ObjectFinder(TEXT("/Engine/VREditor/TransformGizmo/BoundingBoxCorner"));
		BoundingBoxCornerMesh = ObjectFinder.Object;
		check(BoundingBoxCornerMesh != nullptr);
	}

	UStaticMesh* BoundingBoxEdgeMesh = nullptr;
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> ObjectFinder(TEXT("/Engine/VREditor/TransformGizmo/BoundingBoxEdge"));
		BoundingBoxEdgeMesh = ObjectFinder.Object;
		check(BoundingBoxEdgeMesh != nullptr);
	}

	const bool bAllowGizmoLighting = false;	// @todo vreditor: Not sure if we want this for gizmos or not yet.  Needs feedback.  Also they're translucent right now.

	for (int32 X = 0; X < 3; ++X)
	{
		for (int32 Y = 0; Y < 3; ++Y)
		{
			for (int32 Z = 0; Z < 3; ++Z)
			{
				FQuVRTransformGizmoHandlePlacement HandlePlacement = GetHandlePlacement(X, Y, Z);

				int32 CenterHandleCount, FacingAxisIndex, CenterAxisIndex;
				HandlePlacement.GetCenterHandleCountAndFacingAxisIndex( /* Out */ CenterHandleCount, /* Out */ FacingAxisIndex, /* Out */ CenterAxisIndex);

				// Don't allow translation/stretching/rotation from the origin
				if (CenterHandleCount < 3)
				{
					EAxisList::Type type;
					const FString HandleName = MakeHandleName(HandlePlacement, type);

					// Stretching handle
					if (CenterHandleCount != 1)	// @todo vreditor: Remove this line if we want to re-enable support for edge stretching handles (rather than only corners).  We disabled this because they sort of got in the way of the rotation gizmo, and weren't very popular to use.
					{
						FString ComponentName = HandleName + TEXT("StretchingHandle");

						UStaticMesh* Mesh = nullptr;

						if (CenterHandleCount == 0)	// Corner?
						{
							Mesh = BoundingBoxCornerMesh;
						}
						else if (CenterHandleCount == 1)	// Edge?
						{
							Mesh = BoundingBoxEdgeMesh;
						}
						else  // Face
						{
							Mesh = StretchingHandleMesh;
						}

						CreateAndAddMeshHandle(Mesh, ComponentName, HandlePlacement);
					}
				}
			}
		}
	}

//	DragOperationComponent->SetDragOperationClass(UStretchGizmoHandleDragOperation::StaticClass());
}


void URuntimeQuVRStretchGizmoHandleGroup::UpdateGizmoHandleGroup(const FTransform& LocalToWorld, const FBox& LocalBounds, const FVector ViewLocation, const bool bAllHandlesVisible, class UActorComponent* DraggingHandle, const TArray< UActorComponent* >& HoveringOverHandles,
	float AnimationAlpha, float GizmoScale, const float GizmoHoverScale, const float GizmoHoverAnimationDuration, bool& bOutIsHoveringOrDraggingThisHandleGroup)
{
	// Call parent implementation (updates hover animation)
	Super::UpdateGizmoHandleGroup(LocalToWorld, LocalBounds, ViewLocation, bAllHandlesVisible, DraggingHandle, HoveringOverHandles,
		AnimationAlpha, GizmoScale, GizmoHoverScale, GizmoHoverAnimationDuration, bOutIsHoveringOrDraggingThisHandleGroup);

	for (int32 HandleIndex = 0; HandleIndex < Handles.Num(); ++HandleIndex)
	{
		FQuVRGizmoHandle& Handle = Handles[HandleIndex];

		UStaticMeshComponent* StretchingHandle = Handle.HandleMesh;
		if (StretchingHandle != nullptr)	// Can be null if no handle for this specific placement
		{
			const FQuVRTransformGizmoHandlePlacement HandlePlacement = MakeHandlePlacementForIndex(HandleIndex);

			float GizmoHandleScale = GizmoScale;

			const float OffsetFromSide = GizmoHandleScale *
				(0.0f +	// @todo vreditor tweak: Hard coded handle offset from side of primitive
				(1.0f - AnimationAlpha) * 10.0f);	// @todo vreditor tweak: Animation offset

													// Make the handle bigger while hovered (but don't affect the offset -- we want it to scale about it's origin)
			GizmoHandleScale *= FMath::Lerp(1.0f, GizmoHoverScale, Handle.HoverAlpha);

			FVector HandleRelativeLocation = FVector::ZeroVector;
			for (int32 AxisIndex = 0; AxisIndex < 3; ++AxisIndex)
			{
				if (HandlePlacement.Axes[AxisIndex] == EQuVRTransformGizmoHandleDirection::QUVR_Negative)	// Negative direction
				{
					HandleRelativeLocation[AxisIndex] = LocalBounds.Min[AxisIndex] - OffsetFromSide;
				}
				else if (HandlePlacement.Axes[AxisIndex] == EQuVRTransformGizmoHandleDirection::QUVR_Positive)	// Positive direction
				{
					HandleRelativeLocation[AxisIndex] = LocalBounds.Max[AxisIndex] + OffsetFromSide;
				}
				else // ETransformGizmoHandleDirection::Center
				{
					HandleRelativeLocation[AxisIndex] = LocalBounds.GetCenter()[AxisIndex];
				}
			}

			StretchingHandle->SetRelativeLocation(HandleRelativeLocation);

			int32 CenterHandleCount, FacingAxisIndex, CenterAxisIndex;
			HandlePlacement.GetCenterHandleCountAndFacingAxisIndex( /* Out */ CenterHandleCount, /* Out */ FacingAxisIndex, /* Out */ CenterAxisIndex);

			FRotator Rotator = FRotator::ZeroRotator;
			{
				// Back bottom left
				if (HandlePlacement.Axes[0] == EQuVRTransformGizmoHandleDirection::QUVR_Negative && HandlePlacement.Axes[1] == EQuVRTransformGizmoHandleDirection::QUVR_Negative && HandlePlacement.Axes[2] == EQuVRTransformGizmoHandleDirection::QUVR_Negative)
				{
					Rotator.Yaw = 0.0f;
					Rotator.Pitch = 0.0f;
				}

				// Back bottom right
				else if (HandlePlacement.Axes[0] == EQuVRTransformGizmoHandleDirection::QUVR_Negative && HandlePlacement.Axes[1] == EQuVRTransformGizmoHandleDirection::QUVR_Positive && HandlePlacement.Axes[2] == EQuVRTransformGizmoHandleDirection::QUVR_Negative)
				{
					Rotator.Yaw = -90.0f;
					Rotator.Pitch = 0.0f;
				}

				// Back top left
				else if (HandlePlacement.Axes[0] == EQuVRTransformGizmoHandleDirection::QUVR_Negative && HandlePlacement.Axes[1] == EQuVRTransformGizmoHandleDirection::QUVR_Negative && HandlePlacement.Axes[2] == EQuVRTransformGizmoHandleDirection::QUVR_Positive)
				{
					Rotator.Yaw = 0.0f;
					Rotator.Pitch = -90.0f;
				}

				// Back top right
				else if (HandlePlacement.Axes[0] == EQuVRTransformGizmoHandleDirection::QUVR_Negative && HandlePlacement.Axes[1] == EQuVRTransformGizmoHandleDirection::QUVR_Positive && HandlePlacement.Axes[2] == EQuVRTransformGizmoHandleDirection::QUVR_Positive)
				{
					Rotator.Yaw = -90.0f;
					Rotator.Pitch = -90.0f;
				}

				// Front bottom left
				else if (HandlePlacement.Axes[0] == EQuVRTransformGizmoHandleDirection::QUVR_Positive && HandlePlacement.Axes[1] == EQuVRTransformGizmoHandleDirection::QUVR_Negative && HandlePlacement.Axes[2] == EQuVRTransformGizmoHandleDirection::QUVR_Negative)
				{
					Rotator.Yaw = 0.0f;
					Rotator.Pitch = 90.0f;
				}

				// Front bottom right
				else if (HandlePlacement.Axes[0] == EQuVRTransformGizmoHandleDirection::QUVR_Positive && HandlePlacement.Axes[1] == EQuVRTransformGizmoHandleDirection::QUVR_Positive && HandlePlacement.Axes[2] == EQuVRTransformGizmoHandleDirection::QUVR_Negative)
				{
					Rotator.Yaw = 90.0f;
					Rotator.Pitch = 90.0f;
				}

				// Front top left
				else if (HandlePlacement.Axes[0] == EQuVRTransformGizmoHandleDirection::QUVR_Positive && HandlePlacement.Axes[1] == EQuVRTransformGizmoHandleDirection::QUVR_Negative && HandlePlacement.Axes[2] == EQuVRTransformGizmoHandleDirection::QUVR_Positive)
				{
					Rotator.Yaw = 0.0f;
					Rotator.Pitch = -180.0f;
				}

				// Front top right
				else if (HandlePlacement.Axes[0] == EQuVRTransformGizmoHandleDirection::QUVR_Positive && HandlePlacement.Axes[1] == EQuVRTransformGizmoHandleDirection::QUVR_Positive && HandlePlacement.Axes[2] == EQuVRTransformGizmoHandleDirection::QUVR_Positive)
				{
					Rotator.Yaw = 180.0f;
					Rotator.Pitch = -90.0f;
				}

				// Back left/right edge
				else if (HandlePlacement.Axes[0] == EQuVRTransformGizmoHandleDirection::QUVR_Negative && HandlePlacement.Axes[1] != EQuVRTransformGizmoHandleDirection::QUVR_Center)
				{
					Rotator.Yaw = 0.0f;
					Rotator.Pitch = 90.0f;
				}

				// Back bottom/top edge
				else if (HandlePlacement.Axes[0] == EQuVRTransformGizmoHandleDirection::QUVR_Negative && HandlePlacement.Axes[2] != EQuVRTransformGizmoHandleDirection::QUVR_Center)
				{
					Rotator.Yaw = 90.0f;
					Rotator.Pitch = 0.0f;
				}

				// Front left/right edge
				else if (HandlePlacement.Axes[0] == EQuVRTransformGizmoHandleDirection::QUVR_Positive && HandlePlacement.Axes[1] != EQuVRTransformGizmoHandleDirection::QUVR_Center)
				{
					Rotator.Yaw = 0.0f;
					Rotator.Pitch = 90.0f;
				}

				// Front bottom/top edge
				else if (HandlePlacement.Axes[0] == EQuVRTransformGizmoHandleDirection::QUVR_Positive && HandlePlacement.Axes[2] != EQuVRTransformGizmoHandleDirection::QUVR_Center)
				{
					Rotator.Yaw = 90.0f;
					Rotator.Pitch = 0.0f;
				}

				else
				{
					// Facing out from center of a face
					if (CenterHandleCount == 2)
					{
						const FQuat GizmoSpaceOrientation = GetAxisVector(FacingAxisIndex, HandlePlacement.Axes[FacingAxisIndex]).ToOrientationQuat();
						Rotator = GizmoSpaceOrientation.Rotator();
					}

					else
					{
						// One of the left/right bottom or top edges
					}
				}
			}

			StretchingHandle->SetRelativeRotation(Rotator);

			StretchingHandle->SetRelativeScale3D(FVector(GizmoHandleScale));

			// Update material
			UpdateHandleColor(FacingAxisIndex, Handle, DraggingHandle, HoveringOverHandles);
		}
	}
}


EQuVRGizmoHandleTypes URuntimeQuVRStretchGizmoHandleGroup::GetHandleType() const
{
	return EQuVRGizmoHandleTypes::QUVR_Scale;
}

bool URuntimeQuVRStretchGizmoHandleGroup::SupportsWorldCoordinateSpace() const
{
	// Stretching only works with local space gizmos
	return false;
}

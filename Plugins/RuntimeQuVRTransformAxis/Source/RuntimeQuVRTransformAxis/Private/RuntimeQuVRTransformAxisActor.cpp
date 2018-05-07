// Copyright 2017 Louis. All Rights Reserved.

#include "RuntimeQuVRTransformAxisActor.h"
#include "RuntimeQuVRTransformAlgorithm.h"
#include "RuntimeQuVRGizmoHandleGroup.h"
#include "RuntimeQuVRAssetContainer.h"
#include "RuntimeQuVRWorldInteraction.h"
#include "Misc/App.h"
#include "Engine/LocalPlayer.h"

using namespace RuntimeQuVRtransformType;

void ARuntimeQuVRTransformAxisActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}
// Sets default values
ARuntimeQuVRTransformAxisActor::ARuntimeQuVRTransformAxisActor():
								WorldInteraction(nullptr), 
								QuVRTransformAlgorithm(nullptr),
								TranslationGizmoHandleGroup(nullptr),
								RotationGizmoHandleGroup(nullptr),
								ScaleGizmoHandleGroup(nullptr),
								StretchGizmoHandleGroup(nullptr),
								QuVRLocalPlayer(nullptr),
								QuVRWorld(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	check(SceneComponent != nullptr);
	RootComponent = SceneComponent;

	CrateHandleGroups();

	bIsMouseButtonDown = false;
	bIsDrag = false;
	bIsHover = false;
}

// Called when the game starts or when spawned
void ARuntimeQuVRTransformAxisActor::BeginPlay()
{
	Super::BeginPlay();
	Init();

}

void ARuntimeQuVRTransformAxisActor::Init()
{
	QuVRTransformAlgorithm = new FRuntimeQuVRTransformAlgorithm();
	QuVRWorld = GetWorld();
	check(QuVRWorld);

	APlayerCameraManager* cameraManager = QuVRWorld->GetFirstPlayerController()->PlayerCameraManager;
	QuVRLocalPlayer = QuVRWorld->GetFirstLocalPlayerFromController();

	APlayerController* PlayController = QuVRLocalPlayer->GetPlayerController(QuVRWorld);
	check(PlayController);
	PlayController->bEnableMouseOverEvents = true;
	PlayController->bShowMouseCursor = true;

	InputPriority = -1;
	EnableInput(PlayController);
	check(InputComponent);

	FInputKeyBinding& LeftButtonPressedBindin = InputComponent->BindKey(EKeys::LeftMouseButton, EInputEvent::IE_Pressed, this, &ARuntimeQuVRTransformAxisActor::LeftButtonPressed);
	LeftButtonPressedBindin.bConsumeInput = false;
	FInputKeyBinding& LeftButtonReleasedBindin = InputComponent->BindKey(EKeys::LeftMouseButton, EInputEvent::IE_Released, this, &ARuntimeQuVRTransformAxisActor::LeftButtonReleased);
	LeftButtonReleasedBindin.bConsumeInput = false;

	WorldInteraction = NewObject<URuntimeQuVRWorldInteraction>();

	SetActorHiddenInGame(false);
}


// Called every frame
void ARuntimeQuVRTransformAxisActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateGizmoAxis();
	
}

void ARuntimeQuVRTransformAxisActor::Destroyed()
{
	delete QuVRTransformAlgorithm;
	QuVRTransformAlgorithm = 0;
	Super::Destroyed();

}

void ARuntimeQuVRTransformAxisActor::StartTracking(class URuntimeQuVRAxisGizmoHandleGroup* InHandleGroup)
{
	FVector2D MousePosition = FVector2D(QuVRLocalPlayer->ViewportClient->Viewport->GetMouseX(), QuVRLocalPlayer->ViewportClient->Viewport->GetMouseY());

	if (QuVRTransformAlgorithm)
	{
		QuVRTransformAlgorithm->ResetInitialTranslationOffset();
		QuVRTransformAlgorithm->ResetDeltaRotation();
	}
	if (InHandleGroup)
	{
		if (InHandleGroup->GetHandleHoveredType() != EQuVRGizmoHandleHoveredTypes::QUVR_VOID)
		{
			bIsHover = true;
		}
		if (bIsHover)
		{
			EAxisList::Type InCurrentAxis = EAxisList::Type::None;

			switch (InHandleGroup->GetHandleHoveredType())
			{
			case EQuVRGizmoHandleHoveredTypes::QUVR_X:
				InCurrentAxis = EAxisList::X;
				break;

			case EQuVRGizmoHandleHoveredTypes::QUVR_Y:
				InCurrentAxis = EAxisList::Y;
				break;

			case EQuVRGizmoHandleHoveredTypes::QUVR_Z:
				InCurrentAxis = EAxisList::Z;
				break;
			default:
				InCurrentAxis = EAxisList::None;
				break;
			}
			if (EQuVRMode::QuVR_WM_Rotate == InHandleGroup->GetHandleType())
			{
				WorldInteraction->StartTracking(MousePosition.X, MousePosition.Y);
			}
			// QuVRTransformAlgorithm StartTracking
			QuVRTransformAlgorithm->SetDragStartPosition(MousePosition);
			QuVRTransformAlgorithm->SetDragging(true);
			QuVRTransformAlgorithm->SetModeType(InHandleGroup->GetHandleType());
			QuVRTransformAlgorithm->SetCurrentAxis(InCurrentAxis);
		}
	}

}

void ARuntimeQuVRTransformAxisActor::EndTracking()
{
	if (QuVRTransformAlgorithm)
	{
		QuVRTransformAlgorithm->SetDragging(false);
		QuVRTransformAlgorithm->ResetDeltaRotation();
		WorldInteraction->EndTracking();
	}
}

void ARuntimeQuVRTransformAxisActor::LeftButtonPressed()
{
	HitObject();
	StartTracking(TranslationGizmoHandleGroup);
//	StartTracking(RotationGizmoHandleGroup);
	bIsMouseButtonDown = true;
}

void ARuntimeQuVRTransformAxisActor::LeftButtonReleased()
{
	ShowMouseCursor(true);
	EndTracking();
	/**	clear state*/
	bIsMouseButtonDown = false;
	bIsDrag = false;
	bIsHover = false;
	if (TranslationGizmoHandleGroup)
	{
		TranslationGizmoHandleGroup->SetIsDrag(bIsDrag);
		TranslationGizmoHandleGroup->EndTracking();
	}
	if (RotationGizmoHandleGroup)
	{
		RotationGizmoHandleGroup->SetIsDrag(bIsDrag);
		RotationGizmoHandleGroup->EndTracking();
	}
	if (ScaleGizmoHandleGroup)
	{
		ScaleGizmoHandleGroup->SetIsDrag(bIsDrag);
		ScaleGizmoHandleGroup->EndTracking();
	}
}

void ARuntimeQuVRTransformAxisActor::RightButtonPressed()
{
	HideGizemo();
}

void ARuntimeQuVRTransformAxisActor::HideGizemo()
{
	SetActorHiddenInGame(true);
}

void ARuntimeQuVRTransformAxisActor::ShowMouseCursor(const bool& inShow)
{

	QuVRLocalPlayer->GetPlayerController(QuVRWorld)->bShowMouseCursor = inShow;
}

void ARuntimeQuVRTransformAxisActor::CrateHandleGroups()
{
	// Get AssetContainer
	const URuntimeQuVRAssetContainer& AssetContainer = URuntimeQuVRAssetContainer::LoadAssetContainer();
	if (!IsValid(&AssetContainer))
	{
		return;
	}

	UMaterialInterface* GizmoMaterial = AssetContainer.TransformGizmoMaterial;
	UMaterialInterface* TranslucentGizmoMaterial = AssetContainer.TranslucentTransformGizmoMaterial;

	// add translation
	TranslationGizmoHandleGroup = CreateDefaultSubobject<URuntimeQuVRPivotTranslationGizmoHandleGroup>(TEXT("TranslationHandles"), true);
	TranslationGizmoHandleGroup->SetTranslucentGizmoMaterial(TranslucentGizmoMaterial);
	TranslationGizmoHandleGroup->SetGizmoMaterial(GizmoMaterial);
	TranslationGizmoHandleGroup->SetupAttachment(GetRootComponent());
	TranslationGizmoHandleGroup->SetQuVROwningTransformGizmo(this);

	AllHandleGroups.Add(TranslationGizmoHandleGroup);
	PlaneTranslationGizmoHandleGroup = CreateDefaultSubobject<URuntimeQuVRPivotPlaneTranslationGizmoHandleGroup>(TEXT("PlaneTranslationHandles"), true);
	PlaneTranslationGizmoHandleGroup->SetTranslucentGizmoMaterial(TranslucentGizmoMaterial);
	PlaneTranslationGizmoHandleGroup->SetGizmoMaterial(GizmoMaterial);
	PlaneTranslationGizmoHandleGroup->SetupAttachment(GetRootComponent());
	PlaneTranslationGizmoHandleGroup->SetQuVROwningTransformGizmo(this);
	AllHandleGroups.Add(PlaneTranslationGizmoHandleGroup);

	// add Stretch
	StretchGizmoHandleGroup = CreateDefaultSubobject<URuntimeQuVRStretchGizmoHandleGroup>(TEXT("StretchHandles"), true);
	StretchGizmoHandleGroup->SetTranslucentGizmoMaterial(TranslucentGizmoMaterial);
	StretchGizmoHandleGroup->SetGizmoMaterial(GizmoMaterial);
	StretchGizmoHandleGroup->SetShowOnUniversalGizmo(false);
	StretchGizmoHandleGroup->SetQuVROwningTransformGizmo(this);
	StretchGizmoHandleGroup->SetupAttachment(SceneComponent);
	AllHandleGroups.Add(StretchGizmoHandleGroup);



	// add Rotation
	RotationGizmoHandleGroup = CreateDefaultSubobject<URuntimeQuVRPivotRotationGizmoHandleGroup>(TEXT("RotationHandles"), true);
	RotationGizmoHandleGroup->SetQuVROwningTransformGizmo(this);
	RotationGizmoHandleGroup->SetTranslucentGizmoMaterial(TranslucentGizmoMaterial);
	RotationGizmoHandleGroup->SetGizmoMaterial(GizmoMaterial);
	RotationGizmoHandleGroup->SetupAttachment(SceneComponent);
	AllHandleGroups.Add(RotationGizmoHandleGroup);

	// add scale
	ScaleGizmoHandleGroup = CreateDefaultSubobject<URuntimeQuVRPivotScaleGizmoHandleGroup>(TEXT("ScaleHandles"), true);
	ScaleGizmoHandleGroup->SetQuVROwningTransformGizmo(this);
	ScaleGizmoHandleGroup->SetTranslucentGizmoMaterial(TranslucentGizmoMaterial);
	ScaleGizmoHandleGroup->SetGizmoMaterial(GizmoMaterial);
	ScaleGizmoHandleGroup->SetupAttachment(SceneComponent);
	AllHandleGroups.Add(ScaleGizmoHandleGroup);



}

float ARuntimeQuVRTransformAxisActor::GetAnimationAlpha()
{
	// Update animation
	float AnimationAlpha = 0.0f;
	{
		const FTimespan CurrentTime = FTimespan::FromSeconds(FApp::GetCurrentTime());
		const float TimeSinceSelectionChange = (CurrentTime - SelectedAtTime).GetTotalSeconds();
		const float AnimLength = QuVR::QuVRGizmoSelectionAnimationDuration->GetFloat();
		if (TimeSinceSelectionChange < AnimLength)
		{
			AnimationAlpha = FMath::Max(0.0f, TimeSinceSelectionChange / AnimLength);
		}
		else
		{
			AnimationAlpha = 1.0f;
		}

		// Apply a bit of a curve to the animation
		AnimationAlpha = FMath::Pow(AnimationAlpha, QuVR::QuVRGizmoSelectionAnimationCurvePower->GetFloat());
	}

	return AnimationAlpha;
}

void ARuntimeQuVRTransformAxisActor::OnNewObjectsSelected()
{
	SelectedAtTime = FTimespan::FromSeconds(FApp::GetCurrentTime());
}

void ARuntimeQuVRTransformAxisActor::UpdateGizmoGroupDraw()
{
	//////////////////////////////////////////////////////////////////////////
	FTransform InLocalToWorld = QuVRLocalPlayer->GetPlayerController(QuVRWorld)->GetActorTransform();
	FBox InLocalBounds;

	FVector InViewLocation = FVector::ZeroVector;
	APlayerCameraManager* cameraManager = QuVRWorld->GetFirstPlayerController()->PlayerCameraManager;
	check(cameraManager);
	InViewLocation = cameraManager->GetCameraLocation();

	float AnimationAlpha = GetAnimationAlpha();
	float InGizmoHoverScale = 1.0f;
	float InGizmoHoverAnimationDuration = 0.10f;
	static TArray< UActorComponent* > HoveringOverHandles;
	bool bIsHoveringOrDraggingThisHandleGroup = false;

	if (RotationGizmoHandleGroup)
	{
		HoveringOverHandles.Reset();
		HoveringOverHandles.Add(RotationGizmoHandleGroup->HoveringOverTransformGizmoComponent);
		RotationGizmoHandleGroup->UpdateGizmoHandleGroup(
			InLocalToWorld,
			InLocalBounds,
			InViewLocation,
			RotationGizmoHandleGroup->GetAllHandlesVisible(),
			RotationGizmoHandleGroup->GetDraggingGizmoComponent(),
			HoveringOverHandles,
			AnimationAlpha,
			InGizmoHoverScale,
			InGizmoHoverScale,
			InGizmoHoverAnimationDuration,
			bIsHoveringOrDraggingThisHandleGroup
		);
	}
	if (TranslationGizmoHandleGroup)
	{

		HoveringOverHandles.Reset();
		HoveringOverHandles.Add(TranslationGizmoHandleGroup->HoveringOverTransformGizmoComponent);

		TranslationGizmoHandleGroup->UpdateGizmoHandleGroup(
			InLocalToWorld,
			InLocalBounds,
			InViewLocation,
			true,
			TranslationGizmoHandleGroup->DraggingTransformGizmoComponent,
			HoveringOverHandles,
			AnimationAlpha,
			InGizmoHoverScale,
			InGizmoHoverScale,
			InGizmoHoverAnimationDuration,
			bIsHoveringOrDraggingThisHandleGroup
		);
	}
	if (ScaleGizmoHandleGroup)
	{
		HoveringOverHandles.Reset();
		HoveringOverHandles.Add(ScaleGizmoHandleGroup->HoveringOverTransformGizmoComponent);

		ScaleGizmoHandleGroup->UpdateGizmoHandleGroup(
			InLocalToWorld,
			InLocalBounds,
			InViewLocation,
			true,
			ScaleGizmoHandleGroup->DraggingTransformGizmoComponent,
			HoveringOverHandles,
			AnimationAlpha,
			InGizmoHoverScale,
			InGizmoHoverScale,
			InGizmoHoverAnimationDuration,
			bIsHoveringOrDraggingThisHandleGroup
		);
	}

}
void ARuntimeQuVRTransformAxisActor::HitObject()
{
	FHitResult HitResult;
	bool bHit = false;
	APlayerController* PlayController = QuVRLocalPlayer->GetPlayerController(QuVRWorld);
	bHit = PlayController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult);
	if (bHit)
	{
		if (HitResult.GetActor())
		{
			if (TranslationGizmoHandleGroup)
			{
				TranslationGizmoHandleGroup->StartTracking(HitResult.GetActor());
			}
			if (RotationGizmoHandleGroup)
			{
				RotationGizmoHandleGroup->StartTracking(HitResult.GetActor());
			}
			if (ScaleGizmoHandleGroup)
			{
				ScaleGizmoHandleGroup->StartTracking(HitResult.GetActor());
			}
			SetActorHiddenInGame(false);
		}
	}
	else
	{
		PlayController->bShowMouseCursor = true;
	}
}
void ARuntimeQuVRTransformAxisActor::GizmoRotation(const FVector& InLocation, FVector& OutDrag, FRotator& OutRotation, FVector& OutScale)
{
	OutDrag = FVector::ZeroVector;
	OutRotation = FRotator::ZeroRotator;
	OutScale = FVector::ZeroVector;
	FVector InOutDragDelta = FVector::ZeroVector;
	bool bUsedDragModifier = false;
	FVector ViewLocation;
	FRotator ViewRotation;
	check(QuVRLocalPlayer->ViewportClient->Viewport);

	FSceneViewFamilyContext viewFamily(
		FSceneViewFamily::ConstructionValues(
			QuVRLocalPlayer->ViewportClient->Viewport,
			QuVRWorld->Scene,
			QuVRLocalPlayer->ViewportClient->EngineShowFlags).SetRealtimeUpdate(true));

	FSceneView* SceneView = QuVRLocalPlayer->CalcSceneView(&viewFamily, ViewLocation, ViewRotation, QuVRLocalPlayer->ViewportClient->Viewport);
	InOutDragDelta = WorldInteraction->GetMouseDelta();
	FVector2D NewOrigin;
	SceneView->ScreenToPixel(SceneView->WorldToScreen(InLocation), NewOrigin);

	FVector DirectionToWidget = SceneView->IsPerspectiveProjection() ? (InLocation - SceneView->ViewMatrices.GetViewOrigin()) : -SceneView->GetViewDirection();
	DirectionToWidget.Normalize();
	QuVRTransformAlgorithm->GetRotationArc(SceneView, RotationGizmoHandleGroup->GetHandleHoveredType(),InLocation, DirectionToWidget,1);
	QuVRTransformAlgorithm->ConvertMouseMovementToAxisMovement(NewOrigin,bUsedDragModifier, InOutDragDelta, OutDrag, OutRotation, OutScale);

}

void ARuntimeQuVRTransformAxisActor::GizmoTranslation(const FVector& InLocation, FVector& OutDrag, FRotator& OutRotation, FVector& OutScale)
{
	OutDrag = FVector::ZeroVector;
	OutRotation = FRotator::ZeroRotator;
	OutScale = FVector::ZeroVector;

	FVector ViewLocation;
	FRotator ViewRotation;
	check(QuVRLocalPlayer->ViewportClient->Viewport);

	FSceneViewFamilyContext viewFamily(
		FSceneViewFamily::ConstructionValues(
			QuVRLocalPlayer->ViewportClient->Viewport,
			QuVRWorld->Scene,
			QuVRLocalPlayer->ViewportClient->EngineShowFlags).SetRealtimeUpdate(true));

	FSceneView* SceneView = QuVRLocalPlayer->CalcSceneView(&viewFamily, ViewLocation, ViewRotation, QuVRLocalPlayer->ViewportClient->Viewport);

	FVector2D MousePosition = FVector2D(QuVRLocalPlayer->ViewportClient->Viewport->GetMouseX(), QuVRLocalPlayer->ViewportClient->Viewport->GetMouseY());
	//calculate mouse position
	check(QuVRLocalPlayer->ViewportClient->Viewport);
	QuVRTransformAlgorithm->AbsoluteTranslationConvertMouseMovementToAxisMovement(SceneView, InLocation, MousePosition, OutDrag, OutRotation, OutScale);

}

class URuntimeQuVRWorldInteraction* ARuntimeQuVRTransformAxisActor::GetOwnerWorldInteraction() const
{
	return WorldInteraction;
}
EQuVRMode ARuntimeQuVRTransformAxisActor::GetGizmoType() const
{
	return GizmoType;
}

void ARuntimeQuVRTransformAxisActor::UpdateGizmoAxis()
{
	// translation
	UpdateTranslation();

	// Rotation
//	UpdateRotation();

	// Scale
//	UpdateScale();

	// update GroupDraw
	UpdateGizmoGroupDraw();
}


/** UpdateTranslation */
void ARuntimeQuVRTransformAxisActor::UpdateTranslation()
{
	// Move
	if (bIsHover && bIsMouseButtonDown)
	{
		bIsDrag = true;
		FRotator rot;
		FVector scale;
		check(TranslationGizmoHandleGroup->GetDragActor());
		if (EComponentMobility::Movable == TranslationGizmoHandleGroup->GetDragActor()->GetDefaultAttachComponent()->Mobility)
		{
			GizmoTranslation(GetActorLocation(), vDragV3D, rot, scale);
			SetActorLocation(GetActorLocation() + vDragV3D);
			TranslationGizmoHandleGroup->SetIsDrag(bIsDrag);
			TranslationGizmoHandleGroup->UpdateAxisToDragActorTranslational(vDragV3D);
			vDragV3D = FVector::ZeroVector;
		}
	}
	else
	{
		bIsDrag = false;
		TranslationGizmoHandleGroup->UpdateDragActorToAxis();
	}
}

/** UpdateRotation */
void ARuntimeQuVRTransformAxisActor::UpdateRotation()
{
	// Rotate
	if (bIsHover && bIsMouseButtonDown)
	{
		QuVRLocalPlayer->GetPlayerController(QuVRWorld)->bShowMouseCursor = false;
		bIsDrag = true;
		FRotator rot;
		FVector scale;
		check(RotationGizmoHandleGroup->GetDragActor());
		if (EComponentMobility::Movable == RotationGizmoHandleGroup->GetDragActor()->GetDefaultAttachComponent()->Mobility)
		{
			float fDeltaX = 0;
			float fDeltaY = 0;
			QuVRLocalPlayer->GetPlayerController(QuVRWorld)->GetInputMouseDelta(fDeltaX, fDeltaY);
			WorldInteraction->AddMouseDelta(fDeltaX, fDeltaY);
			GizmoRotation(GetActorLocation(), vDragV3D, rot, scale);

			if (!rot.IsZero())
			{
				QuVRTransformAlgorithm->UpdateDeltaRotation();
			}

			RotationGizmoHandleGroup->SetIsDrag(bIsDrag);
			RotationGizmoHandleGroup->UpdateAxisToDragActorRotation(rot);
			vDragV3D = FVector::ZeroVector;
			rot = FRotator::ZeroRotator;
		}
	}
	else
	{
		bIsDrag = false;
		RotationGizmoHandleGroup->UpdateDragActorToAxis();
	}
}

/** UpdateScale */
void ARuntimeQuVRTransformAxisActor::UpdateScale()
{

}

/*
FTransform InLocalToWorld = FTransform::Identity;
FBox InLocalBounds(32.0f);
FVector InViewLocation(0, 0, 0);
bool bInAllHandlesVisible = true;
bool AnimationAlpha = true;
float InGizmoHoverScale = 1.0f;
float InGizmoHoverAnimationDuration = 0.10f;
static TArray< UActorComponent* > HoveringOverHandles;
HoveringOverHandles.Reset();
HoveringOverHandles.Add(TranslationGizmoHandleGroup);

// Update all the handles
for (URuntimeQuVRGizmoHandleGroup* HandleGroup : AllHandleGroups)
{
	if (HandleGroup != nullptr)
	{
		bool bIsHoveringOrDraggingThisHandleGroup = false;

		const float Scale = 1.0;

		HandleGroup->UpdateGizmoHandleGroup(InLocalToWorld, InLocalBounds, InViewLocation, bInAllHandlesVisible, TranslationGizmoHandleGroup,
			HoveringOverHandles, AnimationAlpha, Scale, InGizmoHoverScale, InGizmoHoverAnimationDuration, / * Out * / bIsHoveringOrDraggingThisHandleGroup);


		if (HandleGroup)
		{
			HandleGroup->UpdateVisibilityAndCollision(EQuVRGizmoHandleTypes::QUVR_Translate, EQuVRCoordSystem::QuVR_COORD_World, bInAllHandlesVisible, false, TranslationGizmoHandleGroup);
		}
	}
}

*/

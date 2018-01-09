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
ARuntimeQuVRTransformAxisActor::ARuntimeQuVRTransformAxisActor():WorldInteraction(nullptr)
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

	SetActorHiddenInGame(true);
}


// Called every frame
void ARuntimeQuVRTransformAxisActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Move
	if (bIsHover && bIsMouseButtonDown)
	{
		bIsDrag = true;
		FRotator rot;
		FVector scale;
		if (EComponentMobility::Movable  == TranslationGizmoHandleGroup->GetDragActor()->GetDefaultAttachComponent()->Mobility)
		{
			TranslationCalac(GetActorLocation(), vDragV3D, rot, scale);
			SetActorLocation(GetActorLocation() + vDragV3D);
			TranslationGizmoHandleGroup->UpdateDragActorTranslate(vDragV3D);
			vDragV3D = FVector::ZeroVector;
		}

	}
	else
	{
		bIsDrag = false;
	}
	UpdateGizmo();
}

void ARuntimeQuVRTransformAxisActor::Destroyed()
{
	delete QuVRTransformAlgorithm;
	QuVRTransformAlgorithm = 0;

	delete QuVRWorld;
	QuVRWorld = 0;

	delete QuVRLocalPlayer;
	QuVRLocalPlayer = 0;
}

void ARuntimeQuVRTransformAxisActor::StartTracking()
{

	if (QuVRTransformAlgorithm)
	{
		QuVRTransformAlgorithm->ResetInitialTranslationOffset();
		QuVRTransformAlgorithm->ResetDeltaRotation();
	}

	if (TranslationGizmoHandleGroup->GetHandleHoveredType() != EQuVRGizmoHandleHoveredTypes::QUVR_VOID)
	{
		bIsHover = true;
	}
	if (bIsHover)
	{
		EAxisList::Type InCurrentAxis = EAxisList::Type::None;

		switch (TranslationGizmoHandleGroup->GetHandleHoveredType())
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
		QuVRTransformAlgorithm->SetCurrentAxis(InCurrentAxis);
	}
//	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString("ButtonClicked"));

}

void ARuntimeQuVRTransformAxisActor::EndTracking()
{
	if (QuVRTransformAlgorithm)
	{
		QuVRTransformAlgorithm->SetDragging(false);
		QuVRTransformAlgorithm->ResetDeltaRotation();
	}
}

void ARuntimeQuVRTransformAxisActor::LeftButtonPressed()
{
	HitObject();
	StartTracking();
	bIsMouseButtonDown = true;
}

void ARuntimeQuVRTransformAxisActor::LeftButtonReleased()
{
//	TranslationGizmoHandleGroup->EndTracking();
	EndTracking();
	bIsMouseButtonDown = false;
	bIsDrag = false;
	bIsHover = false;
}

void ARuntimeQuVRTransformAxisActor::RightButtonPressed()
{
	//	TranslationGizmoHandleGroup->EndTracking();
	HideGizemo();
}

void ARuntimeQuVRTransformAxisActor::HideGizemo()
{
	SetActorHiddenInGame(true);
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

	TranslationGizmoHandleGroup = CreateDefaultSubobject<URuntimeQuVRPivotTranslationGizmoHandleGroup>(TEXT("TranslationHandles"), true);
	TranslationGizmoHandleGroup->SetTranslucentGizmoMaterial(TranslucentGizmoMaterial);
	TranslationGizmoHandleGroup->SetGizmoMaterial(GizmoMaterial);
	TranslationGizmoHandleGroup->SetupAttachment(GetRootComponent());
	AllHandleGroups.Add(TranslationGizmoHandleGroup);
	PlaneTranslationGizmoHandleGroup = CreateDefaultSubobject<URuntimeQuVRPivotPlaneTranslationGizmoHandleGroup>(TEXT("PlaneTranslationHandles"), true);
	PlaneTranslationGizmoHandleGroup->SetTranslucentGizmoMaterial(TranslucentGizmoMaterial);
	PlaneTranslationGizmoHandleGroup->SetGizmoMaterial(GizmoMaterial);
	PlaneTranslationGizmoHandleGroup->SetupAttachment(GetRootComponent());
	AllHandleGroups.Add(PlaneTranslationGizmoHandleGroup);



/*
	StretchGizmoHandleGroup = CreateDefaultSubobject<URuntimeQuVRStretchGizmoHandleGroup>(TEXT("StretchHandles"), true);
	StretchGizmoHandleGroup->SetTranslucentGizmoMaterial(TranslucentGizmoMaterial);
	StretchGizmoHandleGroup->SetGizmoMaterial(GizmoMaterial);
	StretchGizmoHandleGroup->SetShowOnUniversalGizmo(false);
	StretchGizmoHandleGroup->SetupAttachment(SceneComponent);
	AllHandleGroups.Add(StretchGizmoHandleGroup);
*/


}


void ARuntimeQuVRTransformAxisActor::OnClicked_AxisX(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
//	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString("Begin++++++++++++ ") + Hit.Location.ToString());
//	FPlatformMisc::MessageBoxExt(EAppMsgType::Ok, TEXT("OnClicked_AxisX"), TEXT("OnClicked_AxisX"));
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

void ARuntimeQuVRTransformAxisActor::UpdateGizmo()
{
	//////////////////////////////////////////////////////////////////////////
	FTransform InLocalToWorld = FTransform::Identity;
	FBox InLocalBounds(32.0f);
	FVector InViewLocation = InLocalToWorld.GetLocation();
	bool bInAllHandlesVisible = true;
	float AnimationAlpha = GetAnimationAlpha();
	float InGizmoHoverScale = 1.0f;
	float InGizmoHoverAnimationDuration = 0.10f;
	static TArray< UActorComponent* > HoveringOverHandles;
	HoveringOverHandles.Reset();
	HoveringOverHandles.Add(TranslationGizmoHandleGroup->HoveringOverTransformGizmoComponent);
	bool bIsHoveringOrDraggingThisHandleGroup = false;
	TranslationGizmoHandleGroup->UpdateGizmoHandleGroup(
		InLocalToWorld,
		InLocalBounds,
		InViewLocation,
		bInAllHandlesVisible,
		TranslationGizmoHandleGroup->HoveringOverTransformGizmoComponent,
		HoveringOverHandles,
		AnimationAlpha,
		InGizmoHoverScale,
		InGizmoHoverScale,
		InGizmoHoverAnimationDuration,
		bIsHoveringOrDraggingThisHandleGroup
	);

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
			TranslationGizmoHandleGroup->StartTracking(HitResult.GetActor());
			SetActorHiddenInGame(false);
		}
	}
}

void ARuntimeQuVRTransformAxisActor::TranslationCalac(const FVector& InLocation, FVector& OutDrag, FRotator& OutRotation, FVector& OutScale)
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
	//////////////////////////////////////////////////////////////////////////
	QuVRTransformAlgorithm->AbsoluteTranslationConvertMouseMovementToAxisMovement(SceneView, InLocation, MousePosition, OutDrag, OutRotation, OutScale);

}

class URuntimeQuVRWorldInteraction* ARuntimeQuVRTransformAxisActor::GetOwnerWorldInteraction() const
{
	return WorldInteraction;
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

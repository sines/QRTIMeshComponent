// Copyright 2017 Louis. All Rights Reserved.

#include "RuntimeQuVRTransformAxisActor.h"
#include "RuntimeQuVRTransformAlgorithm.h"
#include "RuntimeQuVRGizmoHandleGroup.h"
#include "RuntimeQuVRAssetContainer.h"
#include "Misc/App.h"

using namespace RuntimeQuVRtransformType;

void ARuntimeQuVRTransformAxisActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}
// Sets default values
ARuntimeQuVRTransformAxisActor::ARuntimeQuVRTransformAxisActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	check(SceneComponent != nullptr);
	RootComponent = SceneComponent;

	CrateHandleGroups();
}

// Called when the game starts or when spawned
void ARuntimeQuVRTransformAxisActor::BeginPlay()
{
	Super::BeginPlay();

	QuVRTransformAlgorithm = new FRuntimeQuVRTransformAlgorithm();
	QuVRWorld = GetWorld();
	check(QuVRWorld);

	APlayerCameraManager* cameraManager = QuVRWorld->GetFirstPlayerController()->PlayerCameraManager;
	QuVRLocalPlayer = QuVRWorld->GetFirstLocalPlayerFromController();
	
	APlayerController* PlayController = QuVRLocalPlayer->GetPlayerController(QuVRWorld);
	check(PlayController);
	EnableInput(PlayController);
	PlayController->InputComponent->BindKey(EKeys::LeftMouseButton, EInputEvent::IE_Pressed, this, &ARuntimeQuVRTransformAxisActor::ButtonPressed);
	PlayController->InputComponent->BindKey(EKeys::LeftMouseButton, EInputEvent::IE_Released, this, &ARuntimeQuVRTransformAxisActor::ButtonReleased);

	//////////////////////////////////////////////////////////////////////////
	FTransform InLocalToWorld = FTransform::Identity;
//	InLocalToWorld.SetRotation();
	FBox InLocalBounds(32.0f);
	FVector InViewLocation= InLocalToWorld.GetLocation();
	bool bInAllHandlesVisible = true;
	float AnimationAlpha = GetAnimationAlpha();
	float InGizmoHoverScale = 1.0f;
	float InGizmoHoverAnimationDuration = 0.10f;
	static TArray< UActorComponent* > HoveringOverHandles;
	HoveringOverHandles.Reset();

	bool bIsHoveringOrDraggingThisHandleGroup = false;
	TranslationGizmoHandleGroup->UpdateGizmoHandleGroup(
		InLocalToWorld,
		InLocalBounds,
		InViewLocation,
		bInAllHandlesVisible,
		NULL,
		HoveringOverHandles,
		AnimationAlpha,
		InGizmoHoverScale,
		InGizmoHoverScale,
		InGizmoHoverAnimationDuration,
		bIsHoveringOrDraggingThisHandleGroup
	);
}

// Called every frame
void ARuntimeQuVRTransformAxisActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

}

void ARuntimeQuVRTransformAxisActor::EndTracking()
{
	if (QuVRTransformAlgorithm)
	{
		QuVRTransformAlgorithm->SetDragging(false);
		QuVRTransformAlgorithm->ResetDeltaRotation();
	}
}

void ARuntimeQuVRTransformAxisActor::ButtonPressed()
{
	StartTracking();

}

void ARuntimeQuVRTransformAxisActor::ButtonReleased()
{
	EndTracking();
}

void ARuntimeQuVRTransformAxisActor::CrateHandleGroups()
{
	// Get AssetContainer
	const URuntimeQuVRAssetContainer& AssetContainer = URuntimeQuVRAssetContainer::LoadAssetContainer();
	UMaterialInterface* GizmoMaterial = AssetContainer.TransformGizmoMaterial;
	UMaterialInterface* TranslucentGizmoMaterial = AssetContainer.TranslucentTransformGizmoMaterial;

	TranslationGizmoHandleGroup = CreateDefaultSubobject<URuntimeQuVRPivotTranslationGizmoHandleGroup>(TEXT("TranslationHandles"), true);
	TranslationGizmoHandleGroup->SetTranslucentGizmoMaterial(TranslucentGizmoMaterial);
	TranslationGizmoHandleGroup->SetGizmoMaterial(GizmoMaterial);
	TranslationGizmoHandleGroup->SetupAttachment(SceneComponent);
	AllHandleGroups.Add(TranslationGizmoHandleGroup);
	PlaneTranslationGizmoHandleGroup = CreateDefaultSubobject<URuntimeQuVRPivotPlaneTranslationGizmoHandleGroup>(TEXT("PlaneTranslationHandles"), true);
	PlaneTranslationGizmoHandleGroup->SetTranslucentGizmoMaterial(TranslucentGizmoMaterial);
	PlaneTranslationGizmoHandleGroup->SetGizmoMaterial(GizmoMaterial);
	PlaneTranslationGizmoHandleGroup->SetupAttachment(SceneComponent);
	AllHandleGroups.Add(PlaneTranslationGizmoHandleGroup);


	URuntimeQuVRHandleMeshComponent* mesh = TranslationGizmoHandleGroup->GetHandleMesh(EAxisList::X);
	mesh->OnBeginCursorOver.AddDynamic(this,&ARuntimeQuVRTransformAxisActor::OnHover_AxisX);
	
/*
	StretchGizmoHandleGroup = CreateDefaultSubobject<URuntimeQuVRStretchGizmoHandleGroup>(TEXT("StretchHandles"), true);
	StretchGizmoHandleGroup->SetTranslucentGizmoMaterial(TranslucentGizmoMaterial);
	StretchGizmoHandleGroup->SetGizmoMaterial(GizmoMaterial);
	StretchGizmoHandleGroup->SetShowOnUniversalGizmo(false);
	StretchGizmoHandleGroup->SetupAttachment(SceneComponent);
	AllHandleGroups.Add(StretchGizmoHandleGroup);
*/


}
void ARuntimeQuVRTransformAxisActor::OnHover_AxisX(class UPrimitiveComponent* OtherComp)
{

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

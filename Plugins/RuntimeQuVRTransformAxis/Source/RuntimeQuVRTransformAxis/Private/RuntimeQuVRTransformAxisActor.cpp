// Copyright 2017 Louis. All Rights Reserved.

#include "RuntimeQuVRTransformAxisActor.h"
#include "RuntimeQuVRTransformAlgorithm.h"

void ARuntimeQuVRTransformAxisActor::OnConstruction(const FTransform& Transform)
{

}
// Sets default values
ARuntimeQuVRTransformAxisActor::ARuntimeQuVRTransformAxisActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
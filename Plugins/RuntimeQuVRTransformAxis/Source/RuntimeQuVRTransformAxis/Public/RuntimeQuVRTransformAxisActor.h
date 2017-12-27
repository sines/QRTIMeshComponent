// Copyright 2017 Louis. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RuntimeQuVRTransformAxisActor.generated.h"

class FRuntimeQuVRTransformAlgorithm;
UCLASS()
class RUNTIMEQUVRTRANSFORMAXIS_API ARuntimeQuVRTransformAxisActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARuntimeQuVRTransformAxisActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed();
	/**
	* Begin tracking at the specified location for the specified viewport.
	*/
	void StartTracking();

	/**
	* Called when a mouse button has been released.  If there are no other
	* mouse buttons being held down, the internal information is reset.
	*/
	void EndTracking();

	/**
	* Adds delta movement into the tracker.
	*/
	void  AddDelta(const int32 InDelta, bool InNudge);

public:	
	// construction
	void OnConstruction(const FTransform& Transform) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	// RuntimeQuVRtransformAlgorithm instance
	FRuntimeQuVRTransformAlgorithm*  QuVRTransformAlgorithm;
	
	// Get GWorld
	UWorld* QuVRWorld;
	// Current LocalPlayer
	ULocalPlayer* QuVRLocalPlayer;

};

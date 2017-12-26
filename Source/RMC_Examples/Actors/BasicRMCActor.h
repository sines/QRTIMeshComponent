// Copyright 2016 Chris Conway (Koderz). All Rights Reserved.

#pragma once

#include "RuntimeMeshObjectBase.h"
#include "QuVRTransformAlgorithm.h"
#include "BasicRMCActor.generated.h"

UENUM(BlueprintType)
enum class ECoordinateAxisType: uint8
{

	None = 0,
	X = 1,
	Y = 2,

	XY = 3,
	
	Z = 4,

	XZ = 5,

	YZ = 6,
//	ZRotation = 6,

	XYZ = 7,
	Screen = 8,
//	Rotate2D = 8,


	All = 15,


	// alias over Screen since it isn't used when the 2d translate rotate widget is being use
};


/**
 * 
 */
UCLASS()
class RMC_EXAMPLES_API ABasicRMCActor : public ARuntimeMeshObjectBase
{
	GENERATED_BODY()
public:

	void OnConstruction(const FTransform& Transform) override;
	
	virtual void Destroyed();

	FQuVRTransformAlgorithm* QuVRTransformAlgorithm;

	UFUNCTION(BlueprintCallable, Category = "ABasicRMCActor|CreateXXX")
	void CreateQuVRCoordinateAxis();
	
	UFUNCTION(BlueprintCallable, Category = "ABasicRMCActor|GetXXX")
	void GetQuVRCoordinateAxis(const FVector& InLocation, FVector& OutDrag, FRotator& OutRotation, FVector& OutScale);
	

	UFUNCTION(BlueprintCallable, Category = "ABasicRMCActor|SetxxType")
		void SetQuVRCoordinateAxisType(ECoordinateAxisType xxxtype);

	UFUNCTION(BlueprintCallable, Category = "ABasicRMCActor|SetxxType")
		ECoordinateAxisType GetQuVRCoordinateAxisType();


	UFUNCTION(BlueprintCallable, Category = "ABasicRMCActor|CreateXXX")
		void GetAxisFloor(const FVector& inPos, FPlane& outPlane);


	UFUNCTION(BlueprintCallable, Category = "ABasicRMCActor|CreateXXX")
		void StartTracking();


	UFUNCTION(BlueprintCallable, Category = "ABasicRMCActor|CreateXXX")
		void EndTracking();
	
	UWorld* world;
	ULocalPlayer* localPlayer;
};

// Copyright 2016 Chris Conway (Koderz). All Rights Reserved.

#pragma once

#include "RuntimeMeshObjectBase.h"
#include "QuVRCoordinateAxis.h"
#include "BasicRMCActor.generated.h"

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

	FQuVRCoordinateAxis* QuVRCoordinateAxis;

//	UFUNCTION(BlueprintCallable, Category = "xxx", meta = (AutoCreateRefTerm = "xxx"))
//	void GetXXX(FSceneView* InView, const FVector& InLocation, const FVector2D& InMousePosition, FVector& OutDrag, FRotator& OutRotation, FVector& OutScale);
	
};

// Copyright 2016 Chris Conway (Koderz). All Rights Reserved.

#pragma once

#include "RuntimeMeshObjectBase.h"
#include "QuVRCoordinateAxis.h"
#include "BasicRMCActor.generated.h"

UENUM(BlueprintType)
enum class EUpdateState : EAxisList::Type
{
	
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

	FQuVRCoordinateAxis* QuVRCoordinateAxis;

	UFUNCTION(BlueprintCallable, Category = "Components|CreateXXX")
	void CreateXXX();
	
	UFUNCTION(BlueprintCallable, Category = "Components|GetXXX")
	void GetXXX(const FVector& InLocation, const FVector2D& InMousePosition, FVector& OutDrag, FRotator& OutRotation, FVector& OutScale);
	

	UFUNCTION(BlueprintCallable, Category = "Components|SetxxType")
		void SetXXXType(EUpdateState::Type xxxtype);

};

// Copyright 2016 Chris Conway (Koderz). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "RuntimeQuVRHandleMeshComponent.generated.h"

/**
 * 
 */
UCLASS(hidecategories = Object)
class URuntimeQuVRHandleMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	

public:

	/** Default constructor */
	URuntimeQuVRHandleMeshComponent();

	//~ Begin UPrimitiveComponent Interface
	virtual class FPrimitiveSceneProxy* CreateSceneProxy() override;
	//~ End UPrimitiveComponent Interface
	
	
};
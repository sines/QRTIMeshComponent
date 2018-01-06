// Copyright 2018 Louis. All Rights Reserved.

#include "RuntimeQuVRAssetContainer.h"


const FString URuntimeQuVRAssetContainer::AssetContainerPath = FString("/Game/RMC_BP/QuVRAssetContainerData");

const class URuntimeQuVRAssetContainer& URuntimeQuVRAssetContainer::LoadAssetContainer()
{
	static const URuntimeQuVRAssetContainer& Instance = *LoadObject<URuntimeQuVRAssetContainer>(nullptr, *URuntimeQuVRAssetContainer::AssetContainerPath);
	return Instance;
}
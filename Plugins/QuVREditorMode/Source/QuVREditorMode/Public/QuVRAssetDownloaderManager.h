// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuVRAssetDownloader.h"
#include "QuVRAssetDownloaderManager.generated.h"

/**
 * 
 */
UCLASS()
class QUVREDITORMODE_API UQuVRAssetDownloaderManager : public UObject
{
	GENERATED_BODY()

public:
	UQuVRAssetDownloaderManager();
	virtual ~UQuVRAssetDownloaderManager();
	/** Get QuVRAssetDownloaderManager Instance*/
	static UQuVRAssetDownloaderManager* GetInstance();

	TSharedPtr<class UQuVRAssetDownloader> CreateAssetDownloader(FString URL);
	void RemoveAssetDownloader(FString URL);
	TSharedPtr<class UQuVRAssetDownloader> GetAssetDownloader(FString URL);

	void ClearAllDownloader();

private:
	static UQuVRAssetDownloaderManager* StaticInstance;
	TMap<FString, TSharedPtr<class UQuVRAssetDownloader>> AssetloaderList;
};

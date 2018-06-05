// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuVRImageDownloader.h"
#include "QuVRImageDownloaderManager.generated.h"

/**
 * UQuVRImageDownloaderManager
 */
UCLASS()
class QUVREDITORMODE_API UQuVRImageDownloaderManager : public UObject
{
	GENERATED_BODY()

public:
	UQuVRImageDownloaderManager();
	virtual ~UQuVRImageDownloaderManager();
	/** Request we're currently processing */
	static UQuVRImageDownloaderManager* GetInstance();

	TSharedPtr<class UQuVRImageDownloader> CreateImageDownloader(FString URL);
	void RemoveImageDownloader(FString URL);
	TSharedPtr<class UQuVRImageDownloader> GetImageDownloader(FString URL);

	void ClearAllDownloader();

private:
	static UQuVRImageDownloaderManager* StaticInstance;
	TMap<FString, TSharedPtr<class UQuVRImageDownloader>> ImageloaderList;
};

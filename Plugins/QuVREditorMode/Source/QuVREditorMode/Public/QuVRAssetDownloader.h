// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/ObjectMacros.h"
#include "Interfaces/IHttpRequest.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "QuVRFileDownloader.h"
#include "QuVRUtils.h"

#include "QuVRAssetDownloader.generated.h"

class UTexture2DDynamic;

/**
 * 
 */
UCLASS()
class QUVREDITORMODE_API UQuVRAssetDownloader : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()

public:
	~UQuVRAssetDownloader();

public:
	// Void DownLoad Asset.zip
	FReply DownloadAssetFile(FString URL);
	bool GetIsDownloadAsset() { return bIsDownloadAsset; };
	float GetAssetProgressRate() { return fAssetProgressRate; };
	FileDownLoadState GetDownLoadState() { return DownloadFileState; };
	
	void ClearDownloaderState();
private:
	void OnDownloadProegress(int32 ReceivedDataInBytes, int32 TotalDataInBytes, const TArray<uint8>& BinaryData);
	void OnDownloadDone(int32 code);

protected:
	float fAssetProgressRate;
	bool bIsDownloadAsset;
	FileDownLoadState DownloadFileState;
	FString AssetURL;
	TWeakObjectPtr<class UQuVRFileDownloader> AsyncTaskDownloadFile;

public:
	FQuVRFileDownloadDoneC2Delegate OnDownloadAssetDone;
};

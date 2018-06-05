// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/ObjectMacros.h"
#include "Interfaces/IHttpRequest.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "QuVRFileDownloader.h"
#include "QuVRUtils.h"

#include "QuVRImageDownloader.generated.h"

class UTexture2DDynamic;

/**
 * 
 */

UCLASS()
class QUVREDITORMODE_API UQuVRImageDownloader : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	~UQuVRImageDownloader();

public:
	// Void DownLoad Asset.zip
	FReply DownloadImage(FString inURL);
	bool GetIsDownloadImage() { return bIsDownloadImage; };
	FileDownLoadState GetDownLoadState() { return DownloadFileState; };

	void ClearDownloaderState();
protected:
	void CallBackImageEvent(UTexture2DDynamic* texture2D);

protected:
	FString URL;
	bool bIsDownloadImage;
	FileDownLoadState DownloadFileState;

	TWeakObjectPtr<class UQuVRFileDownloader> AsyncTaskDownloadFile;

public:
	FQuVRDownloadImageC2Delegate OnDownloadImageDone;
};

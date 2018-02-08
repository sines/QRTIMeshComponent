// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/ObjectMacros.h"
#include "Interfaces/IHttpRequest.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "QuVRFileDownloader.generated.h"

class UTexture2DDynamic;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FQuVRFileDownloadUpdateProgressDelegate, int32, ReceivedDataInBytes, int32, TotalDataInBytes, const TArray<uint8>&, BinaryData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuVRDownloadImageDelegate, UTexture2DDynamic*, Texture);

DECLARE_MULTICAST_DELEGATE_OneParam(FQuVRDownloadImageRes, class UTexture2DDynamic*);

/**
 * 
 */
UCLASS()
class QUVREDITORMODE_API UQuVRFileDownloader : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()

public:
	static UQuVRFileDownloader* DownloadFile(FString URL);
	void StartDownloadImageFile(FString URL);
private:
	void HandleImageRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSent, int32 BytesReceived);
	void HandleImageRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
public:
	FQuVRFileDownloadUpdateProgressDelegate OnFileDownloadDone;
	FQuVRFileDownloadUpdateProgressDelegate OnUlpdataProegress;

	FQuVRDownloadImageDelegate OnDownloadImageSuccess;
	FQuVRDownloadImageDelegate OnDownloadImageFail;

	FQuVRDownloadImageRes OnDownloadImageRes;
protected:
/** Http Response code */
	int32 ResponseCode;
	FString StatusInfo;
	int32 FileLength;

	FString FileUrl;
	FString LocalFilePath;
};

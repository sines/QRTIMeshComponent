// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/ObjectMacros.h"
#include "Interfaces/IHttpRequest.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "QuVRFileDownloader.generated.h"

class UTexture2DDynamic;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FQuVRFileDownloadUpdateProgressBPDelegate, int32, ReceivedDataInBytes, int32, TotalDataInBytes, const TArray<uint8>&, BinaryData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuVRDownloadImageBPDelegate, UTexture2DDynamic*, Texture);

DECLARE_MULTICAST_DELEGATE_OneParam(FQuVRDownloadImageC2Delegate, class UTexture2DDynamic*);
DECLARE_MULTICAST_DELEGATE_OneParam(FQuVRFileDownloadDoneC2Delegate, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FQuVRDownloadFileC2Delegate, FString);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FQuVRFileDownloadUpdateProgressC2Delegate, int32, int32, const TArray<uint8>&);

// Asset Down Load State
enum class FileDownLoadState : uint8
{
	Unknown,
	Start,
	Post,
	InProgress,
	Finish,
};

/**
*	QuVR File Downloader
*/

UCLASS()
class QUVREDITORMODE_API UQuVRFileDownloader :public UObject
{
	GENERATED_UCLASS_BODY()

public:
	~UQuVRFileDownloader();
	static UQuVRFileDownloader* DownloadImageLoader(FString URL);
	static UQuVRFileDownloader* DownloadZipLoader(FString URL);
	void StartDownloadImageFile(FString URL);
	void StartDownloadZipFile(FString URL);
	void ClearDownloadState();
private:
	void HandleZipRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
	void HandleImageRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
	void HandleRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSent, int32 BytesReceived);
public:
	FQuVRDownloadImageBPDelegate OnDownloadImageSuccess;
	FQuVRDownloadImageBPDelegate OnDownloadImageFail;

	FQuVRDownloadImageC2Delegate OnDownloadImageRes;
	FQuVRFileDownloadDoneC2Delegate OnDownloadFileDone;
	FQuVRFileDownloadUpdateProgressC2Delegate OnUlpdataProegress;

protected:
	TSharedPtr<IHttpRequest> HttpRequest;
/** Http Response code */
	int32 ResponseCode;
	FString StatusInfo;
	int32 FileLength;

	FString FileURL;
	bool IsDownload;
};

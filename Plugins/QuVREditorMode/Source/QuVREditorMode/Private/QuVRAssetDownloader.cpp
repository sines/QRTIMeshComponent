// Fill out your copyright notice in the Description page of Project Settings.

#include "QuVRAssetDownloader.h"
#include "Modules/ModuleManager.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture2DDynamic.h"
#include "Runtime/Online/HTTP/Public/HttpManager.h"
#include "Interfaces/IImageWrapperModule.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "QuVRUtils.h"
#include "QuVRAssetDownloaderManager.h"

float InverseLerp(float A, float B, float Value)
{
	if (FMath::IsNearlyEqual(A, B))
	{
		if (Value < A)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return ((Value - A) / (B - A));
	}
}



//----------------------------------------------------------------------//
// DownloadImage
//----------------------------------------------------------------------//


UQuVRAssetDownloader::~UQuVRAssetDownloader()
{
	
}

UQuVRAssetDownloader::UQuVRAssetDownloader(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	fAssetProgressRate(0.0f),
	bIsDownloadAsset(false),
	DownloadFileState(FileDownLoadState::Start),
	AssetURL(FString(TEXT(""))),
	AsyncTaskDownloadFile(nullptr)
{

}


FReply UQuVRAssetDownloader::DownloadAssetFile(FString URL)
{
	AssetURL = URL;
	if (UQuVRUtils::CheckFileExists(URL))
	{
		bIsDownloadAsset = true;
		DownloadFileState = FileDownLoadState::Finish;
		return FReply::Handled();
	}

	if (5 < URL.Len())
	{
		if (!UQuVRUtils::CheckFileExists(URL))
		{
			if (AsyncTaskDownloadFile.IsValid())
			{
				AsyncTaskDownloadFile->StartDownloadZipFile(URL);
			}
			else
			{

				AsyncTaskDownloadFile = UQuVRFileDownloader::DownloadZipLoader(URL);
				AsyncTaskDownloadFile->OnDownloadFileDone.AddUObject(this, &UQuVRAssetDownloader::OnDownloadDone);
				AsyncTaskDownloadFile->OnUlpdataProegress.AddUObject(this, &UQuVRAssetDownloader::OnDownloadProegress);

			}
			DownloadFileState = FileDownLoadState::Post;
		}
	}
	return FReply::Handled();
}

void UQuVRAssetDownloader::OnDownloadProegress(int32 ReceivedDataInBytes, int32 TotalDataInBytes, const TArray<uint8>& BinaryData)
{
	DownloadFileState = FileDownLoadState::InProgress;
	fAssetProgressRate = InverseLerp(0.0f, (float)TotalDataInBytes, (float)ReceivedDataInBytes);
}

void UQuVRAssetDownloader::OnDownloadDone(int32 code)
{
	DownloadFileState = FileDownLoadState::Finish;
	if (OnDownloadAssetDone.IsBound())
	{
		OnDownloadAssetDone.Broadcast(code);
	}

	UQuVRAssetDownloaderManager::GetInstance()->RemoveAssetDownloader(AssetURL);
}

void UQuVRAssetDownloader::ClearDownloaderState()
{
	DownloadFileState = FileDownLoadState::Start;
	OnDownloadAssetDone.Clear();
	AsyncTaskDownloadFile->ClearDownloadState();
}
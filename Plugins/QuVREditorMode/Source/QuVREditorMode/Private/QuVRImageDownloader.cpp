// Fill out your copyright notice in the Description page of Project Settings.

#include "QuVRImageDownloader.h"
#include "Modules/ModuleManager.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture2DDynamic.h"
#include "Runtime/Online/HTTP/Public/HttpManager.h"
#include "Interfaces/IImageWrapperModule.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "QuVRUtils.h"


//----------------------------------------------------------------------//
// DownloadImage
//----------------------------------------------------------------------//


UQuVRImageDownloader::~UQuVRImageDownloader()
{
	
}

UQuVRImageDownloader::UQuVRImageDownloader(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	bIsDownloadImage(false),
	DownloadFileState(FileDownLoadState::Start),
	URL(FString(TEXT(""))),
	AsyncTaskDownloadFile(nullptr)
{

}

FReply UQuVRImageDownloader::DownloadImage(FString inURL)
{
	URL = inURL;
	if (UQuVRUtils::CheckTempTextureExists(URL))
	{
		bIsDownloadImage = true;
		DownloadFileState = FileDownLoadState::Finish;
		CallBackImageEvent(UQuVRUtils::LoadDyna2DPath(UQuVRUtils::GetSavedTempTextureDir(URL)));
	}
	else
	{
		if (5 < URL.Len())
		{
				if (AsyncTaskDownloadFile.IsValid())
				{
					AsyncTaskDownloadFile->StartDownloadImageFile(URL);
				}
				else
				{
					AsyncTaskDownloadFile = UQuVRFileDownloader::DownloadImageLoader(URL);
					AsyncTaskDownloadFile->OnDownloadImageRes.AddUObject(this, &UQuVRImageDownloader::CallBackImageEvent);

				}

				DownloadFileState = FileDownLoadState::Post;
		}
	}
	return FReply::Handled();
}

void UQuVRImageDownloader::ClearDownloaderState()
{
	DownloadFileState = FileDownLoadState::Start;
	if (OnDownloadImageDone.IsBound())
	{
		OnDownloadImageDone.Clear();
	}

	if (AsyncTaskDownloadFile.IsValid())
	{
		AsyncTaskDownloadFile->ClearDownloadState();
	}
}

void UQuVRImageDownloader::CallBackImageEvent(UTexture2DDynamic* texture2D)
{
	if (texture2D)
	{
		if (OnDownloadImageDone.IsBound())
		{
			OnDownloadImageDone.Broadcast(texture2D);
		}
		bIsDownloadImage = true;
	}
	DownloadFileState = FileDownLoadState::Finish;
	// remove Image Downloader
	UQuVRImageDownloaderManager::GetInstance()->RemoveImageDownloader(URL);
}

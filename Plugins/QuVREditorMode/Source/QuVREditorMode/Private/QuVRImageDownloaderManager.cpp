// Fill out your copyright notice in the Description page of Project Settings.

#include "QuVRImageDownloaderManager.h"

//////////////////////////////////////////////////////////////////////////
/**QuVRImageDownloaderManager*/
//////////////////////////////////////////////////////////////////////////
UQuVRImageDownloaderManager::UQuVRImageDownloaderManager()
{
	ImageloaderList.Empty();
	ImageloaderList.Reset();
}
UQuVRImageDownloaderManager::~UQuVRImageDownloaderManager()
{
	if (StaticInstance)
	{
		StaticInstance->RemoveFromRoot();
	}
}
UQuVRImageDownloaderManager* UQuVRImageDownloaderManager::StaticInstance;
UQuVRImageDownloaderManager* UQuVRImageDownloaderManager::GetInstance()
{
	if (!StaticInstance)
	{
		StaticInstance = NewObject<UQuVRImageDownloaderManager>();
		StaticInstance->AddToRoot();
	}
	return StaticInstance;
}

TSharedPtr<class UQuVRImageDownloader> UQuVRImageDownloaderManager::CreateImageDownloader(FString URL)
{
	if (false == ImageloaderList.Contains(URL))
	{
		TSharedPtr<class UQuVRImageDownloader> ImageDownloaderPtr = MakeShareable(NewObject<UQuVRImageDownloader>());
		ImageDownloaderPtr->AddToRoot();
		ImageloaderList.Add(URL, ImageDownloaderPtr);
		return ImageDownloaderPtr;
	}
	else
	{
		return GetImageDownloader(URL);
	}

}

void UQuVRImageDownloaderManager::RemoveImageDownloader(FString URL)
{
	if (ImageloaderList.Contains(URL))
	{
		TSharedPtr<class UQuVRImageDownloader> CurDownloader  = ImageloaderList.FindAndRemoveChecked(URL);
		CurDownloader->ClearDownloaderState();
		CurDownloader->ConditionalBeginDestroy();
		ImageloaderList.Remove(URL);
	}
}

TSharedPtr<class UQuVRImageDownloader> UQuVRImageDownloaderManager::GetImageDownloader(FString URL)
{

	TSharedPtr<class UQuVRImageDownloader> Result = nullptr;

	TSharedPtr<class UQuVRImageDownloader>* ImageDownloaderPtr = ImageloaderList.Find(URL);
	if (ImageDownloaderPtr != nullptr)
	{
		Result = *ImageDownloaderPtr;
	}
	return Result;
}

void UQuVRImageDownloaderManager::ClearAllDownloader()
{
	for (TMap<FString, TSharedPtr<class UQuVRImageDownloader>>::TConstIterator It(ImageloaderList); It; ++It)
	{
		ImageloaderList.Remove(It.Key());

		TSharedPtr<class UQuVRImageDownloader> Result = It.Value();
		if (Result.IsValid())
		{
			Result->RemoveFromRoot();
			Result->ClearDownloaderState();
			ImageloaderList.Remove(It.Key());
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "QuVRAssetDownloaderManager.h"

//////////////////////////////////////////////////////////////////////////
/**QuVRAssetDownloaderManager*/
//////////////////////////////////////////////////////////////////////////
UQuVRAssetDownloaderManager::UQuVRAssetDownloaderManager()
{
	AssetloaderList.Empty();
	AssetloaderList.Reset();
}
UQuVRAssetDownloaderManager::~UQuVRAssetDownloaderManager()
{
	if (StaticInstance)
	{
		StaticInstance->RemoveFromRoot();
	}

}
UQuVRAssetDownloaderManager* UQuVRAssetDownloaderManager::StaticInstance;

UQuVRAssetDownloaderManager* UQuVRAssetDownloaderManager::GetInstance()
{
	if (!StaticInstance)
	{
		StaticInstance = NewObject<UQuVRAssetDownloaderManager>();
		StaticInstance->AddToRoot();
	}
	return StaticInstance;
}


TSharedPtr<class UQuVRAssetDownloader> UQuVRAssetDownloaderManager::CreateAssetDownloader(FString URL)
{
	if (false == AssetloaderList.Contains(URL))
	{
		TSharedPtr<class UQuVRAssetDownloader> ImageDownloaderPtr = MakeShareable(NewObject<UQuVRAssetDownloader>());
		ImageDownloaderPtr->AddToRoot();
		AssetloaderList.Add(URL, ImageDownloaderPtr);
		return ImageDownloaderPtr;
	}
	else
	{
		return GetAssetDownloader(URL);
	}
}

void UQuVRAssetDownloaderManager::RemoveAssetDownloader(FString URL)
{
	if (AssetloaderList.Contains(URL))
	{
		TSharedPtr<class UQuVRAssetDownloader> CurDownloader = AssetloaderList.FindAndRemoveChecked(URL);
		CurDownloader->ClearDownloaderState();
		CurDownloader->ConditionalBeginDestroy();
		AssetloaderList.Remove(URL);
	}
}

TSharedPtr<class UQuVRAssetDownloader> UQuVRAssetDownloaderManager::GetAssetDownloader(FString URL)
{

	TSharedPtr<class UQuVRAssetDownloader> Result = nullptr;

	TSharedPtr<class UQuVRAssetDownloader>* ImageDownloaderPtr = AssetloaderList.Find(URL);
	if (ImageDownloaderPtr != nullptr)
	{
		Result = *ImageDownloaderPtr;
	}
	return Result;
}

void UQuVRAssetDownloaderManager::ClearAllDownloader()
{
	TSharedPtr<class UQuVRAssetDownloader> Result = nullptr;
	for (TMap<FString, TSharedPtr<class UQuVRAssetDownloader>>::TConstIterator It(AssetloaderList); It; ++It)
	{
		Result = It.Value();
		if (Result.IsValid())
		{
			Result->RemoveFromRoot();
			Result->ClearDownloaderState();
			Result->ConditionalBeginDestroy();
		}
		AssetloaderList.Remove(It.Key());
	}
}
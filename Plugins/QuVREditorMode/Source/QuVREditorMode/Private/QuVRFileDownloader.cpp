// Fill out your copyright notice in the Description page of Project Settings.

#include "QuVRFileDownloader.h"
#include "Modules/ModuleManager.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture2DDynamic.h"
#include "Interfaces/IImageWrapperModule.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "QuVRUtils.h"


//----------------------------------------------------------------------//
// DownloadImage
//----------------------------------------------------------------------//

#if !UE_SERVER
static void WriteRawToTexture_RenderThread(FTexture2DDynamicResource* TextureResource, const TArray<uint8>& RawData, bool bUseSRGB = true)
{
	check(IsInRenderingThread());

	FTexture2DRHIParamRef TextureRHI = TextureResource->GetTexture2DRHI();

	int32 Width = TextureRHI->GetSizeX();
	int32 Height = TextureRHI->GetSizeY();

	uint32 DestStride = 0;
	uint8* DestData = reinterpret_cast<uint8*>(RHILockTexture2D(TextureRHI, 0, RLM_WriteOnly, DestStride, false, false));

	for (int32 y = 0; y < Height; y++)
	{
		uint8* DestPtr = &DestData[(Height - 1 - y) * DestStride];

		const FColor* SrcPtr = &((FColor*)(RawData.GetData()))[(Height - 1 - y) * Width];
		for (int32 x = 0; x < Width; x++)
		{
			*DestPtr++ = SrcPtr->B;
			*DestPtr++ = SrcPtr->G;
			*DestPtr++ = SrcPtr->R;
			*DestPtr++ = SrcPtr->A;
			SrcPtr++;
		}
	}

	RHIUnlockTexture2D(TextureRHI, 0, false, false);
}

#endif


UQuVRFileDownloader::UQuVRFileDownloader(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IsDownload = false;
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		AddToRoot();
	}
}
UQuVRFileDownloader* UQuVRFileDownloader::DownloadZipLoader(FString URL)
{

	UQuVRFileDownloader* DownloadTask = NewObject<UQuVRFileDownloader>();
	DownloadTask->AddToRoot();
	DownloadTask->StartDownloadZipFile(URL);
	return DownloadTask;
}

UQuVRFileDownloader* UQuVRFileDownloader::DownloadImageLoader(FString URL)
{
	UQuVRFileDownloader* DownloadTask = NewObject<UQuVRFileDownloader>();
	DownloadTask->AddToRoot();
	DownloadTask->StartDownloadImageFile(URL);
	return DownloadTask;
}

void UQuVRFileDownloader::StartDownloadImageFile(FString URL)
{
	FileURL = URL;
#if !UE_SERVER
	if (false == IsDownload)
	{
		// Create the Http request and add to pending request list
		HttpRequest->OnProcessRequestComplete().Unbind();
		HttpRequest->OnRequestProgress().Unbind();
		HttpRequest->CancelRequest();
		HttpRequest->OnProcessRequestComplete().BindUObject(this, &UQuVRFileDownloader::HandleImageRequestComplete);
		HttpRequest->OnRequestProgress().BindUObject(this, &UQuVRFileDownloader::HandleRequestProgress);
		HttpRequest->SetURL(URL);
		HttpRequest->SetVerb(TEXT("GET"));
		HttpRequest->ProcessRequest();
	}
	#else
		// On the server we don't execute fail or success we just don't fire the request.
		RemoveFromRoot();
	#endif

}

void UQuVRFileDownloader::StartDownloadZipFile(FString URL)
{
	FileURL = URL;
#if !UE_SERVER
	if (false == IsDownload)
	{
		// Create the Http request and add to pending request list
		HttpRequest->OnProcessRequestComplete().Unbind();
		HttpRequest->OnRequestProgress().Unbind();
		HttpRequest->CancelRequest();
		HttpRequest->OnProcessRequestComplete().BindUObject(this, &UQuVRFileDownloader::HandleZipRequestComplete);
		HttpRequest->OnRequestProgress().BindUObject(this, &UQuVRFileDownloader::HandleRequestProgress);
		HttpRequest->SetURL(URL);
		HttpRequest->SetVerb(TEXT("GET"));
		HttpRequest->ProcessRequest();
	}
#else
	// On the server we don't execute fail or success we just don't fire the request.
	RemoveFromRoot();
#endif

}

void UQuVRFileDownloader::HandleZipRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
#if !UE_SERVER
	RemoveFromRoot();

	if (bSucceeded && HttpResponse.IsValid() && HttpResponse->GetContentLength() > 0)
	{
		// Save File
		if (!UQuVRUtils::CheckFileExists(FileURL))
		{
			FileLength = HttpResponse->GetContentLength();
			const TArray<uint8>& fileContent = HttpResponse->GetContent();
			FString FullPath; FString FilePath;
			UQuVRUtils::GetObjectPath(FileURL, FullPath, FilePath);
			FFileHelper::SaveArrayToFile(fileContent, *(FullPath));
			UQuVRUtils::UnzipFile(FullPath, FilePath);
			IsDownload = true;
		}
	}

	OnDownloadFileDone.Broadcast(HttpResponse->GetResponseCode());
#endif
}

void UQuVRFileDownloader::HandleImageRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
#if !UE_SERVER

	RemoveFromRoot();

	if (bSucceeded && HttpResponse.IsValid() && HttpResponse->GetContentLength() > 0)
	{
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		IImageWrapperPtr ImageWrappers[3] =
		{
			ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG),
			ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG),
			ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP),
		};

		for (auto ImageWrapper : ImageWrappers)
		{
			if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(HttpResponse->GetContent().GetData(), HttpResponse->GetContentLength()))
			{
				const TArray<uint8>* RawData = NULL;
				if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
				{
					if (UTexture2DDynamic* Texture = UTexture2DDynamic::Create(ImageWrapper->GetWidth(), ImageWrapper->GetHeight()))
					{
						Texture->SRGB = true;
						Texture->UpdateResource();

						ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
							FWriteRawDataToTexture,
							FTexture2DDynamicResource*, TextureResource, static_cast<FTexture2DDynamicResource*>(Texture->Resource),
							TArray<uint8>, RawData, *RawData,
							{
								WriteRawToTexture_RenderThread(TextureResource, RawData);
							});
						OnDownloadImageRes.Broadcast(Texture);
						OnDownloadImageSuccess.Broadcast(Texture);
						IsDownload = true;
						return;
					}
				}
			}
		}
	}

	if (HttpResponse.IsValid())
	{
		OnDownloadFileDone.Broadcast(HttpResponse->GetResponseCode());
	}
	OnDownloadImageFail.Broadcast(nullptr);

#endif
}

void UQuVRFileDownloader::HandleRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSent, int32 BytesReceived)
{
#if !UE_SERVER
	if (HttpRequest->GetResponse()->GetContentLength() > 0)
	{
		TArray<uint8> ProgressEmptyData;
		OnUlpdataProegress.Broadcast(BytesReceived, HttpRequest->GetResponse()->GetContentLength(), ProgressEmptyData);
	}
#endif
}

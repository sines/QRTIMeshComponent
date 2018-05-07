// Fill out your copyright notice in the Description page of Project Settings.
#include "QuVRUtils.h"
#include "Runtime/RHI/Public/RHIResources.h"


const FString UQuVRUtils::ResRootPath = FString(TEXT("QuVRResource"));

static IImageWrapperPtr GetImageWrapperByExtention(const FString InImagePath)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	if (InImagePath.EndsWith(".png"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	}
	else if (InImagePath.EndsWith(".jpg") || InImagePath.EndsWith(".jpeg"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	}
	else if (InImagePath.EndsWith(".bmp"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP);
	}
	else if (InImagePath.EndsWith(".ico"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICO);
	}
	else if (InImagePath.EndsWith(".exr"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::EXR);
	}
	else if (InImagePath.EndsWith(".icns"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICNS);
	}

	return nullptr;
}


UTexture2D* UQuVRUtils::LoadTexture2DbyPath(const FString& ImagePath, bool& IsValid)
{
	UTexture2D* Texture = nullptr;
	IsValid = false;

	// To avoid log spam, make sure it exists before doing anything else.
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ImagePath))
	{
		return nullptr;
	}

	TArray<uint8> CompressedData;
	if (!FFileHelper::LoadFileToArray(CompressedData, *ImagePath))
	{
		return nullptr;
	}

	IImageWrapperPtr ImageWrapper = GetImageWrapperByExtention(ImagePath);

	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(CompressedData.GetData(), CompressedData.Num()))
	{
		const TArray<uint8>* UncompressedRGBA = nullptr;

		if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, UncompressedRGBA))
		{
			Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

			if (Texture != nullptr)
			{
				IsValid = true;

				void* TextureData = Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(TextureData, UncompressedRGBA->GetData(), UncompressedRGBA->Num());
				Texture->PlatformData->Mips[0].BulkData.Unlock();
				Texture->UpdateResource();
			}
		}
	}

	return Texture;
}

UTexture2DDynamic* UQuVRUtils::LoadDyna2DPath(const FString& ImagePath)
{
	// To avoid log spam, make sure it exists before doing anything else.
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ImagePath))
	{
		return nullptr;
	}

	TArray<uint8> CompressedData;
	if (!FFileHelper::LoadFileToArray(CompressedData, *ImagePath))
	{
		return nullptr;
	}

	IImageWrapperPtr ImageWrapper = GetImageWrapperByExtention(ImagePath);

	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(CompressedData.GetData(), CompressedData.Num()))
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
				return Texture;
			}
		}
	}

	return nullptr;
}

UTexture2D* UQuVRUtils::ConvertImage(UTexture2DDynamic * DynTex)
{
	int32 Width = DynTex->SizeX;
	int32 Height = DynTex->SizeY;

	UTexture2D* ResultTexture = UTexture2D::CreateTransient(Width, Height);

	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
		FConvertTextures,
		FTexture2DDynamicResource*, TextureResource, static_cast<FTexture2DDynamicResource*>(DynTex->Resource),
		UTexture2D*, ResultTexture, ResultTexture,
		{
			FTexture2DRHIParamRef TextureRHI = TextureResource->GetTexture2DRHI();
	int32 Width = TextureRHI->GetSizeX();
	int32 Height = TextureRHI->GetSizeY();

	uint32 DestStride = 0;
	uint8* ReadData = reinterpret_cast<uint8*>(RHILockTexture2D(TextureRHI, 0, RLM_ReadOnly, DestStride, false, false));

	FTexture2DMipMap& Mip = ResultTexture->PlatformData->Mips[0];
	uint8* Data = (uint8*)Mip.BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(Data, ReadData, Width * Height * 4);
	Mip.BulkData.Unlock();
	ResultTexture->UpdateResource();
		});

	return ResultTexture;
}

/************************************************************************/
/*  Content Path Function												*/
/************************************************************************/

FQuVRPackageModel GetPackageObject(const FString& InPackageUrl)
{
	FQuVRPackageModel model;
	if (false == InPackageUrl.IsEmpty())
	{
		TArray<FString> ParseURL;
		InPackageUrl.ParseIntoArray(ParseURL, TEXT("/"));
		FString FullName = ParseURL.Last(0);

		TArray<FString>ParseName;
		FullName.ParseIntoArray(ParseName, TEXT("_"));
		if (ParseName[0] == FString(TEXT("SK")) || 
			ParseName[0] == FString(TEXT("SM")) || 
			ParseName[0] == FString(TEXT("QuVRSK")) ||
			ParseName[0] == FString(TEXT("QuVRSM")) ||
			ParseName[0] == FString(TEXT("QuVRTianQiu")))
		{
			model.type = FString(TEXT("Models"));//EQuVRObjectType::Model;
		}
		else if (ParseName[0] == FString(TEXT("M")))
		{
			model.type = FString(TEXT("Materials"));//EQuVRObjectType::Material;
		
		}

		TArray<FString>modelName;
		FullName.ParseIntoArray(modelName, TEXT("."));
		model.FullName = FullName;
		model.Name = modelName[0];

		return model;
	}
	return model;
}

FString UQuVRUtils::GetContentPath()
{
	FString ContentDir = FPaths::GameContentDir();
	FString GameFullDir = FPaths::Combine(ContentDir, ResRootPath);
	return FString(GameFullDir);
}

void UQuVRUtils::GetObjectPath(const FString& InPackageUrl, FString& ZipPath, FString& FilePath)
{
	if (InPackageUrl.IsEmpty())
	{
		return ;
	}

	FQuVRPackageModel model= GetPackageObject(InPackageUrl);
	FilePath = FPaths::Combine(GetContentPath(), model.type, model.Name);
	ZipPath = FPaths::Combine(GetContentPath(), model.type, model.Name, model.FullName);

	FilePath = FPaths::ConvertRelativePathToFull(FilePath);
	ZipPath = FPaths::ConvertRelativePathToFull(ZipPath);
}

FString UQuVRUtils::GetObjectName(const FString& InFilePath)
{
	TArray<FString> File;
	InFilePath.ParseIntoArray(File, TEXT("/"));
	FString FullName = File.Last(0);
	TArray<FString> name;
	FullName.ParseIntoArray(name, TEXT("."));
	if (0 < name.Num())
	{
		return name[0];
	}
	return FString(TEXT(""));
}
/************************************************************************/
/*                              Zip Folder								*/
/************************************************************************/
FString UQuVRUtils::GetZipToolPath()
{
	// Search the zip tool in serveral path, 
	FString ToolPath = FPaths::EngineDir() / TEXT("Binaries/Win64/ZipDirectory.exe");
	if (FPaths::FileExists(ToolPath))
		return ToolPath;

	ToolPath = FPaths::GamePluginsDir() / TEXT("QuVREditorMode/Binaries/Win64/ZipDirectory.exe");
	if (FPaths::FileExists(ToolPath))
		return ToolPath;

	ToolPath = FPaths::EnginePluginsDir() / TEXT("QuVREditorMode/Binaries/Win64/ZipDirectory.exe");
	if (FPaths::FileExists(ToolPath))
		return ToolPath;
	else
		return "";
}

void UQuVRUtils::UnzipFile(const FString& InZipPath, const FString& InTargetPath)
{

	FString ZipToolPath = GetZipToolPath();
	if (ZipToolPath.IsEmpty())
	{
		FWindowsPlatformMisc::MessageBoxExt(EAppMsgType::Ok, *FString(TEXT("ZipDirectory NOT FOUND£¡")), *FString(TEXT("ZipDirectory ERROR")));
		return;
	}
	FString FullCommandLine = FString::Printf(TEXT("-u \"%s\" \"%s\""), *InZipPath, *InTargetPath);
	TSharedPtr<FMonitoredProcess> ZipProcess = MakeShareable(new FMonitoredProcess(
		*FPaths::ConvertRelativePathToFull(ZipToolPath), FullCommandLine, true));

	// Output the process message!
	ZipProcess->OnOutput().BindLambda([&](FString Message)
	{
	//	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, FString("Done: ")+Message);
	});

	ZipProcess->Launch();
	//ugly spin lock
	while (ZipProcess->Update())
	{
		FPlatformProcess::Sleep(0.1f);
	}

	DeleteZipFile(InZipPath);
}

void UQuVRUtils::DeleteZipFile(const FString& InZipPath)
{
/*
	if (IFileManager::Get().DirectoryExists(*ExportPath))
	{
		IFileManager::Get().DeleteDirectory(*ExportPath);
	}
*/

	if (FPaths::FileExists(*InZipPath))
	{
		IFileManager::Get().Delete(*InZipPath);
	}
	
}

bool UQuVRUtils::CheckFileExists(const FString& InPackageUrl)
{

	if (InPackageUrl.IsEmpty())
	{
		return false;
	}
	
	FString FilePath;
	FString FullPath;
	GetObjectPath(InPackageUrl, FullPath, FilePath);
	FString name = GetObjectName(FilePath);
	FilePath = FPaths::Combine(FilePath,name);
	// Test File Exist!
	FString file = FilePath + FString(TEXT(".uasset"));
	return FPaths::FileExists(*file);
}

/************************************************************************/
/* StaticLoadObject Asset Path                                          */
/************************************************************************/
FString UQuVRUtils::GetAssetPath(const FString& InPackageUrl)
{
	
	FString FilePath;
	FQuVRPackageModel model = GetPackageObject(InPackageUrl);
	FilePath = FPaths::Combine(FString(TEXT("/Game/QuVRResource")), model.type, model.Name, model.Name);
	FilePath += FString(TEXT("."))+ model.Name;
	return FilePath;
}

/************************************************************************/
/* Saved Path Function											        */
/************************************************************************/
FString UQuVRUtils::GetSavedTempTextureDir(const FString& InPackageUrl)
{
	TArray<FString> parseFile;
	InPackageUrl.ParseIntoArray(parseFile, TEXT("/"));
	if (parseFile.Num() > 2)
	{
		FString DirName = FPaths::Combine(parseFile.Last(1), parseFile.Last(0));
		FString SrcFilePath = FPaths::GameSavedDir() / TEXT("Catalog") / DirName;
		return SrcFilePath;
	}
	return FString(TEXT(""));
}

bool UQuVRUtils::CheckTempTextureExists(const FString& InPackageUrl)
{
	TArray<FString> parseFile;
	InPackageUrl.ParseIntoArray(parseFile, TEXT("/"));
	if (parseFile.Num() > 2)
	{
		FString DirName = FPaths::Combine(parseFile.Last(1), parseFile.Last(0));
		FString SrcFilePath = FPaths::GameSavedDir() / TEXT("Catalog") / DirName;
		return FPaths::FileExists(*SrcFilePath);
	}
	return false;
	
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "QuVRUtils.h"

const FString UQuVRUtils::ResRootPath = FString(TEXT("QuVRResource"));

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
		if (ParseName[0] == FString(TEXT("SK")) || ParseName[0] == FString(TEXT("SM")))
		{
			model.type = EQuVRObjectType::Model;
		}
		else if (ParseName[0] == FString(TEXT("M")))
		{
			model.type = EQuVRObjectType::Material;
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
	FString FullDir = FPaths::ConvertRelativePathToFull(GameFullDir);
	return FString(FullDir);
}

void UQuVRUtils::GetObjectPath(const FString& InPackageUrl, FString& FullPath, FString& FilePath)
{
	FQuVRPackageModel model= GetPackageObject(InPackageUrl);
	FilePath = FPaths::Combine(GetContentPath(), model.Name);
	FullPath = FPaths::Combine(GetContentPath(), model.Name, model.FullName); 
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
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, FString("Done: ")+Message);
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
	FString FilePath;
	FString FullPath;
	GetObjectPath(InPackageUrl, FullPath, FilePath);
	FString name = GetObjectName(FullPath);
	// Test File Exist!
	FString file = FPaths::Combine(FilePath, name);
	return FPaths::FileExists(*file);
}

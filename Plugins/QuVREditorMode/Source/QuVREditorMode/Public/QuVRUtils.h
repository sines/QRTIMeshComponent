// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "QuVRUtils.generated.h"

class UTexture2DDynamic;

enum class EQuVRObjectType : uint8
{
	Unknown,
	Model,
	Material,
	Common,
};


USTRUCT()
struct QUVREDITORMODE_API FQuVRPackageModel
{
	GENERATED_USTRUCT_BODY()
	FQuVRPackageModel() 
	{
		type = FString(TEXT("Unknown"));
		FullName = FString(TEXT("Unknown"));
		Name = FString(TEXT("Unknown"));
	};
	
	FString type;
	FString FullName;
	FString Name;
};

/**
 * Utils Static Function
 */
UCLASS()
class   UQuVRUtils : public UObject
{
	GENERATED_BODY()
public:
	/************************************************************************/
	/* Content Path Function                                                */
	/************************************************************************/
	static void GetObjectPath(const FString& InPackageUrl, FString& ZipPath, FString& FilePath);

	static FString GetObjectName(const FString& InFilePath);
	static FString GetContentPath();
	static const FString ResRootPath;

	/************************************************************************/
	/* Saved Path Function											        */
	/************************************************************************/
	static FString GetSavedTempTextureDir(const FString& InPackageUrl);
	static bool CheckTempTextureExists(const FString& InPackageUrl);
	/************************************************************************/
	/* StaticLoadObject Asset Path                                          */
	/************************************************************************/
	static FString GetAssetPath(const FString& InPackageUrl);

	/************************************************************************/
	/*                        Zip Folder                           */
	/************************************************************************/
	FString static GetZipToolPath();
	static void UnzipFile(const FString& InZipPath, const FString& InTargetPath);
	static void DeleteZipFile(const FString& InZipPath);
	static bool  CheckFileExists(const FString& InPackageUrl);

	/************************************************************************/
	/*    Load Image                                                        */
	/************************************************************************/
	static UTexture2D* LoadTexture2DbyPath(const FString& ImagePath, bool& IsValid);
	static UTexture2DDynamic* LoadDyna2DPath(const FString& ImagePath);
	static UTexture2D* ConvertImage(UTexture2DDynamic * DynTex);
};

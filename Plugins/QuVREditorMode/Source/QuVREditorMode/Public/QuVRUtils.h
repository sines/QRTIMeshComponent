// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuVRUtils.generated.h"

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
class QUVREDITORMODE_API UQuVRUtils : public UObject
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
};

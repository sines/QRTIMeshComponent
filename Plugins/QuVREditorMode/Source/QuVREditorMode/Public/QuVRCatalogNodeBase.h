// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Http.h"
#include "Json.h"
#include "UObject/NoExportTypes.h"
#include "QuVRCatalogWidget.h"
#include "QuVRCatalogNodeBase.generated.h"

class UTexture2DDynamic;

DECLARE_MULTICAST_DELEGATE(FQuVRCatalogAssetInfoImageDownloadDone);

USTRUCT()
struct QUVREDITORMODE_API FQuVRCatalogNodeInfo
{
		GENERATED_USTRUCT_BODY()
			FQuVRCatalogNodeInfo()
		{
			Id = FString(TEXT("-1"));
			PId = FString(TEXT("-1"));
			Name = FString(TEXT("-1"));
			DisplayName = FString(TEXT("-1"));
			Description = FString(TEXT("-1"));
			OrderNo = FString(TEXT("-1"));
			CatalogType = FString(TEXT("-1"));
			HasChilder = false;
			ZOrder = 0;
		}

// Json Data
	FString Id;
	FString PId;
	FString Name;
	FString DisplayName;
	FString Description;
	FString OrderNo;
	FString CatalogType;
	bool HasChilder;

// layer ZOrder
	int32 ZOrder;
};

/**
* UQuVRCatalogAssetInfo
*/
UCLASS()
class QUVREDITORMODE_API UQuVRCatalogAssetInfo:public UObject
{
public:

	GENERATED_BODY()
		UQuVRCatalogAssetInfo();

	// Json Data
	FString Id;
	int32 ObjectType;
	FString ObjectTypeDesc;
	FString Name;
	FString DisplayName;
	FString Description;
	FString AssetRelativePath;
	FString MainCategoryID;
	FString SubCategoryID;
	FString MainCategory;
	FString SubCategory;
	FString ImageUrl;
	UTexture2DDynamic* Texture2Dimage;
	FString	PackageUrl;
	int32 Size;

	bool IsDownload;
	FQuVRCatalogAssetInfoImageDownloadDone ImageDownloadDone;
public:
	void Initialise(TSharedPtr<FQuVRCatalogNode> node);
	void DownloadImage(UTexture2DDynamic* texture2D);

public:
};

class QUVREDITORMODE_API FQuVRCatalogNode : public FGCObject
{
public:

	FQuVRCatalogNode():ParentNode(nullptr)
	{
		ChildList.Reset();

		HasAssetList = false;
		AssetList.Reset();
	};

public:
	void ClearAllData();
	void ClearChildAssetlist();
	void ClearChildNodelist();

public:
	FQuVRCatalogNodeInfo NodeData;
	
	TSharedPtr<class FQuVRCatalogNode> ParentNode;
	TArray<TSharedPtr<class FQuVRCatalogNode>> ChildList;
	TArray<class UQuVRCatalogAssetInfo*> AssetList;
	// Asset Data
	bool HasAssetList;

public:
	virtual void AddReferencedObjects(FReferenceCollector& Collector)override {};
};
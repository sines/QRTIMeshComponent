// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Http.h"
#include "Json.h"
#include "UObject/NoExportTypes.h"
#include "QuVRCatalogWidget.h"
#include "QuVRFileDownloader.h"
#include "QuVRCatalogNodeBase.generated.h"

class UTexture2DDynamic;
class UQuVRFileDownloader;

 DECLARE_MULTICAST_DELEGATE(FQuVRCatalogAssetInfoImageDownloadDone);

 namespace QuVREditorMode
{
	 /** Coordinate system identifiers. */
	 enum class EQuVRCatalogDownloadState
	 {
		 QuVR_Catalog_None = -1,
		 QuVR_Catalog_Start = 0,
		 QuVR_Catalog_Downloading,
		 QuVR_Catalog_Finish,
	 };
}

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
			ChildNum = 0;
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
	int32 ChildNum;
};

/**
* UQuVRCatalogAssetInfo
*/

UCLASS()
class QUVREDITORMODE_API UQuVRCatalogAssetInfo:public UObject
{
public:

	GENERATED_BODY()
public:
	UQuVRCatalogAssetInfo();
	virtual ~UQuVRCatalogAssetInfo();
	// Json Data
	FString Id;
	/*     
	ObjectType =      
	SK_ 骨骼模型 0
	SM_  静态模型 1
	M_  材质球 2
	T_   贴图 3
	P_   特效 4
	*/
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

	QuVREditorMode::EQuVRCatalogDownloadState IsImageDownload;
	FQuVRDownloadImageC2Delegate ImageDownloadDone;
private:
	UQuVRFileDownloader* AsyncTaskDownloadImage;
	TArray<FDelegateHandle> ImageEventHandleList;
public:
	void Initialise();
	void AddImageEventHandle(FDelegateHandle handle);
	void ClearImageEventHandle();
	void DownloadImage(UTexture2DDynamic* texture2D);
	void DownloadDone(int32 code);
	void ClearDownloadState();

};

class QUVREDITORMODE_API FQuVRCatalogNode : public FGCObject
{
public:

	FQuVRCatalogNode():ParentNode(nullptr)
	{
		ChildList.Reset();
		AssetList.Reset();
	};

public:
	void ClearAllData();
	void ClearChildAssetlist();
	void ClearChildNodelist();
	bool HasChildAsset(const UQuVRCatalogAssetInfo& AssetInfo);
	UQuVRCatalogAssetInfo* GetChildAsset(const UQuVRCatalogAssetInfo& AssetInfo);
public:
	FQuVRCatalogNodeInfo NodeData;
	
	TSharedPtr<class FQuVRCatalogNode> ParentNode;
	TArray<TSharedPtr<class FQuVRCatalogNode>> ChildList;
	TArray<class UQuVRCatalogAssetInfo*> AssetList;
	// Asset Data

public:
	virtual void AddReferencedObjects(FReferenceCollector& Collector)override {};
};
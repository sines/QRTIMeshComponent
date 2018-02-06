// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Http.h"
#include "Json.h"
#include "UObject/NoExportTypes.h"
#include "QuVRCatalogWidget.h"
#include "QuVRAssetDownNet.generated.h"

class UTexture2DDynamic;

DECLARE_LOG_CATEGORY_EXTERN(UploadAssetDialog, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequestNodeDataDoneDelegate, UQuVRAssetDownNet*, net);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequestAssetDataDoneDelegate, UQuVRAssetDownNet*, net);


	USTRUCT()
	struct QUVREDITORMODE_API FQuVRCatalogItem
	{
		GENERATED_USTRUCT_BODY()
			FQuVRCatalogItem()
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

// attribute Json Data
	FString Id;
	FString PId;
	FString Name;
	FString DisplayName;
	FString Description;
	FString OrderNo;
	FString CatalogType;
	bool HasChilder;

// button Iteration 
	int32 ZOrder;
};

UCLASS()
class QUVREDITORMODE_API UQuVRcatalogAsset : public UObject
{
public:

	GENERATED_BODY()
		UQuVRcatalogAsset():Texture2Dimage(NULL)
	{
		Id="0";
		ObjectType = 0;
		ObjectTypeDesc = "0";
		Name = "0";
		DisplayName = "0";
		Description = "0";
		AssetRelativePath = "0";
		MainCategoryID = "0";
		SubCategoryID = "0";
		MainCategory = "0";
		SubCategory = "0";
		ImageUrl = "0";
		PackageUrl = "0";
		Size =0;
		IsDownload = false;
	}

	~UQuVRcatalogAsset() {};
	// attribute Json Data
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

public:
	void Initialise();
	void downImagexx(UTexture2DDynamic* texture2D);

public:
};


class QUVREDITORMODE_API FQuVRCatalogNode : public FGCObject
{
public:

	FQuVRCatalogNode():ParentNode(nullptr)
	{
		ChildList.Empty();
		ChildList.Reset();

		HasAssetList = false;
		AssetList.Empty();
		AssetList.Reset();
	};

	FQuVRCatalogItem NodeData;
	
	TSharedPtr<FQuVRCatalogNode> ParentNode;
	TArray<TSharedPtr<FQuVRCatalogNode>> ChildList;
	TArray<TSharedPtr<UQuVRcatalogAsset>> AssetList;
	// Asset Data
	bool HasAssetList;
public:
	virtual void AddReferencedObjects(FReferenceCollector& Collector)override {};
};

/**
 * UQuVRAssetDownNet
 */
UCLASS()
class QUVREDITORMODE_API UQuVRAssetDownNet : public UObject
{
	GENERATED_BODY()

public:
	UQuVRAssetDownNet();
	/** Request we're currently processing */
	static UQuVRAssetDownNet* GetInstance();
	void GetAllCatalogNodeListFromUrl();

	void GetCatalogNodeAssetFromUrl(TSharedPtr<FQuVRCatalogNode>& node);

	TSharedPtr<FQuVRCatalogNode>& GetRootNode() { return RootNode; }
	TArray<TSharedPtr<FQuVRCatalogNode>>& GetNodeList() { return RootNode->ChildList; }
	void SetWidget(const TSharedPtr<SQuVRCatalogWidget> widget) { Catawidget = widget; }

protected:
	void ClearChildNodelist(TSharedRef<FQuVRCatalogNode> node);
private:
	static const FString CatalogNodeHttpURL;
	static const FString CatalogAssetHttpURL;

	TSharedRef<IHttpRequest> HttpNodeListRequest = FHttpModule::Get().CreateRequest();

	TSharedRef<IHttpRequest> HttpAssetListRequest = FHttpModule::Get().CreateRequest();
	int32 ResponseCode;

	FString ResponseContent;

	// Process Node Catalog
	void OnProcessNodeRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void GenerateNodeCatalog(TSharedRef<FQuVRCatalogNode> node);
	
	// Process Asset Catalog
	void OnProcessAssetRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void GenerateAssetCatalog(TSharedRef<FQuVRCatalogNode> node);

private:
	void Initial();

	// Node Catalog Data
	void ParseNodeItemData(TSharedPtr<FQuVRCatalogNode> node,TSharedPtr<FJsonValue> JsonValue);
	void ParseNodeChildData(TSharedPtr<FQuVRCatalogNode> node,TSharedPtr<FJsonValue> JsonValue);
	void ParseNodeListData(TSharedPtr<FQuVRCatalogNode> node, TArray<TSharedPtr<FJsonValue>> JsonValue);

	// Asset Catalog Data
	void ParseAssetItemData(TSharedPtr<FQuVRCatalogNode> node, TSharedPtr<FJsonValue> JsonValue);
	void ParseAssetListData(TArray<TSharedPtr<FJsonValue>> JsonValue);
public:
	
private:
	TSharedPtr<FQuVRCatalogNode>  RootNode;
	TSharedPtr<FQuVRCatalogNode> CurrentNode;
//	TArray<FQuVRCatalogNode> NodeList;
	static UQuVRAssetDownNet* StaticInstance;
	TSharedPtr<SQuVRCatalogWidget> Catawidget;
public:
	FRequestAssetDataDoneDelegate OnRequestAssetDataDone;
	FRequestNodeDataDoneDelegate OnRequestNodeDataDone;

	//////////////////////////////////////////////////////////////////////////
	void xxdownImage(UTexture2DDynamic* texture2D);
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Http.h"
#include "Json.h"
#include "UObject/NoExportTypes.h"
#include "QuVRCatalogWidget.h"
#include "QuVRCatalogNodeBase.h"
#include "QuVRCatalogDataManager.generated.h"

class UTexture2DDynamic;
class FQuVRCatalogNode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequestNodeDataDoneDelegate, UQuVRCatalogDataManager*, net);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequestAssetDataDoneDelegate, UQuVRCatalogDataManager*, net);

/**
 * UQuVRCatalogDataManager
 */
UCLASS()
class QUVREDITORMODE_API UQuVRCatalogDataManager : public UObject
{
	GENERATED_BODY()

public:
	UQuVRCatalogDataManager();
	virtual ~UQuVRCatalogDataManager();
	/** Request we're currently processing */
	static UQuVRCatalogDataManager* GetInstance();
	void GetAllCatalogNodeListFromUrl();
	void GetCatalogListACountFromUrl();

	void GetCatalogNodeAssetFromUrl(TSharedPtr<FQuVRCatalogNode>& node, int32 skip=0,int32 limit=10);
	void GetCatalogNodeChildNumFromUrl(TSharedPtr<class SQuVRCatalogPlaneWidget>& plane, TSharedPtr<FQuVRCatalogNode>& node);
	
	void HoldCatalogNodeAssetFromUrl(TSharedPtr<FQuVRCatalogNode>& node, int32 skip = 0, int32 limit = 10);

	TSharedPtr<FQuVRCatalogNode>& GetRootNode() { return RootNode; }
	TArray<TSharedPtr<FQuVRCatalogNode>>& GetNodeList() { return RootNode->ChildList; }
	void SetWidget(const TSharedPtr<SQuVRCatalogWidget> widget) { Catalogwidget = widget; }

private:
	static const FString CatalogNodeHttpURL;
	static const FString CatalogAssetHttpURL;
	static const FString CatalogChildNumHttpURL;
	static const FString CatalogObjectsCountURL;

	TSharedRef<IHttpRequest> HttpNodeListRequest = FHttpModule::Get().CreateRequest();
	TSharedRef<IHttpRequest> HttpAssetListRequest = FHttpModule::Get().CreateRequest();
	TSharedRef<IHttpRequest> HttpChildNumRequest = FHttpModule::Get().CreateRequest();
	TSharedRef<IHttpRequest> HttpObjectCountRequest = FHttpModule::Get().CreateRequest();

	int32 ResponseCode;

	FString ResponseContent;

	// Clear http state
	void ClearHttpNodeListRequest();
	void ClearHttpAssetListRequest();
	void ClearHttpChildNumRequest();

	void ClearAssetListWidget();
	// Process Node Catalog
	void OnProcessNodeRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void GenerateNodeCatalog(TSharedRef<FQuVRCatalogNode> node);
	
	// Process Asset Catalog
	void OnProcessAssetRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void GenerateAssetCatalog(TSharedRef<FQuVRCatalogNode> node, bool InHold=false);

	void OnHoldAssetRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	// Process ChildNum Catalog
	void OnProcessChildNumRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void ParseNodeAssetChildNum(TSharedPtr<FQuVRCatalogNode> node, TArray<TSharedPtr<FJsonValue>> JsonValue);

	//Process ObjectsCountURL Catalog
	void OnProcessObjectsCountRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
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
	TSharedPtr<FQuVRCatalogNode> GetCurrentNode() { return CurrentNode; };
private:
	TSharedPtr<FQuVRCatalogNode>  RootNode;
	TSharedPtr<FQuVRCatalogNode> CurrentNode;

	TSharedPtr<FQuVRCatalogNode>  SeekNode;
	static UQuVRCatalogDataManager* StaticInstance;
	TSharedPtr<SQuVRCatalogWidget> Catalogwidget;
	TSharedPtr<SQuVRCatalogPlaneWidget> CatalogPlane;

protected:
	TMap<FString, int32> CatalogObjectsCount;
public:
	FRequestAssetDataDoneDelegate OnRequestAssetDataDone;
	FRequestNodeDataDoneDelegate OnRequestNodeDataDone;
};
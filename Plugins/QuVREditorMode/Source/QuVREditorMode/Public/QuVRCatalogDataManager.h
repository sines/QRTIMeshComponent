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

	void GetCatalogNodeAssetFromUrl(TSharedPtr<FQuVRCatalogNode>& node);

	TSharedPtr<FQuVRCatalogNode>& GetRootNode() { return RootNode; }
	TArray<TSharedPtr<FQuVRCatalogNode>>& GetNodeList() { return RootNode->ChildList; }
	void SetWidget(const TSharedPtr<SQuVRCatalogWidget> widget) { Catawidget = widget; }

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
	static UQuVRCatalogDataManager* StaticInstance;
	TSharedPtr<SQuVRCatalogWidget> Catawidget;
public:
	FRequestAssetDataDoneDelegate OnRequestAssetDataDone;
	FRequestNodeDataDoneDelegate OnRequestNodeDataDone;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Http.h"
#include "Json.h"
#include "UObject/NoExportTypes.h"
#include "QuVRCatalogWidget.h"
#include "QuVRAssetDownNet.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(UploadAssetDialog, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequestAllSelectTypeDataDoneDelegate, UQuVRAssetDownNet*, net);

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
		}
		FString Id;
		FString PId;
		FString Name;
		FString DisplayName;
		FString Description;
		FString OrderNo;
		FString CatalogType;
		bool HasChilder;
	};

	USTRUCT()
	struct QUVREDITORMODE_API  FQuVRCatalogNode
	{
		GENERATED_USTRUCT_BODY()
			FQuVRCatalogNode()
		{
			ChildList.Empty();
			ChildList.Reset();
		}
		//	UPROPERTY()
		FQuVRCatalogItem NodeData;
		//	UPROPERTY()
		TArray<FQuVRCatalogItem> ChildList;
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
	void GetAllTypeDataFromUrl();
	TArray<FQuVRCatalogNode>& GetNodeList() { return NodeList; }
	void SetWidget(const TSharedPtr<SQuVRCatalogWidget> widget) { Catawidget = widget; }
private:
	static const FString CatalogHttpURL;

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();

	int32 ResponseCode;

	FString ResponseContent;

	void OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void GenerateCatalog();
private:
	void Initial();

	void ParseSelectorTypeData(TArray<TSharedPtr<FJsonValue>> JsonValue);

	void ParseSecondTypeData(TArray<TSharedPtr<FJsonValue>> JsonValue);

	void ParseLastTypeData(TArray<TSharedPtr<FJsonValue>> JsonValue);

private:
	FQuVRCatalogNode CurrentNode;
	TArray<FQuVRCatalogNode> NodeList;
	static TSharedPtr<UQuVRAssetDownNet> StaticInstance;
	TSharedPtr<SQuVRCatalogWidget> Catawidget;
public:
	FRequestAllSelectTypeDataDoneDelegate OnRequestAllSelectTypeDataDone;
};

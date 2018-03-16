// Fill out your copyright notice in the Description page of Project Settings.

#include "QuVRCatalogDataManager.h"
#include "QuVRFileDownloader.h"
#include "QuVRCatalogEntryWidget.h"
#include "QuVRCatalogNodeBase.h"

const FString UQuVRCatalogDataManager::CatalogNodeHttpURL = FString(TEXT("http://dev-vrservice.jtl3d.com/api/VRCatalog/GetCatalogsByType?type=modelCatalog"));
const FString UQuVRCatalogDataManager::CatalogAssetHttpURL = FString(TEXT("http://dev-vrservice.jtl3d.com/api/VRCatalog/SearchVRObjects"));

//////////////////////////////////////////////////////////////////////////
/**UQuVRCatalogDataManager*/
//////////////////////////////////////////////////////////////////////////
UQuVRCatalogDataManager::UQuVRCatalogDataManager()
{
	RootNode = MakeShareable(new(FQuVRCatalogNode));
}
 
UQuVRCatalogDataManager::~UQuVRCatalogDataManager()
{
	if (HttpNodeListRequest->DoesSharedInstanceExist())
	{
		HttpNodeListRequest->OnProcessRequestComplete().Unbind();
		HttpNodeListRequest->OnRequestProgress().Unbind();
		HttpNodeListRequest->CancelRequest();
	}
	if (HttpAssetListRequest->DoesSharedInstanceExist())
	{
		HttpAssetListRequest->OnProcessRequestComplete().Unbind();
		HttpAssetListRequest->OnRequestProgress().Unbind();
		HttpAssetListRequest->CancelRequest();
	}
}


void UQuVRCatalogDataManager::OnProcessAssetRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// Check we have a response and save response code as int32
	if (Response.IsValid())
	{
		ResponseCode = Response->GetResponseCode();
	}

	// Check we have result to process futher
	if (!bWasSuccessful || !Response.IsValid())
	{
		return;
	}

	// Save response data as a string	
	ResponseContent = Response->GetContentAsString();
#if 0 // Read Local json
	FString FileName = FPaths::GameDir() + FString(TEXT("uuu.Json"));
	FFileHelper::SaveStringToFile(ResponseContent + FString(TEXT("{end}")), *(FileName));

#endif
	// Analyze Json Data
	//Read Json Data
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(ResponseContent);
	bool bResult = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	check(bResult);
	bResult = JsonObject.IsValid();
	check(bResult);

	TArray<TSharedPtr<FJsonValue>>TypeData = JsonObject->GetArrayField(TEXT("items"));

	ParseAssetListData(TypeData);
	// OnRequset Data Done
	OnRequestAssetDataDone.Broadcast(this);
}

void UQuVRCatalogDataManager::OnProcessNodeRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// Check we have a response and save response code as int32
	if (Response.IsValid())
	{
		ResponseCode = Response->GetResponseCode();
	}

	// Check we have result to process futher
	if (!bWasSuccessful || !Response.IsValid())
	{
		return;
	}

	// Save response data as a string	
	ResponseContent = Response->GetContentAsString();
	
	//////////////////////////////////////////////////////////////////////////
	// TEST LOCAL JSON
#if 0 // Read Local json
	FString FileName = FPaths::GameDir() + FString(TEXT("format.json"));
	FString myString;
	if (FPaths::FileExists(FileName))
	{
		if (FFileHelper::LoadFileToString(myString, *(FileName)))
		{
			ResponseContent = myString;
		}
	}

#endif
	//////////////////////////////////////////////////////////////////////////

	// Analyze Json Data
	//Read Json Data
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(ResponseContent);
	bool bResult = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	check(bResult);
	bResult = JsonObject.IsValid();
	check(bResult);

	TArray<TSharedPtr<FJsonValue>>TypeData = JsonObject->GetArrayField(TEXT("items"));
	ParseNodeListData(RootNode,TypeData);
	GenerateNodeCatalog(RootNode.ToSharedRef());
	// OnRequset Data Done
	OnRequestNodeDataDone.Broadcast(this);
}

UQuVRCatalogDataManager* UQuVRCatalogDataManager::StaticInstance;
UQuVRCatalogDataManager* UQuVRCatalogDataManager::GetInstance()
{
	if (!StaticInstance)
	{
		StaticInstance = NewObject<UQuVRCatalogDataManager>();
		StaticInstance->AddToRoot();
		StaticInstance->Initial();
	}

	return StaticInstance;
}

void UQuVRCatalogDataManager::Initial()
{
	HttpNodeListRequest = FHttpModule::Get().CreateRequest();
	HttpAssetListRequest = FHttpModule::Get().CreateRequest();

}

void UQuVRCatalogDataManager::ParseAssetListData(TArray<TSharedPtr<FJsonValue>> JsonValue)
{
	if (CurrentNode.IsValid())
	{
		//CurrentNode->ClearChildAssetlist();
		for (auto Value : JsonValue)
		{
			ParseAssetItemData(CurrentNode,Value);
		}

		 GenerateAssetCatalog(CurrentNode.ToSharedRef());
	}
}

void UQuVRCatalogDataManager::ParseAssetItemData(TSharedPtr<FQuVRCatalogNode> node, TSharedPtr<FJsonValue> JsonValue)
{
	if (node.IsValid())
	{
		UQuVRCatalogAssetInfo* AssetItem =NewObject<UQuVRCatalogAssetInfo>();
		AssetItem->AddToRoot();
		TSharedPtr<FJsonObject>TempJsonObject = JsonValue->AsObject();
		AssetItem->Id = TempJsonObject->GetStringField(TEXT("ID"));

		AssetItem->ObjectType = TempJsonObject->GetIntegerField(TEXT("ObjectType"));
		AssetItem->ObjectTypeDesc = TempJsonObject->GetStringField(TEXT("ObjectTypeDesc"));
		AssetItem->Name = TempJsonObject->GetStringField(TEXT("Name"));
		AssetItem->DisplayName = TempJsonObject->GetStringField(TEXT("DisplayName"));
		AssetItem->Description = TempJsonObject->GetStringField(TEXT("Description"));
		AssetItem->Size = TempJsonObject->GetIntegerField(TEXT("Size"));
		AssetItem->AssetRelativePath = TempJsonObject->GetStringField(TEXT("AssetRelativePath"));
		AssetItem->MainCategoryID = TempJsonObject->GetStringField(TEXT("MainCategoryID"));
		AssetItem->SubCategoryID = TempJsonObject->GetStringField(TEXT("SubCategoryID"));
		AssetItem->MainCategory = TempJsonObject->GetStringField(TEXT("MainCategory"));
		AssetItem->SubCategory = TempJsonObject->GetStringField(TEXT("SubCategory"));
		AssetItem->ImageUrl = TempJsonObject->GetStringField(TEXT("ImageUrl"));
		AssetItem->PackageUrl = TempJsonObject->GetStringField(TEXT("PackageUrl"));
		if (false == node->HasChildAsset(*AssetItem))
		{
			AssetItem->Initialise();
			node->AssetList.AddUnique(AssetItem);
		}
	}

}


void UQuVRCatalogDataManager::GetAllCatalogNodeListFromUrl()
{
	FString TrimmedUrl = CatalogNodeHttpURL;
	TrimmedUrl.Trim();
	TrimmedUrl.TrimTrailing();
	HttpNodeListRequest->OnProcessRequestComplete().Unbind();
	HttpNodeListRequest->CancelRequest();
	HttpNodeListRequest->SetURL(TrimmedUrl);
	HttpNodeListRequest->SetVerb(TEXT("GET"));
	HttpNodeListRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	// Bind event
	HttpNodeListRequest->OnProcessRequestComplete().BindUObject(this, &UQuVRCatalogDataManager::OnProcessNodeRequestComplete);
	// Execute the request
	HttpNodeListRequest->ProcessRequest();

	// Clear NodeData
	RootNode->ClearAllData();
}


void UQuVRCatalogDataManager::GetCatalogNodeAssetFromUrl(TSharedPtr<FQuVRCatalogNode>& node)
{
	if(node.IsValid()&& node->ParentNode.IsValid())
	{
		CurrentNode = node;
		FString format_CatalogType(TEXT("ModelCatalog"));
		FString format_MainCatalogID = node->ParentNode->NodeData.Id;
		FString format_SubCatalogID = node->NodeData.Id;
		int32 format_skip = 0;
		int32 format_limit = 100;
		FString TransientArchetypeString = FString::Printf(TEXT("{\"CatalogType\":\"%s\",\"MainCatalogID\":\"%s\",\"SubCatalogID\":\"%s\",\"skip\":%d,\"limit\":%d}"), *format_CatalogType, *format_MainCatalogID, *format_SubCatalogID, format_skip, format_limit);		
		FString TrimmedUrl = CatalogAssetHttpURL;
		TrimmedUrl.Trim();
		TrimmedUrl.TrimTrailing();
		HttpAssetListRequest->OnProcessRequestComplete().Unbind();
		HttpAssetListRequest->CancelRequest();
		HttpAssetListRequest->SetURL(TrimmedUrl);
		HttpAssetListRequest->SetVerb(TEXT("POST"));
		HttpAssetListRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		HttpAssetListRequest->SetContentAsString(TransientArchetypeString);
		// Bind event
		HttpAssetListRequest->OnProcessRequestComplete().BindUObject(this, &UQuVRCatalogDataManager::OnProcessAssetRequestComplete);
		// Execute the request
		HttpAssetListRequest->ProcessRequest();
	}

}


void UQuVRCatalogDataManager::ParseNodeItemData(TSharedPtr<FQuVRCatalogNode> node, TSharedPtr<FJsonValue> JsonValue)
{
	if (node.IsValid())
	{
		FQuVRCatalogNodeInfo item;
		TSharedPtr<FJsonObject>TempJsonObject = JsonValue->AsObject();
		item.Id = TempJsonObject->GetStringField(TEXT("ID"));
		item.PId = TempJsonObject->GetStringField(TEXT("PID"));
		item.Name = TempJsonObject->GetStringField(TEXT("Name"));
		item.DisplayName = TempJsonObject->GetStringField(TEXT("DisplayName"));
		item.Description = TempJsonObject->GetStringField(TEXT("Description"));
		item.OrderNo = TempJsonObject->GetStringField(TEXT("OrderNo"));
		item.CatalogType = TempJsonObject->GetStringField(TEXT("CatalogType"));
		node->NodeData = item;
		if (node->ParentNode.IsValid())
		{
			node->NodeData.ZOrder = node->ParentNode->NodeData.ZOrder + 1;
		}
	}
}


void UQuVRCatalogDataManager::ParseNodeListData(TSharedPtr<FQuVRCatalogNode> node, TArray<TSharedPtr<FJsonValue>> JsonValue)
{
	for (auto Value : JsonValue)
	{
		TSharedPtr<FQuVRCatalogNode> ChildNode = MakeShareable(new(FQuVRCatalogNode));
		ChildNode->ParentNode = node;
		node->ChildList.Add(ChildNode);
		ParseNodeChildData(ChildNode, Value);
	}
}


void UQuVRCatalogDataManager::ParseNodeChildData(TSharedPtr<FQuVRCatalogNode> node, TSharedPtr<FJsonValue> JsonValue)
{
		ParseNodeItemData(node, JsonValue);
		TSharedPtr<FJsonObject>TempJsonObject = JsonValue->AsObject();
		if (TempJsonObject->HasField(TEXT("Children")))
		{
			TArray<TSharedPtr<FJsonValue>>ChildrenData = TempJsonObject->GetArrayField(TEXT("Children"));
			if (0 < ChildrenData.Num())
			{
				for (auto Value : ChildrenData)
				{
					TSharedPtr<FQuVRCatalogNode> addNode = MakeShareable(new(FQuVRCatalogNode));
					addNode->ParentNode = node;
					node->ChildList.Add(addNode);
					ParseNodeChildData(addNode, Value);
				}
			}
		}
}

void UQuVRCatalogDataManager::GenerateNodeCatalog(TSharedRef<FQuVRCatalogNode> node)
{
	if (Catawidget.IsValid())
	{
		Catawidget->CreateGroupGroupTabRoot(node);
	}
}


void UQuVRCatalogDataManager::GenerateAssetCatalog(TSharedRef<FQuVRCatalogNode> node)
{
	Catawidget->ClearAssetList();
	if (Catawidget.IsValid())
	{
		Catawidget->CreateCatalogGroupTabAssetList(node);
	}
}
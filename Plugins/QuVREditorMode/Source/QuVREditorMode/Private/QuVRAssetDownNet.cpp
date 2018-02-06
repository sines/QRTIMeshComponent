// Fill out your copyright notice in the Description page of Project Settings.

#include "QuVRAssetDownNet.h"
#include "AsyncTaskDownloadImage.h"

const FString UQuVRAssetDownNet::CatalogNodeHttpURL = FString(TEXT("http://dev-vrservice.jtl3d.com/api/VRCatalog/GetCatalogsByType?type=modelCatalog"));
const FString UQuVRAssetDownNet::CatalogAssetHttpURL = FString(TEXT("http://dev-vrservice.jtl3d.com/api/VRCatalog/SearchVRObjects"));

void UQuVRcatalogAsset::Initialise()
{
//	DownloadImage.Add
	TScriptDelegate<> t_de1;
	t_de1.BindUFunction(this, FName(TEXT("downImagexx")));
	UAsyncTaskDownloadImage* AsyncTaskDownloadImage = UAsyncTaskDownloadImage::DownloadImage(ImageUrl);
	AsyncTaskDownloadImage->OnSuccess.Add(t_de1);
	AsyncTaskDownloadImage->Start(ImageUrl);
}

void UQuVRcatalogAsset::downImagexx(UTexture2DDynamic* texture2D)
{
	Texture2Dimage = texture2D;
}


//////////////////////////////////////////////////////////////////////////
/**UQuVRAssetDownNet*/
//////////////////////////////////////////////////////////////////////////
UQuVRAssetDownNet::UQuVRAssetDownNet()
{
	RootNode = MakeShareable(new(FQuVRCatalogNode));
}


void UQuVRAssetDownNet::OnProcessAssetRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
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

void UQuVRAssetDownNet::OnProcessNodeRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
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

UQuVRAssetDownNet* UQuVRAssetDownNet::StaticInstance;
UQuVRAssetDownNet* UQuVRAssetDownNet::GetInstance()
{
	if (!StaticInstance)
	{
		StaticInstance = NewObject<UQuVRAssetDownNet>();
		StaticInstance->Initial();
	}

	return StaticInstance;
}

void UQuVRAssetDownNet::Initial()
{
	HttpNodeListRequest = FHttpModule::Get().CreateRequest();
	HttpAssetListRequest = FHttpModule::Get().CreateRequest();
}

void UQuVRAssetDownNet::ParseAssetListData(TArray<TSharedPtr<FJsonValue>> JsonValue)
{
	if (CurrentNode.IsValid())
	{
		for (auto Value : JsonValue)
		{
			ParseAssetItemData(CurrentNode,Value);
		}

		 GenerateAssetCatalog(CurrentNode.ToSharedRef());
	}
}

void UQuVRAssetDownNet::ParseAssetItemData(TSharedPtr<FQuVRCatalogNode> node, TSharedPtr<FJsonValue> JsonValue)
{
	if (node.IsValid())
	{
		UQuVRcatalogAsset* AssetItem =NewObject<UQuVRcatalogAsset>();
		TSharedPtr<FJsonObject>TempJsonObject = JsonValue->AsObject();
		AssetItem->Id = TempJsonObject->GetStringField(TEXT("ID"));
		for (auto insItem : node->AssetList)
		{
			if (insItem->Id == AssetItem->Id)
			{
				return;
			}
		}

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
		AssetItem->Initialise();
		node->HasAssetList = true;

		node->AssetList.AddUnique(MakeShareable(AssetItem));
	}

}

void UQuVRAssetDownNet::xxdownImage(UTexture2DDynamic* texture2D)
{

}
void UQuVRAssetDownNet::ClearChildNodelist(TSharedRef<FQuVRCatalogNode> node)
{
	for (auto cdNode:node->ChildList)
	{
		if (0 < cdNode->ChildList.Num())
		{
			ClearChildNodelist(cdNode.ToSharedRef());
		}
		else
		{
			cdNode->ChildList.Empty();
			cdNode->ChildList.Reset();
			cdNode->ParentNode.Reset();
			cdNode.Reset();
		}
	}

}

void UQuVRAssetDownNet::GetAllCatalogNodeListFromUrl()
{
	ClearChildNodelist(RootNode.ToSharedRef());
	FString TrimmedUrl = CatalogNodeHttpURL;
	TrimmedUrl.Trim();
	TrimmedUrl.TrimTrailing();

	HttpNodeListRequest->SetURL(TrimmedUrl);
	HttpNodeListRequest->SetVerb(TEXT("GET"));
	HttpNodeListRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	// Bind event
	HttpNodeListRequest->OnProcessRequestComplete().BindUObject(this, &UQuVRAssetDownNet::OnProcessNodeRequestComplete);

	// Execute the request
	HttpNodeListRequest->ProcessRequest();

}

void UQuVRAssetDownNet::GetCatalogNodeAssetFromUrl(TSharedPtr<FQuVRCatalogNode>& node)
{
	if(node.IsValid())
	{
		CurrentNode = node;
		FString format_CatalogType(TEXT("ModelCatalog"));
		FString format_MainCatalogID/*(TEXT("c9ec07c9-f944-40c9-9228-bfbe4e2249b8"));*/ = node->ParentNode->NodeData.Id;
		FString format_SubCatalogID/*(TEXT("5559a4c7-1e02-4dc1-ac21-ab5f29134a71"));*/= node->NodeData.Id;
		int32 format_skip = 0;
		int32 format_limit = 100;
		FString TransientArchetypeString = FString::Printf(TEXT("{\"CatalogType\":\"%s\",\"MainCatalogID\":\"%s\",\"SubCatalogID\":\"%s\",\"skip\":%d,\"limit\":%d}"), *format_CatalogType, *format_MainCatalogID, *format_SubCatalogID, format_skip, format_limit);

#if 0 // Read Local json
		FString FileName = FPaths::GameDir() + FString(TEXT("xxx.json"));
		FFileHelper::SaveStringToFile(TransientArchetypeString, *(FileName));
#endif
		
		FString TrimmedUrl = CatalogAssetHttpURL;
		TrimmedUrl.Trim();
		TrimmedUrl.TrimTrailing();

		HttpAssetListRequest->SetURL(TrimmedUrl);
		HttpAssetListRequest->SetVerb(TEXT("POST"));
		HttpAssetListRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		HttpAssetListRequest->SetContentAsString(TransientArchetypeString);
		// Bind event
		HttpAssetListRequest->OnProcessRequestComplete().BindUObject(this, &UQuVRAssetDownNet::OnProcessAssetRequestComplete);
		// Execute the request
		HttpAssetListRequest->ProcessRequest();
	}

}

void UQuVRAssetDownNet::ParseNodeItemData(TSharedPtr<FQuVRCatalogNode> node, TSharedPtr<FJsonValue> JsonValue)
{
	if (node.IsValid())
	{
		FQuVRCatalogItem item;
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


void UQuVRAssetDownNet::ParseNodeListData(TSharedPtr<FQuVRCatalogNode> node, TArray<TSharedPtr<FJsonValue>> JsonValue)
{
	for (auto Value : JsonValue)
	{
		TSharedPtr<FQuVRCatalogNode> ChildNode = MakeShareable(new(FQuVRCatalogNode));
		ChildNode->ParentNode = node;
		node->ChildList.Add(ChildNode);
		ParseNodeChildData(ChildNode, Value);
	}
}


void UQuVRAssetDownNet::ParseNodeChildData(TSharedPtr<FQuVRCatalogNode> node, TSharedPtr<FJsonValue> JsonValue)
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

void UQuVRAssetDownNet::GenerateNodeCatalog(TSharedRef<FQuVRCatalogNode> node)
{
	if (Catawidget.IsValid())
	{
		Catawidget->CreateGroupGroupTabRoot(node);
	}
}

void UQuVRAssetDownNet::GenerateAssetCatalog(TSharedRef<FQuVRCatalogNode> node)
{
	Catawidget->ClearAssetList();
	if (Catawidget.IsValid() && node->HasAssetList)
	{
		Catawidget->CreateCatalogGroupTabAssetList(node);
	}
}
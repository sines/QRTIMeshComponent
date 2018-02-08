// Fill out your copyright notice in the Description page of Project Settings.

#include "QuVRAssetDownNet.h"
#include "QuVRFileDownloader.h"
#include "QuVRCatalogEntryWidget.h"

const FString UQuVRAssetDownNet::CatalogNodeHttpURL = FString(TEXT("http://dev-vrservice.jtl3d.com/api/VRCatalog/GetCatalogsByType?type=modelCatalog"));
const FString UQuVRAssetDownNet::CatalogAssetHttpURL = FString(TEXT("http://dev-vrservice.jtl3d.com/api/VRCatalog/SearchVRObjects"));

/*FQuVRCatalogNode*/
void FQuVRCatalogNode::ClearAllData()
{
	for (auto cdNode : ChildList)
	{
		cdNode->ClearChildNodelist();
	}
	ChildList.Reset();
}

void FQuVRCatalogNode::ClearChildNodelist()
{
	for (auto cdNode : ChildList)
	{
		if (cdNode.IsValid())
		{
			cdNode->ClearChildNodelist();
		}
	}
	ClearChildAssetlist();
	ChildList.Reset();
}
static bool IsDataNull(const TSharedPtr<UQuVRcatalogAssetInfo> Dependency)
{
	return Dependency.IsValid();
}

void FQuVRCatalogNode::ClearChildAssetlist()
{
	AssetList.Reset();
	HasAssetList = false;
}

/*UQuVRcatalogAssetInfo*/

UQuVRcatalogAssetInfo::UQuVRcatalogAssetInfo() :Texture2Dimage(NULL)
{
	Id = "0";
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
	Size = 0;
	IsDownload = false;
}


void UQuVRcatalogAssetInfo::Initialise(TSharedPtr<FQuVRCatalogNode> node)
{
	if (node.IsValid())
	{
		UQuVRFileDownloader* AsyncTaskDownloadImage = UQuVRFileDownloader::DownloadFile(ImageUrl);
		AsyncTaskDownloadImage->OnDownloadImageRes.AddUObject(this, &UQuVRcatalogAssetInfo::DownloadImage);
		AsyncTaskDownloadImage->AddToRoot();
	}

}

void UQuVRcatalogAssetInfo::DownloadImage(UTexture2DDynamic* texture2D)
{
	Texture2Dimage = texture2D;
	Texture2Dimage->AddToRoot();
	ImageDownloadDone.Broadcast();
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
		StaticInstance->AddToRoot();
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
		CurrentNode->ClearChildAssetlist();
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
		UQuVRcatalogAssetInfo* AssetItem =NewObject<UQuVRcatalogAssetInfo>();
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
		AssetItem->Initialise(node);
		node->HasAssetList = true;

		node->AssetList.AddUnique(AssetItem);
	}

}


void UQuVRAssetDownNet::GetAllCatalogNodeListFromUrl()
{
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

	// Clear NodeData
	RootNode->ClearAllData();
}

void UQuVRAssetDownNet::GetCatalogNodeAssetFromUrl(TSharedPtr<FQuVRCatalogNode>& node)
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
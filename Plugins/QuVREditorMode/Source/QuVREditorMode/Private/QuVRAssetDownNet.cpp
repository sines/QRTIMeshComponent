// Fill out your copyright notice in the Description page of Project Settings.

#include "QuVRAssetDownNet.h"

const FString UQuVRAssetDownNet::CatalogHttpURL = FString(TEXT("http://dev-vrservice.jtl3d.com/api/VRCatalog/GetCatalogsByType?type=modelCatalog"));

//////////////////////////////////////////////////////////////////////////
/**UQuVRAssetDownNet*/
//////////////////////////////////////////////////////////////////////////
UQuVRAssetDownNet::UQuVRAssetDownNet()
{
	RootNode = MakeShareable(new(FQuVRCatalogNode));
}


void UQuVRAssetDownNet::OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
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
#if 1 // Read Local json
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
	ParseListData(RootNode,TypeData);
	GenerateCatalog();
	// OnRequset Data Done
	OnRequestAllSelectTypeDataDone.Broadcast(this);
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
	HttpRequest = FHttpModule::Get().CreateRequest();
}

void UQuVRAssetDownNet::GetAllTypeDataFromUrl()
{
	RootNode->ChildList.Empty();
	RootNode->ChildList.Reset();
	FString TrimmedUrl = CatalogHttpURL;
	TrimmedUrl.Trim();
	TrimmedUrl.TrimTrailing();

	HttpRequest->SetURL(TrimmedUrl);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	// Bind event
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UQuVRAssetDownNet::OnProcessRequestComplete);

	// Execute the request
	HttpRequest->ProcessRequest();

}

void UQuVRAssetDownNet::ParseItemData(TSharedPtr<FQuVRCatalogNode> node, TSharedPtr<FJsonValue> JsonValue)
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

void UQuVRAssetDownNet::ParseListData(TSharedPtr<FQuVRCatalogNode> node, TArray<TSharedPtr<FJsonValue>> JsonValue)
{
	for (auto Value : JsonValue)
	{
		TSharedPtr<FQuVRCatalogNode> ChildNode = MakeShareable(new(FQuVRCatalogNode));
		ChildNode->ParentNode = node;
		node->ChildList.Add(ChildNode);
		ParseChildData(ChildNode, Value);
	}
}


void UQuVRAssetDownNet::ParseChildData(TSharedPtr<FQuVRCatalogNode> node, TSharedPtr<FJsonValue> JsonValue)
{
		ParseItemData(node, JsonValue);
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
					ParseChildData(addNode, Value);
				}
			}
		}
}

void UQuVRAssetDownNet::GenerateCatalog()
{
	if (Catawidget.IsValid())
	{
	//	Catawidget->CreateGroupGroupTabPrimaryList(RootNode->ChildList);
		Catawidget->CreateGroupGroupTabRoot(RootNode);
	}
}
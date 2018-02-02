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
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString("Request failed (%d): %s ")+ FString::FromInt(ResponseCode)+ *Request->GetURL());
		return;
	}

	// Save response data as a string
	ResponseContent = Response->GetContentAsString();
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString("Response(%d) : %sJSON(%s%s%s)JSON")+ FString::FromInt(ResponseCode)+ *ResponseContent);
	
	// Analyze Json Data
	//Read Json Data
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(ResponseContent);
	bool bResult = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	check(bResult);
	bResult = JsonObject.IsValid();
	check(bResult);

	TArray<TSharedPtr<FJsonValue>>TypeData = JsonObject->GetArrayField(TEXT("items"));
	ParseListData(TypeData);
	GenerateCatalog();
	// OnRequset Data Done
	OnRequestAllSelectTypeDataDone.Broadcast(this);
}

TSharedPtr<UQuVRAssetDownNet> UQuVRAssetDownNet::StaticInstance;
UQuVRAssetDownNet* UQuVRAssetDownNet::GetInstance()
{
	if (!StaticInstance.IsValid())
	{
		StaticInstance = MakeShareable(NewObject<UQuVRAssetDownNet>());
		StaticInstance->Initial();
	}

	return StaticInstance.Get();
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

void UQuVRAssetDownNet::ParseItemData(FQuVRCatalogNode& node, TArray<TSharedPtr<FJsonValue>> JsonValue)
{
	node.ChildList.Empty();
	node.ChildList.Reset();
	for (auto Value : JsonValue)
	{
		FQuVRCatalogNode* item = new FQuVRCatalogNode();
		TSharedPtr<FJsonObject>TempJsonObject = Value->AsObject();
		item->NodeData.Id=TempJsonObject->GetStringField(TEXT("ID"));
		item->NodeData.PId = TempJsonObject->GetStringField(TEXT("PID"));
		item->NodeData.Name = TempJsonObject->GetStringField(TEXT("Name"));
		item->NodeData.DisplayName = TempJsonObject->GetStringField(TEXT("DisplayName"));
		item->NodeData.Description = TempJsonObject->GetStringField(TEXT("Description"));
		item->NodeData.OrderNo = TempJsonObject->GetStringField(TEXT("OrderNo"));
		item->NodeData.CatalogType = TempJsonObject->GetStringField(TEXT("CatalogType"));
		node.ChildList.Add(MakeShareable(item));
	}
}

void UQuVRAssetDownNet::ParseListData(TArray<TSharedPtr<FJsonValue>> JsonValue)
{
	for (auto Value : JsonValue)
	{
		auto item = new FQuVRCatalogNode();
		TSharedPtr<FJsonObject>TempJsonObject = Value->AsObject();
		item->NodeData.Id = TempJsonObject->GetStringField(TEXT("ID"));
		item->NodeData.PId = TempJsonObject->GetStringField(TEXT("PID"));
		item->NodeData.Name = TempJsonObject->GetStringField(TEXT("Name"));
		item->NodeData.DisplayName = TempJsonObject->GetStringField(TEXT("DisplayName"));
		item->NodeData.Description = TempJsonObject->GetStringField(TEXT("Description"));
		item->NodeData.OrderNo = TempJsonObject->GetStringField(TEXT("OrderNo"));
		item->NodeData.CatalogType = TempJsonObject->GetStringField(TEXT("CatalogType"));
		RootNode->ChildList.Add(MakeShareable(item));
		
		if (TempJsonObject->HasField(TEXT("Children")))
		{
			TArray<TSharedPtr<FJsonValue>>ChildrenData = TempJsonObject->GetArrayField(TEXT("Children"));
			if (0 < ChildrenData.Num())
			{
				ParseListData(ChildrenData);
			}
		}
	
	//	RootNode->ChildList.Add(item);
	}
}

void UQuVRAssetDownNet::GenerateCatalog()
{
	if (Catawidget.IsValid())
	{
	//	Catawidget->CreateGroupGroupTabPrimaryList(NodeList);
	}
}
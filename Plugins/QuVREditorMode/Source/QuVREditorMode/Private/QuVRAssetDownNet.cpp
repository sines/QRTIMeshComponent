// Fill out your copyright notice in the Description page of Project Settings.

#include "QuVRAssetDownNet.h"

const FString UQuVRAssetDownNet::CatalogHttpURL = FString(TEXT("http://dev-vrservice.jtl3d.com/api/VRCatalog/GetCatalogsByType?type=modelCatalog"));

UQuVRAssetDownNet::UQuVRAssetDownNet()
{

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
	ParseSelectorTypeData(TypeData);
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
	NodeList.Empty();
	NodeList.Reset();
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

void UQuVRAssetDownNet::ParseSelectorTypeData(TArray<TSharedPtr<FJsonValue>> JsonValue)
{
	for (auto Value : JsonValue)
	{
		FQuVRCatalogNode item;
		TSharedPtr<FJsonObject>TempJsonObject = Value->AsObject();
		item.NodeData.Id=TempJsonObject->GetStringField(TEXT("ID"));
		item.NodeData.PId = TempJsonObject->GetStringField(TEXT("PID"));
		item.NodeData.Name = TempJsonObject->GetStringField(TEXT("Name"));
		item.NodeData.DisplayName = TempJsonObject->GetStringField(TEXT("DisplayName"));
		item.NodeData.Description = TempJsonObject->GetStringField(TEXT("Description"));
		item.NodeData.OrderNo = TempJsonObject->GetStringField(TEXT("OrderNo"));
		item.NodeData.CatalogType = TempJsonObject->GetStringField(TEXT("CatalogType"));
		NodeList.Add(item);
	}
}

void UQuVRAssetDownNet::ParseSecondTypeData(TArray<TSharedPtr<FJsonValue>> JsonValue)
{

}

void UQuVRAssetDownNet::ParseLastTypeData(TArray<TSharedPtr<FJsonValue>> JsonValue)
{

}

void UQuVRAssetDownNet::GenerateCatalog()
{
	if (Catawidget.IsValid())
	{
		Catawidget->test();
	}
}
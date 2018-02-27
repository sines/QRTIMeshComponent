// Fill out your copyright notice in the Description page of Project Settings.

#include "QuVRCatalogNodeBase.h"
#include "QuVRFileDownloader.h"
#include "QuVRCatalogEntryWidget.h"

/************************************************************************/
/*             FQuVRCatalogNode                                         */
/************************************************************************/
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
static bool IsDataNull(const TSharedPtr<UQuVRCatalogAssetInfo> Dependency)
{
	return Dependency.IsValid();
}

void FQuVRCatalogNode::ClearChildAssetlist()
{
	AssetList.Reset();
	HasAssetList = false;
}

/************************************************************************/
/*  UQuVRCatalogAssetInfo							                    */
/************************************************************************/

UQuVRCatalogAssetInfo::UQuVRCatalogAssetInfo() :Texture2Dimage(NULL)
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


void UQuVRCatalogAssetInfo::Initialise(TSharedPtr<FQuVRCatalogNode> node)
{
	if (node.IsValid())
	{
		UQuVRFileDownloader* AsyncTaskDownloadImage = UQuVRFileDownloader::DownloadImageLoader(ImageUrl);
		AsyncTaskDownloadImage->OnDownloadImageRes.AddUObject(this, &UQuVRCatalogAssetInfo::DownloadImage);
	}

}

void UQuVRCatalogAssetInfo::DownloadImage(UTexture2DDynamic* texture2D)
{
	Texture2Dimage = texture2D;
	Texture2Dimage->AddToRoot();
	ImageDownloadDone.Broadcast();
}
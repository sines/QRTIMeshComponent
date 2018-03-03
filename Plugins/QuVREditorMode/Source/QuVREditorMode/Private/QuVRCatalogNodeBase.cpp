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
/* 
ObjectType
SK_ 骨骼模型 0
SM_  静态模型 1
M_  材质球 2
T_   贴图 3
P_   特效 4
*/

UQuVRCatalogAssetInfo::UQuVRCatalogAssetInfo() :Texture2Dimage(NULL)
{
	Id = "-1";
	ObjectType = -1;
	ObjectTypeDesc = "NULL";
	Name = "-1";
	DisplayName = "NULL";
	Description = "NULL";
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
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
}
UQuVRCatalogAssetInfo* FQuVRCatalogNode::GetChildAsset(const UQuVRCatalogAssetInfo& AssetInfo)
{
	for (auto asset : AssetList)
	{
		if (asset)
		{
			if (asset->Name == AssetInfo.Name)
			{
				return asset;
			}
		}
	}
	return nullptr;
}

bool FQuVRCatalogNode::HasChildAsset(const UQuVRCatalogAssetInfo& AssetInfo)
{
	for (auto asset : AssetList)
	{
		if (asset)
		{
			if (asset->Name == AssetInfo.Name)
			{
				return true;
			}
		}
	}
	return false;
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
	IsImageDownload = false;
	AsyncTaskDownloadImage = nullptr;
	Texture2Dimage = NULL;
	ImageEventHandleList.Empty();
}
UQuVRCatalogAssetInfo::~UQuVRCatalogAssetInfo()
{
}

void UQuVRCatalogAssetInfo::AddImageEventHandle(FDelegateHandle handle)
{
	ImageEventHandleList.Add(handle);
}

void UQuVRCatalogAssetInfo::ClearImageEventHandle()
{
	for (auto handle : ImageEventHandleList)
	{
		ImageDownloadDone.Remove(handle);
	}
	ImageEventHandleList.Reset();
}

void UQuVRCatalogAssetInfo::Initialise()
{
	if (true == IsImageDownload)
	{
		ImageDownloadDone.Broadcast(Texture2Dimage);
	}
	else
	{
		AsyncTaskDownloadImage = UQuVRFileDownloader::DownloadImageLoader(ImageUrl);
		AsyncTaskDownloadImage->OnDownloadImageRes.AddUObject(this, &UQuVRCatalogAssetInfo::DownloadImage);
		AsyncTaskDownloadImage->OnDownloadFileDone.AddUObject(this, &UQuVRCatalogAssetInfo::DownloadDone);
	}
}

void UQuVRCatalogAssetInfo::DownloadImage(UTexture2DDynamic* texture2D)
{
	IsImageDownload = true;
	Texture2Dimage = texture2D;
	Texture2Dimage->AddToRoot();
	ImageDownloadDone.Broadcast(texture2D);
}

void UQuVRCatalogAssetInfo::ClearDownloadState()
{
	if (AsyncTaskDownloadImage)
	{
		AsyncTaskDownloadImage->ClearDownloadState();
	}
	ImageDownloadDone.Clear();
}

void UQuVRCatalogAssetInfo::DownloadDone(int32 code)
{
	ClearDownloadState();
}

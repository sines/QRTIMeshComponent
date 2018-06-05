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
SK_ ����ģ�� 0
SM_  ��̬ģ�� 1
M_  ������ 2
T_   ��ͼ 3
P_   ��Ч 4
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
	Texture2Dimage = NULL;
}

UQuVRCatalogAssetInfo::~UQuVRCatalogAssetInfo()
{
}
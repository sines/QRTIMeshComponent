// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "QuVREditorMode.generated.dep.h"
PRAGMA_DISABLE_OPTIMIZATION
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCode1QuVREditorMode() {}
#if USE_COMPILED_IN_NATIVES
// Cross Module References
	UNREALED_API class UClass* Z_Construct_UClass_UActorFactory();
	ENGINE_API class UClass* Z_Construct_UClass_UTexture2DDynamic_NoRegister();
	ENGINE_API class UClass* Z_Construct_UClass_UBlueprintAsyncActionBase();
	COREUOBJECT_API class UClass* Z_Construct_UClass_UObject();

	QUVREDITORMODE_API class UClass* Z_Construct_UClass_UQuVRAssetFactoryAnimation_NoRegister();
	QUVREDITORMODE_API class UClass* Z_Construct_UClass_UQuVRAssetFactoryAnimation();
	QUVREDITORMODE_API class UClass* Z_Construct_UClass_UQuVRAssetFactoryStaticMesh_NoRegister();
	QUVREDITORMODE_API class UClass* Z_Construct_UClass_UQuVRAssetFactoryStaticMesh();
	QUVREDITORMODE_API class UClass* Z_Construct_UClass_UQuVRAssetFactoryModel_NoRegister();
	QUVREDITORMODE_API class UClass* Z_Construct_UClass_UQuVRAssetFactoryModel();
	QUVREDITORMODE_API class UFunction* Z_Construct_UDelegateFunction_QuVREditorMode_QuVRDownloadImageBPDelegate__DelegateSignature();
	QUVREDITORMODE_API class UFunction* Z_Construct_UDelegateFunction_QuVREditorMode_QuVRFileDownloadUpdateProgressBPDelegate__DelegateSignature();
	QUVREDITORMODE_API class UClass* Z_Construct_UClass_UQuVRFileDownloader_NoRegister();
	QUVREDITORMODE_API class UClass* Z_Construct_UClass_UQuVRFileDownloader();
	QUVREDITORMODE_API class UScriptStruct* Z_Construct_UScriptStruct_FQuVRCatalogNodeInfo();
	QUVREDITORMODE_API class UClass* Z_Construct_UClass_UQuVRCatalogAssetInfo_NoRegister();
	QUVREDITORMODE_API class UClass* Z_Construct_UClass_UQuVRCatalogAssetInfo();
	QUVREDITORMODE_API class UFunction* Z_Construct_UDelegateFunction_QuVREditorMode_RequestAssetDataDoneDelegate__DelegateSignature();
	QUVREDITORMODE_API class UFunction* Z_Construct_UDelegateFunction_QuVREditorMode_RequestNodeDataDoneDelegate__DelegateSignature();
	QUVREDITORMODE_API class UClass* Z_Construct_UClass_UQuVRCatalogDataManager_NoRegister();
	QUVREDITORMODE_API class UClass* Z_Construct_UClass_UQuVRCatalogDataManager();
	QUVREDITORMODE_API class UScriptStruct* Z_Construct_UScriptStruct_FQuVRPackageModel();
	QUVREDITORMODE_API class UClass* Z_Construct_UClass_UQuVRUtils_NoRegister();
	QUVREDITORMODE_API class UClass* Z_Construct_UClass_UQuVRUtils();
	QUVREDITORMODE_API class UPackage* Z_Construct_UPackage__Script_QuVREditorMode();
	void UQuVRAssetFactoryAnimation::StaticRegisterNativesUQuVRAssetFactoryAnimation()
	{
	}
	UClass* Z_Construct_UClass_UQuVRAssetFactoryAnimation_NoRegister()
	{
		return UQuVRAssetFactoryAnimation::StaticClass();
	}
	UClass* Z_Construct_UClass_UQuVRAssetFactoryAnimation()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_UActorFactory();
			Z_Construct_UPackage__Script_QuVREditorMode();
			OuterClass = UQuVRAssetFactoryAnimation::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= 0x2008308C;


				OuterClass->ClassConfigName = FName(TEXT("Editor"));
				static TCppClassTypeInfo<TCppClassTypeTraits<UQuVRAssetFactoryAnimation> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("HideCategories"), TEXT("Object Object"));
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("QuVRAssetFactoryAnimation.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Public/QuVRAssetFactoryAnimation.h"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(UQuVRAssetFactoryAnimation, 2727040762);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UQuVRAssetFactoryAnimation(Z_Construct_UClass_UQuVRAssetFactoryAnimation, &UQuVRAssetFactoryAnimation::StaticClass, TEXT("/Script/QuVREditorMode"), TEXT("UQuVRAssetFactoryAnimation"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UQuVRAssetFactoryAnimation);
	void UQuVRAssetFactoryStaticMesh::StaticRegisterNativesUQuVRAssetFactoryStaticMesh()
	{
	}
	UClass* Z_Construct_UClass_UQuVRAssetFactoryStaticMesh_NoRegister()
	{
		return UQuVRAssetFactoryStaticMesh::StaticClass();
	}
	UClass* Z_Construct_UClass_UQuVRAssetFactoryStaticMesh()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_UActorFactory();
			Z_Construct_UPackage__Script_QuVREditorMode();
			OuterClass = UQuVRAssetFactoryStaticMesh::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= 0x2008308C;


				OuterClass->ClassConfigName = FName(TEXT("Editor"));
				static TCppClassTypeInfo<TCppClassTypeTraits<UQuVRAssetFactoryStaticMesh> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("HideCategories"), TEXT("Object"));
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("QuVRAssetFactoryStaticMesh.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Public/QuVRAssetFactoryStaticMesh.h"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(UQuVRAssetFactoryStaticMesh, 3012029937);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UQuVRAssetFactoryStaticMesh(Z_Construct_UClass_UQuVRAssetFactoryStaticMesh, &UQuVRAssetFactoryStaticMesh::StaticClass, TEXT("/Script/QuVREditorMode"), TEXT("UQuVRAssetFactoryStaticMesh"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UQuVRAssetFactoryStaticMesh);
	void UQuVRAssetFactoryModel::StaticRegisterNativesUQuVRAssetFactoryModel()
	{
	}
	UClass* Z_Construct_UClass_UQuVRAssetFactoryModel_NoRegister()
	{
		return UQuVRAssetFactoryModel::StaticClass();
	}
	UClass* Z_Construct_UClass_UQuVRAssetFactoryModel()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_UQuVRAssetFactoryStaticMesh();
			Z_Construct_UPackage__Script_QuVREditorMode();
			OuterClass = UQuVRAssetFactoryModel::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= 0x2008308C;


				OuterClass->ClassConfigName = FName(TEXT("Editor"));
				static TCppClassTypeInfo<TCppClassTypeTraits<UQuVRAssetFactoryModel> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("HideCategories"), TEXT("Object Object"));
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("QuVRAssetFactoryModel.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Public/QuVRAssetFactoryModel.h"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(UQuVRAssetFactoryModel, 1966048630);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UQuVRAssetFactoryModel(Z_Construct_UClass_UQuVRAssetFactoryModel, &UQuVRAssetFactoryModel::StaticClass, TEXT("/Script/QuVREditorMode"), TEXT("UQuVRAssetFactoryModel"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UQuVRAssetFactoryModel);
	UFunction* Z_Construct_UDelegateFunction_QuVREditorMode_QuVRDownloadImageBPDelegate__DelegateSignature()
	{
		struct _Script_QuVREditorMode_eventQuVRDownloadImageBPDelegate_Parms
		{
			UTexture2DDynamic* Texture;
		};
		UObject* Outer=Z_Construct_UPackage__Script_QuVREditorMode();
		static UFunction* ReturnFunction = NULL;
		if (!ReturnFunction)
		{
			ReturnFunction = new(EC_InternalUseOnlyConstructor, Outer, TEXT("QuVRDownloadImageBPDelegate__DelegateSignature"), RF_Public|RF_Transient|RF_MarkAsNative) UDelegateFunction(FObjectInitializer(), NULL, 0x00130000, 65535, sizeof(_Script_QuVREditorMode_eventQuVRDownloadImageBPDelegate_Parms));
			UProperty* NewProp_Texture = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("Texture"), RF_Public|RF_Transient|RF_MarkAsNative) UObjectProperty(CPP_PROPERTY_BASE(Texture, _Script_QuVREditorMode_eventQuVRDownloadImageBPDelegate_Parms), 0x0010000000000080, Z_Construct_UClass_UTexture2DDynamic_NoRegister());
			ReturnFunction->Bind();
			ReturnFunction->StaticLink();
#if WITH_METADATA
			UMetaData* MetaData = ReturnFunction->GetOutermost()->GetMetaData();
			MetaData->SetValue(ReturnFunction, TEXT("ModuleRelativePath"), TEXT("Public/QuVRFileDownloader.h"));
#endif
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UDelegateFunction_QuVREditorMode_QuVRFileDownloadUpdateProgressBPDelegate__DelegateSignature()
	{
		struct _Script_QuVREditorMode_eventQuVRFileDownloadUpdateProgressBPDelegate_Parms
		{
			int32 ReceivedDataInBytes;
			int32 TotalDataInBytes;
			TArray<uint8> BinaryData;
		};
		UObject* Outer=Z_Construct_UPackage__Script_QuVREditorMode();
		static UFunction* ReturnFunction = NULL;
		if (!ReturnFunction)
		{
			ReturnFunction = new(EC_InternalUseOnlyConstructor, Outer, TEXT("QuVRFileDownloadUpdateProgressBPDelegate__DelegateSignature"), RF_Public|RF_Transient|RF_MarkAsNative) UDelegateFunction(FObjectInitializer(), NULL, 0x00130000, 65535, sizeof(_Script_QuVREditorMode_eventQuVRFileDownloadUpdateProgressBPDelegate_Parms));
			UProperty* NewProp_BinaryData = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("BinaryData"), RF_Public|RF_Transient|RF_MarkAsNative) UArrayProperty(CPP_PROPERTY_BASE(BinaryData, _Script_QuVREditorMode_eventQuVRFileDownloadUpdateProgressBPDelegate_Parms), 0x0010000008000182);
			UProperty* NewProp_BinaryData_Inner = new(EC_InternalUseOnlyConstructor, NewProp_BinaryData, TEXT("BinaryData"), RF_Public|RF_Transient|RF_MarkAsNative) UByteProperty(FObjectInitializer(), EC_CppProperty, 0, 0x0000000000000000);
			UProperty* NewProp_TotalDataInBytes = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("TotalDataInBytes"), RF_Public|RF_Transient|RF_MarkAsNative) UIntProperty(CPP_PROPERTY_BASE(TotalDataInBytes, _Script_QuVREditorMode_eventQuVRFileDownloadUpdateProgressBPDelegate_Parms), 0x0010000000000080);
			UProperty* NewProp_ReceivedDataInBytes = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("ReceivedDataInBytes"), RF_Public|RF_Transient|RF_MarkAsNative) UIntProperty(CPP_PROPERTY_BASE(ReceivedDataInBytes, _Script_QuVREditorMode_eventQuVRFileDownloadUpdateProgressBPDelegate_Parms), 0x0010000000000080);
			ReturnFunction->Bind();
			ReturnFunction->StaticLink();
#if WITH_METADATA
			UMetaData* MetaData = ReturnFunction->GetOutermost()->GetMetaData();
			MetaData->SetValue(ReturnFunction, TEXT("ModuleRelativePath"), TEXT("Public/QuVRFileDownloader.h"));
			MetaData->SetValue(NewProp_BinaryData, TEXT("NativeConst"), TEXT(""));
#endif
		}
		return ReturnFunction;
	}
	void UQuVRFileDownloader::StaticRegisterNativesUQuVRFileDownloader()
	{
	}
	UClass* Z_Construct_UClass_UQuVRFileDownloader_NoRegister()
	{
		return UQuVRFileDownloader::StaticClass();
	}
	UClass* Z_Construct_UClass_UQuVRFileDownloader()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_UBlueprintAsyncActionBase();
			Z_Construct_UPackage__Script_QuVREditorMode();
			OuterClass = UQuVRFileDownloader::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= 0x20100080;


				static TCppClassTypeInfo<TCppClassTypeTraits<UQuVRFileDownloader> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("QuVRFileDownloader.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Public/QuVRFileDownloader.h"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(UQuVRFileDownloader, 4102138049);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UQuVRFileDownloader(Z_Construct_UClass_UQuVRFileDownloader, &UQuVRFileDownloader::StaticClass, TEXT("/Script/QuVREditorMode"), TEXT("UQuVRFileDownloader"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UQuVRFileDownloader);
class UScriptStruct* FQuVRCatalogNodeInfo::StaticStruct()
{
	extern QUVREDITORMODE_API class UPackage* Z_Construct_UPackage__Script_QuVREditorMode();
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern QUVREDITORMODE_API class UScriptStruct* Z_Construct_UScriptStruct_FQuVRCatalogNodeInfo();
		extern QUVREDITORMODE_API uint32 Get_Z_Construct_UScriptStruct_FQuVRCatalogNodeInfo_CRC();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FQuVRCatalogNodeInfo, Z_Construct_UPackage__Script_QuVREditorMode(), TEXT("QuVRCatalogNodeInfo"), sizeof(FQuVRCatalogNodeInfo), Get_Z_Construct_UScriptStruct_FQuVRCatalogNodeInfo_CRC());
	}
	return Singleton;
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FQuVRCatalogNodeInfo(FQuVRCatalogNodeInfo::StaticStruct, TEXT("/Script/QuVREditorMode"), TEXT("QuVRCatalogNodeInfo"), false, nullptr, nullptr);
static struct FScriptStruct_QuVREditorMode_StaticRegisterNativesFQuVRCatalogNodeInfo
{
	FScriptStruct_QuVREditorMode_StaticRegisterNativesFQuVRCatalogNodeInfo()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("QuVRCatalogNodeInfo")),new UScriptStruct::TCppStructOps<FQuVRCatalogNodeInfo>);
	}
} ScriptStruct_QuVREditorMode_StaticRegisterNativesFQuVRCatalogNodeInfo;
	UScriptStruct* Z_Construct_UScriptStruct_FQuVRCatalogNodeInfo()
	{
		UPackage* Outer = Z_Construct_UPackage__Script_QuVREditorMode();
		extern uint32 Get_Z_Construct_UScriptStruct_FQuVRCatalogNodeInfo_CRC();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("QuVRCatalogNodeInfo"), sizeof(FQuVRCatalogNodeInfo), Get_Z_Construct_UScriptStruct_FQuVRCatalogNodeInfo_CRC(), false);
		if (!ReturnStruct)
		{
			ReturnStruct = new(EC_InternalUseOnlyConstructor, Outer, TEXT("QuVRCatalogNodeInfo"), RF_Public|RF_Transient|RF_MarkAsNative) UScriptStruct(FObjectInitializer(), NULL, new UScriptStruct::TCppStructOps<FQuVRCatalogNodeInfo>, EStructFlags(0x00000201));
			ReturnStruct->StaticLink();
#if WITH_METADATA
			UMetaData* MetaData = ReturnStruct->GetOutermost()->GetMetaData();
			MetaData->SetValue(ReturnStruct, TEXT("ModuleRelativePath"), TEXT("Public/QuVRCatalogNodeBase.h"));
#endif
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FQuVRCatalogNodeInfo_CRC() { return 1990642453U; }
	void UQuVRCatalogAssetInfo::StaticRegisterNativesUQuVRCatalogAssetInfo()
	{
	}
	UClass* Z_Construct_UClass_UQuVRCatalogAssetInfo_NoRegister()
	{
		return UQuVRCatalogAssetInfo::StaticClass();
	}
	UClass* Z_Construct_UClass_UQuVRCatalogAssetInfo()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_UObject();
			Z_Construct_UPackage__Script_QuVREditorMode();
			OuterClass = UQuVRCatalogAssetInfo::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= 0x20100080;


				static TCppClassTypeInfo<TCppClassTypeTraits<UQuVRCatalogAssetInfo> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("QuVRCatalogNodeBase.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Public/QuVRCatalogNodeBase.h"));
				MetaData->SetValue(OuterClass, TEXT("ToolTip"), TEXT("UQuVRCatalogAssetInfo"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(UQuVRCatalogAssetInfo, 4037215461);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UQuVRCatalogAssetInfo(Z_Construct_UClass_UQuVRCatalogAssetInfo, &UQuVRCatalogAssetInfo::StaticClass, TEXT("/Script/QuVREditorMode"), TEXT("UQuVRCatalogAssetInfo"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UQuVRCatalogAssetInfo);
	UFunction* Z_Construct_UDelegateFunction_QuVREditorMode_RequestAssetDataDoneDelegate__DelegateSignature()
	{
		struct _Script_QuVREditorMode_eventRequestAssetDataDoneDelegate_Parms
		{
			UQuVRCatalogDataManager* net;
		};
		UObject* Outer=Z_Construct_UPackage__Script_QuVREditorMode();
		static UFunction* ReturnFunction = NULL;
		if (!ReturnFunction)
		{
			ReturnFunction = new(EC_InternalUseOnlyConstructor, Outer, TEXT("RequestAssetDataDoneDelegate__DelegateSignature"), RF_Public|RF_Transient|RF_MarkAsNative) UDelegateFunction(FObjectInitializer(), NULL, 0x00130000, 65535, sizeof(_Script_QuVREditorMode_eventRequestAssetDataDoneDelegate_Parms));
			UProperty* NewProp_net = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("net"), RF_Public|RF_Transient|RF_MarkAsNative) UObjectProperty(CPP_PROPERTY_BASE(net, _Script_QuVREditorMode_eventRequestAssetDataDoneDelegate_Parms), 0x0010000000000080, Z_Construct_UClass_UQuVRCatalogDataManager_NoRegister());
			ReturnFunction->Bind();
			ReturnFunction->StaticLink();
#if WITH_METADATA
			UMetaData* MetaData = ReturnFunction->GetOutermost()->GetMetaData();
			MetaData->SetValue(ReturnFunction, TEXT("ModuleRelativePath"), TEXT("Public/QuVRCatalogDataManager.h"));
#endif
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UDelegateFunction_QuVREditorMode_RequestNodeDataDoneDelegate__DelegateSignature()
	{
		struct _Script_QuVREditorMode_eventRequestNodeDataDoneDelegate_Parms
		{
			UQuVRCatalogDataManager* net;
		};
		UObject* Outer=Z_Construct_UPackage__Script_QuVREditorMode();
		static UFunction* ReturnFunction = NULL;
		if (!ReturnFunction)
		{
			ReturnFunction = new(EC_InternalUseOnlyConstructor, Outer, TEXT("RequestNodeDataDoneDelegate__DelegateSignature"), RF_Public|RF_Transient|RF_MarkAsNative) UDelegateFunction(FObjectInitializer(), NULL, 0x00130000, 65535, sizeof(_Script_QuVREditorMode_eventRequestNodeDataDoneDelegate_Parms));
			UProperty* NewProp_net = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("net"), RF_Public|RF_Transient|RF_MarkAsNative) UObjectProperty(CPP_PROPERTY_BASE(net, _Script_QuVREditorMode_eventRequestNodeDataDoneDelegate_Parms), 0x0010000000000080, Z_Construct_UClass_UQuVRCatalogDataManager_NoRegister());
			ReturnFunction->Bind();
			ReturnFunction->StaticLink();
#if WITH_METADATA
			UMetaData* MetaData = ReturnFunction->GetOutermost()->GetMetaData();
			MetaData->SetValue(ReturnFunction, TEXT("ModuleRelativePath"), TEXT("Public/QuVRCatalogDataManager.h"));
#endif
		}
		return ReturnFunction;
	}
	void UQuVRCatalogDataManager::StaticRegisterNativesUQuVRCatalogDataManager()
	{
	}
	UClass* Z_Construct_UClass_UQuVRCatalogDataManager_NoRegister()
	{
		return UQuVRCatalogDataManager::StaticClass();
	}
	UClass* Z_Construct_UClass_UQuVRCatalogDataManager()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_UObject();
			Z_Construct_UPackage__Script_QuVREditorMode();
			OuterClass = UQuVRCatalogDataManager::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= 0x20100080;


				static TCppClassTypeInfo<TCppClassTypeTraits<UQuVRCatalogDataManager> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("QuVRCatalogDataManager.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Public/QuVRCatalogDataManager.h"));
				MetaData->SetValue(OuterClass, TEXT("ToolTip"), TEXT("UQuVRCatalogDataManager"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(UQuVRCatalogDataManager, 1546665062);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UQuVRCatalogDataManager(Z_Construct_UClass_UQuVRCatalogDataManager, &UQuVRCatalogDataManager::StaticClass, TEXT("/Script/QuVREditorMode"), TEXT("UQuVRCatalogDataManager"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UQuVRCatalogDataManager);
class UScriptStruct* FQuVRPackageModel::StaticStruct()
{
	extern QUVREDITORMODE_API class UPackage* Z_Construct_UPackage__Script_QuVREditorMode();
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern QUVREDITORMODE_API class UScriptStruct* Z_Construct_UScriptStruct_FQuVRPackageModel();
		extern QUVREDITORMODE_API uint32 Get_Z_Construct_UScriptStruct_FQuVRPackageModel_CRC();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FQuVRPackageModel, Z_Construct_UPackage__Script_QuVREditorMode(), TEXT("QuVRPackageModel"), sizeof(FQuVRPackageModel), Get_Z_Construct_UScriptStruct_FQuVRPackageModel_CRC());
	}
	return Singleton;
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FQuVRPackageModel(FQuVRPackageModel::StaticStruct, TEXT("/Script/QuVREditorMode"), TEXT("QuVRPackageModel"), false, nullptr, nullptr);
static struct FScriptStruct_QuVREditorMode_StaticRegisterNativesFQuVRPackageModel
{
	FScriptStruct_QuVREditorMode_StaticRegisterNativesFQuVRPackageModel()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("QuVRPackageModel")),new UScriptStruct::TCppStructOps<FQuVRPackageModel>);
	}
} ScriptStruct_QuVREditorMode_StaticRegisterNativesFQuVRPackageModel;
	UScriptStruct* Z_Construct_UScriptStruct_FQuVRPackageModel()
	{
		UPackage* Outer = Z_Construct_UPackage__Script_QuVREditorMode();
		extern uint32 Get_Z_Construct_UScriptStruct_FQuVRPackageModel_CRC();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("QuVRPackageModel"), sizeof(FQuVRPackageModel), Get_Z_Construct_UScriptStruct_FQuVRPackageModel_CRC(), false);
		if (!ReturnStruct)
		{
			ReturnStruct = new(EC_InternalUseOnlyConstructor, Outer, TEXT("QuVRPackageModel"), RF_Public|RF_Transient|RF_MarkAsNative) UScriptStruct(FObjectInitializer(), NULL, new UScriptStruct::TCppStructOps<FQuVRPackageModel>, EStructFlags(0x00000201));
			ReturnStruct->StaticLink();
#if WITH_METADATA
			UMetaData* MetaData = ReturnStruct->GetOutermost()->GetMetaData();
			MetaData->SetValue(ReturnStruct, TEXT("ModuleRelativePath"), TEXT("Public/QuVRUtils.h"));
#endif
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FQuVRPackageModel_CRC() { return 3224636203U; }
	void UQuVRUtils::StaticRegisterNativesUQuVRUtils()
	{
	}
	UClass* Z_Construct_UClass_UQuVRUtils_NoRegister()
	{
		return UQuVRUtils::StaticClass();
	}
	UClass* Z_Construct_UClass_UQuVRUtils()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_UObject();
			Z_Construct_UPackage__Script_QuVREditorMode();
			OuterClass = UQuVRUtils::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= 0x20000080;


				static TCppClassTypeInfo<TCppClassTypeTraits<UQuVRUtils> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("QuVRUtils.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Public/QuVRUtils.h"));
				MetaData->SetValue(OuterClass, TEXT("ToolTip"), TEXT("Utils Static Function"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(UQuVRUtils, 569613789);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UQuVRUtils(Z_Construct_UClass_UQuVRUtils, &UQuVRUtils::StaticClass, TEXT("/Script/QuVREditorMode"), TEXT("UQuVRUtils"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UQuVRUtils);
	UPackage* Z_Construct_UPackage__Script_QuVREditorMode()
	{
		static UPackage* ReturnPackage = nullptr;
		if (!ReturnPackage)
		{
			ReturnPackage = CastChecked<UPackage>(StaticFindObjectFast(UPackage::StaticClass(), nullptr, FName(TEXT("/Script/QuVREditorMode")), false, false));
			ReturnPackage->SetPackageFlags(PKG_CompiledIn | 0x00000040);
			FGuid Guid;
			Guid.A = 0x23937E3B;
			Guid.B = 0x4191CE27;
			Guid.C = 0x00000000;
			Guid.D = 0x00000000;
			ReturnPackage->SetGuid(Guid);

			Z_Construct_UDelegateFunction_QuVREditorMode_QuVRFileDownloadUpdateProgressBPDelegate__DelegateSignature();
			Z_Construct_UDelegateFunction_QuVREditorMode_QuVRDownloadImageBPDelegate__DelegateSignature();
			Z_Construct_UDelegateFunction_QuVREditorMode_RequestNodeDataDoneDelegate__DelegateSignature();
			Z_Construct_UDelegateFunction_QuVREditorMode_RequestAssetDataDoneDelegate__DelegateSignature();
		}
		return ReturnPackage;
	}
#endif
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
PRAGMA_ENABLE_OPTIMIZATION

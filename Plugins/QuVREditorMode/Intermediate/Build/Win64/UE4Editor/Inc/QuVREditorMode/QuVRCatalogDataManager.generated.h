// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UQuVRCatalogDataManager;
#ifdef QUVREDITORMODE_QuVRCatalogDataManager_generated_h
#error "QuVRCatalogDataManager.generated.h already included, missing '#pragma once' in QuVRCatalogDataManager.h"
#endif
#define QUVREDITORMODE_QuVRCatalogDataManager_generated_h

#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_18_DELEGATE \
struct _Script_QuVREditorMode_eventRequestAssetDataDoneDelegate_Parms \
{ \
	UQuVRCatalogDataManager* net; \
}; \
static inline void FRequestAssetDataDoneDelegate_DelegateWrapper(const FMulticastScriptDelegate& RequestAssetDataDoneDelegate, UQuVRCatalogDataManager* net) \
{ \
	_Script_QuVREditorMode_eventRequestAssetDataDoneDelegate_Parms Parms; \
	Parms.net=net; \
	RequestAssetDataDoneDelegate.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_17_DELEGATE \
struct _Script_QuVREditorMode_eventRequestNodeDataDoneDelegate_Parms \
{ \
	UQuVRCatalogDataManager* net; \
}; \
static inline void FRequestNodeDataDoneDelegate_DelegateWrapper(const FMulticastScriptDelegate& RequestNodeDataDoneDelegate, UQuVRCatalogDataManager* net) \
{ \
	_Script_QuVREditorMode_eventRequestNodeDataDoneDelegate_Parms Parms; \
	Parms.net=net; \
	RequestNodeDataDoneDelegate.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_26_RPC_WRAPPERS
#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_26_RPC_WRAPPERS_NO_PURE_DECLS
#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_26_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUQuVRCatalogDataManager(); \
	friend QUVREDITORMODE_API class UClass* Z_Construct_UClass_UQuVRCatalogDataManager(); \
public: \
	DECLARE_CLASS(UQuVRCatalogDataManager, UObject, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/QuVREditorMode"), NO_API) \
	DECLARE_SERIALIZER(UQuVRCatalogDataManager) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_26_INCLASS \
private: \
	static void StaticRegisterNativesUQuVRCatalogDataManager(); \
	friend QUVREDITORMODE_API class UClass* Z_Construct_UClass_UQuVRCatalogDataManager(); \
public: \
	DECLARE_CLASS(UQuVRCatalogDataManager, UObject, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/QuVREditorMode"), NO_API) \
	DECLARE_SERIALIZER(UQuVRCatalogDataManager) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_26_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UQuVRCatalogDataManager(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UQuVRCatalogDataManager) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UQuVRCatalogDataManager); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UQuVRCatalogDataManager); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UQuVRCatalogDataManager(UQuVRCatalogDataManager&&); \
	NO_API UQuVRCatalogDataManager(const UQuVRCatalogDataManager&); \
public:


#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_26_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UQuVRCatalogDataManager(UQuVRCatalogDataManager&&); \
	NO_API UQuVRCatalogDataManager(const UQuVRCatalogDataManager&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UQuVRCatalogDataManager); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UQuVRCatalogDataManager); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UQuVRCatalogDataManager)


#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_26_PRIVATE_PROPERTY_OFFSET
#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_23_PROLOG
#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_26_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_26_PRIVATE_PROPERTY_OFFSET \
	QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_26_RPC_WRAPPERS \
	QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_26_INCLASS \
	QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_26_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_26_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_26_PRIVATE_PROPERTY_OFFSET \
	QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_26_RPC_WRAPPERS_NO_PURE_DECLS \
	QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_26_INCLASS_NO_PURE_DECLS \
	QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h_26_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRCatalogDataManager_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS

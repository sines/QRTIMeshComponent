// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UTexture2DDynamic;
#ifdef QUVREDITORMODE_QuVRFileDownloader_generated_h
#error "QuVRFileDownloader.generated.h already included, missing '#pragma once' in QuVRFileDownloader.h"
#endif
#define QUVREDITORMODE_QuVRFileDownloader_generated_h

#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_15_DELEGATE \
struct _Script_QuVREditorMode_eventQuVRDownloadImageBPDelegate_Parms \
{ \
	UTexture2DDynamic* Texture; \
}; \
static inline void FQuVRDownloadImageBPDelegate_DelegateWrapper(const FMulticastScriptDelegate& QuVRDownloadImageBPDelegate, UTexture2DDynamic* Texture) \
{ \
	_Script_QuVREditorMode_eventQuVRDownloadImageBPDelegate_Parms Parms; \
	Parms.Texture=Texture; \
	QuVRDownloadImageBPDelegate.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_14_DELEGATE \
struct _Script_QuVREditorMode_eventQuVRFileDownloadUpdateProgressBPDelegate_Parms \
{ \
	int32 ReceivedDataInBytes; \
	int32 TotalDataInBytes; \
	TArray<uint8> BinaryData; \
}; \
static inline void FQuVRFileDownloadUpdateProgressBPDelegate_DelegateWrapper(const FMulticastScriptDelegate& QuVRFileDownloadUpdateProgressBPDelegate, int32 ReceivedDataInBytes, int32 TotalDataInBytes, TArray<uint8> const& BinaryData) \
{ \
	_Script_QuVREditorMode_eventQuVRFileDownloadUpdateProgressBPDelegate_Parms Parms; \
	Parms.ReceivedDataInBytes=ReceivedDataInBytes; \
	Parms.TotalDataInBytes=TotalDataInBytes; \
	Parms.BinaryData=BinaryData; \
	QuVRFileDownloadUpdateProgressBPDelegate.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_27_RPC_WRAPPERS
#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_27_RPC_WRAPPERS_NO_PURE_DECLS
#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_27_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUQuVRFileDownloader(); \
	friend QUVREDITORMODE_API class UClass* Z_Construct_UClass_UQuVRFileDownloader(); \
public: \
	DECLARE_CLASS(UQuVRFileDownloader, UBlueprintAsyncActionBase, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/QuVREditorMode"), NO_API) \
	DECLARE_SERIALIZER(UQuVRFileDownloader) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_27_INCLASS \
private: \
	static void StaticRegisterNativesUQuVRFileDownloader(); \
	friend QUVREDITORMODE_API class UClass* Z_Construct_UClass_UQuVRFileDownloader(); \
public: \
	DECLARE_CLASS(UQuVRFileDownloader, UBlueprintAsyncActionBase, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/QuVREditorMode"), NO_API) \
	DECLARE_SERIALIZER(UQuVRFileDownloader) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_27_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UQuVRFileDownloader(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UQuVRFileDownloader) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UQuVRFileDownloader); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UQuVRFileDownloader); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UQuVRFileDownloader(UQuVRFileDownloader&&); \
	NO_API UQuVRFileDownloader(const UQuVRFileDownloader&); \
public:


#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_27_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UQuVRFileDownloader(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UQuVRFileDownloader(UQuVRFileDownloader&&); \
	NO_API UQuVRFileDownloader(const UQuVRFileDownloader&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UQuVRFileDownloader); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UQuVRFileDownloader); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UQuVRFileDownloader)


#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_27_PRIVATE_PROPERTY_OFFSET
#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_24_PROLOG
#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_27_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_27_PRIVATE_PROPERTY_OFFSET \
	QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_27_RPC_WRAPPERS \
	QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_27_INCLASS \
	QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_27_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_27_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_27_PRIVATE_PROPERTY_OFFSET \
	QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_27_RPC_WRAPPERS_NO_PURE_DECLS \
	QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_27_INCLASS_NO_PURE_DECLS \
	QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h_27_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class QuVRFileDownloader."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID QRTIMeshComponent_Plugins_QuVREditorMode_Source_QuVREditorMode_Public_QuVRFileDownloader_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS

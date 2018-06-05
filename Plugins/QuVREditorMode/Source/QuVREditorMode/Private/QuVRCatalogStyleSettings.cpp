// Fill out your copyright notice in the Description page of Project Settings.

#include "QuVRCatalogStyleSettings.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "SlateGameResources.h"
#include "IPluginManager.h"

TSharedPtr< FSlateStyleSet > FQuVRCatalogStyleSettings::StyleInstance = NULL;

void FQuVRCatalogStyleSettings::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FQuVRCatalogStyleSettings::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FQuVRCatalogStyleSettings::GetStyleSetName()
{
	static FName StyleSetName(TEXT("QuVRCatalogStyleSettings"));
	return StyleSetName;
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon50x50(50.0f, 50.0f);
const FVector2D Icon113x27(113.0f, 27.0f);
const FVector2D Icon128x128(128.0f, 128.0f);
const FVector2D Icon256x256(256.f, 256.f);
const FVector2D Icon1000x1(1000.0f, 1.0f);

TSharedRef< FSlateStyleSet > FQuVRCatalogStyleSettings::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("FQuVRCatalogStyleSettings"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("QuVREditorMode")->GetBaseDir() / TEXT("Resources"));

	Style->Set("QuVREditorMode.DownLoad", new IMAGE_BRUSH(TEXT("DownLoad"), Icon113x27));
	Style->Set("QuVREditorMode.Icon128", new IMAGE_BRUSH(TEXT("Icon128"), Icon40x40));
	Style->Set("QuVREditorMode.Refresh", new IMAGE_BRUSH(TEXT("Refresh"), Icon128x128));

	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT

void FQuVRCatalogStyleSettings::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FQuVRCatalogStyleSettings::Get()
{
	return *StyleInstance;
}

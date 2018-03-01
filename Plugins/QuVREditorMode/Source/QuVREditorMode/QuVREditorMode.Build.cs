// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class QuVREditorMode : ModuleRules
{
	public QuVREditorMode(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"QuVREditorMode/Public"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"QuVREditorMode/Private",
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CoreUObject",
                "Slate",
                "SlateCore",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
                "RHI",

                "Slate",
				"SlateCore",
                "RenderCore",

                "InputCore",

                "UnrealEd",
				"LevelEditor",
                "EditorStyle",
                "PlacementMode",

                "HTTP",
                "Json",
				// ... add private dependencies that you statically link with here ...	
			}
			);


        PrivateIncludePathModuleNames.AddRange(
              new string[] {
                "ImageWrapper",
                 "TargetPlatform",
              }
          );

        if (Target.Type != TargetType.Server)
        {
            PrivateIncludePathModuleNames.AddRange(
                new string[] {
                    "SlateRHIRenderer",
                }
            );

            DynamicallyLoadedModuleNames.AddRange(
                new string[] {
                    "ImageWrapper",
                    "SlateRHIRenderer",
                    "AssetTools",
                }
            );
        };
    }
}

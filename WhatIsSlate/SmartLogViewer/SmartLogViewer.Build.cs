// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SmartLogViewer : ModuleRules
{
	public SmartLogViewer(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicIncludePaths.Add("Runtime/Launch/Public");

		PrivateIncludePaths.Add("Runtime/Launch/Private");		// For LaunchEngineLoop.cpp include
		
		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"AppFramework",
				//"Engine",
				"Core",
				"RenderCore",
				"ApplicationCore",
				"Projects",
				"Slate",
				"SlateCore",
				"StandaloneRenderer",
				"SourceCodeAccess",
				"EditorStyle",
				"DesktopPlatform",
				//"MainFrame"
			}
		);

		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"SlateReflector",
			}
		);

		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
				"SlateReflector",
			}
		);
	}
}

// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class UnrealSerial : ModuleRules
	{
		public UnrealSerial(ReadOnlyTargetRules Target) : base(Target)
		{

			//PCHUsage = ModuleRules.PCHUsageMode.NoSharedPCHs;
			PrivatePCHHeaderFile = "Private/UnrealSerialPrivatePCH.h";
			bEnableUndefinedIdentifierWarnings = false;
			PublicIncludePaths.AddRange(
				new string[] {
					// ... add public include paths required here ...
				}
				);

			PrivateIncludePaths.AddRange(
				new string[] {
					"UnrealSerial/Private"
				}
				);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Engine",
					"InputCore",
					"Core",
					"Slate",
					"SlateCore",
					"EditorStyle",
					"CoreUObject"
				}
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					// ... add private dependencies that you statically link with here ...
				}
				);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
				);
		}
	}
}

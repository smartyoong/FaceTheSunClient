// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FaceTheSun : ModuleRules
{
	public FaceTheSun(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}

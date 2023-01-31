// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FaceTheSun : ModuleRules
{
	public FaceTheSun(ReadOnlyTargetRules Target) : base(Target)
	{
		bEnableExceptions = true;
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput","UMG","Networking","Sockets","SlateCore","AIModule", "GameplayTasks", "ApexDestruction" });
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MultiPKS : ModuleRules
{
	public MultiPKS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "PoseSearch", "Json", "JsonUtilities" });
	}
}

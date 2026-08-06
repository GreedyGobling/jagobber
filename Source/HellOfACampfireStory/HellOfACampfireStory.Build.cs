// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HellOfACampfireStory : ModuleRules
{
	public HellOfACampfireStory(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"Niagara",
			"UMG",
			"Slate",
			"DeveloperSettings"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "CommonUI" });

		PublicIncludePaths.AddRange(new string[] {
			"HellOfACampfireStory",
			"HellOfACampfireStory/Variant_Strategy",
			"HellOfACampfireStory/Variant_Strategy/UI",
			"HellOfACampfireStory/Variant_TwinStick",
			"HellOfACampfireStory/Variant_TwinStick/AI",
			"HellOfACampfireStory/Variant_TwinStick/Gameplay",
			"HellOfACampfireStory/Variant_TwinStick/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

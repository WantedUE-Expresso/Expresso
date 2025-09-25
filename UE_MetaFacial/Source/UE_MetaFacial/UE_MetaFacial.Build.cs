// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE_MetaFacial : ModuleRules
{
	public UE_MetaFacial(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"UE_MetaFacial",
			"UE_MetaFacial/Variant_Platforming",
			"UE_MetaFacial/Variant_Platforming/Animation",
			"UE_MetaFacial/Variant_Combat",
			"UE_MetaFacial/Variant_Combat/AI",
			"UE_MetaFacial/Variant_Combat/Animation",
			"UE_MetaFacial/Variant_Combat/Gameplay",
			"UE_MetaFacial/Variant_Combat/Interfaces",
			"UE_MetaFacial/Variant_Combat/UI",
			"UE_MetaFacial/Variant_SideScrolling",
			"UE_MetaFacial/Variant_SideScrolling/AI",
			"UE_MetaFacial/Variant_SideScrolling/Gameplay",
			"UE_MetaFacial/Variant_SideScrolling/Interfaces",
			"UE_MetaFacial/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

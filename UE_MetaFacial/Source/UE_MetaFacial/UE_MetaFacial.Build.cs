// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE_MetaFacial : ModuleRules
{
	public UE_MetaFacial(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core", "CoreUObject", "Engine", 
			"LiveLink", "LiveLinkInterface", "LiveLinkAnimationCore",
			"ControlRig", "AnimationBlueprintLibrary",
			"Networking", "Sockets", "HTTP", "Json", "JsonUtilities",
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "WebSockets" });
		
		PublicIncludePaths.Add(ModuleDirectory);

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

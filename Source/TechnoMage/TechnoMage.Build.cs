// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TechnoMage : ModuleRules
{
	public TechnoMage(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore",
			"UnrealEd", // Обязательно для UFactory
			"EditorSubsystem"
		});
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",         // Для Enhanced Input System
			"Niagara",               // Для работы с Niagara (эффекты)
			"GameplayTasks",         // Для работы с AI и задачами
			"AIModule",              // Для AIController и поведения
			"NavigationSystem",       // Для NavMesh и навигации
			"UMG",
			"Slate",
			"SlateCore",
            "EditorScriptingUtilities",
            "GameplayCameras"
        });
    }
}
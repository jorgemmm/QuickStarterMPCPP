// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class QucikStarter_MP_CPPEditorTarget : TargetRules
{
	public QucikStarter_MP_CPPEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("QucikStarter_MP_CPP");
	}
}

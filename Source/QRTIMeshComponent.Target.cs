// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class QRTIMeshComponentTarget : TargetRules
{
	public QRTIMeshComponentTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "QRTIMeshComponent" } );
	}
}

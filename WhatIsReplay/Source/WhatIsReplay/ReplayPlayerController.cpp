// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplayPlayerController.h"

#include "Engine/DemoNetDriver.h"

void AReplayPlayerController::ReStartRecording()
{
	if(UWorld* World = GetWorld())
	{
		if(UDemoNetDriver* DemoNetDriver = World->DemoNetDriver)
		{
			DemoNetDriver->GotoTimeInSeconds(0.f);
		}
	}
}

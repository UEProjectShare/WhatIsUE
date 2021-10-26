// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplayGameInstance.h"

UReplayGameInstance::UReplayGameInstance()
{
	RecordingName = "MyReplay";
	FriendlyRecordingName = "My Replay";
}

void UReplayGameInstance::StartRecording()
{
	StartRecordingReplay(RecordingName, FriendlyRecordingName);
}

void UReplayGameInstance::StopRecording()
{
	StopRecordingReplay();
}

void UReplayGameInstance::StartReplay()
{
	PlayReplay(RecordingName, nullptr);
}

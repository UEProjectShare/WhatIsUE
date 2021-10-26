// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ReplayGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WHATISREPLAY_API UReplayGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UReplayGameInstance();
	
	UPROPERTY(EditDefaultsOnly, Category = "Replay")
	FString RecordingName;

	UPROPERTY(EditDefaultsOnly, Category = "Replay")
	FString FriendlyRecordingName;

	UFUNCTION(BlueprintCallable, Category = "Replay")
	void StartRecording();

	UFUNCTION(BlueprintCallable, Category = "Replay")
	void StopRecording();

	UFUNCTION(BlueprintCallable, Category = "Replay")
	void StartReplay();
};

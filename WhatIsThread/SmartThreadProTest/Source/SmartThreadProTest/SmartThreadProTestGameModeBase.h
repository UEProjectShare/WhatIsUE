// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SmartThreadProTestGameModeBase.generated.h"

UCLASS()
class SMARTTHREADPROTEST_API ASmartThreadProTestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	//测试 UObject
	UFUNCTION()
	void Test1(int32 i);
	
	//测试 UFunction
	UFUNCTION()
	void Test2(int32 i, FString Mes);
	
	UFUNCTION()
	void Do();

	UFUNCTION()
	void Do2();
	
	UFUNCTION()
	void Run();
	
	UFUNCTION()
	void OK();
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;

	FTimerHandle Handle;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FSoftObjectPath> ObjectPath;
};

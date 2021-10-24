// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WhatIsJSONGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class WHATISJSON_API AWhatIsJSONGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};

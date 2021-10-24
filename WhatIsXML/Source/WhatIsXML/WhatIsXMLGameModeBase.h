// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WhatIsXMLGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class WHATISXML_API AWhatIsXMLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};

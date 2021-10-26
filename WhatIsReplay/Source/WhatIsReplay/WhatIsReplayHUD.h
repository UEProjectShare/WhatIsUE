// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WhatIsReplayHUD.generated.h"

UCLASS()
class AWhatIsReplayHUD : public AHUD
{
	GENERATED_BODY()

public:
	AWhatIsReplayHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};


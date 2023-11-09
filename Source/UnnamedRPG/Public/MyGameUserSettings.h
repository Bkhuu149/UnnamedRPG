// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "MyGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class UNNAMEDRPG_API UMyGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

protected:
	UPROPERTY(Config)
	float MouseSensitivity = 50.f;
public:
	UFUNCTION(BlueprintCallable)
	void SetMouseSensitivity(float NewSensitivity) { MouseSensitivity = NewSensitivity; }
};

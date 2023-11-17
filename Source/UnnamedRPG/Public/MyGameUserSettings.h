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
	float MouseSensitivity = 1.f;
	UPROPERTY(Config)
		float MasterVolume = 1.f;
	UPROPERTY(Config)
		float SoundEffectVolume = 1.f;
	UPROPERTY(Config)
		float CharacterVolume = 1.f;
	UPROPERTY(Config)
		float BGMVolume = 1.f;



public:

	UFUNCTION(BlueprintCallable)
		static UMyGameUserSettings* GetMyGameGameUserSettings() { return Cast<UMyGameUserSettings>(UGameUserSettings::GetGameUserSettings()); }

	UFUNCTION(BlueprintCallable)
	void SetMouseSensitivity(float NewSensitivity) { MouseSensitivity = FMath::Clamp(NewSensitivity, 0.01, 5.00f); }

	UFUNCTION(BlueprintCallable)
	void SetMasterVolume(float NewVolume) { MasterVolume = FMath::Clamp(NewVolume, 0.0, 1.f); }

	UFUNCTION(BlueprintCallable)
	void SetSoundEffectVolume(float NewVolume) { SoundEffectVolume = FMath::Clamp(NewVolume, 0.0, 1.f); }	
};

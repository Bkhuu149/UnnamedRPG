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
	UPROPERTY(Config)
		int FOV = 90;



public:

	UFUNCTION(BlueprintCallable)
		static UMyGameUserSettings* GetMyGameGameUserSettings() { return Cast<UMyGameUserSettings>(UGameUserSettings::GetGameUserSettings()); }

	UFUNCTION(BlueprintCallable, Category = "Game Settings")
		void SetMouseSensitivityValue(float NewSensitivity) { MouseSensitivity = FMath::Clamp(NewSensitivity, 0.01, 5.00f); }

	UFUNCTION(BlueprintCallable, Category = "Game Settings")
		float GetMouseSensitivityValue() { return MouseSensitivity; }

	UFUNCTION(BlueprintCallable, Category = "Audio Settings")
		void SetMasterVolumeValue(float NewVolume) { MasterVolume = FMath::Clamp(NewVolume, 0.0, 1.f); }

	UFUNCTION(BlueprintCallable, Category = "Audio Settings")
		float GetMasterVolumeValue() { return MasterVolume; }

	UFUNCTION(BlueprintCallable, Category = "Audio Settings")
		void SetSoundEffectVolumeValue(float NewVolume) { SoundEffectVolume = FMath::Clamp(NewVolume, 0.0, 1.f); }	

	UFUNCTION(BlueprintCallable, Category = "Audio Settings")
		float GetSoundEffectVolumeValue() { return SoundEffectVolume; }

	UFUNCTION(BlueprintCallable, Category = "Audio Settings")
		void SetCharacterVolumeValue(float NewVolume) { CharacterVolume = FMath::Clamp(NewVolume, 0.0, 1.f); }

	UFUNCTION(BlueprintCallable, Category = "Audio Settings")
		float GetCharacterVolumeValue() { return CharacterVolume; }

	UFUNCTION(BlueprintCallable, Category = "Audio Settings")
		void SetBGMVolumeValue(float NewVolume) { BGMVolume = FMath::Clamp(NewVolume, 0.0, 1.f); }

	UFUNCTION(BlueprintCallable, Category = "Audio Settings")
		float GetBGMVolumeValue() { return BGMVolume; }

	UFUNCTION(BlueprintCallable, Category = "Game Settings")
		void SetFOVValue(int NewValue) { FOV = FMath::Clamp(NewValue, 5, 170); }

	UFUNCTION(BlueprintCallable, Category = "GameSettings")
		int GetFOVValue() { return FOV; }

};

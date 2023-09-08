// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExperienceComponent.generated.h"

class AMyRPGCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNNAMEDRPG_API UExperienceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UExperienceComponent();

private:
	AMyRPGCharacter* Player;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		int StoredXP = 0;
	
	UPROPERTY(EditAnywhere)
	TMap<FName, int> PlayerStatMap;

	float HealthUpgradeAmount = 5.f;
	float ManaUpgradeAmount = 5.f;
	float StaminaUpgradeAmount = 5.f;
	float StrengthUpgradeAmount = 0.05f;
	float DefenseUpgradeAmount = 0.05f;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void GiveXP(int Xp);

	UFUNCTION(BlueprintCallable)
		void RemoveXP(int Xp);

	UFUNCTION(BlueprintCallable)
		int GetStoredXP() { return StoredXP; }

	UFUNCTION(BlueprintCallable)
		int GetStatLevel(FName StatName) { return PlayerStatMap[StatName]; }
	
	UFUNCTION(BlueprintCallable)
	float GetHealthUpgradeAmount() { return HealthUpgradeAmount; }
	UFUNCTION(BlueprintCallable)
	float GetManaUpgradeAmount() { return ManaUpgradeAmount; }
	UFUNCTION(BlueprintCallable)
	float GetStaminaUpgradeAmount() { return StaminaUpgradeAmount; }
	UFUNCTION(BlueprintCallable)
	float GetStrengthUpgradeAmount() { return StrengthUpgradeAmount; }
	UFUNCTION(BlueprintCallable)
	float GetDefenseUpgradeAmount() { return DefenseUpgradeAmount; }

	UFUNCTION(BlueprintCallable)
	int GetCostByLevel(int Level);

	UFUNCTION(BlueprintCallable)
		void GetStatUpgradeInformation(FName StatName, int& CurrentLevel, int& NewLevel, int& Cost, int& NewExperience);

	void UpgradeMaxHealth();
	void UpgradeMaxMana();
	void UpgradeMaxStamina();
	void UpgradeBaseDefense();
	void UpgradeBaseDamage();
	
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateExperienceAmmount();
};

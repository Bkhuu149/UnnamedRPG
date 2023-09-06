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

	int GetCostByLevel(int Level);

	UPROPERTY(EditAnywhere)
		int StoredXP = 0;
	
	UPROPERTY(EditAnywhere)
	TMap<FName, int> PlayerStatMap;

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

	void UpgradeMaxHealth();
	void UpgradeMaxMana();
	void UpgradeMaxStamina();
	void UpgradeBaseDefense();
	void UpgradeBaseDamage();
	
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateExperienceAmmount();
};

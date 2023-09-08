// Fill out your copyright notice in the Description page of Project Settings.


#include "ExperienceComponent.h"
#include "../MyRPGCharacter.h"

// Sets default values for this component's properties
UExperienceComponent::UExperienceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//default values
	Player = Cast<AMyRPGCharacter>(GetOwner());
	PlayerStatMap.Add(FName("Health"), 0);
	PlayerStatMap.Add(FName("Mana"), 1);
	PlayerStatMap.Add(FName("Stamina"), 2);
	PlayerStatMap.Add(FName("Defense"), 3);
	PlayerStatMap.Add(FName("Damage"), 4);
	StoredXP = 100000;
}


// Called when the game starts
void UExperienceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UExperienceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UExperienceComponent::GiveXP(int Xp) {
	StoredXP += Xp;
	Player->UpdateExperienceAmmount();
}

void UExperienceComponent::RemoveXP(int Xp) {
	if (StoredXP - Xp <= 0) {
		StoredXP = 0;
		UpdateExperienceAmmount();
		return;
	}
	StoredXP -= Xp;
	UpdateExperienceAmmount();
}

int UExperienceComponent::GetCostByLevel(int Level) {
	return FMath::CeilToInt(FMath::Pow(((Level+1)*50), 1.5f));
}

void UExperienceComponent::UpgradeMaxHealth() {
	//If stat level is already 20 which is max or player doesn't have enough xp to level stat
	int StatLevel = PlayerStatMap[FName("Health")];
	int StatCost = GetCostByLevel(PlayerStatMap[FName("Health")]);
	if (StatLevel >= 20 || StoredXP < StatCost) { return; }
	RemoveXP(StatCost);
	PlayerStatMap[FName("Health")]++;
	Player->SetHealthMax(Player->GetHealthMax() + HealthUpgradeAmount);
	Player->UpdateHealhBar();
}

void UExperienceComponent::UpgradeMaxMana() {
	//If stat level is already 20 which is max or player doesn't have enough xp to level stat

	int StatLevel = PlayerStatMap[FName("Mana")];
	int StatCost = GetCostByLevel(PlayerStatMap[FName("Mana")]);
	if (StatLevel >= 20 || StoredXP < StatCost) { return; }
	PlayerStatMap[FName("Mana")]++;
	Player->SetManaMax(Player->GetManaMax() + ManaUpgradeAmount);
	Player->UpdateManaBar();
}

void UExperienceComponent::UpgradeMaxStamina() {
	//If stat level is already 20 which is max or player doesn't have enough xp to level stat
	int StatLevel = PlayerStatMap[FName("Stamina")];
	int StatCost = GetCostByLevel(PlayerStatMap[FName("Stamina")]);
	if (StatLevel >= 20 || StoredXP < StatCost) { return; }
	PlayerStatMap[FName("Stamina")]++;
	Player->SetStaminaMax(Player->GetStaminaMax() + StaminaUpgradeAmount);
	Player->UpdateStaminaBar();
}

void UExperienceComponent::UpgradeBaseDefense() {

}

void UExperienceComponent::UpgradeBaseDamage() {

}

void UExperienceComponent::GetStatUpgradeInformation(FName StatName, int& CurrentLevel, int& NewLevel, int& Cost, int& NewExperience) {
	CurrentLevel = PlayerStatMap[StatName];
	NewLevel = CurrentLevel + 1;
	Cost = GetCostByLevel(CurrentLevel);
	NewExperience = StoredXP - Cost;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "ExperienceComponent.h"

// Sets default values for this component's properties
UExperienceComponent::UExperienceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
}

void UExperienceComponent::RemoveXP(int Xp) {

}

int UExperienceComponent::GetCostByLevel(int Level) {
	return FMath::Floor(FMath::Pow(((Level+1)*50), 1.5f));
}

void UExperienceComponent::UpgradeMaxHealth() {

}

void UExperienceComponent::UpgradeMaxMana() {

}

void UExperienceComponent::UpgradeMaxStamina() {

}

void UExperienceComponent::UpgradeBaseDefense() {

}

void UExperienceComponent::UpgradeBaseDamage() {

}

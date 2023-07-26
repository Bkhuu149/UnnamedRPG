// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusComponent.h"

// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickInterval(1.f);
	StatusEffects.Add("TestEffect", 10);
}


// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DecrimentEffects();
	// ...
}

void UStatusComponent::DecrimentEffects() {
	for (TPair<FName, int>& Effect : StatusEffects) {
		Effect.Value--;
	}
	RemoveFinishedEffects();
}

void UStatusComponent::RemoveFinishedEffects() {
	TArray<FName> Keys;
	StatusEffects.GetKeys(Keys);
	for (FName Key : Keys) {
		if (StatusEffects[Key] <= 0) {
			StatusEffects.Remove(Key);
		}
	}
}

void UStatusComponent::RemoveEffect(FName Effect) {
	//For use outside of class
	StatusEffects.Remove(Effect);
}

void UStatusComponent::AddItemEffect(FName ItemEffect, int Time) {
	//For use outside of class
	StatusEffects.Add(ItemEffect, Time);
}

void UStatusComponent::AddDebuff(EDamageType Type, float Damage) {
	int DebuffTime = CalculateEffectBuildupFromDamage(Damage);
	FName DesiredDebuff;
	switch (Type) {
	case EDamageType::NONE:
		DesiredDebuff = FName("Elementless");
		break;
	case EDamageType::FIRE:
		DesiredDebuff = FName("Fire");
		break;
	case EDamageType::WATER:
		DesiredDebuff = FName("Water");
		break;
	case EDamageType::EARTH:
		DesiredDebuff = FName("Earth");
		break;
	case EDamageType::WIND:
		DesiredDebuff = FName("Wind");
		break;
	case EDamageType::ICE:
		DesiredDebuff = FName("Ice");
		break;
	case EDamageType::LIGHTNING:
		DesiredDebuff = FName("Lightning");
		break;
	case EDamageType::SAND:
		DesiredDebuff = FName("Sand");
		break;
	case EDamageType::SMOKE:
		DesiredDebuff = FName("Smoke");
		break;
	}
	if (StatusEffects.Contains(DesiredDebuff)) {
		//Player already had some buildup in debuff, add more
		StatusEffects[DesiredDebuff] += DebuffTime;
		return;
	}
	//Player doesn't have any buildup for this debuff, create buildup
	StatusEffects.Add(DesiredDebuff, DebuffTime);
}

int UStatusComponent::CalculateEffectBuildupFromDamage(float Damage) {
	//Returns a temporary value, function to be made to calculate damage later
	return FMath::CeilToInt(Damage/2);
}

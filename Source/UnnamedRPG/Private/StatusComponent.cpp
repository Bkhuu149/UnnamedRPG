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
	StatusEffects.Add(EStatus::BURN, 10);
}


// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DecrimentEffects();
	// ...
}

void UStatusComponent::DecrimentEffects() {
	for (TPair<EStatus, int>& Effect : StatusEffects) {
		Effect.Value--;
	}
	RemoveFinishedEffects();
}

void UStatusComponent::RemoveFinishedEffects() {
	TArray<EStatus> Keys;
	StatusEffects.GetKeys(Keys);
	for (EStatus Key : Keys) {
		if (StatusEffects[Key] <= 0) {
			StatusEffects.Remove(Key);
		}
	}
}

void UStatusComponent::RemoveEffect(EStatus Effect) {
	//For use outside of class
	StatusEffects.Remove(Effect);
}

void UStatusComponent::AddItemEffect(EStatus ItemEffect, int Time) {
	//For use outside of class
	StatusEffects.Add(ItemEffect, Time);
}

void UStatusComponent::AddDebuff(EDamageType Type, float Damage) {
	int DebuffTime = CalculateEffectBuildupFromDamage(Damage);
	EStatus DesiredDebuff;
	switch (Type) {
	case EDamageType::NONE:
		return;
	case EDamageType::FIRE:
		DesiredDebuff = EStatus::BURN;
		break;
	case EDamageType::WATER:
		DesiredDebuff = EStatus::WET;
		break;
	case EDamageType::EARTH:
		DesiredDebuff = EStatus::DIRT;
		break;
	case EDamageType::WIND:
		DesiredDebuff = EStatus::UNSTEADY;
		break;
	case EDamageType::ICE:
		DesiredDebuff = EStatus::SLOWED;
		break;
	case EDamageType::LIGHTNING:
		DesiredDebuff = EStatus::PARALIZED;
		break;
	case EDamageType::SAND:
		DesiredDebuff = EStatus::DUST;
		break;
	case EDamageType::SMOKE:
		DesiredDebuff = EStatus::SMOKE;
		break;
	}
	if (StatusEffects.Contains(DesiredDebuff)) {
		//Player already had some buildup in debuff, add more
		StatusEffects[DesiredDebuff] += DebuffTime;
		//If Buildup reaches thresshold, activate effect

		return;
	}
	//Player doesn't have any buildup for this debuff, create buildup
	StatusEffects.Add(DesiredDebuff, DebuffTime);
}

int UStatusComponent::CalculateEffectBuildupFromDamage(float Damage) {
	//Returns a temporary value, function to be made to calculate damage later
	return FMath::CeilToInt(Damage/2);
}

void UStatusComponent::ActivateEffect(EStatus Effect) {
	switch (Effect)
	{
	case EStatus::BURN:
		//Damage over time
		break;
	case EStatus::WET:
		//Takes more damage
		break;
	case EStatus::DIRT:
		//Mana recovers slower
		break;
	case EStatus::UNSTEADY:
		//Stamina recovers slower
		break;
	case EStatus::SLOWED:
		//movement slowed
		break;
	case EStatus::DUST:
		//Deals less damage
		break;
	case EStatus::PARALIZED:
		//Occasional stun
		break;
	case EStatus::SMOKE:
		//Temprory vision impairment for player
		break;
	case EStatus::REGEN:
		break;
	case EStatus::DAMAGEBUFF:
		//Deals more damage
		break;
	case EStatus::STAMINABUFF:
		//Actions use less stamina
		break;
	case EStatus::DEFENSEBUFF:
		//Take less damage
		break;
	default:
		break;
	}
}

void UStatusComponent::DeactivateEffect(EStatus Effect) {
	switch (Effect)
	{
	case EStatus::BURN:
		break;
	case EStatus::WET:
		break;
	case EStatus::DIRT:
		break;
	case EStatus::UNSTEADY:
		break;
	case EStatus::SLOWED:
		break;
	case EStatus::DUST:
		break;
	case EStatus::PARALIZED:
		break;
	case EStatus::SMOKE:
		break;
	case EStatus::REGEN:
		break;
	case EStatus::DAMAGEBUFF:
		break;
	case EStatus::STAMINABUFF:
		break;
	case EStatus::DEFENSEBUFF:
		break;
	default:
		break;
	}
}

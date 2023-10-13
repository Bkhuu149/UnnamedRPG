// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusComponentBase.h"

// Sets default values for this component's properties
UStatusComponentBase::UStatusComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatusComponentBase::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickInterval(1.f);
	// ...
	
}

// Called every frame
void UStatusComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DecrimentEffects(StatusEffectBuildups);
	DecrimentEffects(ActiveStatusEffects);
	// ...
}

void UStatusComponentBase::DecrimentEffects(TMap<EStatus, int>& StatusMap) {
	for (TPair<EStatus, int>& Effect : StatusMap) {
		Effect.Value--;
	}
	RemoveFinishedEffects(StatusMap);
}

void UStatusComponentBase::RemoveFinishedEffects(TMap<EStatus, int>& StatusMap) {
	TArray<EStatus> Keys;
	StatusMap.GetKeys(Keys);
	for (EStatus Key : Keys) {
		if (StatusMap[Key] <= 0) {
			StatusMap.Remove(Key);
			DeactivateEffect(Key);
		}
	}
}

void UStatusComponentBase::RemoveEffect(EStatus Effect) {
	//Removes all data of the effect given
	StatusEffectBuildups.Remove(Effect);
	ActiveStatusEffects.Remove(Effect);
	DeactivateEffect(Effect);

}

int UStatusComponentBase::CalculateEffectBuildupFromDamage(float Damage) {
	//Returns a temporary value, function to be made to calculate damage later
	//return FMath::CeilToInt(Damage * 2);
	return 100;
}

void UStatusComponentBase::AddDebuff(EDamageType Type, float Damage) {
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
		DesiredDebuff = EStatus::HEAVY;
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

	default:
		return;
	}

	if (ActiveStatusEffects.Contains(DesiredDebuff)) {
		//Debuff already active, increase time for debuff but keep it below threshold
		ActiveStatusEffects[DesiredDebuff] = FMath::Clamp(ActiveStatusEffects[DesiredDebuff] + DebuffTime, 0, 100);
		return;
	}
	else if (StatusEffectBuildups.Contains(DesiredDebuff)) {
		//Debuff is not active but player has some buildup for it
		int NewDebuffTime = FMath::Clamp(StatusEffectBuildups[DesiredDebuff] + DebuffTime, 0, 100);
		StatusEffectBuildups.Add(DesiredDebuff, NewDebuffTime);
		//StatusEffectBuildups[DesiredDebuff] = FMath::Clamp(NewDebuffTime, 0, 100);
		//If Buildup reaches thresshold, activate effect
		if (NewDebuffTime >= 99) {
			ActivateEffect(DesiredDebuff);
		}
		return;
	}
	//Player doesn't have any buildup for this debuff, create buildup
	StatusEffectBuildups.Add(DesiredDebuff, DebuffTime);
}

int UStatusComponentBase::GetStatusTimeRemaining(EStatus Effect) {
	if (ActiveStatusEffects.Contains(Effect)) {
		return ActiveStatusEffects[Effect];
	}
	return 0;
}

TArray<EStatus> UStatusComponentBase::GetActiveStatuses() {
	TArray<EStatus> Statuses;
	ActiveStatusEffects.ValueSort([](int A, int B) {
		return A < B;
		});
	ActiveStatusEffects.GenerateKeyArray(Statuses);
	return Statuses;
}
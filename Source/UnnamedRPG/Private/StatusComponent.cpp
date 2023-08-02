// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusComponent.h"
#include "../MyRPGCharacter.h"


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
	Player = Cast<AMyRPGCharacter>(GetOwner());

	//ActiveStatusEffects.Add(EStatus::BURN, 100);
}


// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DecrimentEffects(StatusEffectBuildups);
	//Perform Active Status Effects;
	if (ActiveStatusEffects.Contains(EStatus::BURN)) {
		//Do FireTickDamage
		Player->DoFireTickDamage();
	}
	DecrimentEffects(ActiveStatusEffects);
	// ...
}

void UStatusComponent::DecrimentEffects(TMap<EStatus, int>& StatusMap) {
	for (TPair<EStatus, int>& Effect : StatusMap) {
		Effect.Value--;
	}
	RemoveFinishedEffects(StatusMap);
}

void UStatusComponent::RemoveFinishedEffects(TMap<EStatus, int>& StatusMap) {
	TArray<EStatus> Keys;
	StatusMap.GetKeys(Keys);
	for (EStatus Key : Keys) {
		if (StatusMap[Key] <= 0) {
			StatusMap.Remove(Key);
			DeactivateEffect(Key);
		}
	}
}

void UStatusComponent::RemoveEffect(EStatus Effect) {
	//Removes all data of the effect given
	StatusEffectBuildups.Remove(Effect);
	ActiveStatusEffects.Remove(Effect);

}

void UStatusComponent::AddItemEffect(EStatus ItemEffect, int Time) {
	//For use outside of class
	ActiveStatusEffects.Add(ItemEffect, Time);
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
		DesiredDebuff = EStatus::SLOWED;
		break;
	case EDamageType::EARTH:
		DesiredDebuff = EStatus::DIRT;
		break;
	case EDamageType::WIND:
		DesiredDebuff = EStatus::UNSTEADY;
		break;
	case EDamageType::ICE:
		DesiredDebuff = EStatus::WET;
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

int UStatusComponent::CalculateEffectBuildupFromDamage(float Damage) {
	//Returns a temporary value, function to be made to calculate damage later
	//return FMath::CeilToInt(Damage/2);
	return 25;
}

void UStatusComponent::ActivateEffect(EStatus Effect) {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Activate Effect %s"), *UEnum::GetValueAsString(Effect)));

	//Move Effect from Buildup map to Active Effects map
	TPair<EStatus, int> ActivatedEffect = TPair<EStatus, int>(Effect, StatusEffectBuildups[Effect]);
	StatusEffectBuildups.Remove(Effect);
	ActiveStatusEffects.Add(ActivatedEffect);


	switch (Effect)
	{
	case EStatus::BURN:
		//Damage over time
		break;
	case EStatus::WET:
		//Player has slippery floor
		Player->GetCharacterMovement()->GroundFriction = 0;
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
		Player->GetCharacterMovement()->GroundFriction = 100;
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
		Player->ResetAttackDamageMultiplier();
		break;
	case EStatus::STAMINABUFF:
		Player->ResetStaminaDrainMultiplier();
		break;
	case EStatus::DEFENSEBUFF:
		Player->ResetEnemyDamageMultiplier();
		break;
	default:
		break;
	}
}

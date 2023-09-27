// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStatusComponent.h"
#include "EnemyClass.h"

// Sets default values for this component's properties
UEnemyStatusComponent::UEnemyStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Enemy = Cast<AEnemyClass>(GetOwner());
	// ...
}


// Called when the game starts
void UEnemyStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickInterval(1.f);
	// ...
	
}


// Called every frame
void UEnemyStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DecrimentEffects(StatusEffectBuildups);
	//Perform Active Status Effects;
	if (ActiveStatusEffects.Contains(EStatus::BURN)) {
		//Do FireTickDamage
		Enemy->DoFireTickDamage();
	}
	DecrimentEffects(ActiveStatusEffects);
	// ...
}


void UEnemyStatusComponent::DecrimentEffects(TMap<EStatus, int>& StatusMap) {
	for (TPair<EStatus, int>& Effect : StatusMap) {
		Effect.Value--;
	}
	RemoveFinishedEffects(StatusMap);
}

void UEnemyStatusComponent::RemoveFinishedEffects(TMap<EStatus, int>& StatusMap) {
	TArray<EStatus> Keys;
	StatusMap.GetKeys(Keys);
	for (EStatus Key : Keys) {
		if (StatusMap[Key] <= 0) {
			StatusMap.Remove(Key);
			DeactivateEffect(Key);
		}
	}
}

void UEnemyStatusComponent::RemoveEffect(EStatus Effect) {
	StatusEffectBuildups.Remove(Effect);
	ActiveStatusEffects.Remove(Effect);
	DeactivateEffect(Effect);
}

void UEnemyStatusComponent::AddDebuff(EDamageType Type, float Damage) {
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

int UEnemyStatusComponent::CalculateEffectBuildupFromDamage(float Damage) {
	//Returns a temporary value, function to be made to calculate damage later
	//return FMath::CeilToInt(Damage * 2);
	return 100;
}

void UEnemyStatusComponent::ActivateEffect(EStatus Effect) {
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
		//Player has decreased mana recovery
		//Enemy can't perform ranged attack
		//Change this
		Enemy->SetCanAttackRange(false);
		break;
	case EStatus::HEAVY:
		//Player can't jump
		//Change this
		break;
	case EStatus::UNSTEADY:
		//Stamina recovers slower
		//Change this
		break;
	case EStatus::SLOWED:
		//movement slowed
		Enemy->SetSpeedMultiplier(.7);
		Enemy->GetCharacterMovement()->MaxWalkSpeed = Enemy->CalculateNewSpeed();
		break;
	case EStatus::DUST:
		//Deals less damage
		break;
	case EStatus::PARALIZED:
		//Occasional stun
		break;
	case EStatus::SMOKE:
		break;
	default:
		break;
	}
}

void UEnemyStatusComponent::DeactivateEffect(EStatus Effect) {
	switch (Effect)
	{
	case EStatus::BURN:
		break;
	case EStatus::WET:
		Enemy->SetCanAttackRange(true);
		break;
	case EStatus::HEAVY:
		break;
	case EStatus::UNSTEADY:
		break;
	case EStatus::SLOWED:
		Enemy->SetSpeedMultiplier(1);
		Enemy->GetCharacterMovement()->MaxWalkSpeed = Enemy->CalculateNewSpeed();
		break;
	case EStatus::DUST:
		break;
	case EStatus::PARALIZED:
		break;
	case EStatus::SMOKE:
		break;
	case EStatus::REGEN:
		break;
	default:
		break;
	}
}


int UEnemyStatusComponent::GetStatusTimeRemaining(EStatus Effect) {
	if (ActiveStatusEffects.Contains(Effect)) {
		return ActiveStatusEffects[Effect];
	}
	return 0;
}
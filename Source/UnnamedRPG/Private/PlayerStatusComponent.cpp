// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatusComponent.h"
#include "../MyRPGCharacter.h"


// Sets default values for this component's properties
UPlayerStatusComponent::UPlayerStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
	Player = Cast<AMyRPGCharacter>(GetOwner());
}


// Called when the game starts
void UPlayerStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	//ActiveStatusEffects.Add(EStatus::BURN, 100);
}


// Called every frame
void UPlayerStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//Perform Active Status Effects;
	if (ActiveStatusEffects.Contains(EStatus::BURN)) {
		//Do FireTickDamage
		Player->DoFireTickDamage();
	}
	Player->UpdateStatusBars();
	// ...
}



void UPlayerStatusComponent::ActivateEffect(EStatus Effect) {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Activate Effect %s"), *UEnum::GetValueAsString(Effect)));

	//Move Effect from Buildup map to Active Effects map
	TPair<EStatus, int> ActivatedEffect = TPair<EStatus, int>(Effect, StatusEffectBuildups[Effect]);
	StatusEffectBuildups.Remove(Effect);
	ActiveStatusEffects.Add(ActivatedEffect);
	Player->AddStatus(Effect);

	switch (Effect)
	{
	case EStatus::BURN:
		//Damage over time
		break;
	case EStatus::WET:
		//Player has decreased mana recovery
		Player->SetManaRestoreMultiplier(.5);
		break;
	case EStatus::HEAVY:
		//Player can't jump
		Player->SetCanJump(false);
		break;
	case EStatus::UNSTEADY:
		//Stamina recovers slower
		Player->SetStaminaRestoreMultiplier(.5);
		break;
	case EStatus::SLOWED:
		//movement slowed
		Player->GetCharacterMovement()->MaxWalkSpeed = 500;
		break;
	case EStatus::DUST:
		//Deals less damage
		Player->SetAttackDebuffMultiplier(.5);
		break;
	case EStatus::PARALIZED:
		//Occasional stun
		Player->StartParalysis();
		break;
	case EStatus::SMOKE:
		//Temprory vision impairment for player
		Player->SetBlindness(true);
		Player->ResetTarget();
		break;
	default:
		break;
	}
}

void UPlayerStatusComponent::DeactivateEffect(EStatus Effect) {

	switch (Effect)
	{
	case EStatus::BURN:
		break;
	case EStatus::WET:
		Player->ResetManaRestoreMultiplier();
		break;
	case EStatus::HEAVY:
		Player->SetCanJump(true);
		break;
	case EStatus::UNSTEADY:
		Player->ResetStaminaRestoreMultiplier();
		break;
	case EStatus::SLOWED:
		Player->GetCharacterMovement()->MaxWalkSpeed = 1000;
		break;
	case EStatus::DUST:
		Player->ResetAttackDebuffMultiplier();
		break;
	case EStatus::PARALIZED:
		Player->EndParalysis();
		break;
	case EStatus::SMOKE:
		Player->SetBlindness(false);
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

void UPlayerStatusComponent::AddItemEffect(EStatus ItemEffect, int Time) {
	//For use outside of class
	ActiveStatusEffects.Add(ItemEffect, Time);
	Player->AddStatus(ItemEffect);
}

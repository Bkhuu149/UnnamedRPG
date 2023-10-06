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
	// ...
	
}


// Called every frame
void UEnemyStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//Perform Active Status Effects;
	if (ActiveStatusEffects.Contains(EStatus::BURN)) {
		//Do FireTickDamage
		Enemy->DoFireTickDamage();
	}
	// ...
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
		//Enemy can't perform ranged attack
		Enemy->SetCanAttackRange(false);
		break;
	case EStatus::HEAVY:
		//Make Attack anims slower
		Enemy->SetCurrentAttackSpeed(Enemy->GetInitialAttackSpeed()*.7);
		break;
	case EStatus::UNSTEADY:
		//Makes enemy attack cooldown longer
		Enemy->SetCurrentCooldownTime(Enemy->GetInitialCooldownTime() * 1.5);
		break;
	case EStatus::SLOWED:
		//movement slowed
		Enemy->SetSpeedMultiplier(.7);
		Enemy->GetCharacterMovement()->MaxWalkSpeed = Enemy->CalculateNewSpeed();
		break;
	case EStatus::DUST:
		//Deals less damage
		Enemy->SetCurrentDamage(Enemy->GetInitialDamage() * 0.7f);
		break;
	case EStatus::PARALIZED:
		//Occasional stun
		Enemy->StartParalysis();
		break;
	case EStatus::SMOKE:
		// Player: Gets bliness and obscures the screen
		// Enemy: Before enemy attacks player enemy gets between [-60, 60] rotation 
		Enemy->SetBlindness(true);
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
		Enemy->SetCurrentAttackSpeed(Enemy->GetInitialAttackSpeed());
		break;
	case EStatus::UNSTEADY:
		Enemy->ResetCooldownTime();
		break;
	case EStatus::SLOWED:
		Enemy->SetSpeedMultiplier(1);
		Enemy->GetCharacterMovement()->MaxWalkSpeed = Enemy->CalculateNewSpeed();
		break;
	case EStatus::DUST:
		Enemy->SetCurrentDamage(Enemy->GetInitialDamage());
		break;
	case EStatus::PARALIZED:
		Enemy->EndParalysis();
		break;
	case EStatus::SMOKE:
		Enemy->SetBlindness(false);
		break;
	default:
		break;
	}
}

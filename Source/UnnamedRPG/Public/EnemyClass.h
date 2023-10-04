// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGBaseClass.h"
#include "EnemyStatusComponent.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "../MyRPGCharacter.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyEnumUtils.h"
#include "EnemyClass.generated.h"



UCLASS()
class UNNAMEDRPG_API AEnemyClass : public ARPGBaseClass
{
	GENERATED_BODY()

public: 
	// Sets default values for this character's properties
	AEnemyClass();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHealthBar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MakeEditWidget = true))
	TArray<FTransform> WalkPath;

	UPROPERTY(EditAnywhere)
	int EXPDropped = 0;

	UPROPERTY(EditAnywhere)
	int CurrentPathNode = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn Sense")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool IsMoving;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool Targeted;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	AActor* Target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<UAnimMontage*> AttackAnimClose;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<UAnimMontage*> AttackAnimFar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	UAnimMontage* StunAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float InitialCooldownTime = 5.f;
	float CurrentCooldownTime = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float InitialDamage = 10.f;
	float CurrentDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMyDamageType> MyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDamageType WeaponType = EDamageType::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle DelayTimer;
	
	UPROPERTY(EditAnywhere)
	EEnemyState CurrentEnemyState = EEnemyState::IDLE;

	UFUNCTION(BlueprintCallable)
	virtual bool DamageChar(float val, EDamageType Type) override;

	virtual void KillCharacter() override;
	
	UFUNCTION(BlueprintCallable)
	void SetState(EEnemyState NewState) { CurrentEnemyState = NewState; };

	UFUNCTION(BlueprintCallable)
		void SetStaggered(bool IsStaggered) { Staggered = IsStaggered; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		UEnemyStatusComponent* StatusComp;

	float SpeedMultiplier = 1.f;

	float EnemyWalkingSpeed = 250;
	float EnemyRunningSpeed = 500;

	bool CanAttackRanged = true;

	UPROPERTY(EditAnywhere)
	float InitialAttackSpeed = 1.f;
	float CurrentAttackSpeed = 1.f;
	
private:

	
	FTimerHandle AttackTimer;

	FTimerHandle DashTimer;

	AAIController* MyController;

	UNavigationSystemV1* NavSys;

	EPathFollowingRequestResult::Type FollowResult;

	enum WalkState { IDLE, FOLLOW, RANDOM};
	WalkState CurrWalkState = IDLE;

	bool Staggered = false;

	bool IsCoolingDown = false;

	bool IsBlind = false;

	void ResetTarget();

	void Rotate(float DeltaTime);

	virtual void Attack();

	virtual void TickStateMachine();

	virtual void StateIdle();

	virtual void StatePathWalking();

	virtual void StateChaseClose();

	virtual void StateChaseFar();

	virtual void StateAttack();

public: 
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION(BlueprintCallable)
	float CalculateNewSpeed();

	bool GetIsTargeted() { return Targeted; }

	UFUNCTION(BlueprintCallable)
	void StartDash();
	void DashTrace();
	UFUNCTION(BlueprintCallable)
	void EndDash();

	void DoFireTickDamage();

	void SetSpeedMultiplier(float NewMultiplier) { SpeedMultiplier = NewMultiplier; }

	void SetCanAttackRange(bool CanPerform) { CanAttackRanged = CanPerform; }

	float GetInitialDamage() { return InitialDamage; }
	
	void SetCurrentDamage(float NewDamage) { 
		CurrentDamage = NewDamage;
		CurrentWeapon->SetDamage(NewDamage);
	}

	void ResetDamage() {
		CurrentDamage = InitialDamage;
		CurrentWeapon->SetDamage(InitialDamage);
	}

	float GetInitialCooldownTime() { return InitialCooldownTime; }

	void SetCurrentCooldownTime(float NewTime) { CurrentCooldownTime = NewTime; }

	void ResetCooldownTime() { CurrentCooldownTime = InitialCooldownTime; }

	float GetInitialAttackSpeed() { return InitialAttackSpeed; }
	void SetCurrentAttackSpeed(float NewSpeed) { CurrentAttackSpeed = NewSpeed; }
	
	void SetBlindness(bool val) { IsBlind = val; }

	void StartParalysis();
	void TriggerStun();
	void EndParalysis();
	FTimerHandle ParalysisTimer;
};

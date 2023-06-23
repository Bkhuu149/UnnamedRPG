// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGBaseClass.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "../MyRPGCharacter.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyClass.generated.h"


UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	IDLE			UMETA(DisplayName = "Idle"), //Default, not doing anything
	PATH_WALKING	UMETA(DisplayName = "Path Walking"), //Currently following a set path
	CHASE_CLOSE		UMETA(DisplayName = "Chase Close"), //Following target closely
	CHASE_FAR		UMETA(DisplayName = "Chase Far"), //Has a target but staying a distance away
	ATTACK			UMETA(DisplayName = "Attack"), //Performing an attack
	STAGGERED			UMETA(DisplayName = "Staggered"), //Being damaged
	DEAD			UMETA(DisplayName = "Dead") //Dead
};

UCLASS()
class UNNAMEDRPG_API AEnemyClass : public ARPGBaseClass
{
	GENERATED_BODY()

public: 
	// Sets default values for this character's properties
	AEnemyClass();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MakeEditWidget = true))
	TArray<FTransform> WalkPath;

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
	FVector SpawnLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<UAnimMontage*> AttackAnimClose;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float CooldownTime = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle DelayTimer;
	
	UPROPERTY(EditAnywhere)
	EEnemyState CurrentEnemyState = EEnemyState::IDLE;

	virtual bool DamageChar(float val) override;

	
	UFUNCTION(BlueprintCallable)
	void SetState(EEnemyState NewState) { CurrentEnemyState = NewState; };

	UFUNCTION(BlueprintCallable)
		void SetStaggered(bool IsStaggered) { Staggered = IsStaggered; }
private:

	FTimerHandle AttackTimer;

	FTimerHandle CirclingTimer;

	AAIController* MyController;

	UNavigationSystemV1* NavSys;

	EPathFollowingRequestResult::Type FollowResult;

	enum WalkState { IDLE, FOLLOW, RANDOM};
	WalkState CurrWalkState = IDLE;

	bool Staggered = false;

	bool IsCoolingDown = false;

	void Walk();

	void ResetTarget();

	void Rotate(float DeltaTime);

	virtual void Attack();

	virtual void TickStateMachine();

	virtual void StateIdle();

	virtual void StatePathWalking();

	virtual void StateChaseClose();

	virtual void StateChaseFar();

	virtual void StateAttack();

	virtual void StateStaggered();

	virtual void StateDead();


public: 
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetIsTargeted() { return Targeted; }


};

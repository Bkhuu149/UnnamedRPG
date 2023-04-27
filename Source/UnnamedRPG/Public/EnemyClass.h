// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGBaseClass.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "../MyRPGCharacter.h"
#include "NavigationSystem.h"
#include "EnemyClass.generated.h"

UCLASS()
class UNNAMEDRPG_API AEnemyClass : public ARPGBaseClass
{
	GENERATED_BODY()

	//UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	//class USphereComponent* TriggerSphere;

public: 
	// Sets default values for this character's properties
	AEnemyClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MakeEditWidget = true))
	TArray<FTransform> WalkPath;

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
	float Cooldown = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle DelayTimer;
private:

	FTimerHandle AttackTimer;

	FTimerHandle CirclingTimer;

	AAIController* MyController;

	UNavigationSystemV1* NavSys;

	EPathFollowingRequestResult::Type FollowResult;

	enum WalkState { IDLE, FOLLOW, RANDOM};
	WalkState CurrWalkState = IDLE;

	bool CoolingDown = false;

	void Walk();

	void ResetTarget();

	void Attack();

	void Rotate(float DeltaTime);

public: 
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetIsTargeted() { return Targeted; }

	//UFUNCTION()
	//void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};

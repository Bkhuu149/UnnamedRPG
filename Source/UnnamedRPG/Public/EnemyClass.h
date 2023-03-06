// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGBaseClass.h"
#include "AIController.h"
#include "../MyRPGCharacter.h"
#include "EnemyClass.generated.h"

UCLASS()
class UNNAMEDRPG_API AEnemyClass : public ARPGBaseClass
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	class USphereComponent* TriggerSphere;

public: 
	// Sets default values for this character's properties
	AEnemyClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool IsMoving;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool Targeted;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	AActor* Target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	FVector SpawnLocation;
private:
	FTimerHandle DelayTimer;

	AAIController* MyController;

	void Walk();

	void ResetTarget();

public: 
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};

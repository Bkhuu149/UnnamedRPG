// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Engine/GameEngine.h"
#include "MyRPGCharacter.generated.h"

UCLASS()
class UNNAMEDRPG_API AMyRPGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyRPGCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool Targeted;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		AActor* Target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool IsJumping = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool IsBlocking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool IsDodging = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool IsSprinting = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool IsDead = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int AttackCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool IsInteracting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool InMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool InInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float Health = 100.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float HealthMax = 100.0;
private:

	//Handle Jump Input
	void OnJumpedPressed();
	FTimerHandle JumpTimer;
	void ReleaseJump();

	//Handle Target Pressed
	void OnTargetPressed();
	void FocusTarget();
	void ResetTarget();

	//Handle Block Inputs
	void OnBlockPressed();
	void OnBlockReleased();
	int64 PressedTime = 0;
	const int PARRY_THRESH = 2000;

	//Handle Healing Inputs
	void OnHealPressed();
	float Mana = 1000;

	//Handle Dodge Input
	void OnDodgePressed();

	//Handle Sprint Input
	void OnSprintPressed();
	void OnSprintReleased();
	float SprintMultiplier = .5f;

	//Handle Attack Pressed
	void OnAttackPressed();
	int CurrentMaxAttackCount = 3;
	void ResetAttack();
	FTimerHandle AttackTimer; 

	//Handle Finisher Pressed
	void DoFinisher();

	//Handle Interact Pressed
	void OnInteractPressed();
	
	//Handle Menu Pressed
	void OnMenuPressed();

	//Handle Inventory Pressed
	void OnInventoryPressed();

	//Handle Health
	void DamageChar(float val);
	void HealChar(float val);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void MoveForwardBack(float value);

	void MoveRightLeft(float value);
	
	bool GetIsBlocking() { return IsBlocking; }

	bool GetIsDodging() { return IsDodging; }

	bool GetIsSprinting() { return IsSprinting; }

	int GetAttackCount() { return AttackCount; }

	float GetHealth() { return Health; }

	bool GetIsInteracting() { return IsInteracting; }

	bool GetIsInMenu() { return InMenu; }

	bool GetIsInInventory() { return InInventory; }

	bool GetIsJumping() { return IsJumping; }

	bool GetIsTargeted() { return Targeted; }

	bool GetIsDead() { return IsDead; }
};


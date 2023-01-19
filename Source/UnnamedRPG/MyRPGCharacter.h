// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

private:

	//Handle Jump Input
	void OnJumpedPressed();
	bool IsJumping = false;
	FTimerHandle* JumpTimer;
	void ReleaseJump();

	//Handle Target Pressed
	void OnTargetPressed();
	bool Targeted;
	AActor* Target;

	//Handle Block Inputs
	void OnBlockPressed();
	void OnBlockReleased();
	bool IsBlocking = false;
	int64 PressedTime = 0;
	const int PARRY_THRESH = 1000;

	//Handle Healing Inputs
	void OnHealPressed();
	float Mana = 1000;

	//Handle Dodge Input
	void OnDodgePressed();
	bool IsDodging = false;

	//Handle Sprint Input
	void OnSprintPressed();
	void OnSprintReleased();
	bool IsSprinting = false;
	float SprintMultiplier = .5f;

	//Handle Attack Pressed
	void OnAttackPressed();
	int AttackCount = 0;
	void ResetAttack();
	FTimerHandle* AttackTimer; 

	//Handle Finisher Pressed
	void OnFinisherPressed();

	//Handle Interact Pressed
	void OnInteractPressed();
	bool IsInteracting;
	
	//Handle Menu Pressed
	void OnMenuPressed();
	bool InMenu;

	//Handle Inventory Pressed
	void OnInventoryPressed();
	bool InInventory;

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

	bool GetIsInteracting() { return IsInteracting; }

	bool GetIsInMenu() { return InMenu; }

	bool GetIsInInventory() { return InInventory; }

	bool GetIsJumping() { return IsJumping; }

	bool GetIsTargeted() { return Targeted; }
};


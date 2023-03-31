// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CollisionQueryParams.h"
#include "Animation/AnimMontage.h"
#include "Public/RPGBaseClass.h"

#include "Abilities/GameplayAbility_Montage.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "AbilitySystemComponent.h"
#include "Public/MyPushComponent.h"

#include "Engine/GameEngine.h"
#include "Engine/DataTable.h"
#include "MyRPGCharacter.generated.h"


USTRUCT(BlueprintType)
struct FAttackStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Index = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UGameplayAbility_Montage> Attack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Unlocked = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsFinisher = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UClass* Weapon = nullptr;

};

class InteractableInterface;

UCLASS()
class UNNAMEDRPG_API AMyRPGCharacter : public ARPGBaseClass , public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyRPGCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		UAbilitySystemComponent* AbilityComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interacts")
		UMyPushComponent* PushComp;

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
		int AttackCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool IsInteracting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool InMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool InInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float ForwardBackInputValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float RightLeftInputValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TArray<FName> AttackCombo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FName Finisher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UClass* WeaponClass;

	//Data Tables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	UDataTable* AbilityTab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	UAnimMontage* DodgeAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	UAnimMontage* ParryAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	UAnimMontage* BlockAnim;
private:
	//TO BE REMOVED LATER, LEFT IN FOR TESTING DAMAGE
	void KILL();

	//Util values

	//Handle Jump Input
	void OnJumpedPressed();
	FTimerHandle JumpTimer;
	void ReleaseJump();

	//Handle Target Pressed
	void OnTargetPressed();
	void FocusTarget(float DeltaTime);
	void ResetTarget();

	//Handle Block Inputs
	void OnBlockPressed();
	void OnBlockReleased();
	int64 PressedTime = 0;
	const int PARRY_THRESH = 2000;

	//Handle Healing Inputs
	void OnHealPressed();
	void IncrementMana();
	void RestoreMana();
	FTimerHandle ManaTimer;
	float Mana = 100.0;
	float ManaMax = 100.0;

	//Handle Dodge Input
	void OnDodgePressed();
	void DodgeFinished();
	FTimerHandle DodgeTimer;
	bool IsBackDodging = false;

	//Handle Sprint Input
	void OnSprintPressed();
	void OnSprintReleased();
	float SprintMultiplier = .5f;

	//Handle Attack Pressed
	void OnAttackPressed();
	void WeaponLineTrace();
	void ResetAttack();
	AActor* CurrentWeapon;
	FTimerHandle AttackTimer; 
	FTimerHandle ColTimer;
	int CurrentMaxAttackCount = 3;
	bool IsAttacking = false;

	UFUNCTION(BlueprintCallable, category = "MYRPGCharacter")
	void BeginSwordEvent();
	UFUNCTION(BlueprintCallable, category = "MyRPGCharacter")
	void EndSwordEvent();


	//Handle Finisher Pressed
	void DoFinisher();

	//Handle Interact Pressed
	void OnInteractPressed();
	
	//Handle Menu Pressed
	void OnMenuPressed();

	//Handle Inventory Pressed
	void OnInventoryPressed();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Handle Health
	virtual void DamageChar(float val) override;
	virtual void HealChar(float val) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void MoveForwardBack(float value);

	void MoveRightLeft(float value);
	
	bool GetIsBlocking() { return IsBlocking; }

	bool GetIsDodging() { return IsDodging; }

	bool GetIsSprinting() { return IsSprinting; }

	int GetAttackCount() { return AttackCount; }

	void SetIsInteracting(bool value) { IsInteracting = value; }
	bool GetIsInteracting() { return IsInteracting; }

	bool GetIsInMenu() { return InMenu; }

	bool GetIsInInventory() { return InInventory; }

	bool GetIsJumping() { return IsJumping; }

	bool GetIsTargeted() { return Targeted; }

	UFUNCTION(BlueprintCallable, Category = "Character Information")
	float GetMana() { return Mana; }

	UFUNCTION(BlueprintCallable, Category = "Character Information")
	float GetManaMax() { return ManaMax; }

	float GetForwardBackValue() { return ForwardBackInputValue; }
	
	float GetRightLeftValue() { return RightLeftInputValue; }


	UAbilitySystemComponent* GetAbilitySystemComponent() const override{ return AbilityComp; };
	UMyPushComponent* GetPushComponent() const { return PushComp; }
};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "CollisionQueryParams.h"
#include "Animation/AnimMontage.h"
#include "Public/RPGBaseClass.h"
#include "Public/InteractableInterface.h"
#include "GameplayAbilitySpec.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility_Montage.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"
#include "Public/MyInteractComponent.h"
#include "Public/InventoryComponent.h"
#include "Public/AttackSkillComponent.h"
#include "Public/StatusComponent.h"
#include "Public/ExperienceComponent.h"
#include "Public/MyEnumUtils.h"

#include "Engine/GameEngine.h"
#include "Engine/DataTable.h"
#include "MyRPGCharacter.generated.h"




class InteractableInterface;


UCLASS()
class UNNAMEDRPG_API AMyRPGCharacter : public ARPGBaseClass //, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyRPGCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		//UAbilitySystemComponent* AbilityComp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interacts")
		UMyInteractComponent* InteractComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		UInventoryComponent* InventoryComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Skills")
		UAttackSkillComponent* AttackSkillComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		UStatusComponent* StatusComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
		UExperienceComponent* ExperienceComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
		bool IsFemale = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsBlind = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
		EPlayerState MyCurrentState = EPlayerState::IDLE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
		bool Targeted;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
		ARPGBaseClass* Target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
		bool IsJumping = false;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
		//bool IsDodging = false;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
		//bool IsSprinting = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
		int AttackCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
		bool IsInteracting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
		bool InMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
		bool InInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
		float ForwardBackInputValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
		float RightLeftInputValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
		bool CanPlayerJump = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Multipliers")
		float StaminaDrainMultiplier = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Multipliers")
		float AttackDamageMultiplier = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Multipliers")
		float AttackDebuffMultiplier = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Multipliers")
		float EnemyDamageMultiplier = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Multipliers")
		float ManaRecoveryMultiplier = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Multipliers")
		float StaminaRecoveryMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UClass* WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* BarrierClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UMyDamageType> BarrierDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	UAnimMontage* DodgeAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	UAnimMontage* ParryAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	UAnimMontage* HealAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	UAnimMontage* StunAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* LevelStartAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* AerialAttackAnim_Katana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* AerialAttackAnim_Greatsword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* AerialAttackAnim_Broadsword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* AerialAttackAnim_Spear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* BarrierParticle;
	
	UFUNCTION(BlueprintCallable)
		void PerformSavedAttack();

	void PerformAerialAttack();

	UFUNCTION(BlueprintCallable)
		void StartBarrier();

	UFUNCTION(BlueprintCallable)
		void EndBarrier();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateAttackBar();


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
	void TransitionCamera(float DeltaTime, FVector3d CamPosition);
	bool GetTargetsInRange();

	//Handle SwitchTarget Pressed
	void OnSwitchTargetPressed();
	TArray<FHitResult> OutHit;
	TArray<ARPGBaseClass*> RecentTargets;
	int TargetIndex = 0;

	//Handle Block Inputs
	void OnBlockPressed();
	AActor* Barrier;
	bool BarrierHit = false;

	//Handle Healing Inputs
	void OnHealPressed();
	void IncrementMana();
	void RestoreMana();
	bool IsRegeningMana = false;
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

	//Stamina values
	float CurrentStamina = 100;
	float StaminaMax = 100;

	//Handle Attack Pressed
	void OnAttackPressed();
	FTimerHandle AttackTimer; 
	float StrengthStat = 1.f;
	float DefenseStat = 0.f;
	int CurrentMaxAttackCount = 3;
	//bool IsAttacking = false;
	enum AttackQueuedType { NONE, COMBO, FINISHER };
	AttackQueuedType SavedAttack = AttackQueuedType::NONE;

	//Handle Combat Timer
	FTimerHandle CombatTimer;
	UPROPERTY(EditAnywhere)
	float CombatTimerLength = 10;
	UFUNCTION(BlueprintCallable)
	void StartCombatTimer(); // Starts or Resets combat timer
	UFUNCTION(BlueprintCallable)
	void CombatTimerEnd(); // Runs at the end of combat timer
	bool InCombat = false;
	TSet<AActor*> CombatantArray;

	//Player Camera Positions
	FVector3d NormalCamPosition = FVector3d(0, 0, 0);
	FVector3d FollowCamPosition = FVector3d(0, 0, 50);

	float PerformAttack(FAttackStruct* Attack);

	//Handle Finisher Pressed
	void DoFinisher();

	//Handle Interact Pressed
	void OnInteractPressed();

	//Handle Menu Pressed
	void OnMenuPressed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateSlot1(int HotbarIndex);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateCombatStatus(bool IsInCombat);

	UFUNCTION(BlueprintImplementableEvent)
		void SetBlindness(bool VisionBlindness);

	UFUNCTION(BlueprintImplementableEvent)
		void RemovePlayerMenu();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHealhBar();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateManaBar();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateStaminaBar();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateExperienceAmmount();

	UFUNCTION(BlueprintCallable)
		void ResetAttack();
	

	void ResetTarget();

	void DoFireTickDamage();

	//Handle Health
	UFUNCTION(BlueprintCallable)
	virtual bool DamageChar(float val, EDamageType Type) override;
	virtual void HealChar(float val) override;

	virtual void KillCharacter() override;

	virtual void RestoreChar();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void MoveForwardBack(float value);

	void MoveRightLeft(float value);
	
//	bool GetIsDodging() { return IsDodging; }

//	bool GetIsSprinting() { return IsSprinting; }

	int GetAttackCount() { return AttackCount; }

	void SetPlayerState(EPlayerState NewState) { MyCurrentState = NewState; }

	EPlayerState GetPlayerState() { return MyCurrentState; }

	void SetIsInteracting(bool value) { IsInteracting = value; }
	bool GetIsInteracting() { return IsInteracting; }

	bool GetIsInMenu() { return InMenu; }

	bool GetIsInInventory() { return InInventory; }

	bool GetIsJumping() { return IsJumping; }

	bool GetIsTargeted() { return Targeted; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	bool GetIsInCombat() { return InCombat; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	float GetMana() { return Mana; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	float GetManaMax() { return ManaMax; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		void SetManaMax(float NewMax) { ManaMax = NewMax; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	void AddManaAttacking();

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	void AddMana(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		float GetStamina() { return CurrentStamina; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		float GetStaminaMax() { return StaminaMax; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		void SetStaminaMax(float NewMax) { StaminaMax = NewMax; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		float GetStaminaDrainMultiplier() { return StaminaDrainMultiplier; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		void SetStaminaDrainMultiplier(float NewMultiplier) { StaminaDrainMultiplier = NewMultiplier; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		void ResetStaminaDrainMultiplier() { StaminaDrainMultiplier = 1.f; };

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		float GetAttackDamageMultiplier() { return AttackDamageMultiplier; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		void SetAttackDamageMultiplier(float NewMultiplier) { AttackDamageMultiplier = NewMultiplier; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		void ResetAttackDamageMultiplier() { AttackDamageMultiplier = 1.f; };

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		float GetEnemyDamageMultiplier() { return EnemyDamageMultiplier; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		void SetEnemyDamageMultiplier(float NewMultiplier) { EnemyDamageMultiplier = NewMultiplier; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		void ResetEnemyDamageMultiplier() { EnemyDamageMultiplier = 1.f; };

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		float GetManaRestoreMultiplier() { return ManaRecoveryMultiplier; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		void SetManaRestoreMultiplier(float NewMultiplier) { ManaRecoveryMultiplier = NewMultiplier; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		void ResetManaRestoreMultiplier() { ManaRecoveryMultiplier = 1.f; };

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		float GetStaminaRestoreMultiplier() { return StaminaRecoveryMultiplier; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		void SetStaminaRestoreMultiplier(float NewMultiplier) { StaminaRecoveryMultiplier = NewMultiplier; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		void ResetStaminaRestoreMultiplier() { StaminaRecoveryMultiplier = 1.f; };

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		float GetAttackDebuffMultiplier() { return AttackDebuffMultiplier; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		void SetAttackDebuffMultiplier(float NewMultiplier) { AttackDebuffMultiplier = NewMultiplier; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		void ResetAttackDebuffMultiplier() { AttackDebuffMultiplier = 0.f; };

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		float GetStrengthStat() { return StrengthStat; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		void SetStrengthStat(float NewStrengthStat) { StrengthStat = NewStrengthStat; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		float GetDefenseStat() { return DefenseStat; }

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		void SetDefenseStat(float NewDefenseStat) { DefenseStat = NewDefenseStat; }

	void SetCanJump(bool IsAllowedToJump) { CanPlayerJump = IsAllowedToJump; }

	float GetForwardBackValue() { return ForwardBackInputValue; }
	
	float GetRightLeftValue() { return RightLeftInputValue; }

	UFUNCTION(BlueprintImplementableEvent)
	void SaveGame();

	UFUNCTION(BlueprintImplementableEvent)
	void SaveInventory();

	UFUNCTION(BlueprintCallable)
	void AddCombatant(AActor* Combatant);

	UFUNCTION(BlueprintCallable)
	void RemoveCombatant(AActor* Combatant);

	//UAbilitySystemComponent* GetAbilitySystemComponent() const override{ return AbilityComp; };
	UMyInteractComponent* GetInteractComponent() const { return InteractComp; }
	UInventoryComponent* GetInventoryComponent() const { return InventoryComp; }
	UAttackSkillComponent* GetAttackSkillComponent() const { return AttackSkillComp; }
	UStatusComponent* GetStatusComponent() const { return StatusComp; }
	UExperienceComponent* GetExperienceComponent() const { return ExperienceComp; }

	void StartParalysis();
	void TriggerStun();
	void EndParalysis();
	FTimerHandle ParalysisTimer;

};


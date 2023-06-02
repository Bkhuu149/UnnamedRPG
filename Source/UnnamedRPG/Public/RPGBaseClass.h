// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "WeaponActor.h"
#include "RPGBaseClass.generated.h"

UCLASS()
class UNNAMEDRPG_API ARPGBaseClass : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGBaseClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private: 

	float Health = 100.0;
	float HealthMax = 100.0;

	FTimerHandle InvincibiltyTimer;

	FTimerHandle DisableColTimer;

	FTimerHandle CheckSpeedTimer;

	UFUNCTION(BlueprintCallable, category = "MYRPGCharacter")
	void StartInvincibility() { IsInvincible = true; }
	UFUNCTION(BlueprintCallable, category = "MYRPGCharacter")
	void ResetInvincibility() { IsInvincible = false; }

	UFUNCTION(BlueprintCallable, category = "MYRPGCharacter")
	void BeginSwordEvent();
	UFUNCTION(BlueprintCallable, category = "MyRPGCharacter")
	void EndSwordEvent();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

	virtual void CheckSpeed(float FallDamage = 0.f);

	//Handle Health
	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual bool DamageChar(float val);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void KillCharacter();

	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual void HealChar(float val);

	bool IsInvincible = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	bool IsDead = false;

	bool GetIsDead() { return IsDead; }

	UFUNCTION(BlueprintCallable, Category = "Character Information")
	float GetHealth() { return Health; }

	UFUNCTION(BlueprintCallable, Category = "Character Information")
	float GetHealthMax() { return HealthMax; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Animations")
	TArray<UAnimMontage*> DeathAnims;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Animations")
	UAnimMontage* HitReactAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UClass* ChosenWeapon;

	AWeaponActor* CurrentWeapon;
};

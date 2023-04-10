// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "MyRPGCharacter.h"

#include "RPGAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class UNNAMEDRPG_API URPGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()




public:

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	AMyRPGCharacter* Character;

	UCharacterMovementComponent* MoveComp;
	UMyInteractComponent* InteractComp;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	bool bIsTargeted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	float GroundSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	FVector Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	bool ShouldMove = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	bool IsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	bool IsSprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	float ForwardBackValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	float LeftRightValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	bool IsPushing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	float PushingDirection = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Information")
	float PushHeight = 0.f;
	
private:
	void CalculateMovementInput();
};

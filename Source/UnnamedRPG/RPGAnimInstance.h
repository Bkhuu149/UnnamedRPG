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

	AMyRPGCharacter* Character;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Information")
		bool bIsTargeted = false;


};

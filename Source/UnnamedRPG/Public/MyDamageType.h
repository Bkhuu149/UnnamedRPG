// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageTypeInterface.h"
#include "GameFramework/DamageType.h"
#include "MyDamageType.generated.h"

/**
 * 
 */
UCLASS()
class UNNAMEDRPG_API UMyDamageType : public UDamageType, public IDamageTypeInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	EDamageType Type = EDamageType::NONE;

	UFUNCTION(BlueprintCallable)
	virtual EDamageType GetDamageType() override { return Type; }

};

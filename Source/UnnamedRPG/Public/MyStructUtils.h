// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyEnumUtils.h"
#include "MyStructUtils.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FAugmentSlotStruct
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDamageType Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsUnlocked = false;

	FAugmentSlotStruct() {
		Type = EDamageType::NONE;
		IsUnlocked = false;
	}

	FAugmentSlotStruct(EDamageType InitialType, bool Acquired) {
		Type = InitialType;
		IsUnlocked = Acquired;
	}

};
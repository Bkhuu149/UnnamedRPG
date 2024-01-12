// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyEnumUtils.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "MyDamageType.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/Texture2D.h"

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

USTRUCT(BlueprintType)
struct FSavePointStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName LevelName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName PointName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText PointDescription;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* Thumbnail;
};

USTRUCT(BlueprintType)
struct FDamageTypeStruct : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText DamageTypeDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<UMyDamageType> Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UNiagaraSystem* TypeWeaponTrail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UNiagaraSystem* TypeWeaponEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* Thumbnail;

};
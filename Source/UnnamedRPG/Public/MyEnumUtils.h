// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyEnumUtils.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8 {
	NONE		UMETA(DisplayName = "None"), //Default, not doing anything
	FIRE		UMETA(DisplayName = "Fire"),
	WATER		UMETA(DisplayName = "Water"),
	EARTH		UMETA(DisplayName = "Earth"),
	WIND		UMETA(DisplayName = "Wind"),
	ICE			UMETA(DisplayName = "Ice"),
	SAND		UMETA(DisplayName = "Sand"),
	LIGHTNING	UMETA(DisplayName = "Lightning"),
	SMOKE		UMETA(DisplayName = "Smoke")
};

UENUM(BlueprintType)
enum class EPlayerState : uint8 {
	IDLE			UMETA(DisplayName = "Idle"), //Default, not doing anything
	DODGING			UMETA(DisplayName = "Dodging"),
	SPRINTING		UMETA(DisplayName = "Sprinting"),
	INTERACTING		UMETA(DisplayName = "Interacting"),
	ATTACKING		UMETA(DisplayName = "Attacking"),
	BLOCKING		UMETA(DisplayName = "Blocking")

};

UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	IDLE			UMETA(DisplayName = "Idle"), //Default, not doing anything
	PATH_WALKING	UMETA(DisplayName = "Path Walking"), //Currently following a set path
	CHASE_CLOSE		UMETA(DisplayName = "Chase Close"), //Following target closely
	CHASE_FAR		UMETA(DisplayName = "Chase Far"), //Has a target but staying a distance away
	ATTACK			UMETA(DisplayName = "Attack") //Performing an attack
};


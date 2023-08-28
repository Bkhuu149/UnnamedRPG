// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyEnumUtils.generated.h"

UENUM(BlueprintType) 
enum class EStatus : uint8 {
	BURN			UMETA(DisplayName = "Burn"),
	WET		 		UMETA(DisplayName = "Wet"),
	HEAVY			UMETA(DisplayName = "Heavy"),
	UNSTEADY		UMETA(DisplayName = "Unsteady"),
	SLOWED			UMETA(DisplayName = "Slowed"),
	DUST			UMETA(DisplayName = "Dust"),
	PARALIZED		UMETA(DisplayName = "Paralized"),
	SMOKE			UMETA(DisplayName = "Smoke"),
	REGEN			UMETA(DisplayName = "Regen"),
	DAMAGEBUFF		UMETA(DisplayName = "Damage Buff"),
	STAMINABUFF		UMETA(DisplayName = "Stamina Buff"),
	DEFENSEBUFF		UMETA(DisplayName = "Defense Buff")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	BROADSWORD		UMETA(DisplayName = "Broadsword"),
	GREATSWORD		UMETA(DisplayName = "Greatsword"),
	SPEAR			UMETA(DisplayName = "Spear"),
	KATANA			UMETA(DisplayName = "Katana"),
	OTHER			UMETA(DisplayName = "Other")
};

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


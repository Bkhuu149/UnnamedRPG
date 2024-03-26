// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyClass.h"
#include "EnemyTwoHandedClass.generated.h"

/**
 * 
 */
UCLASS()
class UNNAMEDRPG_API AEnemyTwoHandedClass : public AEnemyClass
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void BeginOffhandSwordEvent();
	UFUNCTION(BlueprintCallable)
		void EndOffhandSwordEvent();
public: 

	AEnemyTwoHandedClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		UClass* ChosenOffhandWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AWeaponActor* CurrentOffhandWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDamageType OffhandWeaponType = EDamageType::NONE;

	void SetCurrentOffhandDamage(float NewDamage) {
		CurrentDamage = NewDamage;
		CurrentOffhandWeapon->SetDamage(NewDamage);
	}

	void ResetOffhandDamage() {
		CurrentDamage = InitialDamage;
		CurrentOffhandWeapon->SetDamage(InitialDamage);
	}
};

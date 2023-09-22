// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyEnumUtils.h"
#include "StatusComponent.generated.h"


class AMyRPGCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNNAMEDRPG_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TMap<EStatus, int> StatusEffectBuildups;

	UPROPERTY(EditAnywhere)
	TMap<EStatus, int> ActiveStatusEffects;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void DecrimentEffects(TMap<EStatus, int>& StatusMap);

	virtual void RemoveFinishedEffects(TMap<EStatus, int>& StatusMap);

	virtual void RemoveEffect(EStatus Effect);

	virtual void AddItemEffect(EStatus ItemEffect, int Time);

	virtual void AddDebuff(EDamageType Type, float Damage);

	virtual int CalculateEffectBuildupFromDamage(float Damage);

	virtual void ActivateEffect(EStatus Effect);

	virtual void DeactivateEffect(EStatus Effect);

	AMyRPGCharacter* Player;

	UFUNCTION(BlueprintCallable)
		int GetStatusTimeRemaining(EStatus Effect);
};

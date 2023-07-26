// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyEnumUtils.h"
#include "StatusComponent.generated.h"


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

private:
	TMap<FName, int> StatusEffects;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void DecrimentEffects();

	void RemoveFinishedEffects();

	void RemoveEffect(FName Effect);

	void AddItemEffect(FName ItemEffect, int Time);

	void AddDebuff(EDamageType Type, float Damage);

	int CalculateEffectBuildupFromDamage(float Damage);
};

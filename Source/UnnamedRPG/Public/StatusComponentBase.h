// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyEnumUtils.h"
#include "Components/ActorComponent.h"
#include "StatusComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNNAMEDRPG_API UStatusComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusComponentBase();

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

	virtual void AddDebuff(EDamageType Type, float Damage);

	virtual int CalculateEffectBuildupFromDamage(float Damage);

	virtual void ActivateEffect(EStatus Effect) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Not Implemented")); };

	virtual void DeactivateEffect(EStatus Effect) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Not Implemented")); };

	UFUNCTION(BlueprintCallable)
		int GetStatusTimeRemaining(EStatus Effect);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyEnumUtils.h"
#include "../MyRPGCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "StatusComponentBase.h"
#include "Components/ActorComponent.h"
#include "EnemyStatusComponent.generated.h"

class AEnemyClass;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNNAMEDRPG_API UEnemyStatusComponent : public UStatusComponentBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyStatusComponent();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AEnemyClass* Enemy;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivateEffect(EStatus Effect) override;

	virtual void DeactivateEffect(EStatus Effect) override;

};

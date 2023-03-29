// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PushableActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyPushComponent.generated.h"

class AMyRPGCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNNAMEDRPG_API UMyPushComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyPushComponent();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//To Change class to be Pushable Object later
	APushableActor* CurrentPushable;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BeginPush(APushableActor* CurrentPushable);
	void EndPush();
	bool IsPushingObject() { return IsValid(CurrentPushable); }
	float GetPushableHeight();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float PushSpeed = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float PushRange = 120.f;
		
};

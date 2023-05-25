// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/GameEngine.h"
#include "PushableActor.generated.h"


class UMyInteractComponent;


UCLASS()
class UNNAMEDRPG_API APushableActor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APushableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (MakeEditWidget = true))
	TArray<FTransform> PushTransformsTemp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	int FindClosestPushTransformIndex1(FVector2D CharacterLocation, float PushRange);

	UFUNCTION(BlueprintCallable)
	virtual void HandleInteraction(ACharacter* Character) override;

	void PushActor(float Strength, FVector Direction, float PushSpeed = 2.f, float Duration = 5.f);
	bool GetIsMoving() { return IsMoving; }

private: 
	FTimerHandle PushTimer;
	FTimerHandle StopPushTimer;
	bool IsPushable = true;
	bool IsMoving = false;

	void StopPush();
	void MoveActor();

	FVector PlayerDirection;
	float PlayerPushSpeed;
	float PlayerPushStrength;
};

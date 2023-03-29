// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyPushComponent.h"
#include "PushableActor.generated.h"

UCLASS()
class UNNAMEDRPG_API APushableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APushableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTransform> PushTransformsTemp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	int FindClosestPushTransformIndex1(FVector2D CharacterLocation, float PushRange);

	UFUNCTION(BlueprintCallable)
	void HandleInteraction1(ACharacter* Character);



};

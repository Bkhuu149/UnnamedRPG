// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "SavePoint.generated.h"

UCLASS()
class UNNAMEDRPG_API ASavePoint : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASavePoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	const FString SaveName = FString("slot1");

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void HandleInteraction(ACharacter* Character) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintSave();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform InteractableLocation;
};

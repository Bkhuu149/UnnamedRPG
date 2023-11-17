// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGBaseClass.h"
#include "InteractableInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "NonPlayerClass.generated.h"

UCLASS()
class UNNAMEDRPG_API ANonPlayerClass : public ARPGBaseClass, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANonPlayerClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		AActor* Target;

private:
	void Rotate(float DeltaTime);

public:
	virtual void Tick(float DeltaTime) override;

	virtual void HandleInteraction(ACharacter* Character) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform InteractableLocation;
};

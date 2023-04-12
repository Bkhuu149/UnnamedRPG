// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "LadderActor.generated.h"


class UMyInteractComponent;


enum class EPosition : uint8 {
	T_Top         UMETA(DisplayName = "Top"),
	T_Bottom      UMETA(DisplayName = "Bottom"),
	T_None        UMETA(DisplayName = "None"),
};


UCLASS()
class UNNAMEDRPG_API ALadderActor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALadderActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Height = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FTransform LadderBottom = FTransform(FRotator(0,180,0), FVector(100, 0, 2.5f), FVector::OneVector);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform LadderTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform LadderTopExit;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	EPosition FindClosestPushTransformIndex(FVector CharacterLocation);

	virtual void HandleInteraction(ACharacter* Character) override;

	FTransform GetLadderTop() { return LadderTop; }

	FTransform GetLadderTopExit() { return LadderTopExit; }

	FTransform GetLadderBottom() { return LadderBottom; }

};

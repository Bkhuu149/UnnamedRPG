// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorActor.generated.h"

UENUM(BlueprintType)
enum class EDoorType : uint8 {
	Gate         UMETA(DisplayName = "Rising Gate"),
	Swivel      UMETA(DisplayName = "Normal Hinge Door"),
	Bridge        UMETA(DisplayName = "Draw Bridge")
};

UCLASS()
class UNNAMEDRPG_API ADoorActor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDoorType DoorType;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void HandleInteraction(ACharacter* Character) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Door;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform TargetTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform InteractableLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsOpen = false;
private:

	FTimerHandle OpenTimer;

	void RaiseGate();

	void LowerBridge();

	void RotateDoor();

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LadderActor.h"
#include "PushableActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyInteractComponent.generated.h"

class AMyRPGCharacter;

enum class EInteractType : uint8 {
	Pushing         UMETA(DisplayName = "Pushing"),
	Climbing      UMETA(DisplayName = "Climbing"),
	None        UMETA(DisplayName = "None"),
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNNAMEDRPG_API UMyInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyInteractComponent();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//To Change class to be Pushable Object later
	ALadderActor* CurrentLadder;
	APushableActor* CurrentPushable;
	ACharacter* OwningCharacter;

	EInteractType CurrentType = EInteractType::None;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BeginPush(APushableActor* CurrentPushable);

	void BeginClimb(ALadderActor* Ladder);
	

	UFUNCTION(BlueprintCallable)
	void EndInteract();

	//Pushing
	UFUNCTION(BlueprintCallable)
	bool IsPushingObject();

	UFUNCTION(BlueprintCallable)
	float GetPushableHeight();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float PushSpeed = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float PushRange = 120.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	float PushDirection = 0.f;

	EInteractType GetInteractType() { return CurrentType; }

	void SetInteractType(EInteractType NewMode) { CurrentType = NewMode; }
		
};

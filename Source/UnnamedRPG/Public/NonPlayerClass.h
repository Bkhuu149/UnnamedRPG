// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGBaseClass.h"
#include "InteractableInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"
#include "NavigationSystem.h"
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

	UPROPERTY(EditAnywhere)
	ENonPlayerState CurrentNonPlayerState = ENonPlayerState::FOLLOW_PATH;

	ENonPlayerState PrevNonPlayerState = ENonPlayerState::IDLE;

	UFUNCTION(BlueprintCallable)
	void SetState(ENonPlayerState NewState) { CurrentNonPlayerState = NewState; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle PathTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MakeEditWidget = true))
	TArray<FTransform> WalkPath;

private:
	void Rotate(float DeltaTime, FRotator RotateTo);
	FRotator DefaultRotation;

	virtual void TickStateMachine(float DeltaTime);

	virtual void StateIdle(float DeltaTime);

	virtual void StateFollowPath();

	virtual void StateTalking(float DeltaTime);

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

	virtual void NextPathNode(bool Forward);

	UNavigationSystemV1* NavSys;

	AAIController* MyController;

	UPROPERTY(EditAnywhere)
	int CurrentPathNode = 0;

	EPathFollowingRequestResult::Type FollowResult;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void HandleInteraction(ACharacter* Character) override;

	virtual void DisableChar() override;

	UFUNCTION(BlueprintCallable)
	ENonPlayerState GetCurrentState() { return CurrentNonPlayerState; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform InteractableLocation;
};

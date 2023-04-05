// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "LadderActor.generated.h"

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

	UPROPERTY(EditAnywhere)
		int Segments = 0;

	UPROPERTY(EditAnywhere)
		UStaticMesh* LadderSegment;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

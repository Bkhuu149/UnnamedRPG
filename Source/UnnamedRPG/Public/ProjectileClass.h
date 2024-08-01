// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "RPGBaseClass.h"
#include "ProjectileClass.generated.h"

UCLASS()
class UNNAMEDRPG_API AProjectileClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 10.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMyDamageType> MyType;

private:
	FTimerHandle ColTimer;
	FVector PrevLocation;
	FVector CollisionSize = FVector(32, 32, 32);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CheckCollision();

	void HandleCollision(AActor* HitActor);

	UFUNCTION(BlueprintCallable)
	void SetSpeed(float NewSpeed) { Speed = NewSpeed; }


};

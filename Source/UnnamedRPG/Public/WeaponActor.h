// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

UCLASS()
class UNNAMEDRPG_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	AActor* Owner;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	UNiagaraSystem* Trail;

	UNiagaraComponent* TrailComp;

	void PlayTrail();
	void EndTrail();

	void SetOwner(AActor* MyActor) { Owner = MyActor; }
};

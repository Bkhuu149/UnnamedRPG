// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyDamageType.h"
#include "MyEnumUtils.h"
#include "MyStructUtils.h"
#include "GameFramework/Actor.h"

#include "TrapActor.generated.h"
UCLASS()
class UNNAMEDRPG_API ATrapActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrapActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UNiagaraSystem* ElementParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsStrong = false;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Type Table")
		UDataTable* DamageTypeTab;

	UFUNCTION(BlueprintCallable)
		bool GetStrongHit() { return IsStrong; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UMyDamageType> MyType;
	UFUNCTION(BlueprintCallable)
		void SetDamageType(EDamageType NewType);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void Activate();
};

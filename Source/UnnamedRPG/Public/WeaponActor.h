// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyEnumUtils.h"
#include "MyStructUtils.h"
#include "Kismet/GameplayStatics.h"
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

	virtual void BeginDestroy() override;

private:
	AActor* Owner;
	FTimerHandle ColTimer;

	void WeaponLineTrace();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Type Table")
	UDataTable* DamageTypeTab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMyDamageType> MyType;
	void SetDamageType(EDamageType NewType);

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;
	void SetDamage(float NewDamage) { Damage = NewDamage; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsStrong = false;
	UFUNCTION(BlueprintCallable)
	void SetStrongHit(bool NewHit) { IsStrong = NewHit; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Element")
	UNiagaraSystem* Trail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Element")
	UNiagaraSystem* ElementParticles;

	UPROPERTY(EditAnywhere)
		EWeaponType Weapon = EWeaponType::OTHER;

	UPROPERTY(EditAnywhere)
		FVector ParticleScale = FVector(1,1,1);

	EWeaponType GetWeaponType() { return Weapon; }

	UNiagaraComponent* TrailComp;

	UFUNCTION(Blueprintcallable)
	void StartLineTrace();

	UFUNCTION(BlueprintCallable)
	void EndLineTrace();

	void SetOwner(AActor* MyActor) { Owner = MyActor; }
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBaseClass.h"

// Sets default values
ARPGBaseClass::ARPGBaseClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARPGBaseClass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARPGBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPGBaseClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARPGBaseClass::DamageChar(float val) {
	if (Health - val <= 0) {
		Health = 0;
		IsDead = true;


		UAnimMontage* DeathMontage = DeathAnims[FMath::FRandRange(0, 2)];

		PlayAnimMontage(DeathMontage);
		return;
	}
	Health -= val;
}

void ARPGBaseClass::HealChar(float val) {
	if (val <= 0) { return; }
	if (val > HealthMax - Health) {
		Health = HealthMax;
		return;
	}
	Health += val;
}


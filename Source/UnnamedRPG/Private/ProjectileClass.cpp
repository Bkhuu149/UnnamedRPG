// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileClass.h"

// Sets default values
AProjectileClass::AProjectileClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectileClass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "SavePoint.h"
#include "../MyRPGCharacter.h"

// Sets default values
ASavePoint::ASavePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASavePoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASavePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASavePoint::InitialSave(ACharacter* Character)
{
	AMyRPGCharacter* RPGCharacter = Cast<AMyRPGCharacter>(Character);
	BlueprintSave();
	RPGCharacter->RestoreChar();
	RPGCharacter->SaveGame();
	RPGCharacter->SaveInventory();
}

void ASavePoint::HandleInteraction(ACharacter* Character)
{
	AMyRPGCharacter* RPGCharacter = Cast<AMyRPGCharacter>(Character);
	BlueprintSave();
	SaveLevel();
	RPGCharacter->RestoreChar();
	RPGCharacter->SaveGame();
	RPGCharacter->SaveInventory();
	ResetLevel();
}
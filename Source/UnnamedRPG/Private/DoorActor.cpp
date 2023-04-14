// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"

// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ADoorActor::HandleInteraction(ACharacter* Character) {
	switch (DoorType){
		case EDoorType::Swivel:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("A Swivel Door"));
			break;

		case EDoorType::Bridge:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("A Bridge Door"));
			break;


		case EDoorType::Gate:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("A Gate Door"));
			break;

	}

}
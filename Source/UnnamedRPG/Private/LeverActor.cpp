// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverActor.h"
#include "../MyRPGCharacter.h"


// Sets default values
ALeverActor::ALeverActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALeverActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALeverActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALeverActor::HandleInteraction(ACharacter* Character)
{
	if (!Character) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, (TEXT("Character not valid")));
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, (TEXT("Do Something")));
	Activate();
	Cast<AMyRPGCharacter>(Character)->SetPlayerState(EPlayerState::IDLE);

}

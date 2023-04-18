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
	
	if (!Door) {
		return;
	}
	IsOpen = true;

	switch (DoorType){
		case EDoorType::Swivel:
			RotateDoor();
			break;

		case EDoorType::Bridge:
			LowerBridge();
			break;


		case EDoorType::Gate:
			RaiseGate();
			break;

	}


}

void ADoorActor::RaiseGate() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("A Gate Door"));
	FTransform Transform = Door->GetRelativeTransform();
}

void ADoorActor::LowerBridge() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("A Bridge Door"));
	FTransform Transform = Door->GetRelativeTransform();
}

void ADoorActor::RotateDoor() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("A Swivel Door"));
	FTransform Transform = Door->GetRelativeTransform();
}
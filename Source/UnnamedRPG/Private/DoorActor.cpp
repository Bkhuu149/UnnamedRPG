// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"
#include "../MyRPGCharacter.h"
#include "MyInteractComponent.h"



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

bool ADoorActor::CheckDistance(FVector CharacterLocation, float PushRange) {
	//Check if character is near button when interacting
	FVector ButtonLocation = UKismetMathLibrary::TransformLocation(GetActorTransform(), InteractableLocation.GetLocation());
	float DistanceSq = FVector::DistSquared(ButtonLocation, CharacterLocation);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Button location at: %s"), *ButtonLocation.ToString()));
	if (DistanceSq < pow(PushRange, 2.0f)) {
		return true;
	}
	return false;
}


void ADoorActor::HandleInteraction(ACharacter* Character) {
	
	if (!Door) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, (TEXT("No door to open")));

		return;
	}
	if (!Character) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, (TEXT("Character not valid")));
		return;
	}

	//Do distance calculations here
	UMyInteractComponent* InteractComp = Cast<UMyInteractComponent>(Character->GetComponentByClass(UMyInteractComponent::StaticClass()));
	if (!InteractComp) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, (TEXT("Character does not have push component")));
		return;
	}

	if (!CheckDistance(Character->GetActorLocation(), InteractComp->PushRange)) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, (TEXT("Character too far away")));
		return;
	}
	
	if (IsOpen) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, (TEXT("Door already open!")));
		return;
	}

	IsOpen = true;

	switch (DoorType){
		case EDoorType::Swivel:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("A Swivel Door"));
			RotateDoor();
			break;

		case EDoorType::Bridge:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("A Bridge Door"));
			LowerBridge();
			break;


		case EDoorType::Gate:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Opening Gate Door"));
			RaiseGate();
			break;

	}


}

void ADoorActor::RaiseGate() {
	FTransform DoorTransform = Door->GetRelativeTransform();
	if (DoorTransform.GetLocation().Z >= TargetTransform.GetLocation().Z) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Done Opening!"));
		return;
	}
	Door->AddLocalOffset(FVector(0,0, 3));
	GetWorld()->GetTimerManager().SetTimer(OpenTimer, this, &ADoorActor::RaiseGate, 0.033f, false);
}

void ADoorActor::LowerBridge() {
	FTransform DoorTransform = Door->GetRelativeTransform();

	FVector CurrentVector = DoorTransform.GetRotation().Vector();
	FVector TargetVector = TargetTransform.GetRotation().Vector();

	//Need to convert to float for some reason
	if ((float)TargetVector.Dot(CurrentVector) == 1) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Done Opening!"));
		return;
	}
	Door->AddLocalRotation(FRotator(1, 0, 0));
	GetWorld()->GetTimerManager().SetTimer(OpenTimer, this, &ADoorActor::LowerBridge, 0.033f, false);
}

void ADoorActor::RotateDoor() {
	FTransform DoorTransform = Door->GetRelativeTransform();

	FVector2D CurrentVector = (FVector2D)DoorTransform.GetRotation().Vector();
	FVector2D TargetVector = (FVector2D)TargetTransform.GetRotation().Vector();

	//Need to convert to float for some reason
	if ((float)TargetVector.Dot(CurrentVector) == 1) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Done Opening!"));
		return;
	}
	Door->AddLocalRotation(FRotator(0, 1, 0));
	GetWorld()->GetTimerManager().SetTimer(OpenTimer, this, &ADoorActor::RotateDoor, 0.033f, false);
}
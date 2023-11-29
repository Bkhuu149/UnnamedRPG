// Fill out your copyright notice in the Description page of Project Settings.


#include "NonPlayerClass.h"

ANonPlayerClass::ANonPlayerClass()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ANonPlayerClass::BeginPlay()
{
	Super::BeginPlay();
	DefaultRotation = GetActorRotation(); //Capture rotation
}

void ANonPlayerClass::Rotate(float DeltaTime, FRotator RotateTo)
{
	//Slowly rotate enemy towards character
	//if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) { return; }
	FRotator CharacterRInterpVal = UKismetMathLibrary::RInterpTo(GetActorRotation(), RotateTo, DeltaTime, 10.f);
	SetActorRotation(CharacterRInterpVal);
}

void ANonPlayerClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickStateMachine(DeltaTime);
}

void ANonPlayerClass::TickStateMachine(float DeltaTime) {
	switch (CurrentNonPlayerState) {
	case ENonPlayerState::IDLE:
		StateIdle(DeltaTime);
		break;
	case ENonPlayerState::FOLLOW_PATH:
		StateFollowPath();
		break;
	case ENonPlayerState::TALKING:
		StateTalking(DeltaTime);
		break;
	}
}

void ANonPlayerClass::StateIdle(float DeltaTime) {
	Rotate(DeltaTime, DefaultRotation);
}

void ANonPlayerClass::StateFollowPath() {
	return;
}

void ANonPlayerClass::StateTalking(float DeltaTime) {
	if (Target) {
		//Rotate Enemy to player if targeted
		FVector CurrentLocation = GetActorLocation();
		FVector TargetLocation = Target->GetActorLocation();
		CurrentLocation.Z = 0;
		TargetLocation.Z = 0;
		FRotator CharacterLookRotator = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);

		Rotate(DeltaTime, CharacterLookRotator);
	}
}

void ANonPlayerClass::HandleInteraction(ACharacter* Character)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Touched NPC")));
	Target = Character;
	CurrentNonPlayerState = ENonPlayerState::TALKING;

	// TEMP Delay for 10 seconds
	FTimerHandle DelayTimer;
	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &ANonPlayerClass::SetState, ENonPlayerState::IDLE);
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, RespawnDelegate, 10, true);
}
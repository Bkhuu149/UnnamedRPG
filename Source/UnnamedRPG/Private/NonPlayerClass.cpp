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
	MyController = static_cast<AAIController*>(GetController());
	NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	if (WalkPath.Num() == 0) {
		WalkPath.Add(GetActorTransform());
	}
	else {
		WalkPath[0] = GetActorTransform();
	}

	for (int i = 1; i < WalkPath.Num(); i++) {
		WalkPath[i] = UKismetMathLibrary::ComposeTransforms(WalkPath[i], GetActorTransform());
	}
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
	if (!NavSys || WalkPath.Num() == 0) { return; }
	if (PathTimer.IsValid()) {
		return;
	}
	if (!MyController->IsFollowingAPath()) {
		//Not currently following a path, find next node in path and walk to it
		//CurrentNonPlayerState = ENonPlayerState::IDLE;
		CurrentPathNode++;
		if (CurrentPathNode >= WalkPath.Num()) {
			CurrentPathNode = 0;
		}
		FVector Location = WalkPath[CurrentPathNode].GetLocation();
		FollowResult = MyController->MoveToLocation(Location, 100.f);
		if (PathTimer.IsValid()) {
			GetWorld()->GetTimerManager().ClearTimer(PathTimer);
			PathTimer.Invalidate();
		}
		GetWorld()->GetTimerManager().SetTimer(PathTimer, [&]() {
			PathTimer.Invalidate(); }, 10.f, false);
	}
}

void ANonPlayerClass::StateTalking(float DeltaTime) {
	if (Target) {
		MyController->StopMovement();
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
	if (CurrentNonPlayerState == ENonPlayerState::TALKING) { return; }
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Touched NPC")));
	Target = Character;
	PrevNonPlayerState = CurrentNonPlayerState;
	CurrentNonPlayerState = ENonPlayerState::TALKING;

	FTimerHandle DelayTimer;
	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &ANonPlayerClass::SetState, PrevNonPlayerState);
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, RespawnDelegate, 10, true);
}
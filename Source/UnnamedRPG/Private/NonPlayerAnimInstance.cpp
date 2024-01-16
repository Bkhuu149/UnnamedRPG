// Fill out your copyright notice in the Description page of Project Settings.


#include "NonPlayerAnimInstance.h"

void UNonPlayerAnimInstance::NativeInitializeAnimation() {
	Character = Cast<ANonPlayerClass>(TryGetPawnOwner());
	if (Character) {
		MoveComp = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
	}
}

void UNonPlayerAnimInstance::NativeUpdateAnimation(float DeltaTimeX) {

	if (!Character) {
		return;
	}

	//Reading character values to determine which state character is in
	Velocity = MoveComp->Velocity;
	GroundSpeed = ((FVector2D)Velocity).Length();
	ShouldMove = (Velocity != FVector(0, 0, 0)) && (GroundSpeed > 3);
	IsFalling = MoveComp->IsFalling();
	IsTalking = (Character->GetCurrentState() == ENonPlayerState::TALKING);

}
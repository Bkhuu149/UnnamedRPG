// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAnimInstance.h"


void URPGAnimInstance::NativeInitializeAnimation() {
	Character = static_cast<AMyRPGCharacter*>(TryGetPawnOwner());
	if (Character) {
		MoveComp = static_cast<UCharacterMovementComponent*>(Character->GetMovementComponent());
	}
}

void URPGAnimInstance::NativeUpdateAnimation(float DeltaTimeX) {

	if (!Character) {
		return;
	}

	bIsTargeted = Character->GetIsTargeted();
	Velocity = MoveComp->Velocity;
	GroundSpeed = ((FVector2D)Velocity).Length();
	ShouldMove = (MoveComp->GetCurrentAcceleration() != FVector(0, 0, 0)) && (GroundSpeed > 3);
	IsFalling = MoveComp->IsFalling();

}
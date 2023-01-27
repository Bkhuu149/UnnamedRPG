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
	
	//Reading character values to determine which state character is in
	bIsTargeted = Character->GetIsTargeted();
	Velocity = MoveComp->Velocity;
	GroundSpeed = ((FVector2D)Velocity).Length();
	Acceleration = MoveComp->GetCurrentAcceleration();
	ShouldMove = (Acceleration != FVector(0, 0, 0)) && (GroundSpeed > 3);
	IsFalling = MoveComp->IsFalling();
	
	CalculateMovementInput();
}

void URPGAnimInstance::CalculateMovementInput() {
	//Calculate movementinput for use in animbp

	FVector2d Acceleration2D = (FVector2D)Acceleration;
	FVector2d ForwardVector2D = (FVector2D)Character->GetActorForwardVector();
	FVector2d RightVector2D = (FVector2D)Character->GetActorRightVector();
	Acceleration2D.Normalize();
	ForwardVector2D.Normalize();
	RightVector2D.Normalize();
	ForwardBackValue = ForwardVector2D.Dot(Acceleration2D);
	LeftRightValue = RightVector2D.Dot(Acceleration2D);

}
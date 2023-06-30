// Fill out your copyright notice in the Description page of Project Settings.


#include "SmallEnemyAnimInstance.h"

void USmallEnemyAnimInstance::NativeInitializeAnimation() {
	Character = Cast<AEnemyClass>(TryGetPawnOwner());
	if (Character) {
		MoveComp = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
	}
}

void USmallEnemyAnimInstance::NativeUpdateAnimation(float DeltaTimeX) {

	if (!Character) {
		return;
	}

	//Reading character values to determine which state character is in
	bIsTargeted = Character->GetIsTargeted();
	Velocity = MoveComp->Velocity;
	GroundSpeed = ((FVector2D)Velocity).Length();
	ShouldMove = (Velocity != FVector(0, 0, 0)) && (GroundSpeed > 3);
	IsFalling = MoveComp->IsFalling();

	//Only care about this value because untargeted, the player rotates to where they move
	//while when targeted, the player moves relative to what they are targeting - Brian
	if (bIsTargeted) {
		CalculateMovementInput();
	}
}

void USmallEnemyAnimInstance::CalculateMovementInput() {
	//Calculate movementinput for use in animbp

	FVector2d Acceleration2D = (FVector2D)Acceleration;
	FVector2d ForwardVector2D = (FVector2D)Character->GetActorForwardVector();
	FVector2d RightVector2D = (FVector2D)Character->GetActorRightVector();
	//Acceleration2D.Normalize();
	//ForwardVector2D.Normalize();
	//RightVector2D.Normalize();
	LeftRightValue = Acceleration2D.Dot(ForwardVector2D) / 10;
	ForwardBackValue = Acceleration2D.Dot(RightVector2D) / 10;

}
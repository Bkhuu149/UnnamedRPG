// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAnimInstance.h"


void URPGAnimInstance::NativeInitializeAnimation() {
	Character = static_cast<AMyRPGCharacter*>(TryGetPawnOwner());
}

void URPGAnimInstance::NativeUpdateAnimation(float DeltaTimeX) {

	if (!Character) {
		return;
	}

	bIsTargeted = Character->GetIsTargeted();
}
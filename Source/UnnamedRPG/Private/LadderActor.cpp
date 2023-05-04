// Fill out your copyright notice in the Description page of Project Settings.


#include "LadderActor.h"
#include "MyInteractComponent.h"
#include "../MyRPGCharacter.h"

// Sets default values
ALadderActor::ALadderActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALadderActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALadderActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EPosition ALadderActor::FindClosestPushTransformIndex(FVector CharacterLocation) {

	FVector Top = UKismetMathLibrary::TransformLocation(GetActorTransform(), LadderTopExit.GetLocation());
	FVector Bottom = UKismetMathLibrary::TransformLocation(GetActorTransform(), LadderBottom.GetLocation());

	float DistanceToTop = FVector::DistSquared(Top, CharacterLocation);
	float DistanceToBottom = FVector::DistSquared(Bottom, CharacterLocation);
	//Check if we are close to the bottom or top of the ladder
	if (DistanceToTop < pow(200.f, 2) || DistanceToBottom < pow(200.f, 2)) {

		if (DistanceToTop < DistanceToBottom) {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Top"));

			return EPosition::T_Top;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Bottom"));

		return EPosition::T_Bottom;
	}
	return EPosition::T_None;

}

void ALadderActor::HandleInteraction(ACharacter* Character) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ladder Interact"));
	EPosition Test = FindClosestPushTransformIndex(Character->GetActorLocation());

	FTransform ClosestTransformLocal;

	if (Test == EPosition::T_None) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("None"));
		return;
	}
	else if (Test == EPosition::T_Top) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Top"));
		ClosestTransformLocal = LadderTop;
		FTransform ClosestTransformGlobal = UKismetMathLibrary::ComposeTransforms(ClosestTransformLocal, GetActorTransform());

		FVector ClimbLocation = ClosestTransformGlobal.GetLocation();
		Character->SetActorTransform(ClosestTransformGlobal);
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Bottom"));
		ClosestTransformLocal = LadderBottom;
		FTransform ClosestTransformGlobal = UKismetMathLibrary::ComposeTransforms(ClosestTransformLocal, GetActorTransform());

		FVector ClimbLocation = ClosestTransformGlobal.GetLocation();
		ClimbLocation.Z += Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		FTransform CharacterNewTransform = FTransform(ClosestTransformGlobal.GetRotation(), ClimbLocation, ClosestTransformGlobal.GetScale3D());
		Character->SetActorTransform(CharacterNewTransform);
	}
	Character->AttachToActor(this,
		FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true));

	Cast<AMyRPGCharacter>(Character)->GetInteractComponent()->BeginClimb(this);

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "LadderActor.h"

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

	FVector Top = UKismetMathLibrary::TransformLocation(GetActorTransform(), LadderTop.GetLocation());
	FVector Bottom = UKismetMathLibrary::TransformLocation(GetActorTransform(), LadderBottom.GetLocation());

	float DistanceToTop = FVector::DistSquared(Top, CharacterLocation);
	float DistanceToBottom = FVector::DistSquared(Bottom, CharacterLocation);
	if (DistanceToTop < pow(120.f, 2) || DistanceToBottom < pow(120.f, 2)) {

		if (DistanceToTop < DistanceToBottom) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Top"));

			return EPosition::T_Top;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Bottom"));

		return EPosition::T_Bottom;
	}
	return EPosition::T_None;

}

void ALadderActor::HandleInteraction(ACharacter* Character) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ladder Interact"));
	EPosition Test = FindClosestPushTransformIndex(Character->GetActorLocation());

	FTransform ClosestTransformLocal;

	if (Test == EPosition::T_None) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("None"));
		return;
	}
	else if (Test == EPosition::T_Top) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Top"));
		ClosestTransformLocal = LadderTop;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Bottom"));
		ClosestTransformLocal = LadderBottom;
	}
	FTransform ClosestTransformGlobal = UKismetMathLibrary::ComposeTransforms(ClosestTransformLocal, GetActorTransform());

	Character->SetActorTransform(ClosestTransformGlobal);

	Character->AttachToActor(this,
		FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true));

	Cast<AMyRPGCharacter>(Character)->SetIsInteracting(true);

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInteractComponent.h"
#include "../MyRPGCharacter.h"

// Sets default values for this component's properties
UMyInteractComponent::UMyInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	OwningCharacter = Cast<ACharacter>(GetOwner());
	SetComponentTickEnabled(false);
	
}


// Called every frame
void UMyInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AMyRPGCharacter* Player = Cast<AMyRPGCharacter>(OwningCharacter);
	if (!Player) {
		return;
	}

	if (!IsValid(CurrentPushable)) {
		return;
	}


	FVector DeltaLocation = Player->GetActorForwardVector() * (UKismetMathLibrary::FCeil(Player->GetForwardBackValue()) * PushSpeed);
	CurrentPushable->AddActorWorldOffset(DeltaLocation, true);



	// ...
}

void UMyInteractComponent::BeginPush(APushableActor* Pushable) {
	//Check if object being pushed is valid
	if (!IsValid(Pushable)) {
		return;
	}

	//Check if character can push
	if (!OwningCharacter) {
		return;
	}

	//Attach character to pushable as a box
	CurrentPushable = Pushable;
	OwningCharacter->AttachToActor(CurrentPushable, 
		FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true));

	//Disable movement except in one axis
	UCharacterMovementComponent* CharMovementComp = OwningCharacter->GetCharacterMovement();
	CharMovementComp->SetPlaneConstraintEnabled(true);
	CharMovementComp->SetPlaneConstraintNormal(OwningCharacter->GetActorRightVector());
	CharMovementComp->bOrientRotationToMovement = false;
	SetComponentTickEnabled(true);
	
	//Set bool value
	Cast<AMyRPGCharacter>(OwningCharacter)->SetIsInteracting(true);
}
void UMyInteractComponent::EndPush() {
	//Check if CurrentPushable is valid
	if (!IsValid(CurrentPushable)) {
		return;
	}
	CurrentPushable = nullptr;

	//Checking if Character is valid
	if (!OwningCharacter) {
		return;
	}

	OwningCharacter->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false));

	//Reenable movement in all axis
	UCharacterMovementComponent* CharMovementComp = OwningCharacter->GetCharacterMovement();
	CharMovementComp->SetPlaneConstraintEnabled(false);
	CharMovementComp->bOrientRotationToMovement = true;
	SetComponentTickEnabled(false);

	Cast<AMyRPGCharacter>(OwningCharacter)->SetIsInteracting(false);
}
float UMyInteractComponent::GetPushableHeight() {
	//Check if CurrentPushable is valid
	if (!IsValid(CurrentPushable)) {
		return 0;
	}

	//Checking if Character is valid
	if (!OwningCharacter) {
		return 0;
	}

	UActorComponent* MeshComponent = CurrentPushable->GetComponentByClass(UStaticMeshComponent::StaticClass());
	UStaticMeshComponent* PushableMesh = Cast<UStaticMeshComponent>(MeshComponent);

	FVector MinBounds = FVector(0, 0, 0);
	FVector MaxBounds = FVector(0, 0, 0);

	PushableMesh->GetLocalBounds(MinBounds, MaxBounds);

	float TopOfObject = (MaxBounds.Z - MinBounds.Z) + CurrentPushable->GetActorLocation().Z;

	float FeetOfCharacter = OwningCharacter->GetActorLocation().Z - OwningCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	return TopOfObject - FeetOfCharacter;
}

bool UMyInteractComponent::IsPushingObject(){
	return IsValid(CurrentPushable);
	//return (CurrentPushable != nullptr); 
}
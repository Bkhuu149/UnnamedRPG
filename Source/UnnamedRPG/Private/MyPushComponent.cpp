// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPushComponent.h"
#include "../MyRPGCharacter.h"

// Sets default values for this component's properties
UMyPushComponent::UMyPushComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyPushComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMyPushComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMyPushComponent::BeginPush(APushableActor* Pushable) {
	if (!IsValid(Pushable)) {
		return;
	}

	ACharacter* OwningCharacter = Cast<ACharacter>(GetOwner());
	if (!OwningCharacter) {
		return;
	}

	CurrentPushable = Pushable;
	OwningCharacter->AttachToActor(CurrentPushable, 
		FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true));

	UCharacterMovementComponent* CharMovementComp = OwningCharacter->GetCharacterMovement();
	CharMovementComp->SetPlaneConstraintEnabled(true);
	CharMovementComp->SetPlaneConstraintNormal(OwningCharacter->GetActorRightVector());
	CharMovementComp->bOrientRotationToMovement = false;
	SetComponentTickEnabled(true);
	Cast<AMyRPGCharacter>(OwningCharacter)->SetIsInteracting(true);
}
void UMyPushComponent::EndPush() {

}
float UMyPushComponent::GetPushableHeight() {
	return 0;
}

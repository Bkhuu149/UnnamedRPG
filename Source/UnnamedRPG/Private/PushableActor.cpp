// Fill out your copyright notice in the Description page of Project Settings.


#include "PushableActor.h"

// Sets default values
APushableActor::APushableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APushableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APushableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int APushableActor::FindClosestPushTransformIndex1(FVector2D CharacterLocation, float PushRange){
	int CurrentTransformIndex = 0;
	int ClosestTransformIndex = -1;
	float CurrentDistanceSq = 0.f;
	float ClosestDistanceSq = 0.f;

	for (int i = 0; i < PushTransformsTemp.Max(); i++)
	{
		FTransform Transform = PushTransformsTemp[i];
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("CurrentPushTransform %i"), i));
		CurrentTransformIndex = i;
		FVector2D MyLocation = FVector2D(UKismetMathLibrary::TransformLocation(GetActorTransform(), Transform.GetLocation()));
		CurrentDistanceSq = UKismetMathLibrary::DistanceSquared2D(MyLocation, CharacterLocation);
		if (CurrentDistanceSq < pow(PushRange, 2) && ((ClosestDistanceSq < CurrentDistanceSq) || ClosestTransformIndex < 0)) {
			ClosestTransformIndex = CurrentTransformIndex;
			ClosestDistanceSq = CurrentDistanceSq;
		}
	}

	return ClosestTransformIndex;
}

void APushableActor::HandleInteraction1(ACharacter* Character) {

	if (!Character) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, (TEXT("Character not valid")));
		return;
	}

	UMyPushComponent* PushComp = Cast<UMyPushComponent>(Character->GetComponentByClass(UMyPushComponent::StaticClass()));
	if (!PushComp) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, (TEXT("Character does not have push component")));
		return;
	}

	FindClosestPushTransformIndex1(FVector2D(Character->GetActorLocation()), PushComp->PushRange);	

	return;
}



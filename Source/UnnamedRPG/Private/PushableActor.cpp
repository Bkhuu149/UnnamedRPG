// Fill out your copyright notice in the Description page of Project Settings.


#include "PushableActor.h"
#include "MyInteractComponent.h"

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
	//AddActorWorldOffset(GetActorUpVector() * -1, true);
}

int APushableActor::FindClosestPushTransformIndex1(FVector2D CharacterLocation, float PushRange){
	//Find closest push index so we know where to attach actor if we can
	int CurrentTransformIndex = 0;
	int ClosestTransformIndex = -1;
	float CurrentDistanceSq = 0.f;
	float ClosestDistanceSq = 0.f;

	for (int i = 0; i < PushTransformsTemp.Max(); i++)
	{
		FTransform Transform = PushTransformsTemp[i];
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("CurrentPushTransform %i"), i));
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

void APushableActor::HandleInteraction(ACharacter* Character) {

	if (!IsPushable) { return; }

	if (!Character) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, (TEXT("Character not valid")));
		return;
	}

	UMyInteractComponent* InteractComp = Cast<UMyInteractComponent>(Character->GetComponentByClass(UMyInteractComponent::StaticClass()));
	if (!InteractComp) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, (TEXT("Character does not have push component")));
		return;
	}

	float BestIndex = FindClosestPushTransformIndex1(FVector2D(Character->GetActorLocation()), InteractComp->PushRange);	

	if (BestIndex < 0) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, (TEXT("No Push Transform Found")));
		return;
	}

	FTransform PushTransform = UKismetMathLibrary::ComposeTransforms(PushTransformsTemp[BestIndex], GetActorTransform());

	FVector PushLocation = PushTransform.GetLocation();
	PushLocation.Z = PushLocation.Z + Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FTransform CharacterNewTransform = FTransform( PushTransform.GetRotation(), PushLocation, PushTransform.GetScale3D());

	UKismetSystemLibrary::DrawDebugSphere(GetWorld(), CharacterNewTransform.GetLocation(), 20.f, 12, FLinearColor::Blue, 2.f, 1.f);

	FVector Start = CharacterNewTransform.GetLocation();
	FVector End = CharacterNewTransform.GetLocation();
	Start.Z += 70.f;
	End.Z -= 100.f;
	float Radius = Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float HalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	AActor* tableinit[] = { this }; // Add self to ignore list
	TArray<AActor*> IgnoreList;
	IgnoreList.Append(tableinit);
	FHitResult OutHit;
	UKismetSystemLibrary::CapsuleTraceSingle(
		GetWorld(), 
		Start, 
		End, 
		Radius, 
		HalfHeight, 
		TraceTypeQuery1, 
		false, 
		IgnoreList, 
		EDrawDebugTrace::Type::ForDuration, 
		OutHit, 
		true);

	if (OutHit.bStartPenetrating) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, (TEXT("No Room to Stand")));
		return;
	}

	if (Character->GetCharacterMovement()->GetWalkableFloorZ() > OutHit.ImpactNormal.Z) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, (TEXT("Floor Not Walkable")));
		return;
	}

	IgnoreList.Append({ Character });

	TArray<FHitResult> OutHits;
	bool BHit = UKismetSystemLibrary::LineTraceMulti(
		GetWorld(),
		GetActorLocation(),
		CharacterNewTransform.GetLocation(),
		TraceTypeQuery1, false,
		IgnoreList,
		EDrawDebugTrace::Type::ForDuration,
		OutHits,
		true);

	if (BHit) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, (TEXT("Wall Between Character and Object")));
		return;
	}

	Character->SetActorLocation(CharacterNewTransform.GetLocation());
	Character->SetActorRotation(CharacterNewTransform.GetRotation());

	InteractComp->BeginPush(this);

	return;
}

void APushableActor::PushActor(float Strength, FVector Direction, float PushSpeed, float Duration) {

	if (PushTimer.IsValid() || Strength == 0.f) {
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, (TEXT("Want to push")));
	PlayerDirection = Direction;
	PlayerPushStrength = Strength;
	PlayerPushSpeed = PushSpeed;

	TArray<class AActor*> ActorsAttached;
	GetAttachedActors(ActorsAttached, false);
	ACharacter* Player = Cast<ACharacter>(ActorsAttached[0]);
	if (!Player) { return; }
	IsMoving = true;
	//start a timer that pushes box
	GetWorld()->GetTimerManager().SetTimer(PushTimer, this, &APushableActor::MoveActor, .02f, true);

	GetWorld()->GetTimerManager().SetTimer(StopPushTimer, this, &APushableActor::StopPush, .1f, false, 2.f);

	UMyInteractComponent* InteractComp = Cast<UMyInteractComponent>(ActorsAttached[0]->GetComponentByClass(UMyInteractComponent::StaticClass()));
	if (!InteractComp) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, (TEXT("Character does not have push component")));
		return;
	}
	InteractComp->PushDirection = Strength;

}

void APushableActor::MoveActor() {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, (TEXT("Moving")));

	FVector DeltaLocation = PlayerDirection * (UKismetMathLibrary::FCeil(PlayerPushStrength) * PlayerPushSpeed);
	FHitResult* Outhit = nullptr;
	AddActorWorldOffset(DeltaLocation, true, Outhit, ETeleportType::None);
	if (Outhit) {
		StopPush();
		GetWorld()->GetTimerManager().ClearTimer(StopPushTimer);
	}
}

void APushableActor::StopPush() {
	GetWorld()->GetTimerManager().ClearTimer(PushTimer);
	PushTimer.Invalidate();
	TArray<class AActor*> ActorsAttached;
	GetAttachedActors(ActorsAttached, false);
	FCollisionShape Shape = FCollisionShape::MakeBox(FVector(50, 50, 50));
	TArray<AActor*> IgnoreList = { this };
	FHitResult Outhit;
	bool bhit = UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() - FVector(0, 0, 20),
		FVector(30, 30, 30) * GetActorScale(),
		FRotator::ZeroRotator,
		TraceTypeQuery2,
		false,
		IgnoreList,
		EDrawDebugTrace::Type::ForDuration,
		Outhit,
		true);
	ACharacter* Player = Cast<ACharacter>(ActorsAttached[0]);
	if (!Player) { return; }
	IsMoving = false;
	UMyInteractComponent* InteractComp = Cast<UMyInteractComponent>(ActorsAttached[0]->GetComponentByClass(UMyInteractComponent::StaticClass()));
	if (!InteractComp) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, (TEXT("Character does not have push component")));
		return;
	}
	InteractComp->PushDirection = 0.f;
	if (bhit) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Outhit.GetActor()->GetName());
		return;
	}
	InteractComp->EndInteract();
	IsPushable = false;
	AddActorWorldOffset(GetActorUpVector() * -1000, true);
}


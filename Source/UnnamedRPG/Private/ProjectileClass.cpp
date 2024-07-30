// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileClass.h"

// Sets default values
AProjectileClass::AProjectileClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectileClass::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(ColTimer, this, &AProjectileClass::CheckCollision, 0.07, true);
}

// Called every frame
void AProjectileClass::Tick(float DeltaTime)
{
	FVector ForwardVector = GetActorForwardVector() * Speed;
	FVector NewLocation = GetActorLocation() + ForwardVector;
	SetActorLocation(NewLocation);
}

void AProjectileClass::CheckCollision()
{
	FHitResult OutHit;
	TArray<AActor*> IgnoreList;
	AActor* tableinit[] = { this, Owner };
	IgnoreList.Append(tableinit);
	FVector CollisionSize = FVector(32, 32, 32);
	bool bHit = UKismetSystemLibrary::BoxTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation(),
		CollisionSize, FRotator::ZeroRotator, TraceTypeQuery2, false, IgnoreList, EDrawDebugTrace::Type::ForDuration, OutHit, true);
	if (bHit) { HandleCollision(OutHit.GetActor()); }
}

void AProjectileClass::HandleCollision(AActor* HitActor)
{
	if (GetOwner() == HitActor) { return; }
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, UKismetSystemLibrary::GetDisplayName(HitActor));
	ARPGBaseClass* HitRPGActor = Cast<ARPGBaseClass>(HitActor);
	if (HitRPGActor) {
		UGameplayStatics::ApplyDamage(HitRPGActor, 10.0, NULL, GetOwner(), MyType);
	}
	this->K2_DestroyActor();
}
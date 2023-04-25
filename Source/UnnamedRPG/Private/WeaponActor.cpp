// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponActor::PlayTrail() {
	TrailComp = UNiagaraFunctionLibrary::SpawnSystemAttached(Trail, Cast<USceneComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass())), TEXT("Middle"), FVector(0, 0, 0), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);

}

void AWeaponActor::EndTrail() {
	TrailComp->Deactivate();
}

void AWeaponActor::WeaponLineTrace() {
	UActorComponent* WeaponComponent = GetComponentByClass(UStaticMeshComponent::StaticClass());
	UStaticMeshComponent* WeaponMesh = Cast<UStaticMeshComponent>(WeaponComponent);
	FVector StartSocket = WeaponMesh->GetSocketLocation("Start");
	FVector EndSocket = WeaponMesh->GetSocketLocation("End");
	FHitResult OutHit;
	AActor* tableinit[] = { Owner }; // Add self to ignore list
	TArray<AActor*> IgnoreList;
	IgnoreList.Append(tableinit);
	bool bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartSocket, EndSocket, TraceTypeQuery2, false, IgnoreList, EDrawDebugTrace::Type::ForDuration, OutHit, true);
	AActor* Parent = this->GetAttachParentActor();
	UGameplayStatics::ApplyDamage(OutHit.GetActor(), 10.f, NULL, Parent, NULL); // Apply 5 damage to the actor being hit

}

void AWeaponActor::StartLineTrace() {
	GetWorld()->GetTimerManager().SetTimer(ColTimer, this, &AWeaponActor::WeaponLineTrace, 0.01, true);
}

void AWeaponActor::EndLineTrace() {
	if (ColTimer.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(ColTimer);
		ColTimer.Invalidate();
	}
}
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
	SetDamageType(EDamageType::NONE);
}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponActor::WeaponLineTrace() {
	//Draw a trace from weapon hilt to tip of weapon defined by sockets in mesh
	UActorComponent* WeaponComponent = GetComponentByClass(UStaticMeshComponent::StaticClass());
	UStaticMeshComponent* WeaponMesh = Cast<UStaticMeshComponent>(WeaponComponent);
	FVector StartSocket = WeaponMesh->GetSocketLocation("Start");
	FVector EndSocket = WeaponMesh->GetSocketLocation("End");
	FHitResult OutHit;
	AActor* tableinit[] = { Owner }; // Add self to ignore list
	TArray<AActor*> IgnoreList;
	IgnoreList.Append(tableinit);
	bool bHIt = UKismetSystemLibrary::BoxTraceSingle(GetWorld(), StartSocket, EndSocket, FVector(5, 5, 5), FRotator::ZeroRotator, TraceTypeQuery2, false, IgnoreList, EDrawDebugTrace::Type::None, OutHit, true);
	//bool bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartSocket, EndSocket, TraceTypeQuery2, false, IgnoreList, EDrawDebugTrace::Type::None, OutHit, true);
	UGameplayStatics::ApplyDamage(OutHit.GetActor(), Damage, NULL, Owner, MyType); // Apply 5 damage to the actor being hit

}

void AWeaponActor::StartLineTrace() {
	//Start timer for weapon hit detection
	if (Trail) {
		TrailComp = UNiagaraFunctionLibrary::SpawnSystemAttached(Trail, Cast<USceneComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass())), TEXT("Middle"), FVector(0, 0, 0), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
	}
	GetWorld()->GetTimerManager().SetTimer(ColTimer, this, &AWeaponActor::WeaponLineTrace, 0.01, true);
	
}

void AWeaponActor::EndLineTrace() {
	//End timer for weapon hit detection
	if (ColTimer.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(ColTimer);
		ColTimer.Invalidate();
	}
	if (TrailComp) {
		TrailComp->Deactivate();
	}
}

void AWeaponActor::SetDamageType(EDamageType NewType) {
	//Do data table lookup to set correct values
	FName DamageTypeTableKey = FName("Elementless");
	switch (NewType) {
	case EDamageType::NONE:
		DamageTypeTableKey = FName("Elementless");
		break;
	case EDamageType::FIRE:
		DamageTypeTableKey = FName("Fire");
		break;
	case EDamageType::WATER:
		DamageTypeTableKey = FName("Water");
		break;
	case EDamageType::EARTH:
		DamageTypeTableKey = FName("Earth");
		break;
	case EDamageType::WIND:
		DamageTypeTableKey = FName("Wind");
		break;
	case EDamageType::ICE:
		DamageTypeTableKey = FName("Ice");
		break;
	case EDamageType::LIGHTNING:
		DamageTypeTableKey = FName("Lightning");
		break;
	case EDamageType::SAND:
		DamageTypeTableKey = FName("Sand");
		break;
	case EDamageType::SMOKE:
		DamageTypeTableKey = FName("Smoke");
		break;
	}

	FDamageTypeStruct* DamageTypeData = DamageTypeTab->FindRow<FDamageTypeStruct>(DamageTypeTableKey, "");

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("MyEnum is %s"), *UEnum::GetValueAsString(NewType)));
	
	if (NewType != EDamageType::NONE) {
		Damage = Damage * .75;
	}

	MyType = DamageTypeData->Type;
	if (DamageTypeData->TypeWeaponEffect){
		ElementParticles = DamageTypeData->TypeWeaponEffect;
	}
	//Still need to set weapon vfx and trail to reflect what element an attack is
	if (ElementParticles) {
		UNiagaraComponent* WeaponEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(ElementParticles, Cast<USceneComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass())), TEXT(""), FVector(0, 0, 0), FRotator::ZeroRotator, EAttachLocation::SnapToTargetIncludingScale, true);
		//Need to scale partcles based on what weapon it is, below is for broadsword
		WeaponEffect->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));
		
	}

	if (DamageTypeData->TypeWeaponTrail) {
		//Need to make more weapon trails for each element
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTwoHandedClass.h"

AEnemyTwoHandedClass::AEnemyTwoHandedClass()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyTwoHandedClass::BeginPlay() 
{
	Super::BeginPlay();
	const FTransform WeaponTransform = GetMesh()->GetSocketTransform("OffhandSocket", ERelativeTransformSpace::RTS_World);
	CurrentOffhandWeapon = Cast<AWeaponActor>(GetWorld()->SpawnActor<AActor>(ChosenOffhandWeapon, WeaponTransform));
	CurrentOffhandWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "OffhandSocket");
	CurrentOffhandWeapon->SetOwner(this);
	CurrentOffhandWeapon->SetDamage(InitialDamage);
	CurrentOffhandWeapon->SetDamageType(OffhandWeaponType);
}

void AEnemyTwoHandedClass::BeginOffhandSwordEvent() {
	//This is called during the attack animation notify
	CurrentOffhandWeapon->StartLineTrace();
}

void AEnemyTwoHandedClass::EndOffhandSwordEvent() {
	//This is called during the attack animation notify
	CurrentOffhandWeapon->EndLineTrace();
}
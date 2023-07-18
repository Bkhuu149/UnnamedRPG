// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBaseClass.h"

// Sets default values
ARPGBaseClass::ARPGBaseClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Interruptable = true;
}

// Called when the game starts or when spawned
void ARPGBaseClass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARPGBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPGBaseClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ARPGBaseClass::DamageChar(float val, EDamageType Type) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Damage"));

	if (IsDead || IsInvincible) { return false; }
	if (Health - val <= 0) {
		KillCharacter();
		return true;
	}
	Health -= val;
	if (Interruptable) { PlayAnimMontage(HitReactAnim); }
	StartInvincibility();
	//GetWorld()->GetTimerManager().SetTimer(InvincibiltyTimer, this, &ARPGBaseClass::ResetInvincibility, 0.5, false);
	return true;

}

void ARPGBaseClass::KillCharacter() {
	Health = 0;
	IsDead = true;
	UAnimMontage* DeathMontage = DeathAnims[FMath::FRandRange(0, DeathAnims.Num())];
	PlayAnimMontage(DeathMontage);
	GetWorld()->GetTimerManager().SetTimer(DisableColTimer, [&]() { SetActorEnableCollision(false); 	PrimaryActorTick.bCanEverTick = false;}, DeathMontage->GetPlayLength(), false);
}

void ARPGBaseClass::HealChar(float val) {
	if (val <= 0) { return; }
	if (val > HealthMax - Health) {
		Health = HealthMax;
		return;
	}
	Health += val;
}

void ARPGBaseClass::BeginSwordEvent() {
	//This is called during the attack animation notify
	CurrentWeapon->StartLineTrace();
	CurrentWeapon->PlayTrail();
}

void ARPGBaseClass::EndSwordEvent() {
	//This is called during the attack animation notify
	CurrentWeapon->EndLineTrace();
	CurrentWeapon->EndTrail();
}

void ARPGBaseClass::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) {
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	if (GetCharacterMovement()->IsFalling()) { CheckSpeed(); }
}

void ARPGBaseClass::CheckSpeed(float FallDamage) {
	if (GetCharacterMovement()->IsFalling()) {
		FVector ZVelocity = FVector(0, 0, GetVelocity().Z);
		FallDamage = FMath::GetMappedRangeValueClamped(FVector2D(1500.f, 2000.f), FVector2D(0.f, 100.f), float(ZVelocity.Size()));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Fall: %f"), FallDamage));
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &ARPGBaseClass::CheckSpeed, FallDamage);
		GetWorld()->GetTimerManager().SetTimer(CheckSpeedTimer, Delegate, 0.001, false);
	} else if (FallDamage > 0){
		//DamageChar(FallDamage);
		if (Health - FallDamage <= 0) {
			KillCharacter();
			return;
		}
		Health -= FallDamage;
	}
}
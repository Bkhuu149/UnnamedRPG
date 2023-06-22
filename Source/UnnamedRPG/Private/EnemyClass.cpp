// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyClass.h"

// Called to bind functionality to input
AEnemyClass::AEnemyClass()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));
	Interruptable = true;

}

void AEnemyClass::BeginPlay()
{
	Super::BeginPlay();
	//Set spawn location to get center of walk radius
	SpawnLocation = GetActorLocation();
	MyController = static_cast<AAIController*>(GetController());
	NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	const FTransform WeaponTransform = GetMesh()->GetSocketTransform("WeaponSocket", ERelativeTransformSpace::RTS_World);
	CurrentWeapon = Cast<AWeaponActor>(GetWorld()->SpawnActor<AActor>(ChosenWeapon, WeaponTransform));
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");
	CurrentWeapon->SetOwner(this);
	CurrentWeapon->SetDamage(Damage);
	if (WalkPath.Num() == 0) {
		WalkPath.Add(GetActorTransform());
	}
	else {
		WalkPath[0] = GetActorTransform();
	}

	for (int i = 0; i < WalkPath.Num(); i++) {
		WalkPath[i] = UKismetMathLibrary::ComposeTransforms(WalkPath[i], GetActorTransform());
	}

	CurrentEnemyState = EEnemyState::IDLE;

}

void AEnemyClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDead || GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) { return; }

	TickStateMachine();
	Rotate(DeltaTime);
	//Use Pawn Sensing component to find player
	//Set player as target if found
	//Else, move randomly

	// If already at target, stop and attack
	/*
	if (FollowResult == EPathFollowingRequestResult::AlreadyAtGoal && CurrWalkState == FOLLOW) {
		Rotate(DeltaTime);
		Attack();
	}
	//If Targeted, approach target. Else, walk to random point within spawn radius
	if (Targeted) {
		DelayTimer.Invalidate();
		if (FVector::Distance(Target->GetActorLocation(), WalkPath[CurrentPathNode].GetLocation()) > 1000.f) {
			ResetTarget();
			return;
		}
		Walk();
	} else {
		if (!DelayTimer.IsValid()){
			GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &AEnemyClass::Walk, 10.f, false);
		}
	}*/
}

void AEnemyClass::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyClass::TickStateMachine() {
	switch (CurrentEnemyState) {
	case EEnemyState::IDLE:
		StateIdle();
		break;
	case EEnemyState::PATH_WALKING:
		StatePathWalking();
		break;
	case EEnemyState::CHASE_CLOSE:
		StateChaseClose();
		break;
	case EEnemyState::CHASE_FAR:
		StateChaseFar();
		break;
	case EEnemyState::ATTACK:
		StateAttack();
		break;
	case EEnemyState::STAGGERED:
		StateStaggered();
		break;
	case EEnemyState::DEAD:
		StateDead();
		break;
	}
}

void AEnemyClass::StateIdle() {

	if (Targeted) {
		DelayTimer.Invalidate();
		CurrentEnemyState = EEnemyState::CHASE_CLOSE;
		return;
	}
	if (DelayTimer.IsValid()) {
		return;
	}
	StatePathWalking();
}

void AEnemyClass::StatePathWalking() {
	if (!NavSys || WalkPath.Num() == 0) { return; }
	if (Targeted) {
		DelayTimer.Invalidate();
		MyController->StopMovement();
		CurrentEnemyState = EEnemyState::CHASE_CLOSE;
		return;
	}
	CurrentEnemyState = EEnemyState::PATH_WALKING;
	if (DelayTimer.IsValid()) {
		return;
	}
	if (!MyController->IsFollowingAPath()) {
		CurrentEnemyState = EEnemyState::IDLE;
		CurrentPathNode++;
		if (CurrentPathNode >= WalkPath.Num()) {
			CurrentPathNode = 0;
		}
		FVector Location = WalkPath[CurrentPathNode].GetLocation();
		FollowResult = MyController->MoveToLocation(Location, 100.f);
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, [&]() {DelayTimer.Invalidate();}, 10.f, false);

	}
}

void AEnemyClass::StateChaseClose() { 
	if (!Target) { return; }
	/*if (FVector::Distance(Target->GetActorLocation(), WalkPath[CurrentPathNode].GetLocation()) > 1000) {
		MyController->StopMovement();
		ResetTarget();
		CurrentEnemyState = EEnemyState::IDLE;
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, [&]() {DelayTimer.Invalidate();}, 10.f, false);

	}
	else */if (Target && FVector::Distance(GetActorLocation(), Target->GetActorLocation()) < 125)
	{
		FVector PlayerDirection = Target->GetActorLocation() - GetActorLocation();
		float LookingDirection = GetActorForwardVector().Dot(PlayerDirection.GetSafeNormal());
		if (LookingDirection > 0.90f && !IsCoolingDown && !Staggered) {
			CurrentEnemyState = EEnemyState::ATTACK;
		}
	}
	else
	{
		FVector Location = Target->GetActorLocation();
		if (!MyController->IsFollowingAPath()) {
			MyController->MoveToActor(Target, 50.f, true, true, false);
		}
	}
}

void AEnemyClass::StateChaseFar() {

}

void AEnemyClass::StateAttack() {
	if (Target && FVector::Distance(GetActorLocation(), Target->GetActorLocation()) < 125) {
		Attack();
	}
	else {
		CurrentEnemyState = EEnemyState::CHASE_CLOSE;
	}
}

void AEnemyClass::StateStaggered() {

}

void AEnemyClass::StateDead() {

}




void AEnemyClass::Walk() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Walk"));

	if (IsDead) { return; }

	//if Target, walk to target.  Else, go to random point
	FVector Location;
	
	//Determine where the enemy should walk
	if (Target)
	{
		CurrWalkState = FOLLOW;
		Location = Target->GetActorLocation();
		FollowResult = MyController->MoveToActor(Target, 50.f, true, true, false);

	} else {
		if (!NavSys || WalkPath.Num() == 0) { return; }
		CurrWalkState = RANDOM;

		CurrentPathNode++;

		if (CurrentPathNode >= WalkPath.Num()) {
			CurrentPathNode = 0;
		}

		Location = WalkPath[CurrentPathNode].GetLocation();
		FollowResult = MyController->MoveToLocation(Location, 100.f);
	}

	//Walk to location
	DelayTimer.Invalidate();
}

void AEnemyClass::ResetTarget() {
	//Call to reset target values
	Targeted = false;
	MyController->StopMovement();
	Target = nullptr;
}

void AEnemyClass::Rotate(float DeltaTime)
{
	//Slowly rotate enemy towards character
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() || !Targeted) { return; }
	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();
	CurrentLocation.Z = 0;
	TargetLocation.Z = 0;
	FRotator CharacterLookRotator = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
	FRotator CharacterRInterpVal = UKismetMathLibrary::RInterpTo(GetActorRotation(), CharacterLookRotator, DeltaTime, 10.f);
	SetActorRotation(CharacterRInterpVal);
}

void AEnemyClass::Attack() 
{
	//Performs random attack in AttackAnimClose array
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() || IsCoolingDown) { return; }
	MyController->StopMovement();
	int AttackIndex = FMath::RandRange(0, AttackAnimClose.Num()-1);
	PlayAnimMontage(AttackAnimClose[AttackIndex]);
	IsCoolingDown = true;
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, [&]() { IsCoolingDown = false; }, CooldownTime, false);
}
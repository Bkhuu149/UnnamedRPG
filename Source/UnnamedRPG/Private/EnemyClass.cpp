// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyClass.h"

// Called to bind functionality to input
AEnemyClass::AEnemyClass()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));

}

void AEnemyClass::BeginPlay()
{
	Super::BeginPlay();
	//Set spawn location to get center of walk radius
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

	for (int i = 1; i < WalkPath.Num(); i++) {
		WalkPath[i] = UKismetMathLibrary::ComposeTransforms(WalkPath[i], GetActorTransform());
	}

	CurrentEnemyState = EEnemyState::IDLE;

}

void AEnemyClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDead || GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) { return; }
	//Enemy State Machine the determines what the enemy is doing
	TickStateMachine();
	if (Target) {
		//Rotate Enemy to player if targeted
		Rotate(DeltaTime);
	}
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
	}
}

void AEnemyClass::StateIdle() {

	if (Targeted) {
		//Exit idle state and move into chasing the player
		DelayTimer.Invalidate();
		CurrentEnemyState = EEnemyState::CHASE_CLOSE;
		return;
	}
	if (DelayTimer.IsValid()) {
		return;
	}
	//Go into path walking in order to determine what should the next state be
	StatePathWalking();
}

void AEnemyClass::StatePathWalking() {
	//If walkpath doesn't exist, then do nothing
	if (!NavSys || WalkPath.Num() == 0) { return; }
	if (Targeted) {
		//If found target, then chase player
		DelayTimer.Invalidate();
		MyController->StopMovement();
		CurrentEnemyState = EEnemyState::CHASE_CLOSE;
		return;
	}
	//Else, walk along the path
	CurrentEnemyState = EEnemyState::PATH_WALKING;
	if (DelayTimer.IsValid()) {
		return;
	}
	if (!MyController->IsFollowingAPath()) {
		//Not currently following a path, find next node in path and walk to it
		CurrentEnemyState = EEnemyState::IDLE;
		CurrentPathNode++;
		if (CurrentPathNode >= WalkPath.Num()) {
			CurrentPathNode = 0;
		}
		FVector Location = WalkPath[CurrentPathNode].GetLocation();
		FollowResult = MyController->MoveToLocation(Location, 100.f);
		if (DelayTimer.IsValid()) {
			DelayTimer.Invalidate();
		}
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, [&]() {DelayTimer.Invalidate();}, 10.f, false);

	}
}

void AEnemyClass::StateChaseClose() { 
	if (!Target) { return; } //No target, return
	if (FVector::Distance(Target->GetActorLocation(), WalkPath[CurrentPathNode].GetLocation()) > 1000) {
		//Player moved too far away from current place along walk path, go back to path
		MyController->StopMovement();
		ResetTarget();
		CurrentEnemyState = EEnemyState::IDLE;
		if (DelayTimer.IsValid()) {
			DelayTimer.Invalidate();
		}
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, [&]() {DelayTimer.Invalidate();}, 10.f, false);

	}
	else if (Target && FVector::Distance(GetActorLocation(), Target->GetActorLocation()) < 125)
	{
		//Player is close enough to perform melee attack
		FVector PlayerDirection = Target->GetActorLocation() - GetActorLocation();
		float LookingDirection = GetActorForwardVector().Dot(PlayerDirection.GetSafeNormal());
		if (LookingDirection > 0.90f && !IsCoolingDown && !Staggered) {
			CurrentEnemyState = EEnemyState::ATTACK;
		}
	}
	else if (Target && FVector::Distance(GetActorLocation(), Target->GetActorLocation()) < 500)
	{
		//Player is medium distance away, try walking to player
		FVector Location = Target->GetActorLocation();
		if (!MyController->IsFollowingAPath()) {
			MyController->MoveToActor(Target, 50.f, true, true, false);
		}
	}
	else{
		//Player is far away from enemy, perform chase attack
		CurrentEnemyState = EEnemyState::CHASE_FAR;
	}
}

void AEnemyClass::StateAttack() {
	if (!Target) { 
		//No Target or they moved away from enemy, try chasing
		CurrentEnemyState = EEnemyState::CHASE_CLOSE; 
		return; 
	}
	float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
	if (Distance < 125) {
		//Target still close, attack
		Attack();
	}
	else{
		//Target far away, try following
		CurrentEnemyState = EEnemyState::CHASE_CLOSE;
	}
}

void AEnemyClass::StateChaseFar() {
	//Player is far away, perform ranged attack
	MyController->StopMovement();
	int AttackIndex = FMath::RandRange(0, AttackAnimFar.Num() - 1);
	PlayAnimMontage(AttackAnimFar[AttackIndex]);
	CurrentEnemyState = EEnemyState::CHASE_CLOSE;
	
}

void AEnemyClass::StartDash() {
	//Start dash hitbox timer
	GetWorld()->GetTimerManager().SetTimer(DashTimer, this, &AEnemyClass::DashTrace, 0.2, true);
}

void AEnemyClass::DashTrace() {
	//Performs a sphere trace every couple of seconds for dash
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { EObjectTypeQuery::ObjectTypeQuery3 };
	TArray<AActor*> IgnoreList = { this };
	TArray<AActor*> OutHits;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 300, ObjectTypes, NULL, IgnoreList, OutHits);
	//DrawDebugSphere(GetWorld(), Barrier->GetActorLocation(), 300, 12, FColor::Blue, true, 10, 0, 2);
	for (AActor* Enemy : OutHits) {
		AMyRPGCharacter* Temp = Cast<AMyRPGCharacter>(Enemy);
		if (Temp) {
			UGameplayStatics::ApplyDamage(Enemy, 10, NULL, this, NULL);
		}
	}
}

void AEnemyClass::EndDash() {
	//Stop dash hitbox timer
	if (DashTimer.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(DashTimer);
		DashTimer.Invalidate();
	}
}

void AEnemyClass::ResetTarget() {
	//Call to reset target values
	Targeted = false;
	//Remove self from player's NumCombatants
	Cast<AMyRPGCharacter>(Target)->RemoveCombatant(this);
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
	if (AttackTimer.IsValid()) {
		AttackTimer.Invalidate();
	}
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, [&]() { IsCoolingDown = false; }, FMath::RandRange(CooldownTime -3.f, CooldownTime + 3.f), false);
}

bool AEnemyClass::DamageChar(float val) {
	bool bHit = Super::DamageChar(val);
	//Reset attack timer to allow player to finish their combo
	if (bHit) { 
		if (AttackTimer.IsValid()) {
			AttackTimer.Invalidate();
		}
		GetWorld()->GetTimerManager().SetTimer(AttackTimer, [&]() { IsCoolingDown = false; }, CooldownTime, false); 
	}
	return bHit;
}

void AEnemyClass::KillCharacter() {
	Super::KillCharacter();
	Cast<AMyRPGCharacter>(Target)->RemoveCombatant(this);
}
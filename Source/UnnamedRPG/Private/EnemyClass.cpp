// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyClass.h"

// Called to bind functionality to input
AEnemyClass::AEnemyClass()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));
	StatusComp = CreateDefaultSubobject<UEnemyStatusComponent>(TEXT("Enemy Status Component"));
}

void AEnemyClass::BeginPlay()
{
	Super::BeginPlay();
	//Set spawn location to get center of walk radius
	MyController = static_cast<AAIController*>(GetController());
	NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	CurrentCooldownTime = InitialCooldownTime;
	CurrentDamage = InitialDamage;
	CurrentAttackSpeed = InitialAttackSpeed;
	MyController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AEnemyClass::OnMoveCompleted);

	const FTransform WeaponTransform = GetMesh()->GetSocketTransform("WeaponSocket", ERelativeTransformSpace::RTS_World);
	CurrentWeapon = Cast<AWeaponActor>(GetWorld()->SpawnActor<AActor>(ChosenWeapon, WeaponTransform));
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");
	CurrentWeapon->SetOwner(this);
	CurrentWeapon->SetDamage(InitialDamage);
	CurrentWeapon->SetDamageType(WeaponType);
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
		GetWorld()->GetTimerManager().ClearTimer(DelayTimer);
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
		GetWorld()->GetTimerManager().ClearTimer(DelayTimer);
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
		FVector Location = WalkPath[CurrentPathNode].GetLocation();
		FollowResult = MyController->MoveToLocation(Location, 100.f);
		if (FVector::Distance(Location, GetActorLocation()) < 1 && !DelayTimer.IsValid()) { NextPathNode(true); } //TEMP, shouldn't rely on DelayTimer
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
			GetWorld()->GetTimerManager().ClearTimer(DelayTimer);
			DelayTimer.Invalidate();
		}
		//GetWorld()->GetTimerManager().SetTimer(DelayTimer, [&]() {DelayTimer.Invalidate();}, 10.f, false);

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
	CurrentEnemyState = EEnemyState::CHASE_CLOSE;
	if (!CanAttackRanged) { return; }
	MyController->StopMovement();
	int AttackIndex = FMath::RandRange(0, AttackAnimFar.Num() - 1);
	PlayAnimMontage(AttackAnimFar[AttackIndex], CurrentAttackSpeed);
	
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
			UGameplayStatics::ApplyDamage(Enemy, CurrentDamage, NULL, this, MyType);
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
	SetHealth(GetHealthMax());
	GetCharacterMovement()->MaxWalkSpeed = CalculateNewSpeed();
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
	if (IsBlind) {
		//If enemy is blind, add random rotation about the z axis to have the enemy's attacks be less accurate
		FRotator RandBlindRot = FRotator(0, FMath::FRandRange(-60, 60), 0);
		AddActorLocalRotation(RandBlindRot);
	}
	MyController->StopMovement();
	int AttackIndex = FMath::RandRange(0, AttackAnimClose.Num()-1);
	PlayAnimMontage(AttackAnimClose[AttackIndex], CurrentAttackSpeed);
	IsCoolingDown = true;
	if (AttackTimer.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
		AttackTimer.Invalidate();
	}
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, [&]() { IsCoolingDown = false; }, FMath::RandRange(CurrentCooldownTime -3.f, CurrentCooldownTime + 3.f), false);
}

bool AEnemyClass::DamageChar(float val, EDamageType Type, bool IsStrong) {
	bool bHit = Super::DamageChar(val, Type, IsStrong);
	//Reset attack timer to allow player to finish their combo
	if (bHit) { 
		StatusComp->AddDebuff(Type, val);
		if (AttackTimer.IsValid()) {
			GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
			AttackTimer.Invalidate();
		}
		if (DelayTimer.IsValid()) {
			GetWorld()->GetTimerManager().ClearTimer(DelayTimer);
			DelayTimer.Invalidate();
		}
		GetWorld()->GetTimerManager().SetTimer(AttackTimer, [&]() { IsCoolingDown = false; }, FMath::RandRange(CurrentCooldownTime - 3.f, CurrentCooldownTime + 3.f), false);
	}
	return bHit;
}

void AEnemyClass::KillCharacter() {
	Super::KillCharacter();
	MyController->StopMovement();
	AMyRPGCharacter* Player = Cast<AMyRPGCharacter>(Target);
	if (Player) {
		Player->GetExperienceComponent()->GiveXP(EXPDropped);
		Player->RemoveCombatant(this);
	}

	if (DelayTimer.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(DelayTimer);
		DelayTimer.Invalidate();
	}

	if (AttackTimer.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
		AttackTimer.Invalidate();
	}

	if (DashTimer.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(DashTimer);
		DashTimer.Invalidate();
	}

	if (ParalysisTimer.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(ParalysisTimer);
		ParalysisTimer.Invalidate();
	}
}

void AEnemyClass::DisableChar() {
	Super::DisableChar();
	StatusComp->PrimaryComponentTick.bCanEverTick = false;
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyClass::DoFireTickDamage() {
	SetHealth(GetHealth() - 5);
	if (GetHealth() <= 0) {
		KillCharacter();
		StatusComp->RemoveEffect(EStatus::BURN);
	}
}

float AEnemyClass::CalculateNewSpeed() {
	
	float Speed = (Targeted) ? EnemyRunningSpeed : EnemyWalkingSpeed;
	return Speed * SpeedMultiplier;
}

void AEnemyClass::StartParalysis() {
	GetWorld()->GetTimerManager().SetTimer(ParalysisTimer, this, &AEnemyClass::TriggerStun, FMath::RandRange(3, 6), false);
}

void AEnemyClass::EndParalysis() {
	GetWorld()->GetTimerManager().ClearTimer(ParalysisTimer);
	ParalysisTimer.Invalidate();
}

void AEnemyClass::TriggerStun() {
	ResetInvincibility();
	CurrentWeapon->EndLineTrace();
	if (IsDead) { return; }
	GetMesh()->GetAnimInstance()->StopAllMontages(1.f);
	PlayAnimMontage(StunAnim);
	GetWorld()->GetTimerManager().SetTimer(ParalysisTimer, this, &AEnemyClass::TriggerStun, FMath::RandRange(3, 6), false);
}

void AEnemyClass::InstantRotateToActor(AActor* OtherActor) {
	//Instant rotation to actor, normally should be used before knockback
	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = OtherActor->GetActorLocation();
	CurrentLocation.Z = 0;
	TargetLocation.Z = 0;
	FRotator CharacterLookRotator = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
	SetActorRotation(CharacterLookRotator);
}

void AEnemyClass::NextPathNode(bool Forward) {
	if (Forward) {
		CurrentPathNode++;
		if (CurrentPathNode >= WalkPath.Num()) {
			CurrentPathNode = 0;
		}
	} else {
		CurrentPathNode--;
		if (CurrentPathNode < 0) {
			CurrentPathNode = WalkPath.Num() - 1;
		}
	}
}

void AEnemyClass::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (CurrentEnemyState != EEnemyState::PATH_WALKING) { return; }

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Evil Schmoove Done")));
	NextPathNode(true);

	if (DelayTimer.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(DelayTimer);
		DelayTimer.Invalidate();
	}
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, [&]() {
		DelayTimer.Invalidate(); }, 10.f, false);
}
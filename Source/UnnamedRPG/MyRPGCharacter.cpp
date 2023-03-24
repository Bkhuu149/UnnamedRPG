// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRPGCharacter.h"

// Sets default values
AMyRPGCharacter::AMyRPGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilityComp = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));

}

// Called when the game starts or when spawned
void AMyRPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	AbilityComp->InitAbilityActorInfo(this, this);


	//CAN ONLY HAVE 3 Attacks for now
	AttackCombo.Add(FName(TEXT("SpearAttack1")));
	//AttackCombo.Add(FName(TEXT("SpearAttack2")));
	//AttackCombo.Add(FName(TEXT("GreatSwordAttack1")));
	//AttackCombo.Add(FName(TEXT("GreatSwordAttack3")));
	AttackCombo.Add(FName(TEXT("SwordAttack1")));
	AttackCombo.Add(FName(TEXT("GreatSwordAttack5")));
	//AttackCombo.Add(FName(TEXT("SwordAttack4")));
	//AttackCombo.Add(FName(TEXT("GreatSwordAttack6")));
	//AttackCombo.Add(FName(TEXT("SwordAttack2")));
	//AttackCombo.Add(FName(TEXT("SwordAttack3")));
	//AttackCombo.Add(FName(TEXT("SwordAttack5")));

	//FINISHERS ONLY 1
	//Finisher = FName(TEXT("SwordAttack2"));
	Finisher = FName(TEXT("GreatSwordAttack2"));
	//Finisher = FName(TEXT("GreatSwordAttack4"));
	//Finisher = FName(TEXT("GreatSwordAttack7"));

}

// Called every frame
void AMyRPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Targeted) {
		FocusTarget(DeltaTime);
	}

	ForwardBackInputValue = GetInputAxisValue("ForwardBack");
	RightLeftInputValue = GetInputAxisValue("RightLeft");

}

// Called to bind functionality to input
void AMyRPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("ForwardBack"), this, &AMyRPGCharacter::MoveForwardBack);
	PlayerInputComponent->BindAxis(TEXT("RightLeft"), this, &AMyRPGCharacter::MoveRightLeft);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnJumpedPressed);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &AMyRPGCharacter::ReleaseJump);

	PlayerInputComponent->BindAction(TEXT("Target"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnTargetPressed);

	PlayerInputComponent->BindAction(TEXT("Block"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnBlockPressed);
	PlayerInputComponent->BindAction(TEXT("Block"), EInputEvent::IE_Released, this, &AMyRPGCharacter::OnBlockReleased);

	PlayerInputComponent->BindAction(TEXT("Heal"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnHealPressed);

	PlayerInputComponent->BindAction(TEXT("Dodge"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnDodgePressed);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnSprintPressed);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMyRPGCharacter::OnSprintReleased);

	PlayerInputComponent->BindAction(TEXT("Attack 1"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnAttackPressed);

	PlayerInputComponent->BindAction(TEXT("Finisher"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::DoFinisher);

	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnInteractPressed);

	PlayerInputComponent->BindAction(TEXT("Menu"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnMenuPressed);

	PlayerInputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnInventoryPressed);

	PlayerInputComponent->BindAction(TEXT("TEMPKILL"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::KILL);
}

void AMyRPGCharacter::KILL() {
	DamageChar(10);
}

void AMyRPGCharacter::MoveForwardBack(float value) 
{
	if (IsInteracting) { return; }
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) { return; }
	IsAttacking = false;
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	Direction.Z = 0;
	Direction.Normalize();
	if (RightLeftInputValue != 0) {
		value = value * .7071;
	}

	AddMovementInput(Direction, value * SprintMultiplier);
}

void AMyRPGCharacter::MoveRightLeft(float value)
{
	if (IsInteracting) { return; }
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) { return; }
	IsAttacking = false;
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	Direction.Z = 0;
	if (ForwardBackInputValue != 0) {
		value = value * .7071;
	}
	AddMovementInput(Direction, value * SprintMultiplier);
}

void AMyRPGCharacter::OnBlockPressed() {
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) { return; }
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Blocked Pressed"));
	//Start a timer to see how long this button was pressed to distinguish between blocking and parrying
	IsBlocking = true;
	PressedTime = FDateTime::Now().GetTicks();
	PlayAnimMontage(BlockAnim);

}

void AMyRPGCharacter::OnBlockReleased() {

	//Condiiton should check if timer is past a certain time limit. If true, then perform parry.  False would release block
	int64 CurrentTime = FDateTime::Now().GetTicks();

	if ((CurrentTime - PressedTime)/1000 <= PARRY_THRESH) {
		PlayAnimMontage(ParryAnim);
	}
	StopAnimMontage(BlockAnim);

	IsBlocking = false;
	return;

}

void AMyRPGCharacter::OnHealPressed() {
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) { return; }
	if (Mana < 100.0) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Not enough Mana"));
		return;
	}
	Mana = 0.0;
	HealChar(20.0);
	IncrementMana();
}

void AMyRPGCharacter::OnDodgePressed() {
	if (GetCharacterMovement()->IsFalling() || GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) { return; }
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Dodge"));
	IsDodging = true;
	float duration = PlayAnimMontage(DodgeAnim);
	GetWorld()->GetTimerManager().SetTimer(DodgeTimer, this, &AMyRPGCharacter::DodgeFinished, duration, false);
	if (ForwardBackInputValue < 0) {
		IsBackDodging = true;
	}
	if (Targeted && !IsSprinting)
	{
		SetActorRotation(GetActorRotation() + FVector(ForwardBackInputValue, RightLeftInputValue, 0).Rotation());
	}
}

void AMyRPGCharacter::DodgeFinished() {
	IsDodging = false;
	IsBackDodging = false;
	if (DodgeTimer.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(DodgeTimer);
		DodgeTimer.Invalidate();
	}
}

void AMyRPGCharacter::OnJumpedPressed() {
	bool falling = GetCharacterMovement()->IsFalling();
	if (falling || GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) {
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Jump"));
	Jump();
	IsJumping = true;
	GetWorld()->GetTimerManager().SetTimer(JumpTimer, this, &AMyRPGCharacter::ReleaseJump, 1.f, false);
}

void AMyRPGCharacter::ReleaseJump() {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Jump Released"));
	if (GetWorld()->GetTimerManager().IsTimerActive(JumpTimer)) {
		GetWorld()->GetTimerManager().ClearTimer(JumpTimer);
	}
	StopJumping();
	IsJumping = false;
}

void AMyRPGCharacter::OnTargetPressed() {
	if (IsDodging) { return; }
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Targeting"));
	if (Targeted) {
		ResetTarget();
		return;
	}
	FVector TraceStart = GetActorLocation();
	FVector TraceDistance = 1000.f * (UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation().Vector());
	FVector TraceEnd = TraceStart + TraceDistance;
	
	FHitResult OutHit;

	bool HitResult;

	FCollisionQueryParams Params = FCollisionQueryParams();
	Params.AddIgnoredActor(this);

	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(500.0f);

	HitResult = GetWorld()->SweepSingleByChannel(OutHit, TraceStart, TraceEnd, FVector(0, 0, 0).ToOrientationQuat(), 
		ECC_GameTraceChannel1, MyColSphere, Params);

	Target = OutHit.HitObjectHandle.FetchActor();

	Targeted = HitResult;
}

void AMyRPGCharacter::OnAttackPressed() {
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) { return; }
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack"));
	if (AttackCount >= CurrentMaxAttackCount) {
		DoFinisher();
		return;
	}
	
	FAttackStruct* TestAttack = AbilityTab->FindRow<FAttackStruct>(AttackCombo[AttackCount], "");
	
	const FTransform WeaponTransform = GetMesh()->GetSocketTransform("WeaponSocket", ERelativeTransformSpace::RTS_World);
	CurrentWeapon = GetWorld()->SpawnActor<AActor>(TestAttack->Weapon, WeaponTransform);
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");

	FGameplayAbilitySpec Test = FGameplayAbilitySpec(TestAttack->Attack.GetDefaultObject(), 1, 0);
	AbilityComp->GiveAbilityAndActivateOnce(Test);

	FTimerHandle AnimTimer;
	GetWorld()->GetTimerManager().SetTimer(AnimTimer, [&]() {
		if (CurrentWeapon != nullptr) {
			GetWorld()->DestroyActor(CurrentWeapon);
		}
		GetWorld()->GetTimerManager().ClearTimer(AnimTimer);
		AnimTimer.Invalidate();
	}, TestAttack->Attack.GetDefaultObject()->MontageToPlay->GetPlayLength() * 0.9, false);

	IsAttacking = true;
	AttackCount++;

	if (AttackTimer.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
		AttackTimer.Invalidate();
	}
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AMyRPGCharacter::ResetAttack, TestAttack->Attack.GetDefaultObject()->MontageToPlay->GetPlayLength() + 2.f, false);
}

void AMyRPGCharacter::BeginSwordEvent() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("lol"));
	GetWorld()->GetTimerManager().SetTimer(ColTimer, this, &AMyRPGCharacter::WeaponLineTrace, 0.01, true);
}

void AMyRPGCharacter::EndSwordEvent() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("lmao"));
	if (ColTimer.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(ColTimer);
		ColTimer.Invalidate();
	}
}

void AMyRPGCharacter::WeaponLineTrace() {
	UActorComponent* WeaponComponent = CurrentWeapon->GetComponentByClass(UStaticMeshComponent::StaticClass());
	UStaticMeshComponent* WeaponMesh = Cast<UStaticMeshComponent>(WeaponComponent);
	FVector StartSocket = WeaponMesh->GetSocketLocation("Start");
	FVector EndSocket = WeaponMesh->GetSocketLocation("End");
	FHitResult OutHit;
	AActor* tableinit[] = { this }; // Add self to ignore list
	TArray<AActor*> IgnoreList;
	IgnoreList.Append(tableinit);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartSocket, EndSocket, TraceTypeQuery2, false, IgnoreList, EDrawDebugTrace::Type::ForDuration, OutHit, true);
	UGameplayStatics::ApplyDamage(OutHit.GetActor(), 5.f, NULL, NULL, NULL);
}

void AMyRPGCharacter::ResetAttack() {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack Reset"));
	if (GetWorld()->GetTimerManager().IsTimerActive(AttackTimer)) {
		GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
	}
	AttackCount = 0;
	IsAttacking = false;
}

void AMyRPGCharacter::DoFinisher() {
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) { return; }
	
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Finisher Pressed"));

	FAttackStruct* FinisherAttack = AbilityTab->FindRow<FAttackStruct>(Finisher, "");
	//If the attack is not a finisher, return for safety
	if (!FinisherAttack->IsFinisher) { return; }

	FTimerHandle AnimTimer;
	GetWorld()->GetTimerManager().SetTimer(AnimTimer, [&]() {
		if (CurrentWeapon != nullptr) {
			GetWorld()->DestroyActor(CurrentWeapon);
		}
		GetWorld()->GetTimerManager().ClearTimer(AnimTimer);
		AnimTimer.Invalidate();
	}, FinisherAttack->Attack.GetDefaultObject()->MontageToPlay->GetPlayLength() * 0.9, false);

	const FTransform WeaponTransform = GetMesh()->GetSocketTransform("WeaponSocket", ERelativeTransformSpace::RTS_World);
	CurrentWeapon = GetWorld()->SpawnActor<AActor>(FinisherAttack->Weapon, WeaponTransform);
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");

	FGameplayAbilitySpec FinalAttack = FGameplayAbilitySpec(FinisherAttack->Attack.GetDefaultObject(), 1, 0);
	IsAttacking = true;
	AbilityComp->GiveAbilityAndActivateOnce(FinalAttack);
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AMyRPGCharacter::ResetAttack, FinisherAttack->Attack.GetDefaultObject()->MontageToPlay->GetPlayLength(), false);

	//ResetAttack();
}

void AMyRPGCharacter::OnInteractPressed() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Interact"));
}

void AMyRPGCharacter::OnMenuPressed() {
	InMenu = !InMenu;
	if (InMenu) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Open Menu"));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Close Menu"));
	}
}

void AMyRPGCharacter::OnInventoryPressed() {
	InInventory = !InInventory;
	if (InInventory) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Open Inventory"));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Close Inventory"));
	}
}

void AMyRPGCharacter::OnSprintPressed() {
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) { return; }
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Sprinting"));
	SprintMultiplier = 3.f;
	IsSprinting = true;
	//bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	
}

void AMyRPGCharacter::OnSprintReleased() {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Stop Sprinting"));
	SprintMultiplier = .5f;
	IsSprinting = false;
	//if (Target) {
		//bUseControllerRotationYaw = true;
		//GetCharacterMovement()->bOrientRotationToMovement = false;
	//}
}

void AMyRPGCharacter::FocusTarget(float DeltaTime) {
	if (!Target) {
		ResetTarget();
		return;
	}

	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();
	float Distance = (CurrentLocation - TargetLocation).Length();
	if (Distance > 2000) {
		ResetTarget();
		return;
	}

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
	FRotator CameraRotation = GetController()->GetControlRotation();
	FRotator RinterpVal = UKismetMathLibrary::RInterpTo(CameraRotation, LookAtRotation, DeltaTime, 10.0);
	GetController()->SetControlRotation(RinterpVal);
	CurrentLocation.Z = 0;
	TargetLocation.Z = 0;
	FRotator CharacterLookRotator = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);

	bool IsSprintandMove = IsSprinting && !GetVelocity().IsZero();

	GetCharacterMovement()->bOrientRotationToMovement = (IsSprintandMove) ? true : false;

	if ((!(IsSprintandMove || IsBackDodging || IsAttacking))) {
		float LerpTime = (IsDodging) ? 1.f : 10.f;
		FRotator CharacterRotationInterpVal = UKismetMathLibrary::RInterpTo(GetActorRotation(), CharacterLookRotator, DeltaTime, LerpTime);
		SetActorRotation(CharacterRotationInterpVal);
		
	}
}

void AMyRPGCharacter::ResetTarget() {
	Target = nullptr;
	Targeted = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AMyRPGCharacter::DamageChar(float val) {
	Super::DamageChar(val);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Damaged: %f"), Health));

}

void AMyRPGCharacter::HealChar(float val) {
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) { return; }
	Super::HealChar(val);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Healed: %f"), Health));
}

void AMyRPGCharacter::RestoreMana() {
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) { return; }
	if (Mana < ManaMax) {
		Mana += .5;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Mana: %f"), Mana));
	}
	if (ManaTimer.IsValid() && ManaMax == Mana) {
		GetWorld()->GetTimerManager().ClearTimer(ManaTimer);
		ManaTimer.Invalidate();
	}
}

void AMyRPGCharacter::IncrementMana() {
	GetWorld()->GetTimerManager().SetTimer(ManaTimer, this, &AMyRPGCharacter::RestoreMana, .05f, true);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRPGCharacter.h"

// Sets default values
AMyRPGCharacter::AMyRPGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AbilityComp = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	InteractComp = CreateDefaultSubobject<UMyInteractComponent>(TEXT("Interact Component"));
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
	AttackSkillComp = CreateDefaultSubobject<UAttackSkillComponent>(TEXT("Attack Skill Component"));
	StatusComp = CreateDefaultSubobject<UStatusComponent>(TEXT("Status Component"));
}

// Called when the game starts or when spawned
void AMyRPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	//AbilityComp->InitAbilityActorInfo(this, this);
}

// Called every frame
void AMyRPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Targeted) {
		FocusTarget(DeltaTime);
		TransitionCamera(DeltaTime, RightCamPosition);
	} else {
		TransitionCamera(DeltaTime, NormalCamPosition);
	}
	
	FRotator CameraRotation;
	FRotator RinterpVal;
	ForwardBackInputValue = GetInputAxisValue("ForwardBack");
	RightLeftInputValue = GetInputAxisValue("RightLeft");
	switch (MyCurrentState) {
	case EPlayerState::INTERACTING:
		CameraRotation = GetController()->GetControlRotation();
		RinterpVal = UKismetMathLibrary::RInterpTo(CameraRotation, GetActorForwardVector().ToOrientationRotator(), DeltaTime, 10.f);
		GetController()->SetControlRotation(RinterpVal);
		break;
	case EPlayerState::SPRINTING:
		if (ForwardBackInputValue != 0 || RightLeftInputValue != 0) {
			CurrentStamina -= .1 * StaminaDrainMultiplier;
		}
		else if (CurrentStamina < StaminaMax) {
			CurrentStamina += .5 * StaminaRecoveryMultiplier;
			FMath::Clamp(CurrentStamina, 0, StaminaMax);
		}
		if (CurrentStamina <= 0) {
			OnSprintReleased();
		}
		break;
	case EPlayerState::IDLE:
		if (CurrentStamina < StaminaMax) {
			CurrentStamina += .5 * StaminaRecoveryMultiplier;
			FMath::Clamp(CurrentStamina, 0, StaminaMax);
		}
		break;
	default:
		break;
	}
	/*
	if (IsInteracting) {
		FRotator CameraRotation = GetController()->GetControlRotation();
		FRotator RinterpVal = UKismetMathLibrary::RInterpTo(CameraRotation, GetActorForwardVector().ToOrientationRotator(), DeltaTime, 10.f);
		GetController()->SetControlRotation(RinterpVal);
	}

	ForwardBackInputValue = GetInputAxisValue("ForwardBack");
	RightLeftInputValue = GetInputAxisValue("RightLeft");

	if (IsSprinting && (ForwardBackInputValue != 0 || RightLeftInputValue != 0)) {
		CurrentStamina -= .1 * StaminaDrainMultiplier;
	}
	else if (!GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() && CurrentStamina != StaminaMax ){
		CurrentStamina += .5;
	}
	CurrentStamina = FMath::Clamp(CurrentStamina, 0, StaminaMax);
	if (CurrentStamina == 0) {
		IsSprinting = false;
		OnSprintReleased();
	}*/

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

	PlayerInputComponent->BindAction(TEXT("Heal"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnHealPressed);

	PlayerInputComponent->BindAction(TEXT("Dodge"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnDodgePressed);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnSprintPressed);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMyRPGCharacter::OnSprintReleased);

	PlayerInputComponent->BindAction(TEXT("Attack 1"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnAttackPressed);

	PlayerInputComponent->BindAction(TEXT("Finisher"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::DoFinisher);

	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnInteractPressed);

	PlayerInputComponent->BindAction(TEXT("Menu"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnMenuPressed);

	PlayerInputComponent->BindAction(TEXT("TEMPKILL"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::KILL);
	


}

void AMyRPGCharacter::KILL() {
	
	AttackSkillComp->AddAttack(FName("SwordAttack1"));
	AttackSkillComp->AddAttack(FName("SwordAttack2"));
	AttackSkillComp->AddAttack(FName("SwordAttack3"));
	AttackSkillComp->AddAttack(FName("SwordAttack4"));
	AttackSkillComp->AddAttack(FName("SwordAttack5"));
	AttackSkillComp->AddAttack(FName("SwordAttack6"));
	AttackSkillComp->AddAttack(FName("SwordAttack7"));

	AttackSkillComp->AddAttack(FName("KatanaAttack1"));
	AttackSkillComp->AddAttack(FName("KatanaAttack2"));
	AttackSkillComp->AddAttack(FName("KatanaAttack3"));
	AttackSkillComp->AddAttack(FName("KatanaAttack4"));
	AttackSkillComp->AddAttack(FName("KatanaAttack5"));
	AttackSkillComp->AddAttack(FName("KatanaAttack6"));
	AttackSkillComp->AddAttack(FName("KatanaAttack7"));

	AttackSkillComp->AddAttack(FName("GreatSwordAttack1"));
	AttackSkillComp->AddAttack(FName("GreatSwordAttack2"));
	AttackSkillComp->AddAttack(FName("GreatSwordAttack3"));
	AttackSkillComp->AddAttack(FName("GreatSwordAttack4"));
	AttackSkillComp->AddAttack(FName("GreatSwordAttack5"));
	AttackSkillComp->AddAttack(FName("GreatSwordAttack6"));	
	AttackSkillComp->AddAttack(FName("GreatSwordAttack7"));

	AttackSkillComp->AddAttack(FName("SpearAttack1"));
	AttackSkillComp->AddAttack(FName("SpearAttack2"));
	AttackSkillComp->AddAttack(FName("SpearAttack3"));
	AttackSkillComp->AddAttack(FName("SpearAttack4"));
	AttackSkillComp->AddAttack(FName("SpearAttack5"));
	AttackSkillComp->AddAttack(FName("SpearAttack6"));
	AttackSkillComp->AddAttack(FName("SpearAttack7"));

	InventoryComp->AddToInventory(FName("HealthPotion"));
	InventoryComp->AddToInventory(FName("ManaPotion"));
	InventoryComp->AddToInventory(FName("TeleportScroll"));
	InventoryComp->AddToInventory(FName("AttackFood"));
	InventoryComp->AddToInventory(FName("StaminaFood"));
	InventoryComp->AddToInventory(FName("DefenseFood"));
	InventoryComp->AddToInventory(FName("WaterScroll"));
	InventoryComp->AddToInventory(FName("FireScroll"));
	InventoryComp->AddToInventory(FName("WindScroll"));
	InventoryComp->AddToInventory(FName("EarthScroll"));
	InventoryComp->AddToInventory(FName("Blanket"));
	InventoryComp->AddToInventory(FName("Duster"));
	InventoryComp->AddToInventory(FName("StaticRemover"));
	InventoryComp->AddToInventory(FName("EyeDrop"));
	



}


void AMyRPGCharacter::MoveForwardBack(float value) 
{
	if (MyCurrentState == EPlayerState::INTERACTING) { return; }
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) { return; }
	if (value != 0 && (MyCurrentState == EPlayerState::ATTACKING)) {
		ResetAttack();
	}
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
	if (MyCurrentState == EPlayerState::INTERACTING) { return; }
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) { return; }
	if (value != 0 && (MyCurrentState == EPlayerState::ATTACKING)) {
		ResetAttack();
	}
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	Direction.Z = 0;
	if (ForwardBackInputValue != 0) {
		value = value * .7071;
	}
	AddMovementInput(Direction, value * SprintMultiplier);
}

void AMyRPGCharacter::OnBlockPressed() {
	if (MyCurrentState != EPlayerState::IDLE || IsDead || IsRegeningMana || GetMesh()->GetAnimInstance()->Montage_IsPlaying(LevelStartAnim)) {
		return;
	}
	Mana -= 50;
	Mana = FMath::Clamp(Mana, 0, ManaMax);
	if (Mana <= 0) {
		IncrementMana();
		IsRegeningMana = true;
	}
	MyCurrentState = EPlayerState::BLOCKING;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Blocked Pressed"));
	PlayAnimMontage(ParryAnim);

}

void AMyRPGCharacter::OnHealPressed() {
	//if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() || IsInteracting || IsRegeningMana) { return; }
	if (MyCurrentState != EPlayerState::IDLE || IsRegeningMana) { return; }
	PlayAnimMontage(HealAnim);
	Mana = 0.0;
	HealChar(GetHealthMax());
	IncrementMana();
	IsRegeningMana = true;
}

void AMyRPGCharacter::OnDodgePressed() {
	if (GetCharacterMovement()->IsFalling() || 
		GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() || 
		(MyCurrentState == EPlayerState::INTERACTING) ||
		30 * StaminaDrainMultiplier > CurrentStamina
		) { return; }
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Dodge"));
//	IsDodging = true;
	MyCurrentState = EPlayerState::DODGING;
	CurrentStamina -= 30 * StaminaDrainMultiplier;
	FMath::Clamp(CurrentStamina, 0, StaminaMax);
	float duration = PlayAnimMontage(DodgeAnim);
	GetWorld()->GetTimerManager().SetTimer(DodgeTimer, this, &AMyRPGCharacter::DodgeFinished, duration, false);
	if (ForwardBackInputValue < 0) {
		IsBackDodging = true;
	}
	if (Targeted && (MyCurrentState != EPlayerState::SPRINTING))
	{
		SetActorRotation(GetActorRotation() + FVector(ForwardBackInputValue, RightLeftInputValue, 0).Rotation());
	}
}

void AMyRPGCharacter::DodgeFinished() {
//	IsDodging = false;
	IsBackDodging = false;
	MyCurrentState = EPlayerState::IDLE;
	if (DodgeTimer.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(DodgeTimer);
		DodgeTimer.Invalidate();
	}
}

void AMyRPGCharacter::OnJumpedPressed() {
	bool falling = GetCharacterMovement()->IsFalling();
	if (!CanPlayerJump || falling || GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() || MyCurrentState == EPlayerState::INTERACTING) {
		return;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Jump"));
	Jump();
	CurrentStamina -= 20;
	FMath::Clamp(CurrentStamina, 0, StaminaMax);
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
	if (MyCurrentState == EPlayerState::DODGING || IsBlind) { return; }
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

	Target = Cast<ARPGBaseClass>(OutHit.GetActor());

	Targeted = HitResult;
}

void AMyRPGCharacter::OnAttackPressed() {
	if ((GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() || (MyCurrentState == EPlayerState::INTERACTING))&&(MyCurrentState != EPlayerState::ATTACKING)) { return; }
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack"));
	FAttackStruct* AttackRow = AttackSkillComp->GetComboAttack(AttackCount);	
	if (!AttackRow) { return; }
	if (AttackRow->StaminaDrain * StaminaDrainMultiplier > CurrentStamina){ return; }
	if (MyCurrentState == EPlayerState::ATTACKING) {
		SavedAttack = AttackQueuedType::COMBO;
		return;
	}
	float AttackLength = PerformAttack(AttackRow);
	CurrentWeapon->SetDamageType(AttackSkillComp->GetAttackAugment(AttackCount-1));
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AMyRPGCharacter::ResetAttack, AttackLength, false);
	UpdateAttackBar();
}

void AMyRPGCharacter::PerformSavedAttack() {
	float AttackLength = 0.f;
	FAttackStruct* AttackRow;
	//FAttackStruct* FinisherAttack;
	switch (SavedAttack) {
	case AttackQueuedType::NONE:
		return;
	case AttackQueuedType::COMBO:
		SavedAttack = AttackQueuedType::NONE;
		if (AttackCount >= CurrentMaxAttackCount) {
			break;
		}
		AttackRow = AttackSkillComp->GetComboAttack(AttackCount);
		if (!AttackRow) { return; }
		AttackLength = PerformAttack(AttackRow);
		CurrentWeapon->SetDamageType(AttackSkillComp->GetAttackAugment(AttackCount-1));
		GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AMyRPGCharacter::ResetAttack, AttackLength, false);
		UpdateAttackBar();
		return;
	}
	SavedAttack = AttackQueuedType::NONE;
	AttackRow = AttackSkillComp->GetFinisherAttack();
	if (!(AttackRow && AttackRow->IsFinisher)) { return; }
	AttackLength = PerformAttack(AttackRow);
	CurrentWeapon->SetDamageType(AttackSkillComp->GetFinisherAugment());
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AMyRPGCharacter::ResetAttack, AttackLength, false);
	return;
	
}

void AMyRPGCharacter::ResetAttack() {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack Reset"));
	if (GetWorld()->GetTimerManager().IsTimerActive(AttackTimer)) {
		GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
	}
	MyCurrentState = EPlayerState::IDLE;
	AttackCount = 0;
//	IsAttacking = false;
	SavedAttack = AttackQueuedType::NONE;
	UpdateAttackBar();
}

void AMyRPGCharacter::DoFinisher() {
	if ((GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() || (MyCurrentState == EPlayerState::INTERACTING)) && (MyCurrentState != EPlayerState::ATTACKING)) { return; }
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Finisher Pressed"));
	FAttackStruct* FinisherAttack = AttackSkillComp->GetFinisherAttack();
	if (!(FinisherAttack && FinisherAttack->IsFinisher)) { return; }
	if (FinisherAttack->StaminaDrain * StaminaDrainMultiplier > CurrentStamina) { return; }
	if (MyCurrentState == EPlayerState::ATTACKING) {
		SavedAttack = AttackQueuedType::FINISHER;
		return;
	}

	float AttackLength = PerformAttack(FinisherAttack);
	CurrentWeapon->SetDamageType(AttackSkillComp->GetFinisherAugment());
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AMyRPGCharacter::ResetAttack, AttackLength, false); 
}

float AMyRPGCharacter::PerformAttack(FAttackStruct* Attack) {
	if ((Attack->StaminaDrain * StaminaDrainMultiplier > CurrentStamina)) { return 0; }
	CurrentStamina -= Attack->StaminaDrain * StaminaDrainMultiplier;
	if (Targeted) {
		//Rotate Player towards targeted enemy if there is one
		FRotator RotationToTarget = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
		RotationToTarget.Pitch = 0;
		SetActorRotation(RotationToTarget);
	}
	//FGameplayAbilitySpec AttackAbility;
	float AttackLength = 0.1;
	if (IsFemale) {
		//AttackAbility = FGameplayAbilitySpec(Attack->FemaleAttack.GetDefaultObject(), 1, 0);
		AttackLength = Attack->FemaleAttackAnimMontage->GetPlayLength();
		PlayAnimMontage(Attack->FemaleAttackAnimMontage);
	}
	else {
		//AttackAbility = FGameplayAbilitySpec(Attack->MaleAttack.GetDefaultObject(), 1, 0);
		AttackLength = Attack->MaleAttackAnimMontage->GetPlayLength();
		PlayAnimMontage(Attack->MaleAttackAnimMontage);
	}
	FTimerHandle AnimTimer;
	/*GetWorld()->GetTimerManager().SetTimer(AnimTimer, [&]() {
		if (CurrentWeapon != nullptr) {
			GetWorld()->DestroyActor(CurrentWeapon);
		}
		GetWorld()->GetTimerManager().ClearTimer(AnimTimer);
		AnimTimer.Invalidate();
		}, AttackLength * 0.9, false);*/
	AttackCount++;
	const FTransform WeaponTransform = GetMesh()->GetSocketTransform("WeaponSocket", ERelativeTransformSpace::RTS_World);
	CurrentWeapon = Cast<AWeaponActor>(GetWorld()->SpawnActor<AActor>(Attack->Weapon, WeaponTransform));
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");
	CurrentWeapon->SetOwner(this);
	CurrentWeapon->SetDamage((AttackCount)*Attack->Damage * AttackDamageMultiplier * (1 - AttackDebuffMultiplier));
	//IsAttacking = true;
	MyCurrentState = EPlayerState::ATTACKING;
	//AbilityComp->GiveAbilityAndActivateOnce(AttackAbility);
	return AttackLength;
}

void AMyRPGCharacter::OnInteractPressed() {

	//Stop pushing if we are pushing something
	if (InteractComp->GetInteractType() != EInteractType::None) {
		InteractComp->EndInteract();
		return;
	}

	if (Targeted) { return; }
	if (InCombat)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Can't use: In combat"));
		return;
	}
	if (GetCharacterMovement()->IsFalling()) { return; }

	FVector Center = GetActorLocation();
	Center.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {EObjectTypeQuery::ObjectTypeQuery2};

	TArray<AActor*> IgnoreList = {this};

	TArray<AActor*> OutHits;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Center, InteractComp->PushRange, ObjectTypes, NULL, IgnoreList, OutHits);

	for (int i = 0; i < OutHits.Num(); i++) {
		IInteractableInterface* InteractableObject = Cast<IInteractableInterface>(OutHits[i]);
		if (InteractableObject) {
			if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) {
				GetMesh()->GetAnimInstance()->StopAllMontages(0.f);
				GetCharacterMovement()->Velocity = FVector::ZeroVector;
			}
			
			InteractableObject->HandleInteraction(this);
			break;
		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Interact"));
}

void AMyRPGCharacter::OnMenuPressed() {

}


void AMyRPGCharacter::OnSprintPressed() {
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() || MyCurrentState == EPlayerState::INTERACTING) { return; }
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Sprinting"));
	SprintMultiplier = 1.f;
	MyCurrentState = EPlayerState::SPRINTING;
//	IsSprinting = true;
	//bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	
}

void AMyRPGCharacter::OnSprintReleased() {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Stop Sprinting"));
	SprintMultiplier = .5f;
	MyCurrentState = EPlayerState::IDLE;
//	IsSprinting = false;
	//if (Target) {
		//bUseControllerRotationYaw = true;
		//GetCharacterMovement()->bOrientRotationToMovement = false;
	//}
}

void AMyRPGCharacter::FocusTarget(float DeltaTime) {
	if (!Target || Target->GetIsDead()) {
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

	bool IsSprintandMove = (MyCurrentState == EPlayerState::SPRINTING) && !GetVelocity().IsZero();

	GetCharacterMovement()->bOrientRotationToMovement = (IsSprintandMove) ? true : false;

	if ((!(IsSprintandMove || IsBackDodging || MyCurrentState == EPlayerState::ATTACKING))) {
		float LerpTime = (MyCurrentState == EPlayerState::DODGING) ? 1.f : 10.f;
		FRotator CharacterRotationInterpVal = UKismetMathLibrary::RInterpTo(GetActorRotation(), CharacterLookRotator, DeltaTime, LerpTime);
		SetActorRotation(CharacterRotationInterpVal);
		
	}
}

void AMyRPGCharacter::TransitionCamera(float DeltaTime, FVector3d CamPosition) {
	USpringArmComponent* CameraBoom = FindComponentByClass<USpringArmComponent>();
	FVector VinterpVal = UKismetMathLibrary::VInterpTo(CameraBoom->SocketOffset, CamPosition, DeltaTime, 10.0);
	CameraBoom->SocketOffset = VinterpVal;
}

void AMyRPGCharacter::ResetTarget() {
	Target = nullptr;
	Targeted = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

bool AMyRPGCharacter::DamageChar(float val, EDamageType Type) {
	bool bHit = Super::DamageChar(EnemyDamageMultiplier * val, Type);
	if (Barrier && !bHit) {
		BarrierHit = true;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Damaged: %f"), GetHealth()));
	if (bHit && CurrentWeapon) {
		GetWorld()->DestroyActor(CurrentWeapon);
	}
	
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("MyEnum is %s"), *UEnum::GetValueAsString(Type)));
	if (!BarrierHit && bHit) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Adding Debuff: %s"), *UEnum::GetValueAsString(Type)));

		StatusComp->AddDebuff(Type, val);
	}
	return bHit;
}

void AMyRPGCharacter::DoFireTickDamage() {
	SetHealth(GetHealth()-5);
	if (GetHealth() <= 0) {
		KillCharacter();
		StatusComp->RemoveEffect(EStatus::BURN);
	}
}


void AMyRPGCharacter::HealChar(float val) {
	Super::HealChar(val);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Healed: %f"), Health));
}

void AMyRPGCharacter::KillCharacter() {
	Super::KillCharacter();

	SaveInventory();
}

void AMyRPGCharacter::RestoreMana() {
	if (Mana < ManaMax) {
		Mana += .025 * ManaRecoveryMultiplier;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Mana: %f"), Mana));
	}
	if (Mana >= ManaMax) {
		Mana = ManaMax;
		IsRegeningMana = false;
		GetWorld()->GetTimerManager().ClearTimer(ManaTimer);
		ManaTimer.Invalidate();
	}
}

void AMyRPGCharacter::RestoreChar() {
	AddMana(GetManaMax());
	HealChar(GetHealthMax());
	CurrentStamina = GetStaminaMax();
}

void AMyRPGCharacter::IncrementMana() {
	GetWorld()->GetTimerManager().SetTimer(ManaTimer, this, &AMyRPGCharacter::RestoreMana, .01f, true);
}

void AMyRPGCharacter::AddManaAttacking() {
	Mana += ManaRecoveryMultiplier * (4 * (AttackCount+1));
	Mana = FMath::Clamp(Mana, 0, ManaMax);
}

void AMyRPGCharacter::AddMana(float Amount) {
	Mana += Amount;
	Mana = FMath::Clamp(Mana, 0, ManaMax);
}


void AMyRPGCharacter::StartBarrier() {
	Barrier = GetWorld()->SpawnActor<AActor>(BarrierClass, GetActorTransform());
	Barrier->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
}

void AMyRPGCharacter::EndBarrier() {
	if (BarrierHit){
		BarrierHit = false;

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { EObjectTypeQuery::ObjectTypeQuery3 };
		TArray<AActor*> IgnoreList = { this };
		TArray<AActor*> OutHits;
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Barrier->GetActorLocation(), 300, ObjectTypes, NULL, IgnoreList, OutHits);
		//DrawDebugSphere(GetWorld(), Barrier->GetActorLocation(), 300, 12, FColor::Blue, true, 10, 0, 2);
		for (AActor* Enemy: OutHits) {
			ARPGBaseClass* Temp = Cast<ARPGBaseClass>(Enemy);
			if (Temp) { Temp->SetInterruptable(true); }
			UGameplayStatics::ApplyDamage(Enemy, 10, NULL, this, BarrierDamageType);
		}
		FTransform ParticleSpawnTransform = FTransform(GetActorRotation(), GetActorLocation(), FVector(.5, .5, .5));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BarrierParticle, ParticleSpawnTransform);

	}
	GetWorld()->DestroyActor(Barrier);
	MyCurrentState = EPlayerState::IDLE;
}

void AMyRPGCharacter::StartCombatTimer() {
	if (GetWorld()->GetTimerManager().IsTimerActive(CombatTimer)) {
		GetWorld()->GetTimerManager().ClearTimer(CombatTimer);
	}
	InCombat = true;
	UpdateCombatStatus(InCombat);
	GetWorld()->GetTimerManager().SetTimer(CombatTimer, this, &AMyRPGCharacter::CombatTimerEnd, CombatTimerLength, false);
}

void AMyRPGCharacter::CombatTimerEnd() {
	if (GetWorld()->GetTimerManager().IsTimerActive(CombatTimer)) {
		GetWorld()->GetTimerManager().ClearTimer(CombatTimer);
	}
	InCombat = false;
	UpdateCombatStatus(InCombat);
}

void AMyRPGCharacter::AddCombatant(AActor* Combatant) {
	CombatantArray.Emplace(Combatant);
	InCombat = true;
	UpdateCombatStatus(InCombat);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("NumCombatants: ++"));
}

void AMyRPGCharacter::RemoveCombatant(AActor* Combatant) {
	CombatantArray.Remove(Combatant);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("NumCombatants: --"));
	if (CombatantArray.Num() == 0) {
		StartCombatTimer();
	}
}

void AMyRPGCharacter::StartParalysis() {
	GetWorld()->GetTimerManager().SetTimer(ParalysisTimer, this, &AMyRPGCharacter::TriggerStun, FMath::RandRange(4,10), false);
}

void AMyRPGCharacter::EndParalysis() {
	GetWorld()->GetTimerManager().ClearTimer(ParalysisTimer);
	ParalysisTimer.Invalidate();
}

void AMyRPGCharacter::TriggerStun() {
	if (Barrier) {
		BarrierHit = false;
		GetWorld()->DestroyActor(Barrier);
	}
	if (CurrentWeapon) {
		GetWorld()->DestroyActor(CurrentWeapon);
	}
	MyCurrentState = EPlayerState::IDLE;
	ResetInvincibility();
	if (IsDead) { return; }
	GetMesh()->GetAnimInstance()->StopAllMontages(1.f);
	PlayAnimMontage(StunAnim);
	GetWorld()->GetTimerManager().SetTimer(ParalysisTimer, this, &AMyRPGCharacter::TriggerStun, FMath::RandRange(4, 10), false);


}

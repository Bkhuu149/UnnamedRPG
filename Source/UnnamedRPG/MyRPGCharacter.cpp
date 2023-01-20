// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRPGCharacter.h"

// Sets default values
AMyRPGCharacter::AMyRPGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyRPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyRPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FocusTarget();

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
}

void AMyRPGCharacter::MoveForwardBack(float value) {
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, value * SprintMultiplier);
}

void AMyRPGCharacter::MoveRightLeft(float value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, value * SprintMultiplier);
}

void AMyRPGCharacter::OnBlockPressed() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Blocked Pressed"));
	//Start a timer to see how long this button was pressed to distinguish between blocking and parrying
	IsBlocking = true;
	PressedTime = FDateTime::Now().GetTicks();


}

void AMyRPGCharacter::OnBlockReleased() {

	//Condiiton should check if timer is past a certain time limit. If true, then perform parry.  False would release block
	int64 CurrentTime = FDateTime::Now().GetTicks();

	if ((CurrentTime - PressedTime)/1000 <= PARRY_THRESH) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Parry"));
		IsBlocking = false;
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Block"));
	IsBlocking = false;
	return;

}

void AMyRPGCharacter::OnHealPressed() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Heal"));
}

void AMyRPGCharacter::OnDodgePressed() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Dodge"));
}

void AMyRPGCharacter::OnJumpedPressed() {
	bool falling = GetCharacterMovement()->IsFalling();
	if (falling) {
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Jump"));
	Jump();
	IsJumping = true;
	GetWorld()->GetTimerManager().SetTimer(JumpTimer, this, &AMyRPGCharacter::ReleaseJump, 1.f, false);
}

void AMyRPGCharacter::ReleaseJump() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Jump Released"));
	if (GetWorld()->GetTimerManager().IsTimerActive(JumpTimer)) {
		GetWorld()->GetTimerManager().ClearTimer(JumpTimer);
	}
	StopJumping();
	IsJumping = false;
}

void AMyRPGCharacter::OnTargetPressed() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Targeting"));
}

void AMyRPGCharacter::OnAttackPressed() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack"));
	AttackCount++;
	if (AttackCount >= CurrentMaxAttackCount) {
		DoFinisher();
		return;
	}

	if (AttackTimer.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
		AttackTimer.Invalidate();
	}
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AMyRPGCharacter::ResetAttack, 2.f, false);
	

}

void AMyRPGCharacter::ResetAttack() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack Reset"));
	if (GetWorld()->GetTimerManager().IsTimerActive(AttackTimer)) {
		GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
	}
	AttackCount = 0;
}

void AMyRPGCharacter::DoFinisher() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Finisher Pressed"));
	ResetAttack();
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
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Sprinting"));
	SprintMultiplier = 3.f;
	IsSprinting = true;
}

void AMyRPGCharacter::OnSprintReleased() {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Stop Sprinting"));
	SprintMultiplier = .5f;
	IsSprinting = false;
}

void AMyRPGCharacter::FocusTarget() {
	if (!Target) {
		return;
	}
	
	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();
	float Distance = (CurrentLocation - TargetLocation).Length();
	if (Distance > 2000) {
		Target = nullptr;
		Targeted = false;
		return;
	}
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
	GetController()->SetControlRotation(LookAtRotation);
}
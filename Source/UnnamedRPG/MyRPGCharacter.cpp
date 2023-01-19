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

}

// Called to bind functionality to input
void AMyRPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("ForwardBack"), this, &AMyRPGCharacter::MoveForwardBack);
	PlayerInputComponent->BindAxis(TEXT("RightLeft"), this, &AMyRPGCharacter::MoveRightLeft);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnJumpedPressed);

	PlayerInputComponent->BindAction(TEXT("Target"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnTargetPressed);

	PlayerInputComponent->BindAction(TEXT("Block"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnBlockPressed);
	PlayerInputComponent->BindAction(TEXT("Block"), EInputEvent::IE_Released, this, &AMyRPGCharacter::OnBlockReleased);

	PlayerInputComponent->BindAction(TEXT("Heal"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnHealPressed);

	PlayerInputComponent->BindAction(TEXT("Dodge"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnDodgePressed);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnSprintPressed);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMyRPGCharacter::OnSprintReleased);

	PlayerInputComponent->BindAction(TEXT("Attack 1"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnAttackPressed);

	PlayerInputComponent->BindAction(TEXT("Finsher"), EInputEvent::IE_Pressed, this, &AMyRPGCharacter::OnFinisherPressed);

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
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Jump"));
}

void AMyRPGCharacter::ReleaseJump() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Jump Released"));
}

void AMyRPGCharacter::OnTargetPressed() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Targeting"));
}

void AMyRPGCharacter::OnAttackPressed() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack"));
}

void AMyRPGCharacter::ResetAttack() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack Reset"));
}

void AMyRPGCharacter::OnFinisherPressed() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Finisher Pressed"));
}

void AMyRPGCharacter::OnInteractPressed() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Interact"));
}

void AMyRPGCharacter::OnMenuPressed() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Open Menu"));
}

void AMyRPGCharacter::OnInventoryPressed() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Open Inventory"));
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
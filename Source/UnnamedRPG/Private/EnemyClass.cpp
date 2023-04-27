// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyClass.h"
#include "Components/SphereComponent.h"

// Called to bind functionality to input
AEnemyClass::AEnemyClass()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));
	
	//TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Sphere"));
	//TriggerSphere->InitSphereRadius(500.f);
	//TriggerSphere->SetCollisionProfileName(TEXT("Trigger"));
	//TriggerSphere->SetupAttachment(RootComponent);

	//TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyClass::OnOverlapBegin);


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

	for (int i = 0; i < WalkPath.Num(); i++) {
		WalkPath[i] = UKismetMathLibrary::ComposeTransforms(WalkPath[i], GetActorTransform());
	}

}

void AEnemyClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDead || GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) { return; }

	//Use Pawn Sensing component to find player
	//Set player as target if found
	//Else, move randomly

	// If already at target, stop and attack
	if (FollowResult == EPathFollowingRequestResult::AlreadyAtGoal && CurrWalkState == FOLLOW) {

		int Chance = FMath::RandRange(0, 2);

		switch (Chance) {
			case 1:
				Rotate(DeltaTime);
				Attack();
				break;
			case 2:
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Circle"));
				break;
				//RangeAttack()

		}
	}
	//If Targeted, approach target. Else, walk to random point within spawn radius
	if (Targeted) {
		DelayTimer.Invalidate();
		if (FVector::Distance(Target->GetActorLocation(), SpawnLocation) > 1000.f) {
			ResetTarget();
			return;
		}
		Walk();
	} else {
		if (!DelayTimer.IsValid()){
			GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &AEnemyClass::Walk, 10.f, false);
		}
	}
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
		//FNavLocation ReachableLocation;
		//NavSys->GetRandomReachablePointInRadius(SpawnLocation, 1000.f, ReachableLocation);
		//Location = ReachableLocation.Location;
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


//void AEnemyClass::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor && (OtherActor != this) && OtherComp && static_cast<AMyRPGCharacter*>(OtherActor))
//	{
//		Targeted = true;
//		Target = OtherActor;
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
//		Walk();
//	}
//}

void AEnemyClass::Rotate(float DeltaTime)
{
	//Slowly rotate enemy towards character
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) { return; }
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
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() || CoolingDown) { return; }
	int AttackIndex = FMath::RandRange(0, AttackAnimClose.Num()-1);
	PlayAnimMontage(AttackAnimClose[AttackIndex]);
	CoolingDown = true;
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, [&]() { CoolingDown = false; }, Cooldown, false);
}

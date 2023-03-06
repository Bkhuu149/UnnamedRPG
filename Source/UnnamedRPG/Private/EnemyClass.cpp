// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyClass.h"
#include "Components/SphereComponent.h"

// Called to bind functionality to input
AEnemyClass::AEnemyClass()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Sphere"));
	TriggerSphere->InitSphereRadius(500.f);
	TriggerSphere->SetCollisionProfileName(TEXT("Trigger"));
	TriggerSphere->SetupAttachment(RootComponent);

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyClass::OnOverlapBegin);


}

void AEnemyClass::BeginPlay()
{
	Super::BeginPlay();
	//Set spawn location to get center of walk radius
	SpawnLocation = GetActorLocation();
	MyController = static_cast<AAIController*>(GetController());
	NavSys = UNavigationSystemV1::GetCurrent(GetWorld());


}

void AEnemyClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	//If Targeted, approach target. Else, walk to random point within spawn radius
	if (Targeted) {
		DelayTimer.Invalidate();
		if (FVector::Distance(Target->GetActorLocation(), SpawnLocation) > 1000.f) {
			ResetTarget();
			return;
		}
		Walk();
	}
	else {
		if (!DelayTimer.IsValid()){
			GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &AEnemyClass::Walk, 10.f, false);
		}
	}
}

void AEnemyClass::Walk() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Walk"));

	//if Target, walk to target.  Else, go to random point
	FVector Location;
	
	//Determine where the enemy should walk
	if (Target) {
		Location = Target->GetActorLocation();
	} 
	else {
		if (!NavSys)
		{
			return;
		}
		FNavLocation ReachableLocation;
		NavSys->GetRandomReachablePointInRadius(SpawnLocation, 1000.f, ReachableLocation);
		Location = ReachableLocation.Location;
	}

	//Walk to location
	MyController->MoveToLocation(Location, 100.f);
	DelayTimer.Invalidate();
}

void AEnemyClass::ResetTarget() {
	//Call to reset target values
	Targeted = false;
	Target = nullptr;
}


void AEnemyClass::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp && static_cast<AMyRPGCharacter*>(OtherActor))
	{
		Targeted = true;
		Target = OtherActor;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
		Walk();
	}
}


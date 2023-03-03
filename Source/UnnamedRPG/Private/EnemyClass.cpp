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
	SpawnLocation = GetActorLocation();
	MyController = static_cast<AAIController*>(GetController());


}

void AEnemyClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Targeted) {

	}
	else {
		if (!DelayTimer.IsValid()){
			GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &AEnemyClass::Walk, 10.f, false);
		}
	}


}

void AEnemyClass::Walk() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Walk"));
	
	DelayTimer.Invalidate();
}


void AEnemyClass::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		Targeted = true;
		Target = OtherActor;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
	}
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "MyEnumUtils.h"
#include "MyDamageType.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponActor.generated.h"


USTRUCT(BlueprintType)
struct FDamageTypeStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DamageTypeDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UMyDamageType> Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* TypeWeaponTrail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* TypeWeaponEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Thumbnail;

};

UCLASS()
class UNNAMEDRPG_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	AActor* Owner;
	FTimerHandle ColTimer;

	void WeaponLineTrace();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Type Table")
	UDataTable* DamageTypeTab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMyDamageType> MyType;
	void SetDamageType(EDamageType NewType);

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;
	void SetDamage(float NewDamage) { Damage = NewDamage; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Element")
	UNiagaraSystem* Trail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Element")
	UNiagaraSystem* ElementParticles;

	UNiagaraComponent* TrailComp;

	void PlayTrail();
	void EndTrail();

	void StartLineTrace();
	void EndLineTrace();

	void SetOwner(AActor* MyActor) { Owner = MyActor; }
};

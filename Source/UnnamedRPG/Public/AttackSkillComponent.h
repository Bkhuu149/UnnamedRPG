// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility_Montage.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"


#include "AttackSkillComponent.generated.h"


USTRUCT(BlueprintType)
struct FAttackStruct : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAnimMontage* FemaleAttackAnimMontage;
	UPROPERTY(EditANywhere, BlueprintReadOnly)
		UAnimMontage* MaleAttackAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsFinisher = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UClass* Weapon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Damage = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float StaminaDrain = 25;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int WeaponGroup = 0;

};

USTRUCT(BlueprintType)
struct FAttackSlotStruct 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName AttackId;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int WeaponGroup = 0;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNNAMEDRPG_API UAttackSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackSkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FAttackSlotStruct> FinishersInventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FAttackSlotStruct> ComboInventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FName> Hotbar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName Finisher;

	int ComboMaxSize = 3;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Database")
		UDataTable* AttackTab;

	void AddAttack(FName AttackID);

	UFUNCTION(BlueprintCallable)
	void SetComboInHotbar(FName AttackId, int HotbarIndex);

	void IncreaseCombo() {}; //To be built later

	UFUNCTION(BlueprintCallable)
	int FindAttackInCombo(FName AttackId, bool& Found);

	FAttackStruct* GetComboAttack(int HotbarIndex);

	FAttackStruct* GetFinisherAttack();

	UFUNCTION(BlueprintCallable)
	void SetComboInventory(TArray<FAttackSlotStruct> SavedCombo) { ComboInventory = SavedCombo; }

	UFUNCTION(BlueprintCallable)
	void SetFinishersInventory(TArray<FAttackSlotStruct> SavedFinishers) { FinishersInventory = SavedFinishers; }

	UFUNCTION(BlueprintCallable)
	void SetHotbar(TArray<FName> SavedHotbar) { Hotbar = SavedHotbar; }

	UFUNCTION(BlueprintCallable)
	void SetFinisher(FName SavedFinisher) { Finisher = SavedFinisher; }

private:
	int FindComboInInventory(FName AttackId, bool& ItemFound);
	int FindFinisherInInventory(FName AttackId, bool& ItemFound);

	FAttackStruct* GetAttackInformation(FName AttackId);

	void AddAttackToFinishers(FAttackSlotStruct Attack);
	void AddAttackToCombos(FAttackSlotStruct Attack);


};

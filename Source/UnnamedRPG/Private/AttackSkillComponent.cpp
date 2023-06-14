// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSkillComponent.h"

// Sets default values for this component's properties
UAttackSkillComponent::UAttackSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttackSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	FAttackSlotStruct Temp = FAttackSlotStruct();
	Temp.AttackId = FName("SwordAttack3");
	Temp.WeaponGroup = 1002;

	FAttackSlotStruct Temp2 = FAttackSlotStruct();
	Temp2.AttackId = FName("SwordAttack4");
	Temp2.WeaponGroup = 1003;

	FAttackSlotStruct Temp3 = FAttackSlotStruct();
	Temp3.AttackId = FName("SwordAttack5");
	Temp3.WeaponGroup = 1004;

	FAttackSlotStruct Temp4 = FAttackSlotStruct();
	Temp4.AttackId = FName("SwordAttack2");
	Temp4.WeaponGroup = 1001;

	ComboInventory.Add(Temp);
	ComboInventory.Add(Temp2);
	ComboInventory.Add(Temp3);
	FinishersInventory.Add(Temp4);

	Hotbar.Add(FName("SwordAttack3"));
	Hotbar.Add(FName("SwordAttack4"));
	Hotbar.Add(FName("SwordAttack5"));

	Finisher = FName("SwordAttack2");
	// ...
	
}


// Called every frame
void UAttackSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UAttackSkillComponent::FindComboInInventory(FName AttackId, bool& Found) {
	for (int i = 0; i < ComboInventory.Num(); i++) {
		if (ComboInventory[i].AttackId == AttackId) {
			Found = true;
			return i;
		}
	}
	Found = false;
	return -1;
}

int UAttackSkillComponent::FindFinisherInInventory(FName AttackId, bool& Found) {
	for (int i = 0; i < FinishersInventory.Num(); i++) {
		if (FinishersInventory[i].AttackId == AttackId) {
			Found = true;
			return i;
		}
	}
	Found = false;
	return -1;
}

FAttackStruct UAttackSkillComponent::GetAttackInformation(FName AttackId) {


	return FAttackStruct();
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSkillComponent.h"

// Sets default values for this component's properties
UAttackSkillComponent::UAttackSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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


// Called when the game starts
void UAttackSkillComponent::BeginPlay()
{
	Super::BeginPlay();

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

FAttackStruct* UAttackSkillComponent::GetAttackInformation(FName AttackId) {
	FAttackStruct* AttackRow = AttackTab->FindRow<FAttackStruct>(AttackId, "");
	return AttackRow;
}

void UAttackSkillComponent::AddAttackToFinishers(FAttackSlotStruct Attack) {
	//Finisher inventory is empty then just add it to the array
	if(FinishersInventory.Num() == 0) {
		FinishersInventory.Add(Attack);
		return;
	}

	//Find where to insert attack into array
	for (int i = 0; i < FinishersInventory.Num(); i++) {
		if (Attack.WeaponGroup < FinishersInventory[i].WeaponGroup) {
			FinishersInventory.Insert(Attack, i);
			return;
		}
	}
	//Attack should go at the end of inventory
	FinishersInventory.Add(Attack);
}

void UAttackSkillComponent::AddAttackToCombos(FAttackSlotStruct Attack) {
	//Combo inventory is empty then just add it to the array
	if (ComboInventory.Num() == 0) {
		ComboInventory.Add(Attack);
		return;
	}

	//Find where to insert attack into array
	for (int i = 0; i < ComboInventory.Num(); i++) {
		if (Attack.WeaponGroup < ComboInventory[i].WeaponGroup) {
			ComboInventory.Insert(Attack, i);
			return;
		}
	}
	//Attack should go at the end of inventory
	ComboInventory.Add(Attack);
}


void UAttackSkillComponent::AddAttack(FName AttackId) {
	FAttackStruct* AttackToBeAdded = GetAttackInformation(AttackId);
	if (!AttackToBeAdded) { return; }

	bool Found = false;
	int Index = -1;

	FAttackSlotStruct Attack = FAttackSlotStruct();
	Attack.AttackId = AttackId;
	Attack.WeaponGroup = AttackToBeAdded->WeaponGroup;
	switch (AttackToBeAdded->IsFinisher)
	{
	case (true):
		Index = FindFinisherInInventory(AttackId, Found);
		//Attack already in finisher inventory, don't add it
		if (Found) { return; }
		AddAttackToFinishers(Attack);
		return;

	case (false):
		Index = FindComboInInventory(AttackId, Found);
		if (Found) { return; }
		AddAttackToCombos(Attack);
		return;
	}
}

void UAttackSkillComponent::SetComboInHotbar(FName AttackId, int Index) {
	
}


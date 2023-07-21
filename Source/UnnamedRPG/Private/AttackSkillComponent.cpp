// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSkillComponent.h"

// Sets default values for this component's properties
UAttackSkillComponent::UAttackSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	FAttackSlotStruct Temp = FAttackSlotStruct("SwordAttack3", 1002);
	FAttackSlotStruct Temp2 = FAttackSlotStruct("SwordAttack4", 1003);
	FAttackSlotStruct Temp3 = FAttackSlotStruct("SwordAttack5", 1004);
	FAttackSlotStruct Temp4 = FAttackSlotStruct("SwordAttack2", 1001);

	ComboInventory.Add(Temp);
	ComboInventory.Add(Temp2);
	ComboInventory.Add(Temp3);
	FinishersInventory.Add(Temp4);

	Hotbar.Add(FName("SwordAttack3"));
	Hotbar.Add(FName("SwordAttack4"));
	Hotbar.Add(FName("SwordAttack5"));

	Finisher = FName("SwordAttack2");

	FAugmentSlotStruct AugmentNone = FAugmentSlotStruct(EDamageType::NONE, true);
	FAugmentSlotStruct AugmentFire = FAugmentSlotStruct(EDamageType::FIRE, true);
	FAugmentSlotStruct AugmentWater = FAugmentSlotStruct(EDamageType::WATER, true);
	FAugmentSlotStruct AugmentEarth = FAugmentSlotStruct(EDamageType::EARTH, true);
	FAugmentSlotStruct AugmentWind = FAugmentSlotStruct(EDamageType::WIND, true);
	FAugmentSlotStruct AugmentIce = FAugmentSlotStruct(EDamageType::ICE, false);
	FAugmentSlotStruct AugmentLightning = FAugmentSlotStruct(EDamageType::LIGHTNING, false);
	FAugmentSlotStruct AugmentSand = FAugmentSlotStruct(EDamageType::SAND, false);
	FAugmentSlotStruct AugmentSmoke = FAugmentSlotStruct(EDamageType::SMOKE, false);

	UnlockedAugments.Add(AugmentNone);
	UnlockedAugments.Add(AugmentFire);
	UnlockedAugments.Add(AugmentWater);
	UnlockedAugments.Add(AugmentEarth);
	UnlockedAugments.Add(AugmentWind);
	UnlockedAugments.Add(AugmentIce);
	UnlockedAugments.Add(AugmentLightning);
	UnlockedAugments.Add(AugmentSand);
	UnlockedAugments.Add(AugmentSmoke);

	AttackAugments.Add(EDamageType::NONE);
	AttackAugments.Add(EDamageType::NONE);
	AttackAugments.Add(EDamageType::NONE);
	
	FinisherDamageType = EDamageType::NONE;
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

	FAttackSlotStruct Attack = FAttackSlotStruct(AttackId, AttackToBeAdded->WeaponGroup);
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

void UAttackSkillComponent::SetComboInHotbar(FName AttackId, int HotbarIndex) {
	bool Found = false;
	int InventoryIndex = FindComboInInventory(AttackId, Found);
	if (!Found) { return; }
	if (HotbarIndex >= 0 && HotbarIndex < ComboMaxSize) {
		Hotbar[HotbarIndex] = AttackId;
	}
}

int UAttackSkillComponent::FindAttackInCombo(FName AttackId, bool& Found) {

	for (int i = 0; i < Hotbar.Num(); i++) {
		if (Hotbar[i] == AttackId) {
			Found = true;
			return i;
		}
	}
	Found = false;
	return -1;
}

FAttackStruct* UAttackSkillComponent::GetComboAttack(int HotbarIndex) {
	if (HotbarIndex >= Hotbar.Num()) { return GetFinisherAttack(); }
	return GetAttackInformation(Hotbar[HotbarIndex]);
}

EDamageType UAttackSkillComponent::GetAttackAugment(int AttackIndex) {
	if (AttackIndex >= Hotbar.Num()) { return GetFinisherAugment(); }
	return AttackAugments[AttackIndex];
}

void UAttackSkillComponent::SetAttackAugment(EDamageType NewType, int AttackIndex) {
	//Set the attack at Attack Index to be type given
	if (AttackIndex >= AttackAugments.Num()) { return; }
	AttackAugments[AttackIndex] = NewType;
}

void UAttackSkillComponent::UnlockDamageType(EDamageType NewType) {
	//Sets value of IsUnlocked for the element to be true
	int Index = -1;
	switch (NewType){
	case EDamageType::NONE:
		Index = 0;
		break;
	case EDamageType::FIRE:
		Index = 1;
		break;
	case EDamageType::WATER:
		Index = 2;
		break;
	case EDamageType::EARTH:
		Index = 3;
		break;
	case EDamageType::WIND:
		Index = 4;
		break;
	case EDamageType::ICE:
		Index = 5;
		break;
	case EDamageType::LIGHTNING:
		Index = 6;
		break;
	case EDamageType::SAND:
		Index = 7;
		break;
	case EDamageType::SMOKE:
		Index = 8;
		break;
	}
	if (Index == -1) { return; }
	UnlockedAugments[Index].IsUnlocked = true;
}


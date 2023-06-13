// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "../MyRPGCharacter.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	FSlotStruct Temp = FSlotStruct();
	Temp.Group = 1000;
	Temp.Quantity = 5;
	Temp.ItemId = FName("HealthPotion");
	Content.Add(Temp);

	FSlotStruct Temp2 = FSlotStruct();
	Temp2.Group = 1001;
	Temp2.Quantity = 5;
	Temp2.ItemId = FName("ManaPotion");
	Content.Add(Temp2);

	FSlotStruct Temp3 = FSlotStruct();
	Temp3.Group = 1002;
	Temp3.Quantity = 50;
	Temp3.ItemId = FName("TeleportScroll");
	Content.Add(Temp3);

	FSlotStruct Temp4 = FSlotStruct();
	Temp4.Group = 2000;
	Temp4.Quantity = 5;
	Temp4.ItemId = FName("AttackFood");
	Content.Add(Temp4);

	FSlotStruct Temp5 = FSlotStruct();
	Temp5.Group = 2001;
	Temp5.Quantity = 5;
	Temp5.ItemId = FName("StaminaFood");
	Content.Add(Temp5);

	FSlotStruct Temp6 = FSlotStruct();
	Temp6.Group = 2002;
	Temp6.Quantity = 5;
	Temp6.ItemId = FName("DefenseFood");
	Content.Add(Temp6);

	
	Hotbar.Add(FName("Empty"));
	Hotbar.Add(FName("Empty"));
	Hotbar.Add(FName("Empty"));
	Hotbar.Add(FName("Empty"));




	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FItemStruct* UInventoryComponent::GetItemInformation(FName ItemId) {
	FItemStruct* ItemRow = ItemTab->FindRow<FItemStruct>(ItemId, "");
	return ItemRow;
}


int UInventoryComponent::FindItemSlot(FName ItemId, bool& ItemFound) {
	//Find index in inventory where item exists.  
	//If item not in inventory then return -1
	for (int i = 0; i < Content.Num(); i++) {
		if (Content[i].ItemId == ItemId) {
			ItemFound = true;
			return i;
		}
	}
	ItemFound = false;
	return -1;
}

int UInventoryComponent::FindItemInHotbar(FName ItemId, bool& ItemFound) {
	//Find index in inventory where item exists.  
	//If item not in inventory then return -1
	for (int i = 0; i < Hotbar.Num(); i++) {
		if (Hotbar[i] == ItemId) {
			ItemFound = true;
			return i;
		}
	}
	ItemFound = false;
	return -1;
}

void UInventoryComponent::AddToStack(int Index) {
	Content[Index].Quantity++;
}

void UInventoryComponent::InsertIntoContent(FSlotStruct& Item) {
	//If first item in inventory, then just add it
	if (Content.Num() == 0) {
		Content.Add(Item);
		return;
	}
	//Find where to insert the new item into inventory
	for (int i = 0; i < Content.Num(); i++) {
		if (Item.Group < Content[i].Group) {
			Content.Insert(Item, i);
			return;
		}
	}
	//Item should go at the end of inventory
	Content.Add(Item);
}




void UInventoryComponent::AddToInventory(FName ItemId) {
	//Check if item to be added is valid
	FItemStruct* ItemData = GetItemInformation(ItemId);
	if (!ItemData) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Attempted to add nonexistant item: " + ItemId.ToString()));
		return;
	}
	bool Found = false;
	int Index = FindItemSlot(ItemId, Found);
	if (!Found) {
		//Item was not in inventory
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Item not in inventory"));
		//Find empty slot to add item to
		FSlotStruct NewItem = FSlotStruct();
		NewItem.ItemId = ItemId;
		NewItem.Quantity = 1;
		NewItem.Group = ItemData->Group;

		InsertIntoContent(NewItem);
		return;
	}
	//Item was found in inventory, need to check if there is space
	FSlotStruct InventorySpace = Content[Index];
	if (InventorySpace.Quantity >= ItemData->StackSize) {
		//Stack Full, cannot add more
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Item in inventory but cannot add more"));

		return;
	}
	//Stack has space, add to stack
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Added item successfully"));
	AddToStack(Index);

}

bool UInventoryComponent::RemoveFromInventory(FName ItemId) {
	bool Found = false;
	int Index = FindItemSlot(ItemId, Found);
	if (!Found) {
		//Item not found, do nothing
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Item not in inventory"));
		return Found;
	}

	//Decriment item quantity in inventory, if zero do nothing
	if (Content[Index].Quantity == 0) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Item in inventory but 0 quantity, do nothing"));
		return false;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Item in inventory, decrimenting"));
	Content[Index].Quantity--;
	return Found;

}

void UInventoryComponent::SetItemInHotbar(FName ItemId, int HotbarIndex) {
	if (HotbarIndex >= 0 && HotbarIndex < 4) {
		Hotbar[HotbarIndex] = ItemId;
		return;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Attempted to add item to hotbar outside of max size"));


}

void UInventoryComponent::UseItem(int HotbarSlotIndex) {
	FName ItemId = Hotbar[HotbarSlotIndex];
	bool IsItemConsumed = RemoveFromInventory(ItemId);
	if (!IsItemConsumed) {
		//Item wasn't consumed, do nothing
		return;
	}
	//Item consumed, do what item says
	FItemStruct* Item = GetItemInformation(ItemId);
	AMyRPGCharacter* Player = Cast<AMyRPGCharacter>(GetOwner());

	switch (Item->ItemEffect) {

	case EItemEffect::Restore:
		
		switch (Item->StatAffected) {
		case EStatAffected::Health:
			Player->HealChar(50);
			return;
		case EStatAffected::Mana:
			Player->AddMana(50);
			return;
		}


		return;

	case EItemEffect::Buff:
		switch (Item->StatAffected) {
		case EStatAffected::Stamina:
			Player->SetStaminaDrainMultiplier(.5);
			return;
		}
		return;

	}
	return;
}

FSlotStruct UInventoryComponent::GetItemFromInventory(FName ItemId) {
	bool Found = false;
	int Index = FindItemSlot(ItemId, Found);
	if (!Found) {
		return FSlotStruct();
	}
	return Content[Index];
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

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
	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UInventoryComponent::GetMaxStackSize(FName ItemId) {

	FItemStruct* ItemRow = ItemTab->FindRow<FItemStruct>(ItemId, "");
	if (!ItemRow) { return -1; }
	return ItemRow->StackSize;
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

void UInventoryComponent::AddToStack(int Index) {
	Content[Index].Quantity++;
}



void UInventoryComponent::AddToInventory(FName ItemId) {
	bool Found = false;
	int Index = FindItemSlot(ItemId, Found);
	if (!Found) {
		//Item was not in inventory
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Item not in inventory"));
		//Find empty slot to add item to

		FSlotStruct NewItem = FSlotStruct();
		NewItem.ItemId = ItemId;
		NewItem.Quantity = 1;

		//Insert into correct position, assuming Content is already sorted
		//which it should be, insortion sort but just inserting into a sorted array.
		Content.Add(NewItem);	
		
		return;
	}
	//Item was found in inventory, need to check if there is space
	FSlotStruct InventorySpace = Content[Index];
	int ItemMaxStack = GetMaxStackSize(ItemId);
	if (InventorySpace.Quantity >= ItemMaxStack) {
		//Stack Full, cannot add more
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Item in inventory but cannot add more"));

		return;
	}
	//Stack has space, add all
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Added item successfully"));
	AddToStack(Index);

}

void UInventoryComponent::RemoveFromInventory(FName ItemId) {
	bool Found = false;
	int Index = FindItemSlot(ItemId, Found);
	if (Found) {
		//Decriment item quantity in inventory, if zero do nothing
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Item in inventory, decrimenting"));

		return;
	}
	//Item not found, do nothing
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Item not in inventory"));

}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"

#include "InventoryComponent.generated.h"


USTRUCT(BlueprintType)
struct FItemStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D*	Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int StackSize = 99;


};


USTRUCT(BlueprintType)
struct FSlotStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Quantity;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNNAMEDRPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int InventorySize = 10;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSlotStruct> Content;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddToInventory(FName ItemId, int Quantity);

	void RemoveFromInventory();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDatabase")
	UDataTable* ItemTab;

private: 
	int FindSlots(FName ItemId, bool& ItemFound);

	int GetMaxStackSize(FName ItemId);

	void AddToStack(int Index, int Quantity);

	int FindNextEmptySlot();
		
};

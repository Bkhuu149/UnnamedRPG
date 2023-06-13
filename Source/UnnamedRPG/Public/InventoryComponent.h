// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"

#include "InventoryComponent.generated.h"

UENUM(BlueprintType)
enum class EStatAffected : uint8 {
	None		UMETA(DisplayName = "None"),
	Health		UMETA(DisplayName = "Health"),
	Mana		UMETA(DisplayName = "Mana"),
	Stamina		UMETA(DisplayName = "Stamina"),
	Defense		UMETA(DisplayName = "Defense"),
	Attack		UMETA(DisplayName = "Attack"),
	Vision		UMETA(DisplayName = "Vision"),
	Movement	UMETA(DisplayName = "Movement")
};

UENUM(BlueprintType)
enum class EItemEffect : uint8 {
	None		UMETA(DisplayName = "None"),
	Restore		UMETA(DisplayName = "Restore"),
	Buff		UMETA(DisplayName = "Buff")
};

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Group = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EStatAffected StatAffected = EStatAffected::None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemEffect ItemEffect = EItemEffect::None;
};


USTRUCT(BlueprintType)
struct FSlotStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemId = FName("Empty");
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Quantity = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Group = 0;

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
	TArray<FSlotStruct> Content;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FName> Hotbar;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddToInventory(FName ItemId);

	UFUNCTION(BlueprintCallable)
	bool RemoveFromInventory(FName ItemId);

	UFUNCTION(BlueprintCallable)
	FSlotStruct GetItemFromInventory(FName ItemId);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDatabase")
	UDataTable* ItemTab;

	UFUNCTION(BlueprintCallable)
	void SetItemInHotbar(FName ItemId, int HotbarIndex);

	UFUNCTION(BlueprintCallable)
	void UseItem(int HotbarIndex);

	UFUNCTION(BlueprintCallable)
	int FindItemInHotbar(FName ItemId, bool& ItemFound);


private: 
	int FindItemSlot(FName ItemId, bool& ItemFound);

	FItemStruct* GetItemInformation(FName ItemId);

	void AddToStack(int Index);

	void InsertIntoContent(FSlotStruct& Item);

		
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MyEnumUtils.h"
#include "DamageTypeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageTypeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNNAMEDRPG_API IDamageTypeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual EDamageType GetDamageType() { return EDamageType::NONE; }
};

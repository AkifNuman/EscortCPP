// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SkeletalMesh.h"
#include "Escort/Weapons/MainWeapon.h"
#include "GameFramework/SaveGame.h"
#include "CharacterItemData.generated.h"

/**
 * 
 */
UCLASS()
class ESCORT_API UCharacterItemData : public USaveGame
{
	GENERATED_BODY()

public:

	UCharacterItemData();

	UPROPERTY(EditAnywhere)
	USkeletalMesh* CharMesh;

	UPROPERTY(EditAnywhere)
	bool CharHasHair;

	UPROPERTY(EditAnywhere)
	bool IsMale;

	UPROPERTY(EditAnywhere)
	AMainWeapon* Weapon;
};

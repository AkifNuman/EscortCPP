// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Escort/PlayerData/CharacterItemData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Escort/Weapons/MainWeapon.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class ESCORT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
	void Equip();

	UFUNCTION(BlueprintCallable)
	void Aim(float Value);

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;

	UFUNCTION(BlueprintCallable)
	bool HasWep();

	UFUNCTION(BlueprintCallable)
	bool IsAim();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Crouched;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMesh* CharMesh;

	UFUNCTION(BlueprintCallable)
	void SetCharacterData(USkeletalMesh* NewMesh, bool HasCharHair, bool CharHairIsMale);

	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	void CharCrouch(float Value);

	void Run(float Value);

	void SetOverlappingWeapon(AMainWeapon* Weapon);
private:
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AMainWeapon* WeaponClass;

	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CharHair;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CharHairMale;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AMainWeapon* LastWeapon);

	UFUNCTION(Server, Reliable)
	void ServerEquip();

	UFUNCTION(Server, Reliable)
	void ServerRun(float Value);

	bool CharHasHair;
	bool IsMale;

	void SaveGame();
	void LoadGame();
};

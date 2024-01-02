// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Escort/Weapons/MainWeapon.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Engine.h"
#include "Escort/Lobby/PlayerData.h"
#include "MainAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Escort/Components/CombatComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mannequin(TEXT("/Game/Characters/Meshs/SK_Character_Male_FBI"));
	//static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP(TEXT("/Game/Characters/Animations/MainCharacterAnimBP"));
	//GetMesh()->SetSkeletalMesh(Mannequin.Object);
	//GetMesh()->SetAnimInstanceClass(AnimBP.Object->GeneratedClass);
	LoadGame();
	if (CharMesh)
	{

		GetMesh()->SetSkeletalMeshAsset(CharMesh);
	}

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	CharHair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharHair"));
	CharHair->SetupAttachment(GetMesh(), "head");
	CharHair->SetRelativeRotation(FRotator(0.f, 0.f, -90.f)); //(Pitch=0.000000,Yaw=0.000000,Roll=-90.000000)
	CharHair->SetVisibility(false);

	CharHairMale = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharHairMale"));
	CharHairMale->SetupAttachment(GetMesh(), "head");
	CharHairMale->SetRelativeRotation(FRotator(0.f, 0.f, -90.f)); //(Pitch=0.000000,Yaw=0.000000,Roll=-90.000000)
	CharHairMale->SetVisibility(false);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 39.f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	bUseControllerRotationYaw = false;
}

void AMainCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AMainCharacter, WeaponClass, COND_OwnerOnly);
}

bool AMainCharacter::HasWep()
{
	return (Combat && Combat->EquippedWeapon);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	LoadGame();

	if (CharMesh)
	{
		GetMesh()->SetSkeletalMeshAsset(CharMesh);
		if (CharHasHair)
		{
			CharHair->SetVisibility(true);
		}
		if (IsMale)
		{
			CharHairMale->SetVisibility(true);
		}
		if (WeaponClass)
		{
			Equip();
		}
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &AMainCharacter::Equip);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookRight", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("Run", this, &AMainCharacter::Run);
	PlayerInputComponent->BindAxis("Crouch", this, &AMainCharacter::CharCrouch);
	PlayerInputComponent->BindAxis("Aim", this, &AMainCharacter::Aim);
}

void AMainCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Combat)
	{
		Combat->Character = this;
	}
}

void AMainCharacter::MoveForward(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AMainCharacter::MoveRight(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AMainCharacter::CharCrouch(float Value)
{
	if (Value)
	{
		Crouch();
		Crouched = true;
	}
	else
	{
		UnCrouch();
		Crouched = false;
	}
	
}

void AMainCharacter::Run(float Value)
{
	ServerRun(Value);
}

void AMainCharacter::ServerRun_Implementation(float Value)
{
	if ((Value != NULL) && (Value > 0))
	{
		GetCharacterMovement()->MaxWalkSpeed = 550.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 225.f;
	}
}

void AMainCharacter::Equip()
{
	if (Combat)
	{
		if (HasAuthority())
		{
			Combat->EquipWeapon(WeaponClass);
		}
		else
		{
			ServerEquip();
		}
	}
}

void AMainCharacter::Aim(float Value)
{
	if (Value)
	{
		if (Combat)
			Combat->SetAiming(true);
	}
	else
	{
		if (Combat)
			Combat->SetAiming(false);
	}

}

bool AMainCharacter::IsAim()
{
	return (Combat && Combat->bAiming);
}

void AMainCharacter::ServerEquip_Implementation()
{
	if (Combat)
	{
		Combat->EquipWeapon(WeaponClass);
	}
}

void AMainCharacter::SetOverlappingWeapon(AMainWeapon* Weapon)
{
	WeaponClass = Weapon;
}

void AMainCharacter::SetCharacterData(USkeletalMesh* NewMesh, bool HasCharHair, bool CharHairIsMale)
{
	CharMesh = NewMesh;

	if (CharMesh)
		GetMesh()->SetSkeletalMeshAsset(CharMesh);

	CharHasHair = HasCharHair;
	IsMale = CharHairIsMale;
	SaveGame();
}

void AMainCharacter::OnRep_OverlappingWeapon(AMainWeapon* LastWeapon)
{

}

void AMainCharacter::SaveGame()
{
	UCharacterItemData* SaveGameInstance = Cast<UCharacterItemData>(UGameplayStatics::CreateSaveGameObject(UCharacterItemData::StaticClass()));
	SaveGameInstance->CharMesh = CharMesh;
	SaveGameInstance->CharHasHair = CharHasHair;
	SaveGameInstance->IsMale = IsMale;
	SaveGameInstance->Weapon = WeaponClass;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Slot0"), 0);
}

void AMainCharacter::LoadGame()
{
	UCharacterItemData* SaveGameInstance = Cast<UCharacterItemData>(UGameplayStatics::CreateSaveGameObject(UCharacterItemData::StaticClass()));

	SaveGameInstance = Cast<UCharacterItemData>(UGameplayStatics::LoadGameFromSlot("Slot0", 0));
	if (SaveGameInstance)
	{
		CharMesh = SaveGameInstance->CharMesh;
		CharHasHair = SaveGameInstance->CharHasHair;
		IsMale = SaveGameInstance->IsMale;
		WeaponClass = SaveGameInstance->Weapon;
	}
}

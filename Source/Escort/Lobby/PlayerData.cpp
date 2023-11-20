// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerData.h"
#include "Components/SphereComponent.h"
#include "Escort/MainCharacter/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerData::APlayerData()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharMesh(TEXT("/Game/Characters/Meshs/SK_Character_Female_FBI"));
	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetupAttachment(RootComponent);
	if (CharMesh.Succeeded())
	{
		CharacterMesh->SetSkeletalMesh(CharMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WepMesh(TEXT("/Game/PolygonHeist/Meshes/Weapons_Rigged/SK_Wep_RifleSwat_01"));
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(CharacterMesh);
	if (WepMesh.Succeeded())
	{
		WeaponMesh->SetSkeletalMesh(WepMesh.Object);
	}

	CharHair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharHair"));
	CharHair->SetupAttachment(CharacterMesh, "head");
	CharHair->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CharacterMesh);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerData::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerData::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerData::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerData::GoWepLoc()
{
	Camera->SetWorldLocation(FVector(-1766.183796f, 781.974192f, 155.681995f));
	Camera->SetWorldRotation(FRotator(-37.993875f, -265.890258f, -2.405321f)); //(Pitch=-37.993875,Yaw=-265.890258,Roll=-2.405321)
}


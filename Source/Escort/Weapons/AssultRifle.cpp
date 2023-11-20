// Fill out your copyright notice in the Description page of Project Settings.


#include "AssultRifle.h"

AAssultRifle::AAssultRifle()
{
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WepMeshh(TEXT("/Game/PolygonHeist/Meshes/Weapons_Rigged/SK_Wep_RifleSwat_01"));
	if (WepMeshh.Succeeded())
	{
		WeaponMesh->SetSkeletalMesh(WepMeshh.Object);
	}
	Mag = 13;
}
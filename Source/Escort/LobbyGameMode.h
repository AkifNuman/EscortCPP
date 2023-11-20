// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ESCORT_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ALobbyGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int ReadyCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 NumberOfPlayers;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName SideName;

	UFUNCTION()
	void ReadyGo();
	class AMainCharacter* Character;

private:

};

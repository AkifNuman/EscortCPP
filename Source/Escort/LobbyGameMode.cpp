// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "Escort/MainCharacter/MainCharacter.h"

ALobbyGameMode::ALobbyGameMode()
{
	DefaultPawnClass = AMainCharacter::StaticClass();
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	NumberOfPlayers = GameState.Get()->PlayerArray.Num();

}

void ALobbyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (NumberOfPlayers == ReadyCount)
	{
		ReadyGo();
	}
}

void ALobbyGameMode::ReadyGo()
{
	UWorld* World = GetWorld();
	if (World)
	{
		bUseSeamlessTravel = true;
		World->ServerTravel(FString("/Game/Map/Demonstration?listen"));
	}
}

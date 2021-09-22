// Copyright RLoris 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
// #include "./Spawners/TankSpawner.h"
#include "TankPlaygroundGameModeBase.generated.h"

class ATankSpawner;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundStartDelegate, int32, TankCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundEndDelegate, bool, Win);

/**
 * 
 */
UCLASS()
class TANKPLAYGROUND_API ATankPlaygroundGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FOnRoundStartDelegate OnRoundStart;
	UPROPERTY()
	FOnRoundEndDelegate OnRoundEnd;
private:
	void StartRound(int32 TankCount);
	void EndRound(bool Win);
public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// called to setup the game
	void SetupGame(ATankSpawner* Spawner);
	// called to end the game
	void GameOver(bool Win);

	ATankSpawner* Spawner = nullptr;
};

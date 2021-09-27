// Copyright RLoris 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankPlaygroundGameModeBase.generated.h"

// forward declarations
class ATankSpawner;
class UTankPlaygroundGameInstance;

UENUM(BlueprintType)
enum class EEndGameType : uint8 {
	NONE,
	LOST,
	WIN
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundStartDelegate, int32, TankCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundEndDelegate, EEndGameType, Win);

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
	// setup defaults
	ATankPlaygroundGameModeBase();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// called once to setup the game
	void SetupGame(ATankSpawner* Spawner);
	// Called to start the game
	UFUNCTION(BlueprintCallable, Category = "Round")
	void StartGame();
	// called to end the game properly
	UFUNCTION(BlueprintCallable, Category = "Round")
	void EndGame(bool Win);
	// called to stop the current game
	UFUNCTION(BlueprintCallable, Category = "Round")
	void StopGame();

	ATankSpawner* Spawner;
	UTankPlaygroundGameInstance* GameInstance;
};

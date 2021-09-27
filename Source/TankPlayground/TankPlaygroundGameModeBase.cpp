// Copyright RLoris 2021

#include "TankPlaygroundGameModeBase.h"
#include "./Spawners/TankSpawner.h"
#include "TankPlaygroundGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/WidgetBase.h"
#include "UObject/UObjectGlobals.h"
#include "Controllers/TankPlayerController.h"

ATankPlaygroundGameModeBase::ATankPlaygroundGameModeBase()
{
	// setup default classes
	PlayerControllerClass = ATankPlayerController::StaticClass();
	DefaultPawnClass = nullptr;
}

void ATankPlaygroundGameModeBase::StartRound(int32 Count)
{
	this->OnRoundStart.Broadcast(Count);
}

void ATankPlaygroundGameModeBase::EndRound(bool Win)
{
	this->OnRoundEnd.Broadcast(Win ? EEndGameType::WIN : EEndGameType::LOST);
}

void ATankPlaygroundGameModeBase::BeginPlay()
{
	this->GameInstance = Cast<UTankPlaygroundGameInstance>(GetGameInstance());
	// is menu level ?
	if (GetWorld()->GetName() == TEXT("00_Menu"))
	{
		// display main menu
		UWidgetBase* MainMenuRef = nullptr;
		static UClass* MainMenuClass = StaticLoadClass(UObject::StaticClass(), NULL, TEXT("WidgetBlueprintGeneratedClass'/Game/Widgets/W_MainMenu.W_MainMenu_C'"), nullptr, LOAD_None);
		this->GameInstance->ShowWidget(UGameplayStatics::GetPlayerController(this, 0), MainMenuClass, MainMenuRef);
	}	
	else
	{
		this->StartGame();
	}
}

void ATankPlaygroundGameModeBase::SetupGame(ATankSpawner* TankSpawner)
{
	this->Spawner = TankSpawner;
}

void ATankPlaygroundGameModeBase::StartGame()
{
	if (this->Spawner)
	{
		this->StartRound(this->Spawner->TankCount - 1);
		// this->GameInstance->HideWidget(UGameplayStatics::GetPlayerController(this, 0));
	}
}

void ATankPlaygroundGameModeBase::EndGame(bool Win)
{
	this->EndRound(Win);
	this->GameInstance->RestartLevel();
}

void ATankPlaygroundGameModeBase::StopGame()
{
	this->OnRoundEnd.Broadcast(EEndGameType::NONE);
	this->GameInstance->RestartLevel();
}

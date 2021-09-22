// Copyright RLoris 2021

#include "./Spawners/TankSpawner.h"
#include "TankPlaygroundGameModeBase.h"

void ATankPlaygroundGameModeBase::StartRound(int32 Count)
{
	this->OnRoundStart.Broadcast(Count);
}

void ATankPlaygroundGameModeBase::EndRound(bool Win)
{
	this->OnRoundEnd.Broadcast(Win);
}

void ATankPlaygroundGameModeBase::BeginPlay()
{

}

void ATankPlaygroundGameModeBase::SetupGame(ATankSpawner* TankSpawner)
{
	this->Spawner = TankSpawner;
	this->StartRound(TankSpawner->TankCount - 1);
}

void ATankPlaygroundGameModeBase::GameOver(bool Win)
{
	this->EndRound(Win);
	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, [this]() { this->StartRound(Spawner->TankCount - 1); }, 2.0f, false);
}

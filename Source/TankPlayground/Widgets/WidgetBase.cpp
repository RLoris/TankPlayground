// Copyright RLoris 2021

#include "WidgetBase.h"
#include "../TankPlaygroundGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../TankPlaygroundGameModeBase.h"
#include "../Controllers/TankPlayerController.h"

void UWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// convenience usage
	this->GameInstance = Cast<UTankPlaygroundGameInstance>(GetGameInstance());
	this->GameMode = Cast<ATankPlaygroundGameModeBase>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
	this->PC = Cast<ATankPlayerController>(GetOwningPlayer());
}

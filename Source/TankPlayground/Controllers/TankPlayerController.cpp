// Copyright RLoris 2021

#include "TankPlayerController.h"
#include "../TankPlaygroundGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../Widgets/WidgetHUD.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// pause pressed toggle
	FInputActionBinding& NewBinding = InputComponent->BindAction(FName("Pause"), EInputEvent::IE_Pressed, this, &ATankPlayerController::TogglePauseGame);
	NewBinding.bExecuteWhenPaused = true;
}

void ATankPlayerController::TogglePauseGame()
{
	if (IsPaused())
	{
		ResumeGame();
	}
	else
	{
		PauseGame();
	}
}

void ATankPlayerController::ShowHUD()
{
	static UClass* HUDMenuClass = StaticLoadClass(UObject::StaticClass(), NULL, TEXT("WidgetBlueprintGeneratedClass'/Game/Widgets/W_HUDMenu.W_HUDMenu_C'"), nullptr, LOAD_None);
	static UTankPlaygroundGameInstance* GameInstance = Cast<UTankPlaygroundGameInstance>(GetGameInstance());
	if (HUDMenuClass->ImplementsInterface(UWidgetHUD::StaticClass()))
	{
		GameInstance->ShowWidget(this, HUDMenuClass, WidgetHUD);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD widget does not implement interface WidgetHUD %s"));
	}
}

void ATankPlayerController::ShowPause()
{
	static UClass* PauseMenuClass = StaticLoadClass(UObject::StaticClass(), NULL, TEXT("WidgetBlueprintGeneratedClass'/Game/Widgets/W_PauseMenu.W_PauseMenu_C'"), nullptr, LOAD_None);
	static UTankPlaygroundGameInstance* GameInstance = Cast<UTankPlaygroundGameInstance>(GetGameInstance());
	UWidgetBase* PauseMenuRef = nullptr;
	GameInstance->ShowWidget(this, PauseMenuClass, PauseMenuRef);
}

void ATankPlayerController::PauseGame()
{
	this->ShowPause();
}

void ATankPlayerController::ResumeGame()
{
	this->ShowHUD();
}

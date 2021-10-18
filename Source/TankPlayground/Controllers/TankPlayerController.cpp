// Copyright RLoris 2021

#include "TankPlayerController.h"
#include "../TankPlaygroundGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../Widgets/WidgetsContainer.h"
#include "../Widgets/WidgetHUD.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankPlayerController::EndPlay(EEndPlayReason::Type Reason)
{
	HideWidget();
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// pause pressed toggle
	FInputActionBinding& NewBinding = InputComponent->BindAction(FName("Pause"), EInputEvent::IE_Pressed, this, &ATankPlayerController::TogglePauseGame);
	NewBinding.bExecuteWhenPaused = true;
}

bool ATankPlayerController::ShowWidget(TSubclassOf<UWidgetBase> WidgetClass, UWidgetBase*& WidgetRef)
{
	if (!GameInstance) 
	{
		GameInstance = Cast<UTankPlaygroundGameInstance>(GetGameInstance());
	}
	if (!GameInstance->GetWidgetsContainerClass())
	{
		UE_LOG(LogTemp, Warning, TEXT("Widgets container class not set in gameinstance"));
		return false;
	}
	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid widget class to show"));
		return false;
	}
	if (!WidgetPlayerContainer)
	{
		WidgetPlayerContainer = CreateWidget<UWidgetsContainer>(this, GameInstance->GetWidgetsContainerClass());
		WidgetPlayerContainer->SetOwningPlayer(this);
	}
	if (WidgetPlayerContainer->Display(WidgetClass))
	{
		WidgetRef = WidgetPlayerContainer->GetDisplayedWidget();
		return true;
	}
	return false;
}

bool ATankPlayerController::HideWidget()
{
	if (WidgetPlayerContainer)
	{
		return WidgetPlayerContainer->Hide();
	}
	return false;
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
	static UClass* HUDMenuClass = LoadClass<UObject>(nullptr, TEXT("WidgetBlueprintGeneratedClass'/Game/Widgets/W_HUDMenu.W_HUDMenu_C'"), nullptr, LOAD_None, nullptr);
	if (HUDMenuClass->ImplementsInterface(UWidgetHUD::StaticClass()))
	{
		ShowWidget(HUDMenuClass, WidgetHUD);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD widget does not implement interface WidgetHUD"));
	}
}

void ATankPlayerController::ShowPause()
{
	static UClass* PauseMenuClass = LoadClass<UObject>(nullptr, TEXT("WidgetBlueprintGeneratedClass'/Game/Widgets/W_PauseMenu.W_PauseMenu_C'"), nullptr, LOAD_None, nullptr);
	UWidgetBase* PauseMenuRef = nullptr;
	ShowWidget(PauseMenuClass, PauseMenuRef);
}

void ATankPlayerController::PauseGame()
{
	ShowPause();
}

void ATankPlayerController::ResumeGame()
{
	ShowHUD();
}

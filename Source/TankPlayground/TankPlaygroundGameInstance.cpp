// Copyright RLoris 2021

#include "TankPlaygroundGameInstance.h"
#include "Widgets/WidgetsContainer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UTankPlaygroundGameInstance::Init()
{
	Super::Init();
	// get default container widget class
	UClass* WidgetContainerClass = StaticLoadClass(UWidgetsContainer::StaticClass(), NULL, TEXT("WidgetBlueprintGeneratedClass'/Game/Widgets/W_WidgetContainer.W_WidgetContainer_C'"), nullptr, LOAD_None);
	this->SetWidgetsContainerClass(WidgetContainerClass);
}

void UTankPlaygroundGameInstance::SetWidgetsContainerClass(TSubclassOf<UWidgetsContainer> WidgetClass)
{
	ContainerClass = WidgetClass;
}

TSubclassOf<UWidgetsContainer> UTankPlaygroundGameInstance::GetWidgetsContainerClass()
{
	return ContainerClass;
}

void UTankPlaygroundGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UTankPlaygroundGameInstance::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetFirstLocalPlayerController(), EQuitPreference::Quit, false);
}

void UTankPlaygroundGameInstance::RestartLevel()
{
	GetFirstLocalPlayerController()->ConsoleCommand(TEXT("RestartLevel"));
}

void UTankPlaygroundGameInstance::OpenLevel(FName LevelName)
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName, false);
}

void UTankPlaygroundGameInstance::OpenLevelByRef(const TSoftObjectPtr<UWorld> Level)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level);
}

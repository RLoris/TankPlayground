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

void UTankPlaygroundGameInstance::Shutdown()
{
	Super::Shutdown();
}

bool UTankPlaygroundGameInstance::ShowWidget(APlayerController* PC, TSubclassOf<UWidgetBase> WidgetClass, UWidgetBase*& WidgetRef)
{
	if (!ContainerClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Widgets container class not set in gameinstance"));
		return false;
	}
	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid widget class to show"));
		return false;
	}
	UWidgetsContainer** pW = WidgetPlayerContainer.Find(PC);
	UWidgetsContainer* W;
	if (!pW)
	{
		W = CreateWidget<UWidgetsContainer>(PC, ContainerClass);
		W->SetOwningPlayer(PC);
		WidgetPlayerContainer.Add(PC, W);
	}
	else
	{
		W = *pW;
	}
	if (W->Display(WidgetClass))
	{
		WidgetRef = W->GetDisplayedWidget();
		return true;
	}
	return false;
}

bool UTankPlaygroundGameInstance::HideWidget(APlayerController* PC)
{
	if (UWidgetsContainer** pW = WidgetPlayerContainer.Find(PC))
	{
		if (*pW)
		{
			return (*pW)->Hide();
		}
		
	}
	return false;
}

void UTankPlaygroundGameInstance::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetFirstLocalPlayerController(), EQuitPreference::Quit, false);
}

void UTankPlaygroundGameInstance::RestartLevel()
{
	// call before widget destruction
	this->ClearWidgets();
	GetFirstLocalPlayerController()->ConsoleCommand(TEXT("RestartLevel"));
}

void UTankPlaygroundGameInstance::OpenLevel(FName LevelName)
{
	// call before widget destruction
	this->ClearWidgets();
	UGameplayStatics::OpenLevel(GetWorld(), LevelName, false);
}

void UTankPlaygroundGameInstance::OpenLevelByRef(const TSoftObjectPtr<UWorld> Level)
{
	// call before widget destruction
	this->ClearWidgets();
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level);
}

void UTankPlaygroundGameInstance::ClearWidgets()
{
	for (auto& Entry : this->WidgetPlayerContainer)
	{
		if (Entry.Value)
		{
			Entry.Value->Hide();
		}
	}
	this->WidgetPlayerContainer.Empty();
}

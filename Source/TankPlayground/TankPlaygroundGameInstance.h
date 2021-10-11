// Copyright RLoris 2021

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Templates/SubclassOf.h"
#include "UObject/SoftObjectPtr.h"
#include "Engine/World.h"
#include "TankPlaygroundGameInstance.generated.h"

// forward declaration
class UWidgetsContainer;

/**
 * 
 */
UCLASS()
class TANKPLAYGROUND_API UTankPlaygroundGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Shutdown() override;
	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetWidgetsContainerClass(TSubclassOf<UWidgetsContainer> WidgetClass);
	UFUNCTION(BlueprintCallable, Category = "Widget")
	TSubclassOf<UWidgetsContainer> GetWidgetsContainerClass();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void QuitGame();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void RestartLevel();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OpenLevel(FName LevelName);
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OpenLevelByRef(const TSoftObjectPtr<UWorld> Level);
private:
	TSubclassOf<UWidgetsContainer> ContainerClass;
};

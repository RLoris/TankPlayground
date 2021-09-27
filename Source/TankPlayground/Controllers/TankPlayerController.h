// Copyright RLoris 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

// forward declaration
class UTankPlaygroundGameInstance;
class UWidgetBase;
/**
 * 
 */
UCLASS()
class TANKPLAYGROUND_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
public:
	UFUNCTION(BlueprintCallable)
	void PauseGame();
	UFUNCTION(BlueprintCallable)
	void ResumeGame();
	UFUNCTION(BlueprintCallable)
	void TogglePauseGame();

	void ShowHUD();
	void ShowPause();

public:
	UWidgetBase* WidgetHUD;
};

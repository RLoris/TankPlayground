// Copyright RLoris 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

// forward declaration
class UTankPlaygroundGameInstance;
class UWidgetBase;
class UWidgetsContainer;
/**
 * 
 */
UCLASS()
class TANKPLAYGROUND_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;
	virtual void SetupInputComponent() override;
public:
	UFUNCTION(BlueprintCallable, Category = "Widget")
	bool ShowWidget(TSubclassOf<UWidgetBase> WidgetClass, UWidgetBase*& WidgetRef);
	UFUNCTION(BlueprintCallable, Category = "Widget")
	bool HideWidget();

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void PauseGame();
	UFUNCTION(BlueprintCallable, Category = "Pause")
	void ResumeGame();
	UFUNCTION(BlueprintCallable, Category = "Pause")
	void TogglePauseGame();

	void ShowHUD();
	void ShowPause();
public:
	UWidgetBase* WidgetHUD;
private:
	UWidgetsContainer* WidgetPlayerContainer;
};

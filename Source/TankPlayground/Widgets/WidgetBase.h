// Copyright RLoris 2021

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetBase.generated.h"

// forward declaration
class UTankPlaygroundGameInstance;
class ATankPlaygroundGameModeBase;
class ATankPlayerController;
/**
 * 
 */
UCLASS(Abstract)
class TANKPLAYGROUND_API UWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	void virtual NativeOnInitialized() override;
	// Called when this widget is shown by the widget container
	UFUNCTION(BlueprintImplementableEvent)
	void OnShown();
	// Called when this widget is hidden by the widget container
	UFUNCTION(BlueprintImplementableEvent)
	void OnHidden();
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	UTankPlaygroundGameInstance* GameInstance;
	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	ATankPlaygroundGameModeBase* GameMode;
	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	ATankPlayerController* PC;
};

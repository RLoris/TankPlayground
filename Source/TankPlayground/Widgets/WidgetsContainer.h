// Copyright RLoris 2021

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "WidgetBase.h"
#include "Templates/SubclassOf.h"
#include "WidgetsContainer.generated.h"

// forward declaration
class UTankPlaygroundGameInstance;
/**
 * 
 */
UCLASS(Abstract)
class TANKPLAYGROUND_API UWidgetsContainer : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UWidgetSwitcher* WSwitcher;
public:
	void virtual NativeOnInitialized() override;
	UFUNCTION(BlueprintCallable)
	bool Display(TSubclassOf<UWidgetBase> Widget);
	UFUNCTION(BlueprintPure)
	UWidgetBase* GetDisplayedWidget();
	UFUNCTION(BlueprintPure)
	TSubclassOf<UWidgetBase> GetDisplayedWidgetClass();
	UFUNCTION(BlueprintCallable)
	bool Hide();
private:
	UTankPlaygroundGameInstance* GameInstance;
	TMap<TSubclassOf<UWidgetBase>, UWidgetBase*> Widgets;
	TSubclassOf<UWidgetBase> CurrentMenu;
};

// Copyright RLoris 2021

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WidgetHUD.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UWidgetHUD : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TANKPLAYGROUND_API IWidgetHUD
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void UpdateHealth(float Health);
};

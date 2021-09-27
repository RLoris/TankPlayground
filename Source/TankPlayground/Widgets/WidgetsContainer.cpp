// Copyright RLoris 2021

#include "WidgetsContainer.h"
#include "WidgetBase.h"
#include "../TankPlaygroundGameInstance.h"

void UWidgetsContainer::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	this->GameInstance = Cast<UTankPlaygroundGameInstance>(GetGameInstance());
	if (this->WSwitcher)
	{
		TArray<UWidget*> Children = this->WSwitcher->GetAllChildren();
		// cache child widget ref for quick referencing and avoid casting
		for (UWidget* Child : Children)
		{
			if (UWidgetBase* Widget = Cast<UWidgetBase>(Child))
			{
				this->Widgets.Add(Widget->GetClass(), Widget);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Child widget of widget switcher is not a widgetbase, ignoring child"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget switcher not initialized"));
	}
}

bool UWidgetsContainer::Display(TSubclassOf<UWidgetBase> Widget)
{
	if (UWidgetBase** pW = this->Widgets.Find(Widget))
	{
		// widget already active
		if (Widget != this->CurrentMenu)
		{
			this->WSwitcher->SetActiveWidget(*pW);
			if (this->CurrentMenu)
			{
				// call hidden on old active widget
				(*this->Widgets.Find(this->CurrentMenu))->OnHidden();
			}
			this->CurrentMenu = Widget;
			// call show on new active widget
			(*pW)->OnShown();
		}
		if (!IsInViewport())
		{
			// add to viewport
			AddToPlayerScreen();
		}
		return true;
	}
	return false;
}

UWidgetBase* UWidgetsContainer::GetDisplayedWidget()
{
	if (this->CurrentMenu)
	{
		return *this->Widgets.Find(this->CurrentMenu);
	}
	return nullptr;
}

TSubclassOf<UWidgetBase> UWidgetsContainer::GetDisplayedWidgetClass()
{
	return this->CurrentMenu;
}

bool UWidgetsContainer::Hide()
{
	if (this->CurrentMenu)
	{
		// call hidden on old active widget
		UWidgetBase** pW = this->Widgets.Find(this->CurrentMenu);
		// call hidden on old active widget
		(*pW)->OnHidden();
		// reset current menu
		this->CurrentMenu = nullptr;
	}
	if (IsInViewport())
	{
		// remove from viewport
		RemoveFromViewport();
		// RemoveFromParent();
	}
	return true;
}

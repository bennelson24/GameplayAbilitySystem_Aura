// Copyright Nelson's Games


#include "UI/HUD/AuraHUD.h"

#include "UnrealWidgetFwd.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

// check to see if a widget controller has been created. If it has return that controller, if not, construct one using WC Params and return it
UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);

		// creates callback functions for when an attribute changes
		OverlayWidgetController->BindCallbacksToDependencies();

		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

// construct the widget controller, construct the widget, set the widget controller, and add to the viewport
void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// checkf will check a condition and provide a formatted message if it fails
	checkf(OverlayWidgetClass, TEXT ("Overlay Wdiget Class uninitialized, please fill out BP_AuraHUD"))
	checkf(OverlayWidgetControllerClass, TEXT ("Overlay Wdiget controler Class uninitialized, please fill out BP_AuraHUD"))
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	// get access to the UAurauserwidget member functions by casting
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	// set the widget controller params so we can construct the widget controller
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

	// Using the params to construct the controller using the getter function after we have the params initialized
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	// Setting the widget controller, firing off the blueprint events to update the widgets that are part of the HUD
	OverlayWidget->SetWidgetController(WidgetController);
	// As long as the Widget controller is set --> Then we can broadcast the values
	WidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}



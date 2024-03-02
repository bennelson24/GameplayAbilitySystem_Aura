// Copyright Nelson's Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	// set the controller that sends data to the widget
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController); 
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetContoller;

protected:

	// Event that is called in blueprints whenever a widget controller is set
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};

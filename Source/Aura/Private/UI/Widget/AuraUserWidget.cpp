// Copyright Nelson's Games


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	// sets the widget controller
	WidgetContoller = InWidgetController;
	// passes the controller onto the Blueprint event
	WidgetControllerSet();
}

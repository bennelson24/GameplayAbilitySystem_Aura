// Copyright Nelson's Games

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

// struct for what information is passed from the gameplay tags
/*
 - Gameplay Tag
 - Message text
 - Widget
 - Image
*/

USTRUCT(BlueprintType /*Usable as a type in Blueprints */)
struct FUIWidgetRow : public FTableRowBase /* FTableRowBase is the base class for a row structure */
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag MessageTag = FGameplayTag();

	// FText are user facing texts shown through widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAuraUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image = nullptr;
	
};


class UAuraUserWidget;


// declaring a delegate that can broadcast a float when an attribute changes.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeSignature, float, NewValue);

// Declaring delegate that can broadcast a widget row when an attribute changes.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);



/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangeSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangeSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangeSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangeSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;
	
	


protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	

	// template function for returning a data table row when matched to the gameplay tag
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
	
};

// template functions get defined in the header file
template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	// if the gameplaytag name matches the data table row, return that row
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
	
}

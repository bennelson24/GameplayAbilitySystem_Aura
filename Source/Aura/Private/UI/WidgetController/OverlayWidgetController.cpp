// Copyright Nelson's Games


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	// Casting to use the attribute set to get the child UAura attribute set member variables such as health and mana
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

// creating callback functions using Lamda functions for when an attribute changes
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// Making sure that UAraAttributes can't be null
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	// Specify which attribute changes-> Then specify which callback function is triggered

	/*## Health Change ##*/
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				// when health is changed, broadcast the value for classes that are listening
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
	/*## MaxHealth Change ##*/
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				// when Maxhealth is changed, broadcast the value for classes that are listening
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	/*## Mana Change ##*/
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				// when mana is changed, broadcast the value for classes that are listening
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);

	/*## MaxMana Change ##*/
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				// when the health is changed, broadcast the value for classes that are listening
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);
	

	// when the broadcast is called, the lamda function will fire and recieve the assset tags from the delegate
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
	// loop through the tag container, for each tag in the container, execute the code in the curly braces
		for (const FGameplayTag& Tag: AssetTags)
			{

			// Check that the tag is part of the "Messages" tag hierachy
			//"Message.Healthpotion".MatchesTag("Message") Will return True, "Message".MatchesTag("Message.Healthpotion") will return False
			FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			if(Tag.MatchesTag(MessageTag))
				{
					// Check the gameplay tag to see if correseponds to a data table row in DT_MessageWidgetData
					// Row that will be broadcasted up to the widget
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				};
				
			}
		}
	);
	
}

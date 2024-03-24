// Copyright Nelson's Games


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	// bind to the effect applied delegate
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	// create a container to store gameplay tags
	FGameplayTagContainer TagContainer;
	// get whatever gameplay tags are applied are store them in the tag container
	EffectSpec.GetAllAssetTags(TagContainer);

	// any class that binds to effectassettags delegate will recieve the tag container.
	EffectAssetTags.Broadcast(TagContainer);
	
	
}

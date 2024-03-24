// Copyright Nelson's Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

// forward declaration
class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()


public:
	AAuraPlayerState();

	// overriding the interface function to get the ability system component
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//getter function to get the attribute set
	UAttributeSet* GetAttributeSet() const {return AttributeSet; }

protected:

	// pointers to the ability system component and attribute set for child classes to override
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
};

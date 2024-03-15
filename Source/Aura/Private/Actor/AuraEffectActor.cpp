// Copyright Nelson's Games


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	// Setting a plan object for the root
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

		

}



void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// using a static blueprint function library to check if the target has an ability system component
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (TargetASC == nullptr) return;
	
	checkf(GameplayEffectClass, TEXT("Please check that there is a GameplayEffectClassSet"));
	// creates a wrapper of the effect context in a wrapper - FGameplayeffectcontexthandle
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	// add information for what caused the effect context in case we ever want to find out down the line
	EffectContextHandle.AddSourceObject(this);
	// create an EffectSpecHandle - also passing in the actor level we created to use the curve table
	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	// need to get the const reference to the effectspec from the data wrapper which is also wrapped from the EffectSpecHandle 
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	//Retreiving the gameplay effect so we can see if the duration policy is infinite
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

	// if the gameplay effect duration is infinite and we plan to remove the effect
	// then we will map the effect handle to the target with the ability system component
	// so we can remove it later in the OnEndOverlap function
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
	
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	// Remove the gameplay effect from the ability system component that we linked up when it was added during the infinite apply policy
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		// create an array to store any pairs that we remove from gameplay effects
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;

		// check our map, see if it contains a key value pair that we stored when the infinite effect was applied
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				// remove the gameplay effect by passing in the key we stored in apply effect to actor
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				// mark the handle to be removed from our map
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		// remove the element from the map
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}



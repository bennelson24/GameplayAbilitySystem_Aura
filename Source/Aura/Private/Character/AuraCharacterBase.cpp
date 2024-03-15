// Copyright Nelson's Games


#include "Character/AuraCharacterBase.h"


AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	// attach the weapon to the socket on the mesh
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	// set no collision on the weapon
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// ability system component interface getter function
UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
	
}




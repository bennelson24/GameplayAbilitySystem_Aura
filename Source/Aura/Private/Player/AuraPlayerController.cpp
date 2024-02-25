// Copyright Nelson's Games


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	// when an entity changes on the server, it is sent down from the server to the rest of the clients
	bReplicates = true;
	
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// checks what is under the player cursor every game ticket
	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if(!CursorHit.bBlockingHit) return;


	LastActor = ThisActor;
	// if cast succeeds and hits an enemy implementing the interface it will be returned
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/*
	 * Line trace from cursor. There are several scenarios:
	 * A. Last actor is null && this actor is null
	 *		- Do nothing
	 * B. Last actor is null && this actor is valid
	 *		- Highlight ThisActor
	 * C. Last Actor is valid && ThisActor is null
	 *		- UnHighlight Last Actor
	 * D. Both actors are valid, but LastActor != This Actor.
	 *		- UnHighlight Last Actor, Highlight this actor
	 * E. Both Actors are valid, and are the same actor
	 *		- Do nothing
	 */
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// case B.
			ThisActor->HighlightActor();
		}
		else
		{
			// both are null, do nothing - Case A.
		}
	}
	else // Last actor is valid
	{
		if (ThisActor == nullptr)
		{
			// Case C
			LastActor->UnHighlightActor();
		}
		else // both actors are valid
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				// Case E - do nothing
			}
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// assert that AuraContext is set. Will halt execution if it's not.
	check(AuraContext);

	// the local player subsystem is what we will use to add mapping contexts
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);

	Subsystem->AddMappingContext(AuraContext, 0);

	// Mouse and cursor related settings
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	// doesn't lock mouse to the viewport
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// converting the input component to an EnhancedInputComponent ptr
	// will get a crash if the cast fails using CastChecked
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	// binding the move action to the function below
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	// zeroing out the pitch and roll since we just care about the yaw
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	// gives us the forward vector that corresponds with our controller's rotation vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		// associated with the W and S keys
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		// associated with the A and D keys
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}



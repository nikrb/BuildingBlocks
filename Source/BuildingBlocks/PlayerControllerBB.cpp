// Fill out your copyright notice in the Description page of Project Settings.

// #include "Engine/GameEngine.h"
//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, 
//	FString::Printf(TEXT("do we have crouch: [%d]"), GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch));

#include "PlayerControllerBB.h"
#include "CharacterBB.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void APlayerControllerBBBase::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	PlayerCharacter = Cast<ACharacterBB>(aPawn);
	checkf(PlayerCharacter, TEXT("APlayerControllerBB derived classes should only possess ACharacterBB"));

	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	checkf(EnhancedInputComponent, TEXT("Unable to get ref to EnhancedInputComponent."));

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	checkf(InputSubsystem, TEXT("Unable to get ref to EnhancedInputLocalPlayerSubsystem"));

	checkf(InputMappingContent, TEXT("InputMappingContent was not specified."));
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(InputMappingContent, 0);

	if (ActionMove) {
		EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this,
			&APlayerControllerBBBase::HandleMove);
	}
	if (ActionLook) {
		EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this,
			&APlayerControllerBBBase::HandleLook);
	}
	if (ActionJump) {
		EnhancedInputComponent->BindAction(ActionJump, ETriggerEvent::Triggered, this,
			&APlayerControllerBBBase::HandleJump);
	}
	if (ActionPsiBlast) {
		EnhancedInputComponent->BindAction(ActionPsiBlast, ETriggerEvent::Triggered, this,
			&APlayerControllerBBBase::HandlePsiBlast);
	}

	if (ActionToggleCrouch) {
		EnhancedInputComponent->BindAction(ActionToggleCrouch, ETriggerEvent::Triggered, this,
			&APlayerControllerBBBase::HandleToggleCrouch);
	}

	if (ActionToggleSprint) {
		EnhancedInputComponent->BindAction(ActionToggleSprint, ETriggerEvent::Triggered, this,
			&APlayerControllerBBBase::HandleToggleSprint);
	}
}

void APlayerControllerBBBase::OnUnPossess()
{
	EnhancedInputComponent->ClearActionBindings();

	Super::OnUnPossess();
}

void APlayerControllerBBBase::HandleLook(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void APlayerControllerBBBase::HandleMove(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	if (PlayerCharacter) {
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorForwardVector(), MovementVector.Y);
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorRightVector(), MovementVector.X);
	}
}

void APlayerControllerBBBase::HandleJump()
{
	if (PlayerCharacter) {
		PlayerCharacter->UnCrouch();
		PlayerCharacter->Jump();
	}
}

void APlayerControllerBBBase::HandlePsiBlast()
{
	if (PlayerCharacter) PlayerCharacter->PsiBlast();
}

void APlayerControllerBBBase::HandleToggleSprint()
{
	if (PlayerCharacter) PlayerCharacter->ToggleRunning();
}

void APlayerControllerBBBase::HandleToggleCrouch()
{
	if (PlayerCharacter && PlayerCharacter->bIsCrouched)
		PlayerCharacter->UnCrouch();
	else
		PlayerCharacter->Crouch();
}

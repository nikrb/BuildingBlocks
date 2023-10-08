// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBB.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACharacterBB::ACharacterBB()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickInterval(0.5f);
	SetActorTickEnabled(true);

}

// Called when the game starts or when spawned
void ACharacterBB::BeginPlay()
{
	Super::BeginPlay();
	if (GetMovementComponent()) {
		GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	}
}

// Called every frame
void ACharacterBB::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#pragma region Update Stamina
	float ActualStaminaRecuperationFactor = StaminaRecuperationFactor;
	if (bHasJumped) ActualStaminaRecuperationFactor = -JumpStaminaCost;
	else if (bHasRan) ActualStaminaRecuperationFactor = -RunStaminaCost;
	else if (bIsCrouched) ActualStaminaRecuperationFactor = RestStaminaRebate;

	// TODO: got to here

#pragma endregion

}

// Called to bind functionality to input
void ACharacterBB::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int ACharacterBB::GetHealth()
{
	return CurrentHealth;
}

int ACharacterBB::GetMaxHealth()
{
	return MaxHealth;
}

void ACharacterBB::UpdateHealth(int DeltaHealth)
{
	if (CurrentHealth <= 0.f) return;
	int OldValue = CurrentHealth;

	CurrentHealth += DeltaHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth, -1.f, MaxHealth);

	if (CurrentHealth != OldValue) {
		OnHealthChanged.Broadcast(OldValue, CurrentHealth, MaxHealth);
	}

	if (CurrentHealth <= 0.f) {
		// TODO: player dies
		OnPlayerDied.Broadcast();
	}
}

void ACharacterBB::RestorToFullHealth()
{
	CurrentHealth = MaxHealth;
}

void ACharacterBB::SetMaxHealth(int NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
}

float ACharacterBB::GetStamina()
{
	return CurrentStamina;
}

float ACharacterBB::GetStaminaRecuperationFactor()
{
	return StaminaRecuperationFactor;
}

void ACharacterBB::SetStaminaRecuperationFactor(float NewStaminaRecuperationFactor)
{
	StaminaRecuperationFactor = NewStaminaRecuperationFactor;
}

float ACharacterBB::GetPsiPower()
{
	return CurrentPsiPower;
}

void ACharacterBB::PsiBlast()
{
	if (CurrentPsiPower >= PsiBlastCost) {
		// TODO: do Psi Blast
		CurrentPsiPower -= PsiBlastCost;
	}
}

void ACharacterBB::AddKey(FString KeyToAdd)
{
	if (KeyWallet.Contains(KeyToAdd)) {
		// TODO: already have key - play a noise?
	}
	else {
		KeyWallet.Add(KeyToAdd);
	}
}

void ACharacterBB::RemoveKey(FString KeyToRemove)
{
	if (KeyWallet.Contains(KeyToRemove)) {
		KeyWallet.Remove(KeyToRemove);
	}
}

bool ACharacterBB::IsPlayerCarryingKey(FString DesiredKey)
{
	return KeyWallet.Contains(DesiredKey);
}


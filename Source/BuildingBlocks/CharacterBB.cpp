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

void ACharacterBB::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	if (bIsRunning && CurrentStamina <= 0) {
		SetRunning(false);
	}
	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
	if (bIsRunning) bHasRan = true;
}

void ACharacterBB::Jump()
{
	if (CurrentStamina - JumpStaminaCost >= 0.f) {
		UnCrouch();
		Super::Jump();
		bHasJumped = true;
	}
}

void ACharacterBB::Crouch(bool bClientSimulation)
{
	SetRunning(false);
	Super::Crouch(bClientSimulation);
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

	const float PreviousStamina = CurrentStamina;

	CurrentStamina = FMath::Clamp(CurrentStamina + ActualStaminaRecuperationFactor, 0.f, MaxStamina);
	if (CurrentStamina != PreviousStamina) {
		OnStaminaChanged.Broadcast(PreviousStamina, CurrentStamina, MaxStamina);
	}

	bHasRan = false;
	bHasJumped = false;

#pragma endregion

#pragma region update psi power
	if (CurrentPsiPower != MaxPsiPower) {
		const float PreviousPsiPower = CurrentPsiPower;
		CurrentPsiPower = FMath::Clamp(CurrentPsiPower + PsiRechargeRate, 0, MaxPsiPower);
		OnPsiPowerChanged.Broadcast(PreviousPsiPower, CurrentPsiPower, MaxPsiPower);
	}
#pragma endregion

	// TODO: temp debug info
}

// Called to bind functionality to input
void ACharacterBB::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBB::SetRunning(bool IsRunning) {
	bIsRunning = IsRunning;
	GetCharacterMovement()->MaxWalkSpeed = bIsRunning ? RunningMaxWalkSpeed : NormalMaxWalkSpeed;
}

void ACharacterBB::ToggleRunning() {
	SetRunning(!bIsRunning);
}

void ACharacterBB::SetHasJumped() {
	bHasJumped = true;
}

void ACharacterBB::SetHasRan() {
	bHasRan = true;
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

void ACharacterBB::RestoreToFullHealth()
{
	if (CurrentHealth < MaxHealth) {
		int OldValue = CurrentHealth;
		CurrentHealth = MaxHealth;
		OnHealthChanged.Broadcast(OldValue, CurrentHealth, MaxHealth);
	}
}

void ACharacterBB::SetMaxHealth(int NewMaxHealth)
{
	int OldValue = MaxHealth;
	MaxHealth = NewMaxHealth;
	if (MaxHealth != OldValue) {
		if (MaxHealth < OldValue) {
			if (CurrentHealth > MaxHealth) CurrentHealth = MaxHealth;
		}
		OnHealthChanged.Broadcast(OldValue, CurrentHealth, MaxHealth);
	}
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
		CurrentPsiPower -= PsiBlastCost;
	}
}

void ACharacterBB::AddKey(FString KeyToAdd)
{
	if (KeyWallet.Contains(KeyToAdd)) {
		OnKeyWalletAction.Broadcast(KeyToAdd, EPlayerKeyAction::AddKey, false);
	}
	else {
		KeyWallet.Add(KeyToAdd);
		OnKeyWalletAction.Broadcast(KeyToAdd, EPlayerKeyAction::AddKey, true);
	}
}

void ACharacterBB::RemoveKey(FString KeyToRemove)
{
	if (KeyWallet.Contains(KeyToRemove)) {
		KeyWallet.Remove(KeyToRemove);
		OnKeyWalletAction.Broadcast(KeyToRemove, EPlayerKeyAction::RemoveKey, true);
	}
	else {
		OnKeyWalletAction.Broadcast(KeyToRemove, EPlayerKeyAction::RemoveKey, false);
	}
}

bool ACharacterBB::IsPlayerCarryingKey(FString DesiredKey)
{
	bool Result = KeyWallet.Contains(DesiredKey);
	OnKeyWalletAction.Broadcast(DesiredKey, EPlayerKeyAction::TestKey, Result);
	return Result;
}


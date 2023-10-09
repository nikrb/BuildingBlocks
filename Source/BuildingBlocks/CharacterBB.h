// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBB.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FIntStatUpdated,
	int32, OldValue,
	int32, NewValue,
	int32, MaxValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerIsDead);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFloatStatUpdated,
	float, OldValue,
	float, NewValue,
	float, MaxValue);

UENUM(BlueprintType)
enum class EPlayerKeyAction : uint8 {
	AddKey UMETA(Tooltip="Attempt to add a key to a player's wallet"),
	RemoveKey UMETA(Tooltip = "Attempt to remove a key from a player's wallet"),
	TestKey UMETA(Tooltip = "Check if the player has a specific key")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FKeyWalletAction,
	FString, KeyString,
	EPlayerKeyAction, KeyAction,
	bool, IsSuccess
);

UCLASS()
class BUILDINGBLOCKS_API ACharacterBB : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBB();

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;
	virtual void Jump() override;
	virtual void Crouch(bool bClientSimulation = false) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement", meta = (AllowPrivateAccess = "true"))
	float NormalMaxWalkSpeed = 400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement", meta = (AllowPrivateAccess = "true"))
	float RunningMaxWalkSpeed = 800.0f;
	
	UFUNCTION(BlueprintCallable, Category="Player|Movement")
	void SetRunning(bool IsRunning);

	UFUNCTION(BlueprintCallable, Category="Player|Movement")
	void ToggleRunning();

	UFUNCTION(BlueprintCallable, Category="Player|Movement")
	void SetHasJumped();

	UFUNCTION(BlueprintCallable, Category="Player|Movement")
	void SetHasRan();

#pragma region Health

	UFUNCTION(BlueprintPure, Category="Player|Health")
	int GetHealth();

	UFUNCTION(BlueprintPure, Category="Player|Health")
	int GetMaxHealth();

	UFUNCTION(BlueprintCallable, Category="Player|Health")
	void UpdateHealth(int DeltaHealth);

	UFUNCTION(BlueprintCallable, Category="Player|Health")
	void RestoreToFullHealth();

	UFUNCTION(BlueprintCallable, Category="Player|Health")
	void SetMaxHealth(int NewMaxHealth);

	UPROPERTY(BlueprintAssignable, Category="Player|Health")
	FIntStatUpdated OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Player|Health")
	FPlayerIsDead OnPlayerDied;

#pragma endregion

#pragma region Stamina

	UFUNCTION(BlueprintPure, Category="Player|Stamina")
	float GetStamina();

	UFUNCTION(BlueprintPure, Category="Player|Stamina")
	float GetStaminaRecuperationFactor();

	// TODO: this was BlueprintPure - not allowed with void return
	UFUNCTION(BlueprintCallable, Category="Player|Stamina")
	void SetStaminaRecuperationFactor(float NewStaminaRecuperationFactor);

	UPROPERTY(BlueprintAssignable, Category="Player|Stamina")
	FFloatStatUpdated OnStaminaChanged;

#pragma endregion

#pragma region Psi Power

	UFUNCTION(BlueprintPure, Category = "Player|PsiPower")
	float GetPsiPower();

	// TODO: another BlueprintPure function originally
	UFUNCTION(BlueprintCallable, Category = "Player|PsiPower")
	void PsiBlast();

	UPROPERTY(BlueprintAssignable, Category = "Player|Psi Power")
	FFloatStatUpdated OnPsiPowerChanged;

#pragma endregion

#pragma region Keys

	UFUNCTION(BlueprintCallable, Category = "Player|KeyWallet")
	void AddKey(FString KeyToAdd);

	UFUNCTION(BlueprintCallable, Category = "Player|KeyWallet")
	void RemoveKey(FString KeyToRemove);

	UFUNCTION(BlueprintPure, Category = "Player|KeyWallet")
	bool IsPlayerCarryingKey(FString DesiredKey);

	UPROPERTY(BlueprintAssignable, Category = "Player|KeyWallet")
	FKeyWalletAction OnKeyWalletAction;

#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool bIsRunning = false;
	bool bHasRan = false;
	bool bHasJumped = false;

	static constexpr int BaseStatValue = 100;
	int MaxHealth = BaseStatValue;
	int CurrentHealth = BaseStatValue;

	static constexpr float MaxStamina = 100.0f;
	static constexpr float JumpStaminaCost = 25.0f;
	static constexpr float RunStaminaCost = 5.0f;
	static constexpr float RestStaminaRebate = 4.0f;
	float CurrentStamina = MaxStamina;
	float StaminaRecuperationFactor = 1.0f;

	static constexpr float MaxPsiPower = 1000.0f;
	static constexpr float PsiRechargeRate = 1.0f;
	static constexpr float PsiBlastCost = 150.0f;
	float CurrentPsiPower = MaxPsiPower;

	TArray<FString> KeyWallet;
};

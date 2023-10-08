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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region Health

	UFUNCTION(BlueprintPure, Cateogory="Player|Health")
	int GetHealth();

	UFUNCTION(BlueprintPure, Cateogory = "Player|Health")
	int GetMaxHealth();

	UFUNCTION(BlueprintCallable, Cateogory = "Player|Health")
	void UpdateHealth(int DeltaHealth);

	UFUNCTION(BlueprintCallable, Cateogory = "Player|Health")
	void RestorToFullHealth();

	UFUNCTION(BlueprintCallable, Cateogory = "Player|Health")
	void SetMaxHealth(int NewMaxHealth);

	UPROPERTY(BlueprintAssignable, Category = "Player|Health")
	FIntStatUpdated OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Categor = "Player|Health")
	FPlayerIsDead OnPlayerDied;

#pragma endregion

#pragma region Stamina

	UFUNCTION(BlueprintPure, Cateogory = "Player|Stamina")
	float GetStamina();

	UFUNCTION(BlueprintPure, Cateogory = "Player|Stamina")
	float GetStaminaRecuperationFactor();

	UFUNCTION(BlueprintPure, Cateogory = "Player|Stamina")
	void SetStaminaRecuperationFactor(float NewStaminaRecuperationFactor);

	UPROPERTY(BlueprintAssignable, Category = "Player|Stamina")
	FFloatStatUpdated OnStaminaChanged;

#pragma endregion

#pragma region Psi Power

	UFUNCTION(BlueprintPure, Cateogory = "Player|PsiPower")
	float GetPsiPower();

	UFUNCTION(BlueprintPure, Cateogory = "Player|PsiPower")
	void PsiBlast();

	UPROPERTY(BlueprintAssignable, Category = "Player|Psi Power")
	FFloatStatusUpdated OnPsiPowerChanged;

#pragma endregion

#pragma region Keys

	UFUNCTION(BlueprintCallable, Cateogory = "Player|KeyWallet")
	void AddKey(FString KeyToAdd);

	UFUNCTION(BlueprintCallable, Cateogory = "Player|KeyWallet")
	void RemoveKey(FString KeyToRemove);

	UFUNCTION(BlueprintPure, Cateogory = "Player|KeyWallet")
	bool IsPlayerCarryingKey(FString DesiredKey);

	UPROPERTY(BlueprintAssignable, Category = "Player|KeyWallet")
	FKeyWalletAction OnKeyWalletAction;

#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
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

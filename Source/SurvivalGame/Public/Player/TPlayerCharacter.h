// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"

#include "GameFramework/Character.h"

#include "TPlayerCharacter.generated.h"

class UBaseEffectUIData;

class UPlayerAttributes;
class UTAbilitySystemComponent;
class UAmmoAttributes;

class USpringArmComponent;
class UCameraComponent;

class AWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDied, class ATPlayerCharacter*, Character);

/**
 * Defines metadata about where the damage came from.
 * Used to display a list of all damage received after death
 */
struct FTakenDamageMetadata
{
	void PushNewDamage(const FString& CauserName, UBaseEffectUIData* UIData)
	{
		TPair<UBaseEffectUIData*, int32>& DamageCount = DamageMap.FindOrAdd(CauserName);
		DamageCount.Key = UIData;
		++DamageCount.Value;
	}

private:

	friend class ATPlayerCharacter;

	// Maps a player name to an array of effects/abilities that damaged this player
	TMap<FString, TPair<UBaseEffectUIData*, int32>> DamageMap;
	
	FTakenDamageMetadata()
	{
		// Reserves enough memory to handle taking damage from a single actor
		DamageMap.Reserve(1);
	}

	~FTakenDamageMetadata()
	{
		DamageMap.Empty();
	}
};

UCLASS(config=Game)
class ATPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{

	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;
	
public:
	
	ATPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UE_NODISCARD bool IsAlive() const;

	UFUNCTION(BlueprintAuthorityOnly)
	virtual void RemoveCharacterAbilities();

	void RemoveAllWeaponsFromInventory();
	virtual void KillCharacter();

	UE_NODISCARD FTakenDamageMetadata& GetTakenDamageMetadata_Mutable() const { return *TakenDamageMetadata; }

	UFUNCTION(Client, Reliable, WithValidation)
	void ReceiveNewDamage(UBaseEffectUIData* UIData);

	UPROPERTY(BlueprintAssignable)
	FOnCharacterDied OnCharacterDied;

	// Attribute Helpers
public:

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetHealth() const;
	void SetHealth(float NewHealth) const;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetShield() const;
	void SetShield(float NewShield) const;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMaxShield() const;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMana() const;
	void SetMana(float NewMana) const;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMaxMana() const;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetStamina() const;
	void SetStamina(float NewStamina) const;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetArmor() const;
	void SetArmor(float NewArmor) const;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMaxArmor() const;

protected:

	// Cached gameplay tags
	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;
	FGameplayTag WeaponChangingDelayReplicationTag;
	FGameplayTag CurrentWeaponTag;
	
	FDelegateHandle WeaponChangingDelayReplicationTagChangedDelegateHandle;
	FDelegateHandle PrimaryReserveAmmoChangedDelegateHandle;
	FDelegateHandle SecondaryReserveAmmoChangedDelegateHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	UAnimMontage* DeathMontage;

	/**
	 * The time until the dead body gets destroyed from the world
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	float CorpseDestroyDelay = 10.f;

	UPROPERTY()
	AWeapon* CurrentWeapon;
	
protected:

	void InitNameplates();
	void BindAscInput();
	void InitializeAttributes();
	
	/// Inputs
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginDestroy() override;
	// End of APawn interface

	void DestroyCorpse();
	void OnAbilityActivationFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& TagContainer);
	void OnAbilityActivated(UGameplayAbility* Ability);
	void CurrentWeaponPrimaryReserveAmmoChanged(const FOnAttributeChangeData& Data);
	void CurrentWeaponSecondaryReserveAmmoChanged(const FOnAttributeChangeData& Data);
	
private:

	FTimerHandle DestroyCorpseHandle;
	FTakenDamageMetadata* TakenDamageMetadata;

	TWeakObjectPtr<UTAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<UPlayerAttributes> PlayerAttributes;
	TWeakObjectPtr<UAmmoAttributes> AmmoAttributes;

	bool bAscInputBound;
};


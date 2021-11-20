#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AmmoAttributes.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class UAmmoAttributes : public UAttributeSet
{
	GENERATED_BODY()

public:

	UAmmoAttributes();

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_RifleReserveAmmo)
	FGameplayAttributeData RifleReserveAmmo;
	ATTRIBUTE_ACCESSORS(UAmmoAttributes, RifleReserveAmmo)

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_MaxRifleReserveAmmo)
	FGameplayAttributeData MaxRifleReserveAmmo;
	ATTRIBUTE_ACCESSORS(UAmmoAttributes, MaxRifleReserveAmmo)

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_SmgReserveAmmo)
	FGameplayAttributeData SmgReserveAmmo;
	ATTRIBUTE_ACCESSORS(UAmmoAttributes, SmgReserveAmmo)
	
	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_MaxSmgReserveAmmo)
	FGameplayAttributeData MaxSmgReserveAmmo;
	ATTRIBUTE_ACCESSORS(UAmmoAttributes, MaxSmgReserveAmmo)

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_PistolReserveAmmo)
	FGameplayAttributeData PistolReserveAmmo;
	ATTRIBUTE_ACCESSORS(UAmmoAttributes, PistolReserveAmmo)
	
	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_MaxPistolReserveAmmo)
	FGameplayAttributeData MaxPistolReserveAmmo;
	ATTRIBUTE_ACCESSORS(UAmmoAttributes, MaxPistolReserveAmmo)

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_RocketReserveAmmo)
	FGameplayAttributeData RocketReserveAmmo;
	ATTRIBUTE_ACCESSORS(UAmmoAttributes, RocketReserveAmmo)
	
	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_MaxRocketReserveAmmo)
	FGameplayAttributeData MaxRocketReserveAmmo;
	ATTRIBUTE_ACCESSORS(UAmmoAttributes, MaxRocketReserveAmmo)

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_ShotgunReserveAmmo)
	FGameplayAttributeData ShotgunReserveAmmo;
	ATTRIBUTE_ACCESSORS(UAmmoAttributes, ShotgunReserveAmmo)
	
	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_MaxShotgunReserveAmmo)
	FGameplayAttributeData MaxShotgunReserveAmmo;
	ATTRIBUTE_ACCESSORS(UAmmoAttributes, MaxShotgunReserveAmmo)

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_ThrowableReserveAmmo)
	FGameplayAttributeData ThrowableReserveAmmo;
	ATTRIBUTE_ACCESSORS(UAmmoAttributes, ThrowableReserveAmmo)

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_MaxThrowableReserveAmmo)
	FGameplayAttributeData MaxThrowableReserveAmmo;
	ATTRIBUTE_ACCESSORS(UAmmoAttributes, MaxThrowableReserveAmmo)

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	static FGameplayAttribute GetReserveAmmoAttributeFromTag(const FGameplayTag& PrimaryAmmoTag);
	static FGameplayAttribute GetMaxReserveAmmoAttributeFromTag(const FGameplayTag& PrimaryAmmoTag);

protected:

	// Cached Ammo tags
	static FGameplayTag RifleAmmoTag;
	static FGameplayTag SmgAmmoTag;
	static FGameplayTag PistolAmmoTag;
	static FGameplayTag RocketAmmoTag;
	static FGameplayTag ShotgunAmmoTag;
	static FGameplayTag ThrowableAmmoTag;

	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
	                                 const FGameplayAttributeData& MaxAttribute, float NewMaxValue,
	                                 const FGameplayAttribute& AffectedAttributeProperty);

	UFUNCTION()
	void OnRep_RifleReserveAmmo(const FGameplayAttributeData& OldRifleReserveAmmo);

	UFUNCTION()
	void OnRep_MaxRifleReserveAmmo(const FGameplayAttributeData& OldMaxRifleReserveAmmo);

	UFUNCTION()
	void OnRep_SmgReserveAmmo(const FGameplayAttributeData& OldSmgReserveAmmo);

	UFUNCTION()
	void OnRep_MaxSmgReserveAmmo(const FGameplayAttributeData& OldMaxSmgReserveAmmo);

	UFUNCTION()
	void OnRep_PistolReserveAmmo(const FGameplayAttributeData& OldPistolReserveAmmo);
	
	UFUNCTION()
	void OnRep_MaxPistolReserveAmmo(const FGameplayAttributeData& OldMaxPistolReserveAmmo);

	UFUNCTION()
	void OnRep_ShotgunReserveAmmo(const FGameplayAttributeData& OldShotgunReserveAmmo);
	
	UFUNCTION()
	void OnRep_MaxShotgunReserveAmmo(const FGameplayAttributeData& OldMaxShotgunReserveAmmo);
	
	UFUNCTION()
    void OnRep_RocketReserveAmmo(const FGameplayAttributeData& OldRocketReserveAmmo);

	UFUNCTION()
	void OnRep_MaxRocketReserveAmmo(const FGameplayAttributeData& OldMaxRocketReserveAmmo);

	UFUNCTION()
	void OnRep_ThrowableReserveAmmo(const FGameplayAttributeData& OldThrowableReserveAmmo);

	UFUNCTION()
	void OnRep_MaxThrowableReserveAmmo(const FGameplayAttributeData& OldMaxThrowableReserveAmmo);

};

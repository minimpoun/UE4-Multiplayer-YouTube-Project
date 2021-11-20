#include "AbilitySystem/Attributes/AmmoAttributes.h"

#include "GameplayEffectExtension.h"

#include "Net/UnrealNetwork.h"

FGameplayTag UAmmoAttributes::RifleAmmoTag = FGameplayTag::RequestGameplayTag(TEXT("Weapon.Ammo.Rifle"));
FGameplayTag UAmmoAttributes::SmgAmmoTag = FGameplayTag::RequestGameplayTag(TEXT("Weapon.Ammo.Smg"));
FGameplayTag UAmmoAttributes::PistolAmmoTag = FGameplayTag::RequestGameplayTag(TEXT("Weapon.Ammo.Pistol"));
FGameplayTag UAmmoAttributes::RocketAmmoTag = FGameplayTag::RequestGameplayTag(TEXT("Weapon.Ammo.Rocket"));
FGameplayTag UAmmoAttributes::ShotgunAmmoTag = FGameplayTag::RequestGameplayTag(TEXT("Weapon.Ammo.Shotgun"));
FGameplayTag UAmmoAttributes::ThrowableAmmoTag = FGameplayTag::RequestGameplayTag(TEXT("Weapon.Ammo.Throwable"));

#define GENERATE_ONREP_FUNCTION(AttributeName)\
void UAmmoAttributes::OnRep_##AttributeName(const FGameplayAttributeData& Old##AttributeName)\
{\
GAMEPLAYATTRIBUTE_REPNOTIFY(UAmmoAttributes, AttributeName, Old##AttributeName);\
}

#define GENERATE_ONREP_FUNCTIONS()\
GENERATE_ONREP_FUNCTION(RifleReserveAmmo)\
GENERATE_ONREP_FUNCTION(MaxRifleReserveAmmo)\
GENERATE_ONREP_FUNCTION(SmgReserveAmmo)\
GENERATE_ONREP_FUNCTION(MaxSmgReserveAmmo)\
GENERATE_ONREP_FUNCTION(PistolReserveAmmo)\
GENERATE_ONREP_FUNCTION(MaxPistolReserveAmmo)\
GENERATE_ONREP_FUNCTION(RocketReserveAmmo)\
GENERATE_ONREP_FUNCTION(MaxRocketReserveAmmo)\
GENERATE_ONREP_FUNCTION(ShotgunReserveAmmo)\
GENERATE_ONREP_FUNCTION(MaxShotgunReserveAmmo)\
GENERATE_ONREP_FUNCTION(ThrowableReserveAmmo)\
GENERATE_ONREP_FUNCTION(MaxThrowableReserveAmmo)

UAmmoAttributes::UAmmoAttributes()
{
}

void UAmmoAttributes::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetRifleReserveAmmoAttribute())
	{
		SetRifleReserveAmmo(FMath::Clamp<float>(GetRifleReserveAmmo(), 0, GetMaxRifleReserveAmmo()));	
	}
	else if (Data.EvaluatedData.Attribute == GetSmgReserveAmmoAttribute())
	{
		SetSmgReserveAmmo(FMath::Clamp<float>(GetSmgReserveAmmo(), 0, GetMaxSmgReserveAmmo()));
	}
	else if (Data.EvaluatedData.Attribute == GetPistolReserveAmmoAttribute())
	{
		SetPistolReserveAmmo(FMath::Clamp<float>(GetPistolReserveAmmo(), 0, GetMaxPistolReserveAmmo()));
	}
	else if (Data.EvaluatedData.Attribute == GetRocketReserveAmmoAttribute())
	{
		SetRocketReserveAmmo(FMath::Clamp<float>(GetRocketReserveAmmo(), 0, GetMaxRocketReserveAmmo()));
	}
	else if (Data.EvaluatedData.Attribute == GetShotgunReserveAmmoAttribute())
	{
		SetShotgunReserveAmmo(FMath::Clamp<float>(GetShotgunReserveAmmo(), 0, GetMaxShotgunReserveAmmo()));
	}
	else if (Data.EvaluatedData.Attribute == GetThrowableReserveAmmoAttribute())
	{
		SetThrowableReserveAmmo(FMath::Clamp<float>(GetThrowableReserveAmmo(), 0, GetMaxThrowableReserveAmmo()));
	}
}

void UAmmoAttributes::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributes, RifleReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributes, MaxRifleReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributes, SmgReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributes, MaxSmgReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributes, PistolReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributes, MaxPistolReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributes, RocketReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributes, MaxRocketReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributes, ShotgunReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributes, MaxShotgunReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributes, ThrowableReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributes, MaxThrowableReserveAmmo, COND_None, REPNOTIFY_Always);
}

FGameplayAttribute UAmmoAttributes::GetReserveAmmoAttributeFromTag(const FGameplayTag& PrimaryAmmoTag)
{
	if (PrimaryAmmoTag == RifleAmmoTag)
	{
		return GetRifleReserveAmmoAttribute();
	}

	if (PrimaryAmmoTag == SmgAmmoTag)
	{
		return GetSmgReserveAmmoAttribute();
	}

	if (PrimaryAmmoTag == PistolAmmoTag)
	{
		return GetPistolReserveAmmoAttribute();
	}

	if (PrimaryAmmoTag == RocketAmmoTag)
	{
		return GetRocketReserveAmmoAttribute();
	}

	if (PrimaryAmmoTag == ShotgunAmmoTag)
	{
		return GetShotgunReserveAmmoAttribute();
	}
	
	if (PrimaryAmmoTag == ThrowableAmmoTag)
	{
		return GetThrowableReserveAmmoAttribute();
	}

	return {};
}

FGameplayAttribute UAmmoAttributes::GetMaxReserveAmmoAttributeFromTag(const FGameplayTag& PrimaryAmmoTag)
{
	if (PrimaryAmmoTag == RifleAmmoTag)
	{
		return GetMaxRifleReserveAmmoAttribute();
	}

	if (PrimaryAmmoTag == SmgAmmoTag)
	{
		return GetMaxSmgReserveAmmoAttribute();
	}

	if (PrimaryAmmoTag == PistolAmmoTag)
	{
		return GetMaxPistolReserveAmmoAttribute();
	}

	if (PrimaryAmmoTag == RocketAmmoTag)
	{
		return GetMaxRocketReserveAmmoAttribute();
	}

	if (PrimaryAmmoTag == ShotgunAmmoTag)
	{
		return GetMaxShotgunReserveAmmoAttribute();
	}
	
	if (PrimaryAmmoTag == ThrowableAmmoTag)
	{
		return GetMaxThrowableReserveAmmoAttribute();
	}

	return {};
}

void UAmmoAttributes::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
                                                  const FGameplayAttributeData& MaxAttribute, float NewMaxValue,
                                                  const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		const float CurrentValue  = AffectedAttribute.GetCurrentValue();
		const float NewDelta  = (CurrentMaxValue > 0.f)
									 ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue
									 : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

GENERATE_ONREP_FUNCTIONS()

#undef GENERATE_ONREP_FUNCTION
#undef GENERATE_ONREP_FUNCTIONS
#include "AbilitySystem/Attributes/BaseAttributes.h"

#include "GameplayEffectExtension.h"

#include "Net/UnrealNetwork.h"

#include "Player/TPlayerCharacter.h"

#include "UserInterface/EffectUIData/BaseEffectUIData.h"

UBaseAttributes::UBaseAttributes()
{
	HeadshotTag = FGameplayTag::RequestGameplayTag(TEXT("Effect.Damage.Headshot"));
}

void UBaseAttributes::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute());
	}
	else if (Attribute == GetMoveSpeedAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 150, 1000);
	}
}

void UBaseAttributes::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	const FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* SourceAbilitySystemComponent = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	/// Get the Target
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;

	// Not guaranteed to be valid
	ATPlayerCharacter* TargetCharacter = nullptr;

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.Get())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ATPlayerCharacter>(TargetActor);
	}

	/// Get the Source
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	ATPlayerCharacter* SourceCharacter = nullptr;

	if (SourceAbilitySystemComponent && SourceAbilitySystemComponent->AbilityActorInfo.IsValid() &&
		SourceAbilitySystemComponent->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = SourceAbilitySystemComponent->AbilityActorInfo->AvatarActor.Get();
		SourceController = SourceAbilitySystemComponent->AbilityActorInfo->PlayerController.Get();
		SourceCharacter = Cast<ATPlayerCharacter>(SourceActor);

		if (!SourceController && ensure(SourceActor); const APawn* Pawn = Cast<APawn>(SourceActor))
		{
			SourceController = Pawn->GetController();
		}

		if (SourceController && !SourceCharacter)
		{
			SourceCharacter = Cast<ATPlayerCharacter>(SourceController->GetPawn());
		}

		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float LocalDamage = GetDamage();
		SetDamage(0.f);

		if (LocalDamage > 0.f)
		{
			bool bWasAlive = true;
			if (TargetCharacter)
			{
				bWasAlive = TargetCharacter->IsAlive();
			}

			const float OldShield = GetShield();
			const float DamageAfterShield = FMath::Abs(LocalDamage - OldShield);
			if (OldShield > 0.f)
			{
				SetShield(FMath::Clamp<float>(OldShield - LocalDamage, 0.f, GetMaxShield()));
			}

			if (DamageAfterShield > 0)
			{
				const float NewHealth = GetHealth() - DamageAfterShield;
				SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
			}

			if (TargetCharacter && bWasAlive)
			{
				if (SourceActor != TargetActor)
				{
					TargetCharacter->ReceiveNewDamage(Cast<UBaseEffectUIData>(Data.EffectSpec.Def->UIData));
					// @TODO: Show damage numbers
				}
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}
	else if (Data.EvaluatedData.Attribute == GetShieldAttribute())
	{
		SetShield(FMath::Clamp(GetShield(), 0.f, GetMaxShield()));
	}
	else if (Data.EvaluatedData.Attribute == GetArmorAttribute())
	{
		SetArmor(FMath::Clamp(GetArmor(), 0.f, GetMaxArmor()));
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
}

void UBaseAttributes::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, HealthRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, ManaRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, StaminaRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, MaxShield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, ShieldRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, MaxArmor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, MoveSpeed, COND_None, REPNOTIFY_Always);
}

void UBaseAttributes::AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,
                                                  const FGameplayAttributeData& MaxAttribute, const float& NewMaxValue,
                                                  const FGameplayAttribute& AffectedAttributeProperty) const
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

#define GENERATE_ONREP_FUNCTION(AttributeName)\
void UBaseAttributes::OnRep_##AttributeName(const FGameplayAttributeData& Old##AttributeName)\
{\
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributes, AttributeName, Old##AttributeName);\
}

#define GENERATE_ONREP_FUNCTIONS()\
GENERATE_ONREP_FUNCTION(Health)\
GENERATE_ONREP_FUNCTION(MaxHealth)\
GENERATE_ONREP_FUNCTION(HealthRegenRate)\
GENERATE_ONREP_FUNCTION(Mana)\
GENERATE_ONREP_FUNCTION(MaxMana)\
GENERATE_ONREP_FUNCTION(ManaRegenRate)\
GENERATE_ONREP_FUNCTION(Stamina)\
GENERATE_ONREP_FUNCTION(MaxStamina)\
GENERATE_ONREP_FUNCTION(StaminaRegenRate)\
GENERATE_ONREP_FUNCTION(Shield)\
GENERATE_ONREP_FUNCTION(MaxShield)\
GENERATE_ONREP_FUNCTION(ShieldRegenRate)\
GENERATE_ONREP_FUNCTION(Armor)\
GENERATE_ONREP_FUNCTION(MaxArmor)\
GENERATE_ONREP_FUNCTION(MoveSpeed)

GENERATE_ONREP_FUNCTIONS()

#undef GENERATE_ONREP_FUNCTION
#undef GENERATE_ONREP_FUNCTIONS
#include "Player/TPlayerState.h"

#include "AbilitySystem/Attributes/PlayerAttributes.h"
#include "AbilitySystem/TAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/AmmoAttributes.h"

ATPlayerState::ATPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	PlayerAttributes = CreateDefaultSubobject<UPlayerAttributes>(TEXT("PlayerAttributes"));
	AmmoAttributes = CreateDefaultSubobject<UAmmoAttributes>(TEXT("AmmoAttributes"));
}

UAbilitySystemComponent* ATPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

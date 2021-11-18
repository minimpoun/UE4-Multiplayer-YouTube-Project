#include "Player/TPlayerState.h"

#include "AbilitySystem/PlayerAttributes.h"
#include "AbilitySystem/TAbilitySystemComponent.h"

ATPlayerState::ATPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	PlayerAttributes = CreateDefaultSubobject<UPlayerAttributes>(TEXT("PlayerAttributes"));
}

UAbilitySystemComponent* ATPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

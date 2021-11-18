#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"

#include "TPlayerState.generated.h"

class UPlayerAttributes;
class UTAbilitySystemComponent;
UCLASS()
class ATPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ATPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	[[nodiscard]] UTAbilitySystemComponent*& GetAbilitySystemComponent()
	{
		return AbilitySystemComponent;
	}

	[[nodiscard]] UPlayerAttributes*& GetPlayerAttributes()
	{
		return PlayerAttributes;
	}

private:

	UPROPERTY()
	UTAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UPlayerAttributes* PlayerAttributes;
};
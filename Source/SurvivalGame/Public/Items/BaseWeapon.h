#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"

#include "BaseWeapon.generated.h"

UCLASS()
class AWeapon : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	void SetOwningCharacter(ACharacter* NewOwner) {}

	FGameplayTag PrimaryAmmoTag;
	FGameplayTag SecondaryAmmoTag;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return nullptr; }

};
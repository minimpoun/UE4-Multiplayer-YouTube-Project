#pragma once

#include "CoreMinimal.h"
#include "GlobalTypes.generated.h"

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	Sprint = 0,
	Jump,
	PrimaryFire,
	SecondaryFire,
	FireModeToggle,
	Reload,
	NextWeapon,
	PreviousWeapon,
	Interact,

	Confirm = 253,
	Cancel = 254,

	None = 255,
	MAX = None UMETA(Hidden)
};
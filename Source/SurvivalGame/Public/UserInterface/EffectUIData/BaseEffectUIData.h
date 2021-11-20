#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectUIData.h"

#include "BaseEffectUIData.generated.h"

UCLASS()
class UBaseEffectUIData : public UGameplayEffectUIData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString PlayerName;

	UPROPERTY(EditDefaultsOnly, Category = "User Interface")
	FSlateBrush EffectIcon;

	UPROPERTY(EditDefaultsOnly, Category = "User Interface")
	FText EffectPrettyName;

	UPROPERTY(EditDefaultsOnly, Category = "User Interface")
	float DamageDealt;
};
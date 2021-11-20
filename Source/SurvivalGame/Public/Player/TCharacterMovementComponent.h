#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "TCharacterMovementComponent.generated.h"

UCLASS()
class UTCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FTSavedMove : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

	public:

		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;

		uint8 SavedRequestToStartSprinting:1;
		uint8 SavedRequestToStartADS:1;
	};

	class FTNetworkPredictionData_Client : public FNetworkPredictionData_Client_Character
	{
		typedef FNetworkPredictionData_Client_Character Super;

	public:

		FTNetworkPredictionData_Client(const UCharacterMovementComponent& ClientMovement);

		virtual FSavedMovePtr AllocateNewMove() override;
	};

public:

	UTCharacterMovementComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement Speed")
	float SprintSpeedMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement Speed")
	float ADSSpeedMultiplier;

	uint8 RequestToSprint:1;
	uint8 RequestToADS:1;

	// Cached GameplayTags
	FGameplayTag InteractingTag;
	FGameplayTag InteractingRemovalTag;

	virtual float GetMaxSpeed() const override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StartSprinting();

	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StopSprinting();

	UFUNCTION(BlueprintCallable, Category = "ADS")
	void StartADS();

	UFUNCTION(BlueprintCallable, Category = "ADS")
	void StopADS();
};
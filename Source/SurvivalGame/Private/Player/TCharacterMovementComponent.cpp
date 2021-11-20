#include "Player/TCharacterMovementComponent.h"

#include "AbilitySystemComponent.h"

#include "Player/TPlayerCharacter.h"

// SavedMove

void UTCharacterMovementComponent::FTSavedMove::Clear()
{
	FSavedMove_Character::Clear();
}

uint8 UTCharacterMovementComponent::FTSavedMove::GetCompressedFlags() const
{
	return FSavedMove_Character::GetCompressedFlags();
}

bool UTCharacterMovementComponent::FTSavedMove::CanCombineWith(const FSavedMovePtr& SavedMove_Character,
	ACharacter* InCharacter, float MaxDelta) const
{
	return FSavedMove_Character::CanCombineWith(SavedMove_Character, InCharacter, MaxDelta);
}

void UTCharacterMovementComponent::FTSavedMove::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel,
	FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);
}

void UTCharacterMovementComponent::FTSavedMove::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);
}

// NetworkPredictionData_Client

UTCharacterMovementComponent::FTNetworkPredictionData_Client::FTNetworkPredictionData_Client(
	const UCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{
}

FSavedMovePtr UTCharacterMovementComponent::FTNetworkPredictionData_Client::AllocateNewMove()
{
	return MakeShared<FTSavedMove>();
}

// CharacterMovementComponent

UTCharacterMovementComponent::UTCharacterMovementComponent()
{
	SprintSpeedMultiplier = 1.5f;
	ADSSpeedMultiplier = 0.8f;

	InteractingTag = FGameplayTag::RequestGameplayTag(TEXT("State.Interacting"));
	InteractingRemovalTag = FGameplayTag::RequestGameplayTag(TEXT("State.InteractingRemoval"));
}

float UTCharacterMovementComponent::GetMaxSpeed() const
{
	if (ATPlayerCharacter* OwningCharacter = Cast<ATPlayerCharacter>(GetOwner()); ensure(OwningCharacter))
	{
		if (!OwningCharacter->IsAlive())
		{
			return 0.f;
		}

		UAbilitySystemComponent* ASC = OwningCharacter->GetAbilitySystemComponent();
		if (ensure(ASC) && ASC->GetTagCount(InteractingTag) > ASC->GetTagCount(InteractingRemovalTag))
		{
			return 0.f;
		}

		if (RequestToSprint)
		{
		//	return OwningCharacter->GetMoveSpeed() * SprintSpeedMultiplier;
		}

		if (RequestToADS)
		{
		//	return OwningCharacter->GetMoveSpeed() * ADSSpeedMultiplier;
		}

	//	return OwningCharacter->GetMoveSpeed();
	}

	return 0.f;
}

void UTCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
}

FNetworkPredictionData_Client* UTCharacterMovementComponent::GetPredictionData_Client() const
{
	return Super::GetPredictionData_Client();
}

void UTCharacterMovementComponent::StartSprinting()
{
}

void UTCharacterMovementComponent::StopSprinting()
{
}

void UTCharacterMovementComponent::StartADS()
{
}

void UTCharacterMovementComponent::StopADS()
{
}

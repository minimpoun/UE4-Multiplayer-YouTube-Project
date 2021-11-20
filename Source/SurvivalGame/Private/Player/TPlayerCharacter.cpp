// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/TPlayerCharacter.h"

#include "AbilitySystem/Attributes/PlayerAttributes.h"
#include "AbilitySystem/Attributes/AmmoAttributes.h"
#include "AbilitySystem/TAbilitySystemComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

#include "Core/TBaseGameMode.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Items/BaseWeapon.h"

#include "Player/TCharacterMovementComponent.h"
#include "Player/TPlayerController.h"
#include "Player/TPlayerState.h"

#include "Types/GlobalTypes.h"

#include "UserInterface/EffectUIData/BaseEffectUIData.h"

//////////////////////////////////////////////////////////////////////////
// TPlayerCharacter

ATPlayerCharacter::ATPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTCharacterMovementComponent>(CharacterMovementComponentName))
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	TakenDamageMetadata = new FTakenDamageMetadata();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATPlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATPlayerCharacter::LookUpAtRate);
}

void ATPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ATPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (ATPlayerState* PS = GetPlayerState<ATPlayerState>())
	{
		AbilitySystemComponent = Cast<UTAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		BindAscInput();

		AbilitySystemComponent->AbilityFailedCallbacks.AddUObject(this, &ThisClass::OnAbilityActivationFailed);
		AbilitySystemComponent->AbilityActivatedCallbacks.AddUObject(this, &ThisClass::OnAbilityActivated);

		PlayerAttributes = PS->GetPlayerAttributeSet();
		AmmoAttributes = PS->GetAmmoAttributeSet();

		InitializeAttributes();

		ATPlayerController* PC = Cast<ATPlayerController>(GetController());
		if (PC)
		{
			PC->SetupGameHUD();
		}

		if (CurrentWeapon)
		{
			AbilitySystemComponent->AddLooseGameplayTag(CurrentWeaponTag);
			CurrentWeapon->SetOwningCharacter(this);

			if (!PrimaryReserveAmmoChangedDelegateHandle.IsValid())
			{
				PrimaryReserveAmmoChangedDelegateHandle = AbilitySystemComponent->
				                                          GetGameplayAttributeValueChangeDelegate(
					                                          UAmmoAttributes::GetReserveAmmoAttributeFromTag(
						                                          CurrentWeapon->PrimaryAmmoTag)).AddUObject(
					                                          this, &ThisClass::CurrentWeaponPrimaryReserveAmmoChanged);

				SecondaryReserveAmmoChangedDelegateHandle = AbilitySystemComponent->
				                                            GetGameplayAttributeValueChangeDelegate(
					                                            UAmmoAttributes::GetReserveAmmoAttributeFromTag(
						                                            CurrentWeapon->SecondaryAmmoTag)).AddUObject(
					                                            this,
					                                            &ThisClass::CurrentWeaponSecondaryReserveAmmoChanged);
			}
		}

		if (AbilitySystemComponent->GetTagCount(DeadTag) > 0)
		{
			SetHealth(GetMaxHealth());
			SetMana(GetMaxMana());
			SetStamina(GetMaxStamina());
			SetShield(GetMaxShield());
			SetArmor(GetMaxArmor());
		}

		InitNameplates();
	}
}

void ATPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ATPlayerCharacter::BeginDestroy()
{
	Super::BeginDestroy();

	TakenDamageMetadata->~FTakenDamageMetadata();
	delete TakenDamageMetadata;
	TakenDamageMetadata = nullptr;
}

void ATPlayerCharacter::DestroyCorpse()
{
	Destroy();
}

void ATPlayerCharacter::OnAbilityActivationFailed(const UGameplayAbility* Ability,
                                                  const FGameplayTagContainer& TagContainer)
{
}

void ATPlayerCharacter::OnAbilityActivated(UGameplayAbility* Ability)
{
}

void ATPlayerCharacter::CurrentWeaponPrimaryReserveAmmoChanged(const FOnAttributeChangeData& Data)
{
}

void ATPlayerCharacter::CurrentWeaponSecondaryReserveAmmoChanged(const FOnAttributeChangeData& Data)
{
}

void ATPlayerCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATPlayerCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

UAbilitySystemComponent* ATPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

bool ATPlayerCharacter::IsAlive() const
{
	return PlayerAttributes->GetHealth() > 0.f;
}

void ATPlayerCharacter::RemoveCharacterAbilities()
{
}

void ATPlayerCharacter::RemoveAllWeaponsFromInventory()
{
}

void ATPlayerCharacter::KillCharacter()
{
	if (!HasAuthority()) return;

	ensure(AbilitySystemComponent.IsValid());

	RemoveCharacterAbilities();
	RemoveAllWeaponsFromInventory();

	AbilitySystemComponent->RegisterGameplayTagEvent(WeaponChangingDelayReplicationTag).Remove(
		WeaponChangingDelayReplicationTagChangedDelegateHandle);

	ATBaseGameMode* GM = GetWorld()->GetAuthGameMode<ATBaseGameMode>();
	check(GM);
	GM->PlayerDied(GetController());

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	OnCharacterDied.Broadcast(this);

	AbilitySystemComponent->CancelAllAbilities();

	FGameplayTagContainer EffectTagsToRemove;
	EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
	AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);
	AbilitySystemComponent->AddLooseGameplayTag(DeadTag);

	check(DeathMontage);
	const float& MontageLength = PlayAnimMontage(DeathMontage) + CorpseDestroyDelay;
	GetWorld()->GetTimerManager().SetTimer(DestroyCorpseHandle, this, &ThisClass::DestroyCorpse, MontageLength);
}

float ATPlayerCharacter::GetHealth() const
{
	return PlayerAttributes->GetHealth();
}

void ATPlayerCharacter::SetHealth(float NewHealth) const
{
	PlayerAttributes->SetHealth(NewHealth);
}

void ATPlayerCharacter::SetShield(float NewShield) const
{
	PlayerAttributes->SetShield(NewShield);
}

void ATPlayerCharacter::SetMana(float NewMana) const
{
	PlayerAttributes->SetMana(NewMana);
}

void ATPlayerCharacter::SetStamina(float NewStamina) const
{
	PlayerAttributes->SetStamina(NewStamina);
}

void ATPlayerCharacter::SetArmor(float NewArmor) const
{
	PlayerAttributes->SetArmor(NewArmor);
}

float ATPlayerCharacter::GetMaxHealth() const
{
	return PlayerAttributes->GetMaxHealth();
}

float ATPlayerCharacter::GetShield() const
{
	return PlayerAttributes->GetShield();
}

float ATPlayerCharacter::GetMaxShield() const
{
	return PlayerAttributes->GetMaxShield();
}

float ATPlayerCharacter::GetMana() const
{
	return PlayerAttributes->GetMana();
}

float ATPlayerCharacter::GetMaxMana() const
{
	return PlayerAttributes->GetMaxMana();
}

float ATPlayerCharacter::GetStamina() const
{
	return PlayerAttributes->GetStamina();
}

float ATPlayerCharacter::GetMaxStamina() const
{
	return PlayerAttributes->GetMaxStamina();
}

float ATPlayerCharacter::GetArmor() const
{
	return PlayerAttributes->GetArmor();
}

float ATPlayerCharacter::GetMaxArmor() const
{
	return PlayerAttributes->GetMaxArmor();
}

void ATPlayerCharacter::ReceiveNewDamage_Implementation(UBaseEffectUIData* UIData)
{
	GetTakenDamageMetadata_Mutable().PushNewDamage(UIData->PlayerName, UIData);
}

bool ATPlayerCharacter::ReceiveNewDamage_Validate(UBaseEffectUIData* UIData)
{
	ensure(UIData);
	return UIData != nullptr;
}

void ATPlayerCharacter::InitNameplates()
{
}

void ATPlayerCharacter::BindAscInput()
{
	if (!bAscInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent,
		                                                              FGameplayAbilityInputBinds(
			                                                              FString("ConfirmTarget"),
			                                                              FString("CancelTarget"),
			                                                              FString("EGSAbilityInputID"),
			                                                              static_cast<int32>(
				                                                              EAbilityInputID::Confirm),
			                                                              static_cast<int32>(
				                                                              EAbilityInputID::Cancel)));

		bAscInputBound = true;
	}
}

void ATPlayerCharacter::InitializeAttributes()
{
}

void ATPlayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATPlayerCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

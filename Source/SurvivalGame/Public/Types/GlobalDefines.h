#pragma once

#define ACTOR_ROLE_AS_FSTRING *(FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true)->GetNameStringByValue(GetLocalRole()))
#define GET_ACTOR_ROLE_AS_FSTRING(Actor) *(FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true)->GetNameStringByValue(Actor->GetLocalRole()))

#define COLLISION_ABILITY						ECollisionChannel::ECC_GameTraceChannel1
#define COLLISION_PROJECTILE					ECollisionChannel::ECC_GameTraceChannel2
#define COLLISION_ABILITYOVERLAPPROJECTILE		ECollisionChannel::ECC_GameTraceChannel3
#define COLLISION_PICKUP						ECollisionChannel::ECC_GameTraceChannel4
#define COLLISION_INTERACTABLE					ECollisionChannel::ECC_GameTraceChannel5
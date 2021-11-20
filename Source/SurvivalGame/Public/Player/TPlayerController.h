#pragma once

#include "CoreMinimal.h"

#include "TPlayerController.generated.h"

UCLASS()
class ATPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	void SetupGameHUD();
};
// 

#pragma once

#include "ISurvivalGameLoadingScreenModule.h"

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSurvivalGameLoadingScreenModule final
    : public ISurvivalGameLoadingScreenModuleInterface
{
public:
    void StartupModule() override;
    void ShutdownModule() override;
};

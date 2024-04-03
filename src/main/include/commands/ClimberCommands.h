#pragma once

#include <frc2/command/Commands.h>
#include <frc/XboxController.h>
#include "subsystems/Climber.h"

namespace ClimberCommands 
{
    frc2::CommandPtr RaiseHook(Climber* climber, frc::XboxController* controller);

    frc2::CommandPtr LowerHook(Climber* climber, frc::XboxController* controller);

    frc2::CommandPtr StopClimber(Climber* climber);
}
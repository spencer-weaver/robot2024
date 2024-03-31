#pragma once

#include <frc2/command/Commands.h>
#include "subsystems/AmpShooter.h"

namespace AmpShooterCommands
{
    frc2::CommandPtr RunIntake(AmpShooter* ampShooter);

    frc2::CommandPtr Outtake(AmpShooter* ampShooter);
}
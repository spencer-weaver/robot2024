#include "commands/AmpShooterCommands.h"
#include "constants/AmpShooterConstants.h"
#include <frc2/command/StartEndCommand.h>
#include <frc2/command/WaitCommand.h>

namespace AmpShooterCommands 
{
    frc2::CommandPtr RunIntake(AmpShooter* ampShooter)
    {
        return frc2::cmd::StartEnd(
            // Start
            [ampShooter] { ampShooter->SetVoltage(constants::ampShooter::intakeVoltage); },

            // End
            [ampShooter] { ampShooter->Stop(); },

            {ampShooter}
        );
    }

    frc2::CommandPtr Outtake(AmpShooter* ampShooter)
    {
        return ampShooter->RunOnce([ampShooter] { ampShooter->SetVoltage(-constants::ampShooter::outtakeVoltage); })
            .AndThen(frc2::cmd::Wait(constants::ampShooter::outtakeTime))
            .AndThen(ampShooter->RunOnce([ampShooter] { ampShooter->Stop(); }));
    }
}
#include "commands/ClimberCommands.h"
#include "constants/ClimberConstants.h"
#include <frc2/command/StartEndCommand.h>

namespace ClimberCommands
{
    frc2::CommandPtr RaiseHook(Climber* climber, frc::XboxController* controller)
    {
        return frc2::cmd::StartEnd(
            // Start
            [climber, controller] {
                if(!climber->ReachedMaxPosition() || controller->GetPOV() == 180)
                    climber->SetVoltage(constants::climber::motorVoltage); 
            },
            
            // End
            [climber] {climber->Stop(); },

            {climber}
        ).Until([climber, controller] { return climber->ReachedMinPosition() && controller->GetPOV() != 180; /*  || climber->CurrentLimitReached(); */ });
    }

    frc2::CommandPtr LowerHook(Climber* climber, frc::XboxController* controller)
    {
        return frc2::cmd::StartEnd(
            // Start
            [climber, controller] {
                if(!climber->ReachedMinPosition() || controller->GetPOV() == 180)
                    climber->SetVoltage(-constants::climber::climbLowerVoltage); 
            },
            
            // End
            [climber] {climber->Stop(); },

            {climber}
        ).Until([climber, controller] { return climber->ReachedMinPosition() && controller->GetPOV() != 180; /*  || climber->CurrentLimitReached();  */});
    }

    frc2::CommandPtr StopClimber(Climber* climber)
    {
        return climber->RunOnce([climber] {climber->Stop(); } );
    }
}
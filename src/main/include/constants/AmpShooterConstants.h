#pragma once

#include <units/voltage.h>
#include <units/time.h>

namespace constants 
{
    namespace ampShooter 
    {
        constexpr units::volt_t intakeVoltage = 4.0_V;
        constexpr units::volt_t outtakeVoltage = 10.0_V;

        constexpr units::second_t outtakeTime = 0.5_s;
    }
}
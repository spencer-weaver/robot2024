#pragma once

#include <units/voltage.h>
#include <units/current.h>
#include <units/time.h>

namespace constants 
{
    namespace intake 
    {
        constexpr units::volt_t intakePower = 8.75_V;
        constexpr units::volt_t ejectPower = -10.0_V;
        constexpr units::volt_t feedToShooterPower = 7.0_V;

        // z t aaa mjJconstexpr units::volt_t intakeFeederPower = 2.0_V;

        constexpr units::volt_t noteRetentionPower = 1.0_V;
        constexpr units::ampere_t maxNoteRetentionCurrent = 16.0_A;

        constexpr units::second_t feedToShooterTime = 0.5_s;

        constexpr units::millisecond_t noteDetectionDelay = 100_ms;
    }
}
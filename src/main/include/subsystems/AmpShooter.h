#pragma once

#include <frc2/command/SubsystemBase.h>
#include <rev/CANSparkMax.h>
#include <units/voltage.h>
#include <memory>

class AmpShooter : public frc2::SubsystemBase
{
public:
    AmpShooter();
    
    void SetVoltage(units::volt_t voltage);
    void Stop();

private:
    std::unique_ptr<rev::CANSparkMax> m_motor;
};
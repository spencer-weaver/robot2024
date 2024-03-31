#include "subsystems/AmpShooter.h"
#include "constants/Ports.h"

AmpShooter::AmpShooter()
{
    fmt::print("Initializing Amp Shooter...\n");

    m_motor = std::make_unique<rev::CANSparkMax>(
        ports::ampShooter::motorCAN,
        rev::CANSparkMax::MotorType::kBrushless);
        
    m_motor->SetInverted(true);
}

void AmpShooter::SetVoltage(units::volt_t voltage)
{
    m_motor->SetVoltage(voltage);
}

void AmpShooter::Stop()
{
    m_motor->StopMotor();
}
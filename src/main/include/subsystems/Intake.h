#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/DigitalInput.h>
#include <frc/filter/Debouncer.h>
#include <rev/CANSparkMax.h>
#include <units/voltage.h>
#include <units/current.h>
#include <memory>

class Intake : public frc2::SubsystemBase
{
public:
    Intake();

    void Periodic() override;
    
    void RunIntake(units::volt_t voltage);
    void StopIntake();
    
    bool IsNoteDetected() const;

    units::ampere_t GetIntakeCurrent() const;

private:
    std::unique_ptr<rev::CANSparkMax> m_intakeMotor;
    std::unique_ptr<frc::DigitalInput> m_photoElectricSensor; 
    
    std::unique_ptr<frc::Debouncer> m_debouncer;
};
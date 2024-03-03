// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/CommandPtr.h>
#include <frc/XboxController.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc2/command/Command.h>
#include "subsystems/DriveBase.h"
#include "subsystems/Intake.h"
#include "subsystems/Shooter.h"

class RobotContainer {
 public:
  RobotContainer();

  frc2::CommandPtr GetAutonomousCommand();

  void TestInit();
  void TestPeriodic();

 private:
  void ConfigureDriveControls();
  void ConfigureShooterControls();

  frc::XboxController m_driveController {0};
  frc::XboxController m_shooterController {1};
  
  DriveBase m_driveBase {};
  Intake m_intake {};
  Shooter m_shooter {};
};

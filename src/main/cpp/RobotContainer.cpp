// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <frc2/command/Commands.h>
#include <frc2/command/FunctionalCommand.h>
#include <frc2/command/button/JoystickButton.h>
#include <frc2/command/button/Trigger.h>
#include <frc2/command/button/POVButton.h>
#include <frc/smartdashboard/SmartDashboard.h>

#include <pathplanner/lib/controllers/PPHolonomicDriveController.h>
#include <pathplanner/lib/commands/PathPlannerAuto.h>
#include <pathplanner/lib/auto/NamedCommands.h>
#include <pathplanner/lib/auto/AutoBuilder.h>

#include "commands/DriveCommand.h"
#include "commands/IntakeCommands.h"
#include "commands/ShooterCommands.h"
#include "commands/AmpShooterCommands.h"
#include "commands/ClimberCommands.h"

#include "constants/GeneralConstants.h"
#include "constants/ShooterConstants.h"
#include "constants/DriveConstants.h"
#include "constants/AutoConstants.h"
#include "lib/Util.h"
#include "lib/FieldUtil.h"

RobotContainer::RobotContainer() {
  if(!constants::enableSysId)
  {
    ConfigureDriveControls();
    ConfigureShooterControls();
  }

  m_autoChooser.SetDefaultOption(constants::autonomous::defaultAuto, constants::autonomous::defaultAuto);
  m_autoChooser.AddOption(constants::autonomous::oneNoteAuto, constants::autonomous::oneNoteAuto);

  for(const std::string& autoName : constants::autonomous::autoNames)
  {
    m_autoChooser.AddOption(autoName, autoName);
  }

  frc::SmartDashboard::PutData(&m_autoChooser);

  pathplanner::NamedCommands::registerCommand("RunIntake", IntakeCommands::RunIntake(&m_intake).WithTimeout(constants::autonomous::intakeTimeLimit));
  pathplanner::NamedCommands::registerCommand("ShootToSpeaker", ShooterCommands::ShootToSpeaker(&m_shooter, &m_intake));
  pathplanner::NamedCommands::registerCommand("RunShooterWheels", ShooterCommands::RunShooterWheels(&m_shooter, constants::shooter::speakerShootSpeed));

  // pathplanner::PPHolonomicDriveController::setRotationTargetOverride([this] () {
  //   return m_vision.GetTargetAngle();
  // });
}

void RobotContainer::ConfigureDriveControls() 
{
  m_driveBase.SetDefaultCommand(DriveCommand(&m_driveBase, m_vision, m_driveController));

  frc2::JoystickButton(&m_driveController, frc::XboxController::Button::kX)
    .OnTrue(frc2::cmd::RunOnce([this] { m_driveBase.ZeroHeading(); m_driveBase.SetNavXHeading(0_rad); }, {}));
}

void RobotContainer::ConfigureShooterControls()
{
  // m_intake.SetDefaultCommand(IntakeCommands::NoteRetentionCommand(&m_intake));

  // Run intake while A is held
  frc2::JoystickButton(&m_shooterController, frc::XboxController::Button::kA)
    .WhileTrue(IntakeCommands::RunIntake(&m_intake));

  frc2::Trigger([this] { return m_intake.IsNoteDetected(); })
    .OnTrue(
      frc2::cmd::RunOnce([this] {
        m_driveController.SetRumble(frc::GenericHID::RumbleType::kBothRumble, 1.0);
      })
      .AndThen(frc2::cmd::Wait(0.1_s))
      .FinallyDo([this] {
        m_driveController.SetRumble(frc::GenericHID::RumbleType::kBothRumble, 0);
      }));

  frc2::POVButton(&m_shooterController, 0)
    .WhileTrue(ShooterCommands::RunShooterWheels(&m_shooter, constants::shooter::speakerShootSpeed))
    .OnFalse(ShooterCommands::StopShooterWheels(&m_shooter));

  // Shoot to speaker with X
  frc2::JoystickButton(&m_shooterController, frc::XboxController::Button::kX)
    .OnTrue(ShooterCommands::ShootToSpeaker(&m_shooter, &m_intake));
    //.OnTrue(ShooterCommands::Shoot(&m_shooter, &m_intake, 1000_rpm));
    
  // Shoot across field
  frc2::JoystickButton(&m_shooterController, frc::XboxController::Button::kY)
    .OnTrue(ShooterCommands::Shoot(&m_shooter, &m_intake, 5250_rpm));
    //.OnTrue(ShooterCommands::ShootToAmp(&m_shooter, &m_intake));
    //.OnFalse(ShooterCommands::StopShooterWheels(&m_shooter));

  // Eject from intake with B
  frc2::JoystickButton(&m_shooterController, frc::XboxController::Button::kB)
    .OnTrue(IntakeCommands::EjectIntake(&m_intake, &m_shooter))
    .OnFalse(IntakeCommands::StopIntake(&m_intake))
    .OnFalse(ShooterCommands::StopFeeder(&m_shooter));

  frc2::Trigger([this] { return m_shooterController.GetLeftTriggerAxis() > constants::controls::axisDeadband; })
    .WhileTrue(ShooterCommands::ShooterIntake(&m_shooter, &m_intake));

  // Amp intake with left trigger
  // frc2::Trigger([this] { return m_shooterController.GetLeftTriggerAxis() > constants::controls::axisDeadband; })
  //   .WhileTrue(AmpShooterCommands::RunIntake(&m_ampShooter));

  // // Amp shoot with right trigger    
  // frc2::Trigger([this] { return m_shooterController.GetRightTriggerAxis() > constants::controls::axisDeadband; })
  //   .OnTrue(AmpShooterCommands::Outtake(&m_ampShooter));

  // Raise climber with left bumper
  frc2::JoystickButton(&m_shooterController, frc::XboxController::Button::kLeftBumper)
    .OnTrue(ClimberCommands::RaiseHook(&m_climber, &m_shooterController))
    .OnFalse(ClimberCommands::StopClimber(&m_climber));

  // Lower climber with right bumper
  frc2::JoystickButton(&m_shooterController, frc::XboxController::Button::kRightBumper)
    .OnTrue(ClimberCommands::LowerHook(&m_climber, &m_shooterController))
    .OnFalse(ClimberCommands::StopClimber(&m_climber));
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand() {
  std::string autoName = m_autoChooser.GetSelected();

  if(autoName == constants::autonomous::noneAuto || autoName == "")
    return frc2::cmd::None();

  if(autoName == constants::autonomous::oneNoteAuto)
    return ShooterCommands::ShootToSpeaker(&m_shooter, &m_intake);

  // frc::Pose2d startingPose = pathplanner::PathPlannerAuto::getStartingPoseFromAutoFile(autoName);  
  // startingPose = frc::Pose2d(startingPose.X(), startingPose.Y(), m_driveBase.GetPose().Rotation());
  // m_driveBase.ResetPose(startingPose);

  return pathplanner::PathPlannerAuto(autoName).ToPtr()
    .OnlyWhile([&] { return m_driveBase.IsNavXAvailable(); })
    .FinallyDo([&] { m_driveBase.Stop(); });
}

void RobotContainer::InitSysId()
{
  auto routine = m_shooter.GetSysIdRoutine();

  frc2::JoystickButton(&m_driveController, frc::XboxController::Button::kA)
     .OnTrue(m_shooter.GetSysIdRoutine());

  frc2::JoystickButton(&m_driveController, frc::XboxController::Button::kX)
     .WhileTrue(m_driveBase.GetSysIdRoutine());
}

void RobotContainer::SetDriveControlMode(ControlMode controlMode)
{
  m_driveBase.SetDriveControlMode(controlMode);
}
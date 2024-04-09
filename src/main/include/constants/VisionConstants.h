#pragma once

#include <photon/PhotonPoseEstimator.h>
#include <frc/geometry/Transform3d.h>
#include <frc/geometry/Rotation3d.h>
#include <wpi/array.h>
#include <units/length.h>
#include <units/angle.h>
#include <string>

namespace constants
{
    namespace vision
    {
        // Main pose strategy
        constexpr photon::PoseStrategy poseStrategy = photon::PoseStrategy::MULTI_TAG_PNP_ON_COPROCESSOR;

        // Backup pose strategy (when only one tag is detected)
        constexpr photon::PoseStrategy backupPoseStrategy = photon::PoseStrategy::LOWEST_AMBIGUITY;

        // Max pose ambiguity (for single tag)
        constexpr double maxAmbiguity = 0.2;
        constexpr double maxMultiTagAmbiguity = 0.3;

        // Max distance (for single tag)
        constexpr units::meter_t maxTagDistance = 4.0_m;
        constexpr units::meter_t maxMultiTagDistance = 5.0_m;
  
        // Default standard deviations 
        constexpr wpi::array<double, 3U> singleTagStdDevs {3.75, 3.75, 7.5};
        constexpr wpi::array<double, 3U> multiTagStdDevs {0.75, 0.75, 1.5};

        // Standard deviations scale factor (scaled as: distance^2 * stdDevsScaleFactor)
        constexpr double stdDevsScaleFactor = 1.0 / 30.0;

        namespace frontCamera 
        {
            constexpr std::string_view name = "Front Arducam";
            const frc::Translation3d relativePosition {2.0_in, -6.25_in, 16.0_in};
            const frc::Rotation3d relativeRotation {180_deg, -29.0_deg, 0.0_deg};
        }

        namespace backCamera
        {
            constexpr std::string_view name = "Back Arducam";
            const frc::Translation3d relativePosition {-8.25_in, 6.75_in, 17.875_in};
            const frc::Rotation3d relativeRotation {180_deg, -21.0_deg, 180.0_deg};
        }

        namespace objectCamera 
        {
            constexpr std::string_view name = "Object Detection Camera";
        }
    }
}
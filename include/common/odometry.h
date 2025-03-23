#pragma once
#include "common/robot_localization_filter.h"
#include "common/dual_imu.h"
#include "pros/adi.hpp"

/**
 * @class Odometry
 * @brief Tracks robot position using encoder pods and IMU
 */
class Odometry {
private:
    // Position tracking
    RobotLocalizationFilter localizationFilter;
    
    // Heading sensor
    DualIMU& imu;
    
    // Encoder pods
    pros::ADIEncoder encoderX;
    pros::ADIEncoder encoderY;
    
    // Tracking parameters
    double lastUpdateTime;
    double encoderXOffset, encoderYOffset;  // Distance from center of rotation
    double encoderXScaleFactor, encoderYScaleFactor;  // Ticks to distance conversion
    
    // Previous encoder values for delta calculation
    int lastEncoderX, lastEncoderY;
    
    // Internal state
    double x, y, theta;  // Current pose estimate
    double vx, vy, omega;  // Current velocity estimate
    
public:
    /**
     * @brief Constructs an Odometry object
     * @param imu Reference to a DualIMU for heading
     * @param xEncoderPorts Ports for X tracking encoder
     * @param yEncoderPorts Ports for Y tracking encoder
     * @param xOffset X encoder offset from center (mm)
     * @param yOffset Y encoder offset from center (mm)
     * @param xScale X encoder scale factor (mm/tick)
     * @param yScale Y encoder scale factor (mm/tick)
     */
    Odometry(
        DualIMU& imu,
        std::pair<int, int> xEncoderPorts,
        std::pair<int, int> yEncoderPorts,
        double xOffset = 0.0,
        double yOffset = 0.0,
        double xScale = 1.0,
        double yScale = 1.0
    );
    
    /**
     * @brief Updates position estimate using encoders and IMU
     */
    void update();
    
    /**
     * @brief Gets current pose estimate
     * @return Pose as (x, y, theta)
     */
    std::array<double, 3> getPose() const;
    
    /**
     * @brief Gets current velocity estimate
     * @return Velocity as (vx, vy, omega)
     */
    std::array<double, 3> getVelocity() const;
    
    /**
     * @brief Resets the odometry to a specific pose
     * @param x X position (mm)
     * @param y Y position (mm)
     * @param theta Heading (radians)
     */
    void reset(double x = 0.0, double y = 0.0, double theta = 0.0);
};

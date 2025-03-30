#pragma once
#include "pros/imu.hpp"
#include "common/localization_filter.h"
#include <cmath>
#include <functional>
#include "../dualEnable.h"

/**
 * @class DualIMU
 * @brief Dual sensor fusion for a pair of pros::IMU to reduce drift.
 */
class DualIMU {
private:
    pros::IMU imu1;
    pros::IMU imu2;
    double driftThreshold;
    double lastUpdateTime;
    double currentTime;  // Track current time as member to avoid static issues
    
    // Raw IMU readings
    double r1, r2;    // rotation readings
    double y1, y2;    // yaw readings
    double p1, p2;    // pitch readings
    double rl1, rl2;  // roll readings
    double h1, h2;    // heading readings
    
    // Time and motion derivatives
    double dt;
    double prevHeadingVelocity;  // Store previous value for acceleration calculation
    double rotationVelocity, rotationAccel;
    double headingVelocity, headingAccel;
    
    // Filter for heading estimation (will be part of robot pose)
    RobotLocalizationFilter *headingFilter;
    
    // Use radians internally
    bool useRadians;
    
    // Helper method for sensor fusion
    inline void fuseSensorReadings();
    
    // Compensate for drift between sensors
    inline bool isDriftExcessive(double val1, double val2) const;
    
    // Angle conversion helpers
    inline double normalizeAngle(double angle) const;
    inline double angleDifference(double a, double b) const;

    // Add these variables to store filtered acceleration values
    double filtered_accel_x;
    double filtered_accel_y;
    double filtered_accel_z;

    // Add reliability tracking for IMUs
    bool imu1Reliable;
    bool imu2Reliable;
    int imu1UnreliableCount;
    int imu2UnreliableCount;
    
    // Constants for reliability management
    static constexpr int MAX_UNRELIABLE_COUNT = 100;  // Number of updates before considering rehabilitation
    static constexpr int REQUIRED_RELIABLE_READINGS = 5;  // Consecutive readings within threshold to rehabilitate
    
    // Enhanced drift detection and reliability management
    void updateReliability();
    bool isIMUReliable(int imuNum) const;
    void rehabilitateIMUs();
    
public:
    /**
     * @brief Constructs a DualIMU object.
     * @param port1 Port number for the first IMU.
     * @param port2 Port number for the second IMU.
     * @param driftThreshold Threshold for drift detection
     * @param useRadians Whether to use radians (true) or degrees (false)
     */
    DualIMU(int port1, int port2, RobotLocalizationFilter &filter, double driftThreshold = 10.0, bool useRadians = false);
    
    /**
     * @brief Updates heading estimation from both IMUs
     */
    void update();
    
    /**
     * @brief Gets the current heading estimate in radians
     * @return Heading in radians [-π, π]
     */
    double getHeading() const;
    
    /**
     * @brief Gets the current heading velocity in radians/sec
     * @return Angular velocity in radians/sec
     */
    double getHeadingVelocity() const;
    
    /**
     * @brief Gets the current yaw estimate
     * @return Yaw angle in degrees
     */
    double getYaw() const;
    
    /**
     * @brief Gets the current pitch estimate
     * @return Pitch angle in degrees
     */
    double getPitch() const;
    
    /**
     * @brief Gets the current roll estimate
     * @return Roll angle in degrees
     */
    double getRoll() const;
    
    /**
     * @brief Resets the IMUs and filter
     * @param blocking Whether to block until calibration is complete
     */
    void reset(bool blocking = true);
    
    /**
     * @brief Checks if either IMU is still calibrating
     * @return True if either IMU is calibrating
     */
    bool isCalibrating() const;
    
    /**
     * @brief Gets the status of the IMUs
     * @return Status of the second IMU
     */
    pros::ImuStatus getStatus() const;
    
    /**
     * @brief Gets the current heading in radians ([-π, π])
     * @return Heading in radians
     */
    double getHeadingRadians() const;
    
    /**
     * @brief Gets the current heading in degrees ([0, 360))
     * @return Heading in degrees
     */
    double getHeadingDegrees() const;

    /**
     * @brief Gets the filtered X acceleration
     * @return X acceleration in m/s²
     */
    double getAccelX() const { return filtered_accel_x; }
    
    /**
     * @brief Gets the filtered Y acceleration
     * @return Y acceleration in m/s²
     */
    double getAccelY() const { return filtered_accel_y; }
    
    /**
     * @brief Gets the filtered Z acceleration
     * @return Z acceleration in m/s²
     */
    double getAccelZ() const { return filtered_accel_z; }

    /**
     * @brief Gets the reliability status of IMU 1
     * @return True if IMU 1 is currently considered reliable
     */
    bool isIMU1Reliable() const { return imu1Reliable; }
    
    /**
     * @brief Gets the reliability status of IMU 2
     * @return True if IMU 2 is currently considered reliable
     */
    bool isIMU2Reliable() const { return imu2Reliable; }
};
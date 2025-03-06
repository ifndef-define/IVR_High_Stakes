#pragma once
#include "pros/imu.hpp"
#include "common/kalman_filter.h"
#include <cmath>
#include <functional>

/**
 * @class DualIMU
 * @brief Dual sensor fusion for a pair of pros::IMU to reduce drift.
 */
class DualIMU {
private:
    pros::IMU imu1;
    pros::IMU imu2;
    double driftThreshold; // Used for outlier detection
    double lastUpdateTime; // For tracking dt between updates
    
    // Reusable sensor reading variables to avoid stack allocations
    double r1, r2;    // rotation readings
    double y1, y2;    // yaw readings
    double p1, p2;    // pitch readings
    double rl1, rl2;  // roll readings
    double h1, h2;    // heading readings
    double dt;        // time delta between updates
    double rotVelocityBefore, rotVelocityAfter, rotAccel; // velocity/acceleration calculation
    double headingVelocityBefore, headingVelocityAfter, headingAccel;
    double yawVelocityBefore, yawVelocityAfter, yawAccel;
    double pitchVelocityBefore, pitchVelocityAfter, pitchAccel;
    double rollVelocityBefore, rollVelocityAfter, rollAccel;

    KalmanFilter rotFilter;
    KalmanFilter headingFilter;
    KalmanFilter yawFilter;
    KalmanFilter pitchFilter;
    KalmanFilter rollFilter;
    
    // Helper method to handle the update logic for a single measurement type
    inline void applyFilter(KalmanFilter& filter, const double& val1, const double& val2);

public:
    /**
     * @brief Constructs a DualIMU object.
     * @param port1 Port number for the first IMU.
     * @param port2 Port number for the second IMU.
     * @param driftThreshold (Optional) A threshold value (in degrees) for outlier handling.
     */
    DualIMU(int port1, int port2, double driftThreshold = 10.0);

    /**
     * @brief Updates each filter with readings from both IMUs.
     */
    void update();

    // Accessor methods
    double get_rotation() const;
    double get_rotation_velocity() const;
    double get_heading() const;
    double get_yaw() const;
    double get_pitch() const;
    double get_roll() const;

    void reset(bool blocking=true);
    bool is_calibrating() const;
    pros::ImuStatus get_status() const;
};
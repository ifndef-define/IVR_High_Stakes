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

    bool imu1Disconnected = false;
    bool imu2Disconnected = false;
    
    // Anti-drift parameters - simplified
    double stationaryVelocityThreshold = 1.0;  // Higher threshold for better stability
    bool isStationary = false; // Flag to track if robot is stationary
    int stationaryCount = 0;   // Counter for consecutive stationary readings
    int stationaryCountThreshold = 5; // Readings needed to confirm stationary
    bool driftCompensationEnabled = true; // Allow enabling/disabling drift compensation
    
    // Last stable values (used when stationary)
    double lastStableRotation = 0.0;
    double lastStableHeading = 0.0;
    double lastStableYaw = 0.0;
    double lastStablePitch = 0.0;
    double lastStableRoll = 0.0;

    KalmanFilter rotFilter;
    KalmanFilter headingFilter;
    KalmanFilter yawFilter;
    KalmanFilter pitchFilter;
    KalmanFilter rollFilter;
    
    // Added member variables to store IMU readings and calculations
    double currentTime;
    double dt;
    double rotVelocityBefore;
    double rotVelocityAfter;
    double rotAccel;
    
    // IMU readings as member variables to reduce stack usage
    double r1, y1, p1, rl1;
    double r2, y2, p2, rl2;
    
    // Helper method to handle the update logic for a single measurement type
    inline void applyFilter(KalmanFilter& filter, double val1, double val2);
    
    // Helper method to determine if robot is stationary - simplified
    void detectStationary();
    
    // Add method to check IMU connections
    void checkIMUConnection();

public:
    /**
     * @brief Constructs a DualIMU object.
     * @param port1 Port number for the first IMU.
     * @param port2 Port number for the second IMU.
     * @param driftThreshold (Optional) A threshold value (in degrees) for outlier handling.
     */
    DualIMU(int port1, int port2, double driftThreshold = 2.0);

    /**
     * @brief Updates each filter with readings from both IMUs.
     */
    void update();

    // Accessor methods
    double get_rotation();
    double get_rotation_velocity();
    double get_heading();
    double get_yaw();
    double get_pitch();
    double get_roll();

    void reset(bool blocking=true);
    bool is_calibrating();
    pros::ImuStatus get_status();
    
    // Set a custom non-linear motion model
    void set_motion_model(std::function<double(double, double, double)> model);
    
    // Anti-drift compensation methods
    void enableDriftCompensation(bool enable);
    void setStationaryVelocityThreshold(double threshold);
    bool getIsStationary() const;
    
    // Debug methods
    double getVelocity() const; // For debugging
    double getAcceleration() const; // For debugging
};

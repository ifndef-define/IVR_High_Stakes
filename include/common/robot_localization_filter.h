#pragma once
#include <array>
#include <functional>
#include <cmath>
#include "main.h"
#include "common/kalman_filter.h"  // Include the existing KalmanFilter class

/**
 * @class RobotLocalizationFilter
 * @brief Specialized Kalman filter for robot localization with state [x, y, θ, vx, vy, ω, ax, ay, α]
 *        where ax, ay, α are accelerations in x, y, and angular directions
 */
class RobotLocalizationFilter {
private:
    // State vector: [x, y, θ, vx, vy, ω, ax, ay, α]
    // Expanded to include accelerations in all dimensions
    std::array<double, 9> state;
    
    // Covariance matrix (9x9 flattened to 1D array)
    std::array<double, 81> P;
    
    // Process noise parameters
    double Q_pos;      // position noise
    double Q_angle;    // angle noise
    double Q_vel;      // velocity noise
    double Q_omega;    // angular velocity noise
    double Q_accel;    // linear acceleration noise
    double Q_alpha;    // angular acceleration noise
    
    // Measurement noise parameters
    double R_pos;      // position measurement noise
    double R_angle;    // angle measurement noise
    double R_vel;      // velocity measurement noise
    double R_accel;    // acceleration measurement noise
    
    // Adaptive filtering
    double adaptiveFactor;
    
    // Angle format (true = radians, false = degrees)
    bool useRadians;
    
    // Runtime safety parameters
    double maxDt;      // Maximum allowed time step
    int maxIterations; // Maximum iterations for normalization loops
    
    // Pre-allocated memory for update calculations to avoid reallocations
    std::array<double, 9> K_x;       // Kalman gain for x
    std::array<double, 9> K_y;       // Kalman gain for y
    std::array<double, 9> K_theta;   // Kalman gain for theta
    std::array<double, 9> K_accel_x; // Kalman gain for x-acceleration
    std::array<double, 9> K_accel_y; // Kalman gain for y-acceleration
    std::array<double, 9> K_accel_z; // Kalman gain for z-acceleration (for IMU readings)
    
    // Individual Kalman filters for specific measurements (optional)
    KalmanFilter accelXFilter;
    KalmanFilter accelYFilter;
    KalmanFilter accelZFilter;
    
    // Helper functions
    void initializeCovariance();
    
    // Non-blocking angle normalization helpers
    double normalizeAngle(double angle) const;
    double angleDifference(double a, double b) const;
    
    // Angle conversion helpers
    double toInternalAngle(double angle) const;
    double fromInternalAngle(double angle) const;
    
    // Timing variables
    double lastPredictTime;    // Last time predict was called
    double lastPositionTime;   // Last time position was updated
    double lastHeadingTime;    // Last time heading was updated
    double lastAccelTime;      // Last time acceleration was updated
    double lastGyroTime;       // Last time gyro was updated
    
public:
    /**
     * @brief Constructs a RobotLocalizationFilter with initial state
     * @param init_x Initial x position
     * @param init_y Initial y position
     * @param init_theta Initial heading (θ)
     * @param init_vx Initial velocity in x
     * @param init_vy Initial velocity in y
     * @param init_omega Initial angular velocity
     * @param init_ax Initial acceleration in x
     * @param init_ay Initial acceleration in y
     * @param init_alpha Initial angular acceleration
     * @param Q_pos Position process noise
     * @param Q_angle Angle process noise
     * @param Q_vel Velocity process noise
     * @param Q_omega Angular velocity noise
     * @param Q_accel Acceleration process noise
     * @param Q_alpha Angular acceleration noise
     * @param R_pos Position measurement noise
     * @param R_angle Angle measurement noise
     * @param R_vel Velocity measurement noise
     * @param R_accel Acceleration measurement noise
     * @param useRadians Whether to use radians (true) or degrees (false)
     */
    RobotLocalizationFilter(
        double init_x = 0.0, double init_y = 0.0, double init_theta = 0.0,
        double init_vx = 0.0, double init_vy = 0.0, double init_omega = 0.0,
        double init_ax = 0.0, double init_ay = 0.0, double init_alpha = 0.0,
        double Q_pos = 0.01, double Q_angle = 0.01, 
        double Q_vel = 0.1, double Q_omega = 0.1,
        double Q_accel = 0.2, double Q_alpha = 0.2,
        double R_pos = 0.1, double R_angle = 0.05,
        double R_vel = 0.2, double R_accel = 0.5,
        bool useRadians = false);
    
    /**
     * @brief Predicts the system state forward in time
     * @param dt Time step in seconds
     */
    void predict(double dt);
    
    /**
     * @brief Updates the filter with a position measurement
     * @param x Measured x position
     * @param y Measured y position
     */
    void updatePosition(double x, double y);
    
    /**
     * @brief Updates the filter with a heading measurement
     * @param theta Measured heading
     */
    void updateHeading(double theta);
    
    /**
     * @brief Updates the filter with IMU acceleration readings
     * @param accel_x X-axis acceleration (m/s²)
     * @param accel_y Y-axis acceleration (m/s²)
     * @param accel_z Z-axis acceleration (m/s²)
     */
    void updateAcceleration(double accel_x, double accel_y, double accel_z);
    
    /**
     * @brief Updates the filter with IMU gyro readings (angular velocity)
     * @param omega Angular velocity (rad/s or deg/s depending on useRadians)
     */
    void updateAngularVelocity(double omega);
    
    /**
     * @brief Updates the filter with a full pose measurement
     * @param x Measured x position
     * @param y Measured y position
     * @param theta Measured heading
     */
    void updatePose(double x, double y, double theta);
    
    /**
     * @brief Resets the filter state
     * @param x New x position
     * @param y New y position
     * @param theta New heading
     * @param vx New x velocity
     * @param vy New y velocity
     * @param omega New angular velocity
     * @param ax New x acceleration
     * @param ay New y acceleration
     * @param alpha New angular acceleration
     */
    void reset(double x = 0.0, double y = 0.0, double theta = 0.0,
               double vx = 0.0, double vy = 0.0, double omega = 0.0,
               double ax = 0.0, double ay = 0.0, double alpha = 0.0);
    
    // Getters
    double getX() const { return state[0]; }
    double getY() const { return state[1]; }
    double getTheta() const { return state[2]; }
    double getVx() const { return state[3]; }
    double getVy() const { return state[4]; }
    double getOmega() const { return state[5]; }
    
    /**
     * @brief Gets the current heading in radians ([-π, π])
     * @return Theta in radians
     */
    double getThetaRadians() const;
    
    /**
     * @brief Gets the current heading in degrees ([0, 360))
     * @return Theta in degrees
     */
    double getThetaDegrees() const;
    
    /**
     * @brief Sets the maximum allowed time step to prevent instability
     * @param max_dt Maximum time step in seconds
     */
    void setMaxDt(double max_dt) { maxDt = max_dt; }
    
    /**
     * @brief Sets the maximum iterations for normalization loops
     * @param max_iter Maximum number of iterations
     */
    void setMaxIterations(int max_iter) { maxIterations = max_iter; }
    
    // Extended getters for acceleration
    double getAx() const { return state[6]; }
    double getAy() const { return state[7]; }
    double getAlpha() const { return state[8]; }

    void setAdaptiveNoiseFactor(double factor);
};

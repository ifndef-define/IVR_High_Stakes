#pragma once
#include "main.h"
#include "common/kalman_filter.h"

/**
 * @class Encoder
 * @brief Handles a single encoder for one axis (X or Y) with filtering
 */
class Encoder {
private:
    pros::adi::Encoder* encoder;
    double lastUpdateTime; // For tracking dt between updates
    
    // Anti-drift parameters
    double stationaryThreshold = 0.5;  // Threshold for detecting stationary state
    bool isStationary = false; // Flag to track if encoder is stationary
    int stationaryCount = 0;   // Counter for consecutive stationary readings
    int stationaryCountThreshold = 10; // Readings needed to confirm stationary
    double lastStablePosition = 0.0; // Last known stable position
    
    // Filter for position, velocity, acceleration
    KalmanFilter positionFilter;
    
    // Variables for calculations
    double currentTime;
    double dt;
    double ticksPerUnit; // Conversion factor from ticks to desired units
    double wheelDiameter; // For calculating distances
    
    // Variables to store readings
    double reading;
    double posVelocityBefore, posVelocityAfter, posAccel;
    
    // Helper methods
    void detectStationary();
    inline void resetFilterStable(double stablePos, double P_pos = 0.001);

public:
    /**
     * @brief Constructs an Encoder object for a single axis.
     * @param enc Pointer to the encoder.
     * @param wheelDiam Diameter of the wheel (for tracking distance).
     * @param ticksPerRev Encoder ticks per revolution.
     */
    Encoder(pros::adi::Encoder* enc, double wheelDiam = 1.36, double ticksPerRev = 8190.0);

    /**
     * @brief Updates the filter with readings from the encoder.
     */
    void update();

    // Accessor methods
    double getPosition() const;
    double getVelocity() const;
    double getAcceleration() const;
    double getDistance() const; // Position converted to distance

    /**
     * @brief Resets the encoder and filter.
     * @param blocking Whether to wait for reset completion.
     */
    void reset(bool blocking = true);
    
    /**
     * @brief Checks if the encoder is still changing values.
     * @return True if the encoder is stationary.
     */
    bool getIsStationary() const;
    
    /**
     * @brief Sets the threshold for detecting stationary state.
     * @param threshold Velocity below this value is considered stationary.
     */
    void setStationaryThreshold(double threshold);
    
    /**
     * @brief Sets the wheel diameter for distance calculations.
     * @param diameter Wheel diameter in desired units.
     */
    void setWheelDiameter(double diameter);
    
    /**
     * @brief Sets the ticks per revolution for the encoder.
     * @param ticks Number of ticks per full revolution.
     */
    void setTicksPerRevolution(double ticks);
};
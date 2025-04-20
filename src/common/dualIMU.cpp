#include "common/dualIMU.h"

// Static variables for temporary storage that don't need to persist between calls
static double current;
static double avgRot, avgYaw, avgPitch, avgRoll;

// Static variables for IMU update function
static double reducedDt;
static double originalR;
static double currentVel;
static double rawVel1, rawVel2;
static bool velocityLow;
static double head1, head2; // For heading values

// Additional static variables for position checks and filter operations
static double currentPosition;  // Used in multiple places to check position
static double positionDiff;     // Used for position difference calculations
static int i;                   // Loop counter
static double testRot, testGyro; // For IMU validation

// Last valid readings now static to persist across function calls
static double lastValidR1 = 0.0;
static double lastValidR2 = 0.0;
static int longTermStationaryCounter = 0;

// Add these static variables for connection tracking
static unsigned long imu1DisconnectTime = 0;
static unsigned long imu2DisconnectTime = 0;
static bool imu1WasDisconnected = false;
static bool imu2WasDisconnected = false;
static const unsigned long RECONNECT_DELAY_MS = 500; // Wait 500ms before attempting reconnect

DualIMU::DualIMU(int port1, int port2, double driftThresh)
  : imu1(port1),
    imu2(port2),
    driftThreshold(driftThresh),
    lastUpdateTime(pros::millis() / 1000.0),
    // Initialize enhanced filters with physics-based model (position, velocity, acceleration, P_pos, P_vel, P_acc, Q_pos, Q_vel, Q_acc, R)
    rotFilter(imu1.get_rotation(), 0.0, 0.0, 1.0, 0.1, 0.05, 0.01, 0.005, 0.001, 0.5),
    headingFilter(imu1.get_heading(), 0.0, 0.0, 1.0, 0.1, 0.05, 0.01, 0.005, 0.001, 0.5),
    yawFilter(imu1.get_yaw(), 0.0, 0.0, 1.0, 0.1, 0.05, 0.01, 0.005, 0.001, 0.5),
    pitchFilter(imu1.get_pitch(), 0.0, 0.0, 1.0, 0.1, 0.05, 0.01, 0.005, 0.001, 0.5),
    rollFilter(imu1.get_roll(), 0.0, 0.0, 1.0, 0.1, 0.05, 0.01, 0.005, 0.001, 0.5),
    // Initialize member variables 
    stationaryVelocityThreshold(1.0), isStationary(false), stationaryCount(0), 
    stationaryCountThreshold(5), driftCompensationEnabled(true),
    currentTime(0), dt(0), 
    rotVelocityBefore(0), rotVelocityAfter(0), rotAccel(0),
    r1(0), y1(0), p1(0), rl1(0),
    r2(0), y2(0), p2(0), rl2(0),
    positionStabilityThreshold(0.01), positionStableCount(0), positionStableThreshold(5), lastRotationReading(0) {
    
    // Store IMU readings in variables first
    r2 = imu2.get_rotation();
    y2 = imu2.get_yaw();
    p2 = imu2.get_pitch();
    rl2 = imu2.get_roll();
    
    // Initial update with second sensor reading
    rotFilter.update(r2);
    yawFilter.update(y2);
    pitchFilter.update(p2);
    rollFilter.update(rl2);

    // Initialize static variables
    lastValidR1 = imu1.get_rotation();
    lastValidR2 = imu2.get_rotation();
    longTermStationaryCounter = 0;
}

// Helper method for regular filters - modified to handle disconnected IMUs
inline void DualIMU::applyFilter(KalmanFilter& filter, double val1, double val2) {
    // If one IMU is disconnected, use only the connected one
    if (imu1Disconnected) {
        filter.update(val2);
        return;
    } else if (imu2Disconnected) {
        filter.update(val1);
        return;
    }
    
    // Both IMUs are connected - check for outliers
    if (std::fabs(val1 - val2) > driftThreshold) {
        // If readings differ too much, use the one closer to current estimate
        current = filter.get_position();
        if(std::fabs(val1 - current) < std::fabs(val2 - current)){
            filter.update(val1);
        } else {
            filter.update(val2);
        }
    } else {
        // No outlier detected, use both readings for sensor fusion
        filter.update(val1);
        filter.update(val2);
    }
}

// Improved IMU connection check with reconnection support
void DualIMU::checkIMUConnection() {
    // Store previous state to detect reconnection
    imu1WasDisconnected = imu1Disconnected;
    imu2WasDisconnected = imu2Disconnected;
    
    // Check IMU status first
    pros::ImuStatus status1 = imu1.get_status();
    pros::ImuStatus status2 = imu2.get_status();
    
    // Initial check based on IMU status
    bool imu1CurrentState = (status1 == pros::ImuStatus::error);
    bool imu2CurrentState = (status2 == pros::ImuStatus::error);
    
    // Store disconnect time when first disconnected
    unsigned long currentTimeMs = pros::millis();
    if (imu1CurrentState && !imu1Disconnected) {
        imu1DisconnectTime = currentTimeMs;
    }
    if (imu2CurrentState && !imu2Disconnected) {
        imu2DisconnectTime = currentTimeMs;
    }
    
    // Update disconnect flags
    imu1Disconnected = imu1CurrentState;
    imu2Disconnected = imu2CurrentState;
    
    // If an IMU was disconnected but now seems available, verify with readings
    if (imu1WasDisconnected && !imu1Disconnected) {
        // Don't reconnect too quickly (wait RECONNECT_DELAY_MS)
        if (currentTimeMs - imu1DisconnectTime < RECONNECT_DELAY_MS) {
            imu1Disconnected = true; // Still consider disconnected during wait period
            return;
        }
        
        try {
            // Test if IMU is really available
            testRot = imu1.get_rotation();
            testGyro = imu1.get_gyro_rate().z;
            
            if (std::isnan(testRot) || std::isinf(testRot) || 
                std::isnan(testGyro) || std::isinf(testGyro) ||
                std::abs(testRot) > 10000.0) {
                imu1Disconnected = true;
            } else {
                // Successfully reconnected - initialize IMU with current filter value
                imu1Disconnected = false;
                initializeReconnectedIMU(1);
            }
        } catch (...) {
            imu1Disconnected = true;
        }
    }
    
    // Same verification for IMU2
    if (imu2WasDisconnected && !imu2Disconnected) {
        // Don't reconnect too quickly
        if (currentTimeMs - imu2DisconnectTime < RECONNECT_DELAY_MS) {
            imu2Disconnected = true;
            return;
        }
        
        try {
            testRot = imu2.get_rotation();
            testGyro = imu2.get_gyro_rate().z;
            
            if (std::isnan(testRot) || std::isinf(testRot) || 
                std::isnan(testGyro) || std::isinf(testGyro) ||
                std::abs(testRot) > 10000.0) {
                imu2Disconnected = true;
            } else {
                // Successfully reconnected - initialize IMU with current filter value
                imu2Disconnected = false;
                initializeReconnectedIMU(2);
            }
        } catch (...) {
            imu2Disconnected = true;
        }
    }
    
    // Secondary check for IMUs that are connected
    if (!imu1Disconnected) {
        try {
            testRot = imu1.get_rotation();
            testGyro = imu1.get_gyro_rate().z;
            
            if (std::isnan(testRot) || std::isinf(testRot) || 
                std::isnan(testGyro) || std::isinf(testGyro) ||
                std::abs(testRot) > 10000.0) {
                imu1Disconnected = true;
            }
        } catch (...) {
            imu1Disconnected = true;
        }
    }
    
    if (!imu2Disconnected) {
        try {
            testRot = imu2.get_rotation();
            testGyro = imu2.get_gyro_rate().z;
            
            if (std::isnan(testRot) || std::isinf(testRot) || 
                std::isnan(testGyro) || std::isinf(testGyro) ||
                std::abs(testRot) > 10000.0) {
                imu2Disconnected = true;
            }
        } catch (...) {
            imu2Disconnected = true;
        }
    }
}

// New method to initialize a reconnected IMU with current filter values
void DualIMU::initializeReconnectedIMU(int imuNumber) {
    // Get the current filter estimates
    double currentRotation = rotFilter.get_position();
    double currentYaw = yawFilter.get_position();
    double currentPitch = pitchFilter.get_position();
    double currentRoll = rollFilter.get_position();
    
    if (imuNumber == 1) {
        // Store the current filter estimates as "last valid" values for IMU1
        lastValidR1 = currentRotation;
        r1 = currentRotation; // Use filter value until IMU reading stabilizes
        y1 = currentYaw;
        p1 = currentPitch;
        rl1 = currentRoll;
        imu1.set_yaw(currentYaw);
        imu1.set_rotation(currentRotation);
        imu1.set_pitch(currentPitch);
        imu1.set_roll(currentRoll);
    } else {
        // Store the current filter estimates as "last valid" values for IMU2
        lastValidR2 = currentRotation;
        r2 = currentRotation; // Use filter value until IMU reading stabilizes
        y2 = currentYaw;
        p2 = currentPitch;
        rl2 = currentRoll;
        imu2.set_yaw(currentYaw);
        imu2.set_rotation(currentRotation);
        imu2.set_pitch(currentPitch);
        imu2.set_roll(currentRoll);
    }
}

// Helper function to validate and clamp readings
double DualIMU::validateReading(double value, double lastValidValue, double maxDelta) {
    // Check if value is valid
    if (std::isnan(value) || std::isinf(value) || std::abs(value) > 10000.0) {
        return lastValidValue; // Return last valid value if this one is invalid
    }
    
    // Clamp changes to prevent sudden jumps
    if (std::abs(value - lastValidValue) > maxDelta) {
        if (value > lastValidValue) {
            return lastValidValue + maxDelta;
        } else {
            return lastValidValue - maxDelta;
        }
    }
    
    return value;
}

// New helper method to check if position is stable
bool DualIMU::isPositionStable() {
    // Using member lastRotationReading instead of local variable
    currentPosition = rotFilter.get_position();
    positionDiff = std::fabs(currentPosition - lastRotationReading);
    
    // Update last reading for next time
    lastRotationReading = currentPosition;
    
    // If position is within threshold, increase counter
    if (positionDiff < positionStabilityThreshold) {
        positionStableCount++;
        return (positionStableCount >= positionStableThreshold);
    } else {
        positionStableCount = 0;
        return false;
    }
}

// Improved stationary detection with position stability
void DualIMU::detectStationary() {
    // Get raw gyro rates from connected IMUs only
    rawVel1 = imu1Disconnected ? 999.0 : std::fabs(imu1.get_gyro_rate().z);
    rawVel2 = imu2Disconnected ? 999.0 : std::fabs(imu2.get_gyro_rate().z);
    
    // Determine if velocity is low based on connected IMUs
    if (imu1Disconnected && imu2Disconnected) {
        velocityLow = std::fabs(rotFilter.get_velocity()) < stationaryVelocityThreshold;
    } else if (imu1Disconnected) {
        velocityLow = (rawVel2 < stationaryVelocityThreshold);
    } else if (imu2Disconnected) {
        velocityLow = (rawVel1 < stationaryVelocityThreshold);
    } else {
        velocityLow = (rawVel1 < stationaryVelocityThreshold && 
                      rawVel2 < stationaryVelocityThreshold);
    }
    
    // Check position stability in addition to velocity
    bool stable = velocityLow || isPositionStable();
    
    // Update stationary counters
    if (stable) {
        stationaryCount++;
        if (stationaryCount == stationaryCountThreshold && !isStationary) {
            isStationary = true;
            
            // Store current state as stable references
            lastStableRotation = rotFilter.get_position();
            lastStableHeading = headingFilter.get_position();
            lastStableYaw = yawFilter.get_position();
            lastStablePitch = pitchFilter.get_position();
            lastStableRoll = rollFilter.get_position();
        } 
    } else {
        // Reset counter if there's significant movement
        stationaryCount = 0;
        isStationary = false;
    }
}

// Helper to reset filter with position and zero velocity while maintaining parameters
inline void DualIMU::resetFilterStable(KalmanFilter& filter, double stablePos, double P_pos) {
    filter.reset(stablePos, 0, 0, P_pos, 0.001, 0.001);
}

void DualIMU::update() {
    // Check IMU connection status before processing
    checkIMUConnection();
    
    // Output IMU status to LCD for debugging
    // if (imu1Disconnected || imu2Disconnected) {
    //     pros::lcd::print(5, "IMU Status: 1:%s 2:%s", 
    //                      imu1Disconnected ? "Disconnected" : "Connected",
    //                      imu2Disconnected ? "Disconnected" : "Connected");
    // }
    
    // Calculate dt since last update
    currentTime = pros::millis() / 1000.0;
    dt = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;
    
    // Ensure dt is valid and not too large
    if (dt <= 0.001 || dt > 0.5) {
        dt = 0.01; // Use reasonable default for invalid times
    }
    
    // If both IMUs are disconnected, freeze the filter state
    if (imu1Disconnected && imu2Disconnected) {
        return;
    }
    
    // Get current rotation velocity before any processing
    rotVelocityBefore = rotFilter.get_velocity();
    
    // Read IMU values with validation
    if (!imu1Disconnected) {
        try {
            r1 = validateReading(imu1.get_rotation(), lastValidR1, 10.0);
            y1 = validateReading(imu1.get_yaw(), yawFilter.get_position(), 10.0);
            p1 = validateReading(imu1.get_pitch(), pitchFilter.get_position(), 10.0);
            rl1 = validateReading(imu1.get_roll(), rollFilter.get_position(), 10.0);
            head1 = validateReading(imu1.get_heading(), headingFilter.get_position(), 10.0);
            lastValidR1 = r1;
        } catch (...) {
            imu1Disconnected = true;
            r1 = lastValidR1;
        }
    }
    
    if (!imu2Disconnected) {
        try {
            r2 = validateReading(imu2.get_rotation(), lastValidR2, 10.0);
            y2 = validateReading(imu2.get_yaw(), yawFilter.get_position(), 10.0);
            p2 = validateReading(imu2.get_pitch(), pitchFilter.get_position(), 10.0);
            rl2 = validateReading(imu2.get_roll(), rollFilter.get_position(), 10.0);
            head2 = validateReading(imu2.get_heading(), headingFilter.get_position(), 10.0);
            lastValidR2 = r2;
        } catch (...) {
            imu2Disconnected = true;
            r2 = lastValidR2;
        }
    }
    
    // Update long-term stationary counter
    if (isStationary) {
        longTermStationaryCounter++;
    } else {
        longTermStationaryCounter = 0;
    }
    
    // DIFFERENT BEHAVIOR BASED ON MOTION STATE
    if (isStationary && driftCompensationEnabled) {
        // WHEN STATIONARY - COMPLETELY PREVENT DRIFT
        
        // After being stationary for a while, lock position with minimal checks
        if (longTermStationaryCounter > 40) {
            // Check each filter once, and reset if it's drifted
            
            // Rotation check - reuse static variables
            currentPosition = rotFilter.get_position();
            positionDiff = std::fabs(currentPosition - lastStableRotation);
            
            if (positionDiff > 0.01) {
                resetFilterStable(rotFilter, lastStableRotation);
            }
            
            // Heading check
            currentPosition = headingFilter.get_position();
            positionDiff = std::fabs(currentPosition - lastStableHeading);
            if (positionDiff > 0.01) {
                resetFilterStable(headingFilter, lastStableHeading);
            }
            
            // Yaw check
            currentPosition = yawFilter.get_position();
            positionDiff = std::fabs(currentPosition - lastStableYaw);
            if (positionDiff > 0.01) {
                resetFilterStable(yawFilter, lastStableYaw);
            }
            
            // Pitch check
            currentPosition = pitchFilter.get_position();
            positionDiff = std::fabs(currentPosition - lastStablePitch);
            if (positionDiff > 0.01) {
                resetFilterStable(pitchFilter, lastStablePitch);
            }
            
            // Roll check
            currentPosition = rollFilter.get_position();
            positionDiff = std::fabs(currentPosition - lastStableRoll);
            if (positionDiff > 0.01) {
                resetFilterStable(rollFilter, lastStableRoll);
            }
            
            // Skip the rest of processing - position is locked
            detectStationary(); // Still update stationary detection
            return;
        }
        
        // Use high measurement noise to heavily discount new readings
        originalR = rotFilter.getR();
        double stationaryNoiseFactor = 50.0;
        
        // Set all filters to high noise at once
        rotFilter.setR(originalR * stationaryNoiseFactor);
        headingFilter.setR(originalR * stationaryNoiseFactor);
        yawFilter.setR(originalR * stationaryNoiseFactor);
        pitchFilter.setR(originalR * stationaryNoiseFactor);
        rollFilter.setR(originalR * stationaryNoiseFactor);
        
        // Apply stable values with shared loop counter
        for (i = 0; i < 5; i++) {
            rotFilter.update(lastStableRotation);
            headingFilter.update(lastStableHeading);
            yawFilter.update(lastStableYaw);  
            pitchFilter.update(lastStablePitch);
            rollFilter.update(lastStableRoll);
        }
        
        // Apply IMU readings with higher noise
        applyFilter(rotFilter, r1, r2);
        applyFilter(headingFilter, head1, head2);
        applyFilter(yawFilter, y1, y2);
        applyFilter(pitchFilter, p1, p2);
        applyFilter(rollFilter, rl1, rl2);
        
        // Restore original measurement noise for all filters at once
        rotFilter.setR(originalR);
        headingFilter.setR(originalR);
        yawFilter.setR(originalR);
        pitchFilter.setR(originalR);
        rollFilter.setR(originalR);
        
        // Reset filters to force zero velocity
        resetFilterStable(rotFilter, rotFilter.get_position(), rotFilter.get_P_pos());
        resetFilterStable(headingFilter, headingFilter.get_position(), headingFilter.get_P_pos());
        resetFilterStable(yawFilter, yawFilter.get_position(), yawFilter.get_P_pos());
        resetFilterStable(pitchFilter, pitchFilter.get_position(), pitchFilter.get_P_pos());
        resetFilterStable(rollFilter, rollFilter.get_position(), rollFilter.get_P_pos());
    } else {
        // WHEN MOVING - NORMAL FILTER OPERATION
        longTermStationaryCounter = 0;
        
        // Standard prediction for all filters
        rotFilter.predict(dt);
        headingFilter.predict(dt);
        yawFilter.predict(dt);
        pitchFilter.predict(dt);
        rollFilter.predict(dt);
        
        // Apply normal filtering
        applyFilter(rotFilter, r1, r2);
        applyFilter(headingFilter, head1, head2);
        applyFilter(yawFilter, y1, y2);
        applyFilter(pitchFilter, p1, p2);
        applyFilter(rollFilter, rl1, rl2);
    }
    
    // Calculate current acceleration
    rotVelocityAfter = rotFilter.get_velocity();
    rotAccel = (rotVelocityAfter - rotVelocityBefore) / dt;
    
    // Clamp acceleration to reasonable values
    if (std::abs(rotAccel) > 100.0) {
        rotAccel = (rotAccel > 0) ? 100.0 : -100.0;
    }
    
    // Dynamically adjust process noise based on motion
    if (isStationary) {
        rotFilter.setAdaptiveNoiseParams(0.01, 0.005, 0.001);
    }
    else if (std::fabs(rotAccel) > 30.0) {
        rotFilter.setAdaptiveNoiseParams(0.2, 0.04, 0.002);
    } else {
        rotFilter.setAdaptiveNoiseParams(0.1, 0.01, 0.001);
    }
    
    // Update stationary detection after processing
    detectStationary();
}

// Getter methods
double DualIMU::get_rotation() { 
    return rotFilter.get_position(); 
}

double DualIMU::get_rotation_velocity() {
    return rotFilter.get_velocity();
}

double DualIMU::get_heading() { return headingFilter.get_position(); }
double DualIMU::get_yaw() { return yawFilter.get_position(); }
double DualIMU::get_pitch() { return pitchFilter.get_position(); }
double DualIMU::get_roll() { return rollFilter.get_position(); }

// Anti-drift methods
void DualIMU::enableDriftCompensation(bool enable) {
    driftCompensationEnabled = enable;
    if (!enable) {
        isStationary = false;
        stationaryCount = 0;
    }
}

void DualIMU::setStationaryVelocityThreshold(double threshold) {
    stationaryVelocityThreshold = threshold;
}

void DualIMU::setPositionStabilityThreshold(double threshold) {
    positionStabilityThreshold = threshold;
}

bool DualIMU::getIsStationary() const {
    return isStationary;
}

double DualIMU::getVelocity() const {
    return rotFilter.get_velocity();
}

double DualIMU::getAcceleration() const {
    return rotFilter.get_acceleration();
}

// Other methods remain the same
void DualIMU::reset(bool blocking) {
    imu1.reset(blocking);
    imu2.reset(blocking);
    
    // Wait a brief moment for the IMUs to reset
    pros::delay(10);
    
    // Get average readings to reset filters
    r1 = imu1.get_rotation();
    y1 = imu1.get_yaw();
    p1 = imu1.get_pitch();
    rl1 = imu1.get_roll();
    
    r2 = imu2.get_rotation();
    y2 = imu2.get_yaw();
    p2 = imu2.get_pitch();
    rl2 = imu2.get_roll();
    
    // Use average of both IMUs for initialization
    avgRot = (r1 + r2) / 2.0;
    avgYaw = (y1 + y2) / 2.0;
    avgPitch = (p1 + p2) / 2.0;
    avgRoll = (rl1 + rl2) / 2.0;
    
    // Reset filters with actual values instead of zeros
    rotFilter.reset(avgRot, 0, 0, 1.0, 0.1, 0.05);
    headingFilter.reset(avgYaw, 0, 0, 1.0, 0.1, 0.05);
    yawFilter.reset(avgYaw, 0, 0, 1.0, 0.1, 0.05);
    pitchFilter.reset(avgPitch, 0, 0, 1.0, 0.1, 0.05);
    rollFilter.reset(avgRoll, 0, 0, 1.0, 0.1, 0.05);
    
    // Reset disconnection flags
    imu1Disconnected = false;
    imu2Disconnected = false;
    
    // Reset the update time
    lastUpdateTime = pros::millis() / 1000.0;
}

pros::ImuStatus DualIMU::get_status() {
    if(imu1Disconnected && imu2Disconnected) {
        return pros::ImuStatus::error;
    } else if(imu1Disconnected) {
        return imu2.get_status();
    } else {
        return imu1.get_status();
    }
}

bool DualIMU::is_calibrating() {
    if(imu1Disconnected && imu2Disconnected) {
        return false; // Both IMUs are disconnected
    } else if(imu1Disconnected) {
        return imu2.is_calibrating();
    } else if(imu2Disconnected) {
        return imu1.is_calibrating();
    } else {
        return imu1.is_calibrating() || imu2.is_calibrating();
    }
}
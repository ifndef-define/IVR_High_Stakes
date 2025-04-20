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
    r2(0), y2(0), p2(0), rl2(0) {
    
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
            // Don't mark as disconnected here - this is just an outlier
        } else {
            filter.update(val2);
            // Don't mark as disconnected here - this is just an outlier
        }
    } else {
        // No outlier detected, use both readings for sensor fusion
        filter.update(val1);
        filter.update(val2);
    }
}

// Check if IMUs are working properly
void DualIMU::checkIMUConnection() {
    // An IMU is considered disconnected if:
    // 1. It returns error status
    // 2. Consecutive failed readings
    
    pros::ImuStatus status1 = imu1.get_status();
    pros::ImuStatus status2 = imu2.get_status();
    
    // Mark IMUs as disconnected based on their status
    imu1Disconnected = (status1 == pros::ImuStatus::error);
    imu2Disconnected = (status2 == pros::ImuStatus::error);
}

// Improved stationary detection
void DualIMU::detectStationary() {
    // Get raw gyro rates from connected IMUs only
    rawVel1 = imu1Disconnected ? 999.0 : std::fabs(imu1.get_gyro_rate().z);
    rawVel2 = imu2Disconnected ? 999.0 : std::fabs(imu2.get_gyro_rate().z);
    
    // Determine if velocity is low based on connected IMUs
    if (imu1Disconnected && imu2Disconnected) {
        // If both are disconnected, use filter velocity
        velocityLow = std::fabs(rotFilter.get_velocity()) < stationaryVelocityThreshold;
    } else if (imu1Disconnected) {
        // Only IMU2 is connected
        velocityLow = (rawVel2 < stationaryVelocityThreshold);
    } else if (imu2Disconnected) {
        // Only IMU1 is connected
        velocityLow = (rawVel1 < stationaryVelocityThreshold);
    } else {
        // Both IMUs are connected
        velocityLow = (rawVel1 < stationaryVelocityThreshold && 
                       rawVel2 < stationaryVelocityThreshold);
    }
    
    if (velocityLow) {
        stationaryCount++;
        if (stationaryCount >= stationaryCountThreshold && !isStationary) {
            // Robot just became stationary - save current values
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

void DualIMU::update() {
    // Check IMU connection status before processing
    checkIMUConnection();
    
    // Calculate dt since last update
    currentTime = pros::millis() / 1000.0;
    dt = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;
    
    // Ensure we have a valid dt (avoid division by zero or negative time)
    if (dt <= 0.001) {
        dt = 0.01; // Use a small default dt if time calculation is invalid
    }
    
    // Get current rotation velocity before prediction
    rotVelocityBefore = rotFilter.get_velocity();
    
    // Read IMU values from connected devices only
    if (!imu1Disconnected) {
        r1 = imu1.get_rotation();
        y1 = imu1.get_yaw();
        p1 = imu1.get_pitch();
        rl1 = imu1.get_roll();
        head1 = imu1.get_heading();
    }
    
    if (!imu2Disconnected) {
        r2 = imu2.get_rotation();
        y2 = imu2.get_yaw();
        p2 = imu2.get_pitch();
        rl2 = imu2.get_roll();
        head2 = imu2.get_heading();
    }
    
    // If both IMUs are disconnected, we can't update
    if (imu1Disconnected && imu2Disconnected) {
        // Just return without updating (maintain last known state)
        return;
    }
    
    // DIFFERENT BEHAVIOR BASED ON MOTION STATE
    if (isStationary && driftCompensationEnabled) {
        // WHEN STATIONARY - MODIFIED APPROACH:
        // 1. Still predict but with very small timestep to minimize drift
        // 2. Apply filter with heavily weighted stable values
        
        // Predict with small timestep (1/10th of actual) to minimize drift
        reducedDt = dt * 0.1;
        
        // Apply limited prediction step
        rotFilter.predict(reducedDt);
        headingFilter.predict(reducedDt);
        yawFilter.predict(reducedDt);
        pitchFilter.predict(reducedDt);
        rollFilter.predict(reducedDt);
        
        // Use high measurement noise to heavily discount new readings
        originalR = rotFilter.getR();
        rotFilter.setR(originalR * 10.0);
        headingFilter.setR(originalR * 10.0);
        yawFilter.setR(originalR * 10.0);
        pitchFilter.setR(originalR * 10.0);
        rollFilter.setR(originalR * 10.0);
        
        // Apply filter with a mix of real readings and stable values
        // This allows small updates while keeping close to stable values
        applyFilter(rotFilter, r1, r2);          // Real IMU readings
        rotFilter.update(lastStableRotation);    // Stable value with high weight
        rotFilter.update(lastStableRotation);    // Added twice for more weight
        
        applyFilter(headingFilter, head1, head2);
        headingFilter.update(lastStableHeading);
        headingFilter.update(lastStableHeading);
        
        applyFilter(yawFilter, y1, y2);
        yawFilter.update(lastStableYaw);
        yawFilter.update(lastStableYaw);
        
        applyFilter(pitchFilter, p1, p2);
        pitchFilter.update(lastStablePitch);
        pitchFilter.update(lastStablePitch);
        
        applyFilter(rollFilter, rl1, rl2);
        rollFilter.update(lastStableRoll);
        rollFilter.update(lastStableRoll);
        
        // Restore original measurement noise
        rotFilter.setR(originalR);
        headingFilter.setR(originalR);
        yawFilter.setR(originalR);
        pitchFilter.setR(originalR);
        rollFilter.setR(originalR);
        
        // Force velocity close to zero but allow small corrections
        currentVel = rotFilter.get_velocity();
        if (std::fabs(currentVel) > 0.1) {
            rotFilter.reset(rotFilter.get_position(), currentVel * 0.5, 0, 
                          rotFilter.get_P_pos(), rotFilter.get_P_vel(), rotFilter.get_P_acc());
        }
    } else {
        // WHEN MOVING - NORMAL FILTER OPERATION
        
        // Predict rotation state using motion model and dt
        rotFilter.predict(dt);
        headingFilter.predict(dt);
        yawFilter.predict(dt);
        pitchFilter.predict(dt);
        rollFilter.predict(dt);
        
        // Normal filtering for all measurements when moving
        applyFilter(rotFilter, r1, r2);
        applyFilter(headingFilter, head1, head2);
        applyFilter(yawFilter, y1, y2);
        applyFilter(pitchFilter, p1, p2);
        applyFilter(rollFilter, rl1, rl2);
    }
    
    // Calculate current acceleration
    rotVelocityAfter = rotFilter.get_velocity();
    rotAccel = (rotVelocityAfter - rotVelocityBefore) / dt;
    
    // Dynamically adjust process noise based on motion
    if (isStationary) {
        // Very low process noise when stationary
        rotFilter.setAdaptiveNoiseParams(0.01, 0.005, 0.001);
    }
    else if (std::fabs(rotAccel) > 30.0) {
        // Higher process noise during high acceleration
        rotFilter.setAdaptiveNoiseParams(0.2, 0.04, 0.002);
    } else {
        // Normal process noise
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
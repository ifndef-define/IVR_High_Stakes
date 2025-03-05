#include "common/dual_imu.h"

DualIMU::DualIMU(int port1, int port2, double driftThresh)
  : imu1(port1),
    imu2(port2),
    driftThreshold(driftThresh),
    lastUpdateTime(pros::millis() / 1000.0),
    r1(0), r2(0), y1(0), y2(0), p1(0), p2(0), rl1(0), rl2(0), h1(0), h2(0),
    dt(0), rotVelocityBefore(0), rotVelocityAfter(0), rotAccel(0),
    // Initialize filters
    rotFilter(imu1.get_rotation(), 0.0, 0.0, 1.0),
    headingFilter(imu1.get_heading(), 0.0, 0.0, 1.0),
    yawFilter(imu1.get_yaw(), 0.0, 0.0, 1.0),
    pitchFilter(imu1.get_pitch(), 0.0, 0.0, 1.0),
    rollFilter(imu1.get_roll(), 0.0, 0.0, 1.0) {
    
    // Initial update with second sensor reading
    rotFilter.update(imu2.get_rotation());
    yawFilter.update(imu2.get_yaw());
    pitchFilter.update(imu2.get_pitch());
    rollFilter.update(imu2.get_roll());
    headingFilter.update(imu2.get_heading());
}

// Helper method optimized to avoid creating local variables
inline void DualIMU::applyFilter(KalmanFilter& filter, const double& val1, const double& val2) {
    // Check for outliers and update the filter accordingly
    if (std::fabs(val1 - val2) > driftThreshold) {
        // Use filter's current position directly without creating a local variable
        filter.update(std::fabs(val1 - filter.get_position()) < std::fabs(val2 - filter.get_position()) ? val1 : val2);
    } else {
        // No outlier detected, use both readings for sensor fusion
        filter.update(val1);
        filter.update(val2);
    }
}

void DualIMU::update() {
    // Calculate dt since last update
    static double currentTime = pros::millis() / 1000.0;
    dt = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;
    
    // Store previous velocities for all filters
    rotVelocityBefore = rotFilter.get_velocity();
    headingVelocityBefore = headingFilter.get_velocity();
    yawVelocityBefore = yawFilter.get_velocity();
    pitchVelocityBefore = pitchFilter.get_velocity();
    rollVelocityBefore = rollFilter.get_velocity();
    
    // Predict all filter states using motion model and dt
    rotFilter.predict(dt);
    headingFilter.predict(dt);
    yawFilter.predict(dt);
    pitchFilter.predict(dt);
    rollFilter.predict(dt);
    
    // Read all IMU values directly into member variables
    r1 = imu1.get_rotation();
    y1 = imu1.get_yaw();
    p1 = imu1.get_pitch();
    rl1 = imu1.get_roll();
    h1 = imu1.get_heading();
    
    r2 = imu2.get_rotation();
    y2 = imu2.get_yaw();
    p2 = imu2.get_pitch();
    rl2 = imu2.get_roll();
    h2 = imu2.get_heading();
    
    // Regular filtering for all measurements
    applyFilter(rotFilter, r1, r2);
    applyFilter(headingFilter, h1, h2);
    applyFilter(yawFilter, y1, y2);
    applyFilter(pitchFilter, p1, p2);
    applyFilter(rollFilter, rl1, rl2);
    
    // Calculate current accelerations for all filters using member variables
    rotVelocityAfter = rotFilter.get_velocity();
    rotAccel = (rotVelocityAfter - rotVelocityBefore) / dt;
    
    headingVelocityAfter = headingFilter.get_velocity();
    headingAccel = (headingVelocityAfter - headingVelocityBefore) / dt;
    
    yawVelocityAfter = yawFilter.get_velocity();
    yawAccel = (yawVelocityAfter - yawVelocityBefore) / dt;
    
    pitchVelocityAfter = pitchFilter.get_velocity();
    pitchAccel = (pitchVelocityAfter - pitchVelocityBefore) / dt;
    
    rollVelocityAfter = rollFilter.get_velocity();
    rollAccel = (rollVelocityAfter - rollVelocityBefore) / dt;
    
    // Dynamically adjust process noise for all filters based on their respective accelerations
    if (std::fabs(rotAccel) > 20.0) {
        rotFilter.setAdaptiveNoiseParams(0.2, 0.04, 0.002);
    } else {
        rotFilter.setAdaptiveNoiseParams(0.1, 0.01, 0.001);
    }
    
    if (std::fabs(headingAccel) > 20.0) {
        headingFilter.setAdaptiveNoiseParams(0.2, 0.04, 0.002);
    } else {
        headingFilter.setAdaptiveNoiseParams(0.1, 0.01, 0.001);
    }
    
    if (std::fabs(yawAccel) > 20.0) {
        yawFilter.setAdaptiveNoiseParams(0.2, 0.04, 0.002);
    } else {
        yawFilter.setAdaptiveNoiseParams(0.1, 0.01, 0.001);
    }
    
    if (std::fabs(pitchAccel) > 20.0) {
        pitchFilter.setAdaptiveNoiseParams(0.2, 0.04, 0.002);
    } else {
        pitchFilter.setAdaptiveNoiseParams(0.1, 0.01, 0.001);
    }
    
    if (std::fabs(rollAccel) > 20.0) {
        rollFilter.setAdaptiveNoiseParams(0.2, 0.04, 0.002);
    } else {
        rollFilter.setAdaptiveNoiseParams(0.1, 0.01, 0.001);
    }
}

double DualIMU::get_rotation() const { 
    return rotFilter.get_position(); 
}

double DualIMU::get_rotation_velocity() const {
    return rotFilter.get_velocity();
}

double DualIMU::get_heading() const { return headingFilter.get_position(); }
double DualIMU::get_yaw() const { return yawFilter.get_position(); }
double DualIMU::get_pitch() const { return pitchFilter.get_position(); }
double DualIMU::get_roll() const { return rollFilter.get_position(); }

// Other methods remain the same with minor adjustments for the enhanced filter
void DualIMU::reset(bool blocking) {
    imu1.reset(blocking);
    imu2.reset(blocking);
    
    // Get readings directly into member variables
    r1 = imu1.get_rotation();
    r2 = imu2.get_rotation();
    y1 = imu1.get_yaw();
    y2 = imu2.get_yaw();
    p1 = imu1.get_pitch();
    p2 = imu2.get_pitch();
    rl1 = imu1.get_roll();
    rl2 = imu2.get_roll();
    h1 = imu1.get_heading();
    h2 = imu2.get_heading();
    
    rotFilter.reset((r1+r2)/2.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    headingFilter.reset((h1+h2)/2.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    yawFilter.reset((y1+y2)/2.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    pitchFilter.reset((p1+p2)/2.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    rollFilter.reset((rl1+rl2)/2.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

pros::ImuStatus DualIMU::get_status() const {
    return imu2.get_status();
}

bool DualIMU::is_calibrating() const {
    return imu1.is_calibrating() || imu2.is_calibrating();
}
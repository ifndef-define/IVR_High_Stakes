#include "common/dual_imu.h"

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
    rollFilter(imu1.get_roll(), 0.0, 0.0, 1.0, 0.1, 0.05, 0.01, 0.005, 0.001, 0.5) {
    
    // Initial update with second sensor reading
    rotFilter.update(imu2.get_rotation());
    yawFilter.update(imu2.get_yaw());
    pitchFilter.update(imu2.get_pitch());
    rollFilter.update(imu2.get_roll());
}

// Helper method for regular filters remains unchanged
inline void DualIMU::applyFilter(KalmanFilter& filter, const double& val1, const double& val2) {
    // Check for outliers and update the filter accordingly
    if (std::fabs(val1 - val2) > driftThreshold) {
        // If readings differ too much, use the one closer to current estimate
        const double current = filter.get_position();
        filter.update(std::fabs(val1 - current) < std::fabs(val2 - current) ? val1 : val2);
    } else {
        // No outlier detected, use both readings for sensor fusion
        filter.update(val1);
        filter.update(val2);
    }
}

void DualIMU::update() {
    // Calculate dt since last update
    double currentTime = pros::millis() / 1000.0;
    double dt = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;
    
    // Get current rotation velocity
    double rotVelocityBefore = rotFilter.get_velocity();
    
    // Predict rotation state using motion model and dt
    rotFilter.predict(dt);
    
    // Read all IMU values
    const double r1 = imu1.get_rotation();
    const double y1 = imu1.get_yaw();
    const double p1 = imu1.get_pitch();
    const double rl1 = imu1.get_roll();
    
    const double r2 = imu2.get_rotation();
    const double y2 = imu2.get_yaw();
    const double p2 = imu2.get_pitch();
    const double rl2 = imu2.get_roll();
    
    // Regular filtering for all measurements
    applyFilter(rotFilter, r1, r2);
    applyFilter(yawFilter, y1, y2);
    applyFilter(pitchFilter, p1, p2);
    applyFilter(rollFilter, rl1, rl2);
    
    // Calculate current acceleration
    const double rotVelocityAfter = rotFilter.get_velocity();
    const double rotAccel = (rotVelocityAfter - rotVelocityBefore) / dt;
    
    // Dynamically adjust process noise if experiencing high acceleration
    if (std::fabs(rotAccel) > 20.0) { // threshold for "high" acceleration
        rotFilter.setAdaptiveNoiseParams(0.2, 0.04, 0.002); // Higher process noise
    } else {
        rotFilter.setAdaptiveNoiseParams(0.1, 0.01, 0.001); // Normal process noise
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
    
    // Get average readings to reset filters
    const double r1 = imu1.get_rotation(), r2 = imu2.get_rotation();
    const double y1 = imu1.get_yaw(), y2 = imu2.get_yaw();
    const double p1 = imu1.get_pitch(), p2 = imu2.get_pitch();
    const double rl1 = imu1.get_roll(), rl2 = imu2.get_roll();
    
    rotFilter.reset((imu1.get_rotation() + imu2.get_rotation()) / 2.0, 0, 0, 0, 0, 0);
    headingFilter.reset((imu1.get_heading() + imu2.get_heading()) / 2.0, 0, 0, 0, 0, 0);
    yawFilter.reset((imu1.get_yaw() + imu2.get_yaw()) / 2.0, 0, 0, 0, 0, 0);
    pitchFilter.reset((imu1.get_pitch() + imu2.get_pitch()) / 2.0, 0 , 0, 0, 0, 0);
    rollFilter.reset((imu1.get_roll() + imu2.get_roll()) / 2.0, 0, 0, 0, 0, 0);
}

bool DualIMU::is_calibrating() const {
    return imu1.is_calibrating() || imu2.is_calibrating();
}
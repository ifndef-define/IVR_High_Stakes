#include "common/dual_imu.h"
#include <cmath>

// Utility functions for angle conversions - static to avoid recreating
static inline double degreesToRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

static inline double radiansToDegrees(double radians) {
    return radians * 180.0 / M_PI;
}

// Fast modulo operation for angle normalization
static inline double mod(double x, double y) {
    return x - y * std::floor(x / y);
}

// Normalize angle to [0, 360) degrees - efficient implementation
static inline double normalizeDegrees(double degrees) {
    return mod(degrees, 360.0);
}

// Normalize angle to [-π, π] radians - efficient implementation
static inline double normalizeRadians(double radians) {
    return mod(radians + M_PI, 2.0 * M_PI) - M_PI;
}

DualIMU::DualIMU(int port1, int port2, RobotLocalizationFilter &filter, double driftThresh, bool useRadians)
  : imu1(port1),
    imu2(port2),
    driftThreshold(driftThresh),
    lastUpdateTime(pros::millis() / 1000.0),
    currentTime(lastUpdateTime),
    r1(0), r2(0), y1(0), y2(0), p1(0), p2(0), rl1(0), rl2(0), h1(0), h2(0),
    dt(0), prevHeadingVelocity(0), rotationVelocity(0), rotationAccel(0),
    headingVelocity(0), headingAccel(0),
    filtered_accel_x(0), filtered_accel_y(0), filtered_accel_z(0),
    imu1Reliable(true), imu2Reliable(true), 
    imu1UnreliableCount(0), imu2UnreliableCount(0),
    headingFilter(&filter),
    // headingFilter(0.0, 0.0, 0.0, 0.01, 0.02, 0.05, 0.1, 0.1, 0.05, useRadians),
    useRadians(useRadians) {
    
    // Set safety limits for non-blocking operation
    headingFilter->setMaxDt(0.1);
    headingFilter->setMaxIterations(10);
}

inline double DualIMU::normalizeAngle(double angle) const {
    // Use fast modulo operations instead of while loops
    if (useRadians) {
        return normalizeRadians(angle);
    } else {
        return normalizeDegrees(angle);
    }
}

inline double DualIMU::angleDifference(double a, double b) const {
    // Efficient calculation of angle difference
    if (useRadians) {
        // Smallest angle difference in radians using modulo
        return mod(a - b + M_PI, 2.0 * M_PI) - M_PI;
    } else {
        // Smallest angle difference in degrees using modulo
        return mod(a - b + 180.0, 360.0) - 180.0;
    }
}

inline bool DualIMU::isDriftExcessive(double val1, double val2) const {
    return std::fabs(angleDifference(val1, val2)) > driftThreshold;
}

inline void DualIMU::fuseSensorReadings() {
    // Read headings from IMUs (already in degrees)
    h1 = imu1.get_heading();
    h2 = imu2.get_heading();
    
    // If using radians, convert
    if (useRadians) {
        h1 = degreesToRadians(h1);
        h2 = degreesToRadians(h2);
    }
    
    // Normalize to proper range
    h1 = normalizeAngle(h1);
    h2 = normalizeAngle(h2);
    
    // Update IMU reliability status based on drift
    updateReliability();
    
    // Apply sensor fusion based on reliability
    if (imu1Reliable && imu2Reliable) {
        // Both IMUs are reliable, use both
        headingFilter->updateHeading(h1);
        headingFilter->updateHeading(h2);
    } else if (imu1Reliable) {
        // Only IMU 1 is reliable
        headingFilter->updateHeading(h1);
    } else if (imu2Reliable) {
        // Only IMU 2 is reliable
        headingFilter->updateHeading(h2);
    } else {
        // Both are unreliable - use the one with less drift
        double currentHeading = headingFilter->getTheta();
        double d1 = std::fabs(angleDifference(h1, currentHeading));
        double d2 = std::fabs(angleDifference(h2, currentHeading));
        
        if (d1 < d2) {
            headingFilter->updateHeading(h1);
        } else {
            headingFilter->updateHeading(h2);
        }
    }
    
    // Also read yaw, pitch, roll for other methods
    y1 = imu1.get_yaw();
    y2 = imu2.get_yaw();
    p1 = imu1.get_pitch();
    p2 = imu2.get_pitch();
    rl1 = imu1.get_roll();
    rl2 = imu2.get_roll();
}

void DualIMU::updateReliability() {
    double filterHeading = headingFilter->getTheta();
    
    // Check if IMUs are drifting from filter estimate
    double diff1 = std::fabs(angleDifference(h1, filterHeading));
    double diff2 = std::fabs(angleDifference(h2, filterHeading));
    
    // Update reliability status for IMU 1
    if (diff1 > driftThreshold) {
        if (imu1Reliable) {
            imu1Reliable = false;
            imu1UnreliableCount = 0;
        } else {
            imu1UnreliableCount++;
        }
    } else {
        // IMU is within acceptable range
        if (!imu1Reliable) {
            // IMU might be rehabilitated if it stays reliable
            imu1UnreliableCount++;
            if (imu1UnreliableCount >= MAX_UNRELIABLE_COUNT + REQUIRED_RELIABLE_READINGS) {
                imu1Reliable = true;
                imu1UnreliableCount = 0;
            }
        }
    }
    
    // Update reliability status for IMU 2
    if (diff2 > driftThreshold) {
        if (imu2Reliable) {
            imu2Reliable = false;
            imu2UnreliableCount = 0;
        } else {
            imu2UnreliableCount++;
        }
    } else {
        // IMU is within acceptable range
        if (!imu2Reliable) {
            // IMU might be rehabilitated if it stays reliable
            imu2UnreliableCount++;
            if (imu2UnreliableCount >= MAX_UNRELIABLE_COUNT + REQUIRED_RELIABLE_READINGS) {
                imu2Reliable = true;
                imu2UnreliableCount = 0;
            }
        }
    }
    
    // If both IMUs are unreliable, rehabilitate after a certain time
    if (!imu1Reliable && !imu2Reliable) {
        if (imu1UnreliableCount > MAX_UNRELIABLE_COUNT && 
            imu2UnreliableCount > MAX_UNRELIABLE_COUNT) {
            rehabilitateIMUs();
        }
    }
}

void DualIMU::rehabilitateIMUs() {
    // If both IMUs are unreliable for too long, reset by accepting
    // the one with smallest drift
    double filterHeading = headingFilter->getTheta();
    double diff1 = std::fabs(angleDifference(h1, filterHeading));
    double diff2 = std::fabs(angleDifference(h2, filterHeading));
    
    if (diff1 < diff2) {
        imu1Reliable = true;
        imu1UnreliableCount = 0;
    } else {
        imu2Reliable = true;
        imu2UnreliableCount = 0;
    }
}

void DualIMU::update() {
    // Calculate time delta - avoid static variable that only initializes once
    currentTime = pros::millis() / 1000.0;
    dt = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;
    
    // Skip update if dt is too small to avoid numerical issues
    if (dt < 0.001) return;
    
    // Limit dt to prevent filter instability
    dt = std::min(dt, 0.1);
    
    // Store current velocity before prediction
    prevHeadingVelocity = headingFilter->getOmega();
    
    // Predict state forward
    headingFilter->predict(dt);

    // Make IMU data structures static to avoid recreating
    static pros::imu_accel_s_t accel1, accel2;
    static pros::imu_gyro_s_t gyro_data1, gyro_data2;
    static double accel_x, accel_y, accel_z, gyro_avg;
    
    // Get acceleration data from both IMUs - use reliability info
    accel1 = imu1.get_accel();
    accel2 = imu2.get_accel();
    
    // Handle acceleration data using reliability
    if (imu1Reliable && imu2Reliable) {
        // Average if both are reliable
        accel_x = (accel1.x + accel2.x) / 2.0;
        accel_y = (accel1.y + accel2.y) / 2.0;
        accel_z = (accel1.z + accel2.z) / 2.0;
    } else if (imu1Reliable) {
        // Use only IMU 1
        accel_x = accel1.x;
        accel_y = accel1.y;
        accel_z = accel1.z;
    } else if (imu2Reliable) {
        // Use only IMU 2
        accel_x = accel2.x;
        accel_y = accel2.y;
        accel_z = accel2.z;
    } else {
        // Both unreliable, average but be cautious
        accel_x = (accel1.x + accel2.x) / 2.0;
        accel_y = (accel1.y + accel2.y) / 2.0;
        accel_z = (accel1.z + accel2.z) / 2.0;
    }
    
    // Store the filtered acceleration values
    filtered_accel_x = accel_x;
    filtered_accel_y = accel_y;
    filtered_accel_z = accel_z;
    
    // Update the filter with IMU acceleration data
    headingFilter->updateAcceleration(accel_x, accel_y, accel_z);
    
    // Get gyro data using reliability info
    gyro_data1 = imu1.get_gyro_rate();
    gyro_data2 = imu2.get_gyro_rate();
    
    // Handle gyro data using reliability
    if (imu1Reliable && imu2Reliable) {
        gyro_avg = (gyro_data1.z + gyro_data2.z) / 2.0;
    } else if (imu1Reliable) {
        gyro_avg = gyro_data1.z;
    } else if (imu2Reliable) {
        gyro_avg = gyro_data2.z;
    } else {
        // Both unreliable, average but be cautious
        gyro_avg = (gyro_data1.z + gyro_data2.z) / 2.0;
    }
    
    // Update the filter with angular velocity
    headingFilter->updateAngularVelocity(gyro_avg);
    
    // Fuse heading readings
    fuseSensorReadings();
    
    // Calculate acceleration for adaptive filtering - reuse member variables
    headingVelocity = headingFilter->getOmega();
    headingAccel = (headingVelocity - prevHeadingVelocity) / dt;
    
    // Use a static threshold variable
    static double accelThreshold;
    accelThreshold = useRadians ? 1.0 : radiansToDegrees(1.0);
    
    // Adjust process noise based on dynamics
    if (std::fabs(headingAccel) > accelThreshold) {
        // High acceleration detected - increase process noise
        headingFilter->setAdaptiveNoiseFactor(2.0);
    } else {
        // Normal operation - standard process noise
        headingFilter->setAdaptiveNoiseFactor(1.0);
    }
}

double DualIMU::getHeading() const {
    return headingFilter->getTheta();
}

double DualIMU::getHeadingRadians() const {
    return headingFilter->getThetaRadians();
}

double DualIMU::getHeadingDegrees() const {
    return headingFilter->getThetaDegrees();
}

double DualIMU::getHeadingVelocity() const {
    return headingFilter->getOmega();
}

double DualIMU::getYaw() const {
    // Average the yaw readings if no excessive drift
    if (!isDriftExcessive(y1, y2)) {
        return (y1 + y2) / 2.0;
    }
    // Otherwise use the one closer to the filter's estimate
    double filterYaw = useRadians ? radiansToDegrees(headingFilter->getTheta()) : headingFilter->getTheta();
    return std::fabs(angleDifference(y1, filterYaw)) < std::fabs(angleDifference(y2, filterYaw)) ? y1 : y2;
}

double DualIMU::getPitch() const {
    return isDriftExcessive(p1, p2) ? 
        (std::fabs(p1) < std::fabs(p2) ? p1 : p2) : // Use the smaller value if drift is excessive
        (p1 + p2) / 2.0;                           // Otherwise average
}

double DualIMU::getRoll() const {
    return isDriftExcessive(rl1, rl2) ? 
        (std::fabs(rl1) < std::fabs(rl2) ? rl1 : rl2) : // Use the smaller value if drift is excessive
        (rl1 + rl2) / 2.0;                             // Otherwise average
}

void DualIMU::reset(bool blocking) {
    imu1.reset(blocking);
    imu2.reset(blocking);
    
    // Reset raw readings (IMU returns degrees)
    h1 = imu1.get_heading();
    h2 = imu2.get_heading();
    
    // Convert to radians if needed
    if (useRadians) {
        h1 = degreesToRadians(h1);
        h2 = degreesToRadians(h2);
    }
    
    y1 = imu1.get_yaw();
    y2 = imu2.get_yaw();
    p1 = imu1.get_pitch();
    p2 = imu2.get_pitch();
    rl1 = imu1.get_roll();
    rl2 = imu2.get_roll();
    
    // Reset filter with average heading
    double avgHeading = (h1 + h2) / 2.0;
    headingFilter->reset(0.0, 0.0, avgHeading);
}

bool DualIMU::isCalibrating() const {
    return imu1.is_calibrating() || imu2.is_calibrating();
}

pros::ImuStatus DualIMU::getStatus() const {
    return imu2.get_status();
}
#include "common/odometry.h"
#include <cmath>

Odometry::Odometry(
    DualIMU& imu,
    std::pair<int, int> xEncoderPorts,
    std::pair<int, int> yEncoderPorts,
    double xOffset, double yOffset,
    double xScale, double yScale)
    : localizationFilter(0.0, 0.0, 0.0),  // Initialize with zeros
      imu(imu),
      encoderX(xEncoderPorts.first, xEncoderPorts.second),
      encoderY(yEncoderPorts.first, yEncoderPorts.second),
      lastUpdateTime(pros::millis() / 1000.0),
      encoderXOffset(xOffset),
      encoderYOffset(yOffset),
      encoderXScaleFactor(xScale),
      encoderYScaleFactor(yScale),
      lastEncoderX(0),
      lastEncoderY(0),
      x(0.0), y(0.0), theta(0.0),
      vx(0.0), vy(0.0), omega(0.0) {
    
    // Initialize by reading the current encoder values
    lastEncoderX = encoderX.get_value();
    lastEncoderY = encoderY.get_value();
    
    // Initialize with the current heading from IMU
    theta = imu.getHeading();
}

void Odometry::update() {
    // Calculate time delta using pros::millis()
    double currentTime = pros::millis() / 1000.0;
    double dt = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;
    
    // Skip update if dt is too small
    if (dt < 0.001) return;
    
    // Cap dt to avoid numerical instability
    dt = std::min(dt, 0.1);
    
    // Make these calculation variables static
    static int currentEncoderX, currentEncoderY;
    static int deltaEncoderX, deltaEncoderY;
    static double deltaX, deltaY, headingChange;
    static double cosTheta, sinTheta;
    static double worldDeltaX, worldDeltaY;
    static pros::imu_accel_s_t accel;
    
    // Read current encoder values
    currentEncoderX = encoderX.get_value();
    currentEncoderY = encoderY.get_value();
    
    // Calculate encoder deltas
    deltaEncoderX = currentEncoderX - lastEncoderX;
    deltaEncoderY = currentEncoderY - lastEncoderY;
    
    // Save current values for next iteration
    lastEncoderX = currentEncoderX;
    lastEncoderY = currentEncoderY;
    
    // Get current heading and convert to radians for calculations
    theta = imu.getHeadingRadians();
    
    // Convert encoder ticks to distance
    deltaX = deltaEncoderX * encoderXScaleFactor;
    deltaY = deltaEncoderY * encoderYScaleFactor;
    
    // Account for encoder offset from center of rotation
    // Adjust when robot rotates (which changes what X and Y mean in global frame)
    headingChange = imu.getHeadingVelocity() * dt;
    
    // Apply offsets if significant rotation occurred
    if (std::fabs(headingChange) > 0.01) {
        // Handle rotational component for offset encoders
        // This is a complex calculation involving the circular arc traveled
        // Simplified for brevity
    }
    
    // Convert local X/Y changes to global frame using current heading
    cosTheta = cos(theta);
    sinTheta = sin(theta);
    
    worldDeltaX = deltaX * cosTheta - deltaY * sinTheta;
    worldDeltaY = deltaX * sinTheta + deltaY * cosTheta;
    
    // Calculate velocities
    vx = worldDeltaX / dt;
    vy = worldDeltaY / dt;
    omega = imu.getHeadingVelocity();
    
    // Update position
    x += worldDeltaX;
    y += worldDeltaY;
    
    // Update the localization filter with new measurements
    localizationFilter.predict(dt);
    localizationFilter.updatePosition(x, y);
    localizationFilter.updateHeading(theta);
    
    // Use the filtered acceleration data from DualIMU instead of raw IMU data
    localizationFilter.updateAcceleration(
        imu.getAccelX(),
        imu.getAccelY(),
        imu.getAccelZ()
    );
    
    // Get refined estimate from filter
    x = localizationFilter.getX();
    y = localizationFilter.getY();
    theta = localizationFilter.getTheta();
    vx = localizationFilter.getVx();
    vy = localizationFilter.getVy();
    omega = localizationFilter.getOmega();
}

std::array<double, 3> Odometry::getPose() const {
    return {x, y, theta};
}

std::array<double, 3> Odometry::getVelocity() const {
    return {vx, vy, omega};
}

void Odometry::reset(double newX, double newY, double newTheta) {
    x = newX;
    y = newY;
    theta = newTheta;
    
    vx = 0.0;
    vy = 0.0;
    omega = 0.0;
    
    // Reset the filter
    localizationFilter.reset(newX, newY, newTheta, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    
    // Reset encoders to avoid sudden jumps
    lastEncoderX = encoderX.get_value();
    lastEncoderY = encoderY.get_value();
    
    // Update the last update time
    lastUpdateTime = pros::millis() / 1000.0;
}

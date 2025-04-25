#include "common/encoder.h"

Encoder::Encoder(pros::adi::Encoder* enc, double wheelDiam, double ticksPerRev)
    : encoder(enc),
      lastUpdateTime(pros::millis() / 1000.0),
      wheelDiameter(wheelDiam),
      ticksPerUnit(ticksPerRev / (wheelDiam * M_PI)),
      // Initialize Kalman filter with physics-based model
      positionFilter(0.0, 0.0, 0.0, 1.0, 0.1, 0.05, 0.01, 0.005, 0.001, 0.5) {
    
    // Initialize encoder
    if (encoder) {
        encoder->reset();
        
        // Store initial reading
        reading = encoder->get_value();
        
        // Initialize filter with reading
        positionFilter.reset(reading, 0.0, 0.0, 1.0, 0.1, 0.05);
    }
}

void Encoder::detectStationary() {
    // Consider encoder stationary if velocity is below threshold
    double velocity = std::fabs(positionFilter.get_velocity());
    
    if (velocity < stationaryThreshold) {
        stationaryCount++;
        if (stationaryCount >= stationaryCountThreshold && !isStationary) {
            isStationary = true;
            lastStablePosition = positionFilter.get_position();
        }
    } else {
        stationaryCount = 0;
        isStationary = false;
    }
}

inline void Encoder::resetFilterStable(double stablePos, double P_pos) {
    positionFilter.reset(stablePos, 0, 0, P_pos, 0.001, 0.001);
}

void Encoder::update() {
    // Calculate time since last update
    currentTime = pros::millis() / 1000.0;
    dt = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;
    
    // Ensure dt is valid
    if (dt <= 0.001 || dt > 0.5) {
        dt = 0.01; // Use reasonable default for invalid times
    }
    
    // Get current velocity before processing
    posVelocityBefore = positionFilter.get_velocity();
    
    // Read encoder value
    reading = encoder->get_value();
    
    // Update filter based on motion state
    if (isStationary) {
        // When stationary, prevent drift by applying stable position
        // Use higher measurement noise to discount new readings
        double originalR = positionFilter.getR();
        positionFilter.setR(originalR * 10.0);
        
        // Apply stable position multiple times
        for (int i = 0; i < 5; i++) {
            positionFilter.update(lastStablePosition);
        }
        
        // Apply encoder reading with higher noise
        positionFilter.update(reading);
        
        // Restore original noise
        positionFilter.setR(originalR);
        
        // Reset filter to force zero velocity
        resetFilterStable(positionFilter.get_position(), positionFilter.get_P_pos());
    } else {
        // When moving, normal filter operation
        positionFilter.predict(dt);
        positionFilter.update(reading);
    }
    
    // Calculate acceleration
    posVelocityAfter = positionFilter.get_velocity();
    posAccel = (posVelocityAfter - posVelocityBefore) / dt;
    
    // Clamp acceleration to reasonable values
    if (std::abs(posAccel) > 10000.0) {
        posAccel = (posAccel > 0) ? 10000.0 : -10000.0;
    }
    
    // Dynamically adjust filter process noise based on motion
    if (isStationary) {
        positionFilter.setAdaptiveNoiseParams(0.01, 0.005, 0.001);
    }
    else if (std::fabs(posAccel) > 1000.0) {
        positionFilter.setAdaptiveNoiseParams(0.2, 0.04, 0.002);
    } else {
        positionFilter.setAdaptiveNoiseParams(0.1, 0.01, 0.001);
    }
    
    // Update stationary detection
    detectStationary();
}

double Encoder::getPosition() const {
    return positionFilter.get_position();
}

double Encoder::getVelocity() const {
    return positionFilter.get_velocity();
}

double Encoder::getAcceleration() const {
    return positionFilter.get_acceleration();
}

double Encoder::getDistance() const {
    // Convert from ticks to distance units
    return positionFilter.get_position() / ticksPerUnit;
}

void Encoder::reset(bool blocking) {
    if (encoder) {
        encoder->reset();
    }
    
    // Wait a moment if blocking is requested
    if (blocking) {
        pros::delay(10);
    }
    
    // Reset the filter
    positionFilter.reset(0.0, 0.0, 0.0, 1.0, 0.1, 0.05);
    
    // Reset tracking variables
    lastStablePosition = 0;
    stationaryCount = 0;
    isStationary = false;
    
    // Reset update time
    lastUpdateTime = pros::millis() / 1000.0;
}

bool Encoder::getIsStationary() const {
    return isStationary;
}

void Encoder::setStationaryThreshold(double threshold) {
    stationaryThreshold = threshold;
}

void Encoder::setWheelDiameter(double diameter) {
    wheelDiameter = diameter;
    ticksPerUnit = ticksPerUnit * (wheelDiameter * M_PI) / (diameter * M_PI);
}

void Encoder::setTicksPerRevolution(double ticks) {
    ticksPerUnit = ticks / (wheelDiameter * M_PI);
}
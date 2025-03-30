#include "common/localization_filter.h"
#include <cmath>

// Helper functions for angle conversions - static to avoid recreating
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

RobotLocalizationFilter::RobotLocalizationFilter(
    double init_x, double init_y, double init_theta,
    double init_vx, double init_vy, double init_omega,
    double init_ax, double init_ay, double init_alpha,
    double Q_pos, double Q_angle, double Q_vel, double Q_omega,
    double Q_accel, double Q_alpha, double R_pos, double R_angle,
    double R_vel, double R_accel, bool useRadians)
    : Q_pos(Q_pos), Q_angle(Q_angle), Q_vel(Q_vel), Q_omega(Q_omega),
      Q_accel(Q_accel), Q_alpha(Q_alpha), R_pos(R_pos), R_angle(R_angle),
      R_vel(R_vel), R_accel(R_accel), adaptiveFactor(1.0), useRadians(useRadians),
      maxDt(0.1), maxIterations(10),
      // Initialize timing variables
      lastPredictTime(pros::millis() / 1000.0),
      lastPositionTime(pros::millis() / 1000.0),
      lastHeadingTime(pros::millis() / 1000.0),
      lastAccelTime(pros::millis() / 1000.0),
      lastGyroTime(pros::millis() / 1000.0),
      // Initialize individual Kalman filters for acceleration
      accelXFilter(init_ax, 0, 0, 0.1, 0.01, 0.01, Q_accel, 0.1, 0.2, R_accel),
      accelYFilter(init_ay, 0, 0, 0.1, 0.01, 0.01, Q_accel, 0.1, 0.2, R_accel),
      accelZFilter(0, 0, 0, 0.1, 0.01, 0.01, Q_accel, 0.1, 0.2, R_accel) {
    
    // Initialize state
    state[0] = init_x;         // x
    state[1] = init_y;         // y
    state[2] = init_theta;     // θ
    state[3] = init_vx;        // vx
    state[4] = init_vy;        // vy
    state[5] = init_omega;     // ω
    state[6] = init_ax;        // ax
    state[7] = init_ay;        // ay
    state[8] = init_alpha;     // α (angular acceleration)
    
    // Normalize the angle
    state[2] = normalizeAngle(state[2]);
    
    // Initialize covariance matrix
    initializeCovariance();
}

double RobotLocalizationFilter::normalizeAngle(double angle) const {
    // Use fast modulo operations instead of while loops
    if (useRadians) {
        // Normalize to [-π, π] using modulo arithmetic
        return mod(angle + M_PI, 2.0 * M_PI) - M_PI;
    } else {
        // Normalize to [0, 360) using modulo arithmetic
        return mod(angle, 360.0);
    }
}

double RobotLocalizationFilter::angleDifference(double a, double b) const {
    // More efficient calculation of angle difference
    if (useRadians) {
        // Smallest angle difference in radians using modulo
        double diff = mod(a - b + M_PI, 2.0 * M_PI) - M_PI;
        return diff;
    } else {
        // Smallest angle difference in degrees using modulo
        double diff = mod(a - b + 180.0, 360.0) - 180.0;
        return diff;
    }
}

double RobotLocalizationFilter::toInternalAngle(double angle) const {
    // Convert to internal angle format
    if (useRadians) {
        return angle; // Already in radians
    } else {
        return angle; // Already in degrees
    }
}

double RobotLocalizationFilter::fromInternalAngle(double angle) const {
    // Convert from internal angle format
    return angle; // No conversion needed, just normalization
}

double RobotLocalizationFilter::getThetaRadians() const {
    if (useRadians) {
        return state[2]; // Already in radians
    } else {
        return degreesToRadians(state[2]);
    }
}

double RobotLocalizationFilter::getThetaDegrees() const {
    if (useRadians) {
        return radiansToDegrees(state[2]);
    } else {
        return state[2]; // Already in degrees
    }
}

void RobotLocalizationFilter::initializeCovariance() {
    // Zero out the covariance matrix
    for (int i = 0; i < 81; i++) {
        P[i] = 0.0;
    }
    
    // Set initial uncertainties on the diagonal
    P[0*9 + 0] = 0.1;   // x variance
    P[1*9 + 1] = 0.1;   // y variance
    P[2*9 + 2] = 0.05;  // θ variance
    P[3*9 + 3] = 0.01;  // vx variance
    P[4*9 + 4] = 0.01;  // vy variance
    P[5*9 + 5] = 0.01;  // ω variance
    P[6*9 + 6] = 0.02;  // ax variance
    P[7*9 + 7] = 0.02;  // ay variance
    P[8*9 + 8] = 0.02;  // α variance
}

void RobotLocalizationFilter::predict(double dt) {
    // Limit dt to avoid numerical instability
    dt = std::min(dt, maxDt);
    
    // If dt is too small, skip the prediction
    if (dt < 1e-6) return;
    
    // Save old state for covariance update - make static to reuse memory
    static double x, y, theta, vx, vy, omega, ax, ay, alpha;
    x = state[0];
    y = state[1];
    theta = state[2];
    vx = state[3];
    vy = state[4];
    omega = state[5];
    ax = state[6];
    ay = state[7];
    alpha = state[8];
    
    // State transition with acceleration
    // Position update with velocity and acceleration
    state[0] = x + vx * dt + 0.5 * ax * dt * dt;  // x position
    state[1] = y + vy * dt + 0.5 * ay * dt * dt;  // y position
    state[2] = theta + omega * dt + 0.5 * alpha * dt * dt;  // heading
    
    // Velocity update with acceleration
    state[3] = vx + ax * dt;  // vx
    state[4] = vy + ay * dt;  // vy
    state[5] = omega + alpha * dt;  // angular velocity
    
    // Accelerations remain constant in prediction step
    // state[6] = ax;  // ax remains the same
    // state[7] = ay;  // ay remains the same
    // state[8] = alpha;  // alpha remains the same
    
    // Normalize theta to proper range
    state[2] = normalizeAngle(state[2]);
    
    // Update covariance matrix: P = F*P*F' + Q
    // In a full implementation, we would compute the Jacobian F and properly propagate covariance
    // For a 9x9 state, a simplified diagonal process noise is added
    
    // Position variances (add velocity and acceleration contributions)
    P[0*9 + 0] += P[3*9 + 3] * dt * dt + P[6*9 + 6] * dt * dt * dt * dt / 4.0 + Q_pos * dt * dt * adaptiveFactor;
    P[1*9 + 1] += P[4*9 + 4] * dt * dt + P[7*9 + 7] * dt * dt * dt * dt / 4.0 + Q_pos * dt * dt * adaptiveFactor;
    P[2*9 + 2] += P[5*9 + 5] * dt * dt + P[8*9 + 8] * dt * dt * dt * dt / 4.0 + Q_angle * dt * dt * adaptiveFactor;
    
    // Velocity variances (add acceleration contributions)
    P[3*9 + 3] += P[6*9 + 6] * dt * dt + Q_vel * dt * adaptiveFactor;
    P[4*9 + 4] += P[7*9 + 7] * dt * dt + Q_vel * dt * adaptiveFactor;
    P[5*9 + 5] += P[8*9 + 8] * dt * dt + Q_omega * dt * adaptiveFactor;
    
    // Acceleration variances (add process noise)
    P[6*9 + 6] += Q_accel * dt * adaptiveFactor;
    P[7*9 + 7] += Q_accel * dt * adaptiveFactor;
    P[8*9 + 8] += Q_alpha * dt * adaptiveFactor;
    
    // Off-diagonal terms would need to be updated too for a complete covariance update
    // This is simplified for now
}

void RobotLocalizationFilter::updatePosition(double x, double y) {
    // Innovation (measured - predicted)
    double dx = x - state[0];
    double dy = y - state[1];
    
    // Innovation covariance S = H*P*H' + R
    double Sxx = P[0*9 + 0] + R_pos;
    double Syy = P[1*9 + 1] + R_pos;
    
    // Prevent division by zero
    if (std::fabs(Sxx) < 1e-9) Sxx = 1e-9;
    if (std::fabs(Syy) < 1e-9) Syy = 1e-9;
    
    // Kalman gain K = P*H' * inv(S) - use pre-allocated arrays
    for (int i = 0; i < 9; i++) {
        K_x[i] = P[i*9 + 0] / Sxx;
        K_y[i] = P[i*9 + 1] / Syy;
    }
    
    // Update state: state = state + K * innovation
    for (int i = 0; i < 9; i++) {
        state[i] += K_x[i] * dx + K_y[i] * dy;
    }
    
    // Normalize theta using efficient modulo
    state[2] = normalizeAngle(state[2]);
    
    // Simple Joseph form covariance update (approximation for efficiency)
    // Full matrix form omitted for brevity and computational efficiency
    P[0*9 + 0] = (1.0 - K_x[0]) * P[0*9 + 0];
    P[1*9 + 1] = (1.0 - K_y[1]) * P[1*9 + 1];
}

void RobotLocalizationFilter::updateHeading(double theta) {
    // Normalize input theta using efficient modulo
    theta = normalizeAngle(theta);
    
    // Calculate smallest angle difference using efficient method
    double dtheta = angleDifference(theta, state[2]);
    
    // Innovation covariance S = H*P*H' + R
    double S = P[2*9 + 2] + R_angle;
    
    // Prevent division by zero
    if (std::fabs(S) < 1e-9) S = 1e-9;
    
    // Kalman gain K = P*H' * inv(S) - use pre-allocated arrays
    for (int i = 0; i < 9; i++) {
        K_theta[i] = P[i*9 + 2] / S;
    }
    
    // Update state: state = state + K * innovation
    for (int i = 0; i < 9; i++) {
        state[i] += K_theta[i] * dtheta;
    }
    
    // Normalize theta using efficient modulo
    state[2] = normalizeAngle(state[2]);
    
    // Simple Joseph form covariance update (approximation for efficiency)
    P[2*9 + 2] = (1.0 - K_theta[2]) * P[2*9 + 2];
}

void RobotLocalizationFilter::updateAcceleration(double accel_x, double accel_y, double accel_z) {
    // Calculate dt since last update
    double currentTime = pros::millis() / 1000.0;
    double dt = currentTime - lastAccelTime;
    lastAccelTime = currentTime;
    
    // Limit dt to avoid numerical instability
    dt = std::min(dt, maxDt);
    if (dt < 1e-6) dt = 0.01; // Use default dt if too small
    
    // First use individual Kalman filters to clean the noisy acceleration data
    accelXFilter.predict(dt);
    accelYFilter.predict(dt);
    accelZFilter.predict(dt);
    
    accelXFilter.update(accel_x);
    accelYFilter.update(accel_y);
    accelZFilter.update(accel_z);
    
    // Get filtered accelerations - make static to reuse memory
    static double filtered_ax, filtered_ay, filtered_az;
    filtered_ax = accelXFilter.get_position();
    filtered_ay = accelYFilter.get_position();
    filtered_az = accelZFilter.get_position();
    
    // Convert body-frame accelerations to world-frame - make static
    static double cos_theta, sin_theta;
    cos_theta = cos(useRadians ? state[2] : degreesToRadians(state[2]));
    sin_theta = sin(useRadians ? state[2] : degreesToRadians(state[2]));
    
    // Rotate acceleration from body to world frame - make static
    static double world_ax, world_ay;
    world_ax = filtered_ax * cos_theta - filtered_ay * sin_theta;
    world_ay = filtered_ax * sin_theta + filtered_ay * cos_theta;
    
    // Innovation (measured - predicted) - make static
    static double dax, day;
    dax = world_ax - state[6];
    day = world_ay - state[7];
    
    // Innovation covariance - make static
    static double Sax, Say;
    Sax = P[6*9 + 6] + R_accel;
    Say = P[7*9 + 7] + R_accel;
    
    // Prevent division by zero
    if (std::fabs(Sax) < 1e-9) Sax = 1e-9;
    if (std::fabs(Say) < 1e-9) Say = 1e-9;
    
    // Kalman gain for accelerations
    for (int i = 0; i < 9; i++) {
        K_accel_x[i] = P[i*9 + 6] / Sax;
        K_accel_y[i] = P[i*9 + 7] / Say;
    }
    
    // Update state with acceleration measurements
    for (int i = 0; i < 9; i++) {
        state[i] += K_accel_x[i] * dax + K_accel_y[i] * day;
    }
    
    // Normalize theta after update
    state[2] = normalizeAngle(state[2]);
    
    // Simplified covariance update
    P[6*9 + 6] = (1.0 - K_accel_x[6]) * P[6*9 + 6];
    P[7*9 + 7] = (1.0 - K_accel_y[7]) * P[7*9 + 7];
    
    // For angular acceleration calculations - make static
    static double alpha_estimate, dalpha, Salpha;
    if (filtered_az != 0) {
        // Use z-acceleration for estimating angular acceleration
        // Note: This is a simplified approach assuming the IMU is aligned with the robot
        alpha_estimate = filtered_az * 0.01; // Scale factor for rotation
        dalpha = alpha_estimate - state[8];
        Salpha = P[8*9 + 8] + R_accel;
        
        if (std::fabs(Salpha) < 1e-9) Salpha = 1e-9;
        
        // Kalman gain for angular acceleration
        for (int i = 0; i < 9; i++) {
            K_accel_z[i] = P[i*9 + 8] / Salpha;
        }
        
        // Update state with angular acceleration
        for (int i = 0; i < 9; i++) {
            state[i] += K_accel_z[i] * dalpha;
        }
        
        // Update covariance
        P[8*9 + 8] = (1.0 - K_accel_z[8]) * P[8*9 + 8];
    }
}

void RobotLocalizationFilter::updateAngularVelocity(double omega) {
    // Convert to radians if needed
    if (!useRadians) {
        omega = degreesToRadians(omega);
    }
    
    // Innovation - make static
    static double domega;
    domega = omega - state[5];
    
    // Innovation covariance - make static
    static double S;
    S = P[5*9 + 5] + R_vel;
    
    // Prevent division by zero
    if (std::fabs(S) < 1e-9) S = 1e-9;
    
    // Kalman gain - use a static array
    static std::array<double, 9> K_omega;
    for (int i = 0; i < 9; i++) {
        K_omega[i] = P[i*9 + 5] / S;
    }
    
    // Update state
    for (int i = 0; i < 9; i++) {
        state[i] += K_omega[i] * domega;
    }
    
    // Normalize theta after update
    state[2] = normalizeAngle(state[2]);
    
    // Simplified covariance update
    P[5*9 + 5] = (1.0 - K_omega[5]) * P[5*9 + 5];
}

void RobotLocalizationFilter::updatePose(double x, double y, double theta) {
    // Sequential updates for position and heading
    updatePosition(x, y);
    updateHeading(theta);
}

void RobotLocalizationFilter::reset(double x, double y, double theta,
                                  double vx, double vy, double omega,
                                  double ax, double ay, double alpha) {
    state[0] = x;
    state[1] = y;
    state[2] = theta;
    state[3] = vx;
    state[4] = vy;
    state[5] = omega;
    state[6] = ax;
    state[7] = ay;
    state[8] = alpha;
    
    // Reset individual filters
    accelXFilter.reset(ax, 0, 0, 0.1, 0.01, 0.01);
    accelYFilter.reset(ay, 0, 0, 0.1, 0.01, 0.01);
    accelZFilter.reset(0, 0, 0, 0.1, 0.01, 0.01);
    
    // Reinitialize covariance
    initializeCovariance();
}

void RobotLocalizationFilter::setAdaptiveNoiseFactor(double factor) {
    adaptiveFactor = factor;
}

#pragma once
#include <functional>
#include <cmath> // Added for std::fabs

class KalmanFilter {
private:
    double position;  // position estimate
    double velocity;  // velocity estimate
    double acceleration; // angular acceleration estimate
    double P_pos;     // position error covariance
    double P_vel;     // velocity error covariance
    double P_acc;     // acceleration error covariance
    double P_pos_vel; // position-velocity cross-covariance
    double P_vel_acc; // velocity-acceleration cross-covariance
    double P_pos_acc; // position-acceleration cross-covariance
    double Q_pos;     // process noise for position
    double Q_vel;     // process noise for velocity
    double Q_acc;     // process noise for acceleration
    double R;         // measurement noise covariance
    double adaptiveFactor;  // Factor to adapt process noise

public:
    inline KalmanFilter(
        double init_pos=0, double init_vel=0, double init_acc=0,
        double init_P_pos=0, double init_P_vel=0, double init_P_acc=0,
        double Q_pos=0, double Q_vel=0, double Q_acc=0, double R=0.1) 
        : position(init_pos), velocity(init_vel), acceleration(init_acc),
          P_pos(init_P_pos), P_vel(init_P_vel), P_acc(init_P_acc),
          P_pos_vel(0.0), P_vel_acc(0.0), P_pos_acc(0.0),
          Q_pos(Q_pos), Q_vel(Q_vel), Q_acc(Q_acc), R(R), adaptiveFactor(1.0) {
    }

    inline void predict(double dt) {
        // Physics mechanics formula: θ = θ₀ + ω₀·t + (1/2)·α·t²
        // For velocity: ω = ω₀ + α·t
        
        // Update position using kinematic equation with acceleration
        position = position + velocity * dt + 0.5 * acceleration * dt * dt;
        
        // Update velocity using acceleration
        velocity = velocity + acceleration * dt;
        
        // Update covariances (full 3-state EKF propagation)
        // P = F·P·F^T + Q where F is the state transition matrix
        
        // Temporary variables to store old values
        const double old_P_pos = P_pos;
        const double old_P_vel = P_vel;
        const double old_P_acc = P_acc;
        const double old_P_pos_vel = P_pos_vel;
        const double old_P_vel_acc = P_vel_acc;
        const double old_P_pos_acc = P_pos_acc;
        
        // Corrected covariance updates for a 3-state (pos, vel, acc) model
        P_pos = old_P_pos
                + 2.0 * dt * old_P_pos_vel
                + dt * dt * (old_P_vel + old_P_pos_acc)
                + 1.5 * dt * dt * dt * old_P_vel_acc
                + 0.25 * dt * dt * dt * dt * old_P_acc
                + Q_pos * adaptiveFactor;

        P_pos_vel = old_P_pos_vel
                    + dt * (old_P_vel + old_P_pos_acc)
                    + 1.5 * dt * dt * old_P_vel_acc
                    + 0.5 * dt * dt * dt * old_P_acc;

        P_pos_acc = old_P_pos_acc
                    + dt * old_P_vel_acc
                    + 0.5 * dt * dt * old_P_acc;

        P_vel = old_P_vel
                + 2.0 * dt * old_P_vel_acc
                + dt * dt * old_P_acc
                + Q_vel * adaptiveFactor;

        P_vel_acc = old_P_vel_acc
                    + dt * old_P_acc;

        P_acc = old_P_acc
                + Q_acc * adaptiveFactor;
    }

    inline void update(const double& measurement) {
        // Kalman gain calculation for position measurement
        const double S = P_pos + R; // Innovation covariance
        const double K_pos = P_pos / S;
        const double K_vel = P_pos_vel / S;
        const double K_acc = P_pos_acc / S;
        
        // Innovation (measurement - prediction)
        const double innovation = measurement - position;
        
        // Update state estimates
        position += K_pos * innovation;
        velocity += K_vel * innovation;
        acceleration += K_acc * innovation;
        
        // Joseph form covariance update (more numerically stable)
        P_pos = P_pos - K_pos * P_pos;
        P_vel = P_vel - K_vel * P_pos_vel;
        P_acc = P_acc - K_acc * P_pos_acc;
        P_pos_vel = P_pos_vel - K_pos * P_pos_vel;
        P_vel_acc = P_vel_acc - K_vel * P_pos_acc;
        P_pos_acc = P_pos_acc - K_pos * P_pos_acc;
    }

    inline void reset(double init_pos, double init_vel, double init_acc, 
                     double init_P_pos, double init_P_vel, double init_P_acc) {
        position = init_pos;
        velocity = init_vel;
        acceleration = init_acc;
        P_pos = init_P_pos;
        P_vel = init_P_vel;
        P_acc = init_P_acc;
        P_pos_vel = 0.0;
        P_vel_acc = 0.0;
        P_pos_acc = 0.0;
    }
    
    inline double get_position() const { return position; }
    inline double get_velocity() const { return velocity; }
    inline double get_acceleration() const { return acceleration; }

    inline void setAdaptiveNoiseParams(double baseQPos, double baseQVel, double baseQAcc) {
        Q_pos = baseQPos;
        Q_vel = baseQVel;
        Q_acc = baseQAcc;
    }
};
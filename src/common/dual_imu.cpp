#include "common/dual_imu.h"

// Constructor: Initializes the two IMU objects and sets an initial fused value.
DualIMU::DualIMU(int port1, int port2, double driftThreshold)
  : imu1(port1), imu2(port2), driftThreshold(driftThreshold) {
  // Initialize fusedValue to the average of the immediate readings.
  r1 = imu1.get_rotation();
  r2 = imu2.get_rotation();
  fusedValue = (r1 + r2) / 2.0;
}

void DualIMU::update() {
    // Read both sensor rotations.
    r1 = imu1.get_rotation();
    r2 = imu2.get_rotation();
    double diff = std::fabs(r1 - r2);
    if(diff <= driftThreshold) {
        fusedValue = (r1 + r2) / 2.0;
    } else {
        // If drifting, select the one closer to the previous fused value.
        fusedValue = (std::fabs(r1 - fusedValue) < std::fabs(r2 - fusedValue)) ? r1 : r2;
    }
}

double DualIMU::get_rotation() const {
    return dual_get([&](const pros::IMU &imu) { return imu.get_rotation(); });
}

double DualIMU::get_yaw() const {
    return dual_get([&](const pros::IMU &imu) { return imu.get_yaw(); });
}

double DualIMU::get_pitch() const {
    return dual_get([&](const pros::IMU &imu) { return imu.get_pitch(); });
}

double DualIMU::get_roll() const {
    return dual_get([&](const pros::IMU &imu) { return imu.get_roll(); });
}

void DualIMU::reset(bool blocking) {
    imu1.reset(blocking);
    imu2.reset(blocking);
}

bool DualIMU::is_calibrating() const {
    return imu1.is_calibrating() || imu2.is_calibrating();
}
#pragma once
#include "pros/imu.hpp"
#include <cmath>

/**
 * @class DualIMU
 * @brief Dual sensor fusion for a pair of pros::IMU to reduce drift.
 *
 * This class updates an internal fused value using readings from two IMUs.
 * If the two readings differ less than driftThreshold, the fused reading is
 * simply the average. Otherwise the reading closer to the previous fused value
 * is used.
 */
class DualIMU {
private:
    pros::IMU imu1;
    pros::IMU imu2;
    double driftThreshold; ///< Drift threshold in degrees.
    double fusedValue;     ///< Last fused rotation value.
    double r1, r2;

    // Helper for scalar getters using a lambda on a single IMU.
    template <typename Func>
    double dual_get(Func f) const {
        double v1 = f(imu1);
        double v2 = f(imu2);
        double diff = std::fabs(v1 - v2);
        if(diff <= driftThreshold)
            return (v1 + v2) / 2.0;
        // When readings disagree, fallback to the stored fused value.
        // (If fusedValue is not set yet, average is used.)
        return (std::fabs(v1 - fusedValue) < std::fabs(v2 - fusedValue)) ? v1 : v2;
    }

public:
    /**
     * @brief Constructs a DualIMU object.
     * @param port1 Port number for the first IMU.
     * @param port2 Port number for the second IMU.
     * @param driftThreshold Threshold (in degrees) above which the readings are considered to be drifting.
     */
    DualIMU(int port1, int port2, double driftThreshold = 10.0);

    /**
     * @brief Updates the internal fused value.
     */
    void update();

    // Read methods using the latest fused value.
    double get_rotation() const;
    double get_yaw() const;
    double get_pitch() const;
    double get_roll() const;

    // Command methods (broadcast to both sensors).
    void calibrate();
    void reset(bool blocking);
    bool is_calibrating() const;
};

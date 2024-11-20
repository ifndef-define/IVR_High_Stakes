#ifndef DUALSTACKEDIMU_HPP
#define DUALSTACKEDIMU_HPP
#include "main.h"

class dualStackedIMU {
    private:
        // References to the IMU objects
        pros::Imu imu1_;
        pros::Imu imu2_;

        // Whether the IMUs are active
        bool imu1_active;
        bool imu2_active;

        // Rotation offsets for the IMUs following the unit circle
        double imu1_offset;
        double imu2_offset;

        /**
         * @brief Checks if the IMUs are working / connected and are aligned. This
         * method will periodically run to determine which IMU if any needs to be turned
         * off due to disconnection or excessive drift.
         */
        void _check_active();

    public:
        // Headings for the IMUs
        struct orientation {
            double _heading;
            double _pitch;
            double _roll;
            double &_yaw = _heading;
        };

        /**
         * @brief Construct a new dual Stacked IMU object
         * 
         * @param imu1 The first IMU object
         * @param imu2 The second IMU object
         * @param Other Params Other parameters
         */
        dualStackedIMU(pros::Imu &imu1, pros::Imu &imu2, /* Other Params */);
        
        /**
         * @brief Reset the IMUs
         * 
         * @param blocking Whether to block the program execution until it is complete
         */
        void reset(bool blocking = true);

        /**
         * @brief Set the orientation object
         * 
         * @param heading the new heading of the imu
         */
        void set_heading(double heading);
        
        /**
         * @brief Get the data from the IMUs
         * 
         * @return the orientation struct containing the heading, pitch, roll, and yaw
         * Example:
         * @code 
         *     dualImu.get().pitch;
         */
        orientation get();

        /**
         * @brief Checks the IMU difference
         * If the difference exceeds 5 degrees, then disable the sensor furtheset from the heading.
         */
        void check_IMU_difference(double heading);
};
#endif
#pragma once
#include "main.h"

// Aliases
using motor = pros::Motor;
using ctrler = pros::Controller;
using motor_g = pros::MotorGroup;

class drive {
    public:
        /**
         * @brief Used by driver control to move the robot
         * 
         * @param thread If true, will run in a separate thread
         *  @todo Need to add a way to stop the thread
         */
        void loop(bool thread=false);

        /**
         * @brief Pauses the drive loop
         *  @todo May not be needed
         */
        void pauseLoop();


        void moveAtRpm(int rpm);
        void turnAtRpm(int rpm);

        /**
         * @brief Stops the drive motors and kills the thread if running
         */
        void stop();
        

        enum drive_mode_e {
            TANK_m = 1,             // Tank Drive
            SINGLE_STICK_ARCADE_R,  // Single Stick Arcade on Right Stick
            SINGLE_STICK_ARCADE_L,  // Single Stick Arcade on Left Stick
            SPLIT_ARCADE_PR,        // Split Arcade with power on Right Stick
            SPLIT_ARCADE_PL,        // Split Arcade with power on Left Stick
            HOLONOMIC_SR,           // Holonomic Drive with Strafe on Right Stick
            HOLONOMIC_SL,           // Holonomic Drive with Strafe on Left Stick
            FIELD_CENTRIC_SR,       // Field Centric Drive with Strafe on Right Stick
            FIELD_CENTRIC_SL,       // Field Centric Drive with Strafe on Left Stick
            CUSTOM_m
        };

        enum drive_config_e {
            TANK_c = 1,
            HOLONOMIC,
            CUSTOM_c
        };

    private:
        drive() = default;

        // Device pointers
        ctrler *drive_ctrler_;
        motor *driveMotors[8];
        motor_g *left_side_;
        motor_g *right_side_;

        // Drive configuration
        short int drive_motor_count_ = 0;
        short int drive_deadzone_ = 0;
        double straight_l_scale_ = 1;
        double straight_r_scale_ = 1;
        double drive_exponential_scale_ = 0;
        double drive_sin_scale_ = 0;
        int max_rpm_ = 0;
        drive_mode_e drive_mode_;
        drive_config_e drive_config_;

        // Drive control variables
        struct ctrler_axis_s {
            int l_x, l_y, r_x, r_y;
        };
        ctrler_axis_s raw_axis;
        ctrler_axis_s calc_axis;
        int right=0, left=0, strafe=0, turn=0, fwd=0;

        // Private functions
        /**
         * @brief Drive loop for control for all drive modes.
         * Built to run with or without a thread
         */
        void driveLoop();

        /**
         * @brief Updates the controller inputs to match the drive mode
         * and scaling configuration
         */
        void updateAxis();

        /**
         * @brief Returns a normalized axis value [-1, 1] to ease drive scaling
         */
        double normalizeAxis(double axis);

        // For Scaling see https://www.desmos.com/calculator/82gzfspsei 
        /**
         * @brief Returns an exponential scaled axis value
         * 
         * @param axis Axis value to scale
         * NOTE: Normalization will happen in the function
         */
        int exponentialScale(int axis);

        /**
         * @brief Returns a sin scaled axis value
         * 
         * @param axis Axis value to scale
         * NOTE: Normalization will happen in the function
         */
        int sinScale(int axis);

    friend class drive_builder;
};

class drive_builder {
    public:
        /**
         * @brief Construct a new drive builder object
         * NOTE: Required to build a drive object
         * 
         * @param ctrler_1 Controller object to use for drive control
         */
        drive_builder(ctrler &ctrler_1);

        /**
         * @brief Set the drive configuration
         * NOTE: Required to build a drive object
         * 
         * @param drive_config Drive configuration to use
         */
        drive_builder &with_drive_config(drive::drive_config_e drive_config);

        /**
         * @brief Set the drive motors
         * NOTE: Required to build a drive object
         * 
         * @param l_motors Left side motors in a list
         * @param r_motors Right side motors in a list
         */
        drive_builder &with_drive_motors(initializer_list<motor*> l_motors, initializer_list<motor*> r_motors);

        /**
         * @brief Set the drive motors
         * NOTE: Required to build a drive object
         * 
         * @param motor_g_1 MotorGroup object for left side
         * @param motor_g_2 MotorGroup object for right side
         */
        drive_builder &with_drive_motors(motor_g &motor_g_1, motor_g &motor_g_2);

        /**
         * @brief Set the drive mode
         * NOTE: Required to build a drive object
         * 
         * @param drive_mode Drive mode to use
         */
        drive_builder &with_drive_mode(drive::drive_mode_e drive_mode);

        /**
         * @brief Set the max RPM. No use case yet
         * 
         * @param rpm Max RPM to set
         */
        drive_builder &add_max_rpm(int rpm);

        /**
         * @brief Set the controller deadzone for stick drift
         * Note: This must be a small positive number in range [0, 10]
         * 
         * @param deadzone Deadzone to set inclusive
         */
        drive_builder &add_ctrler_deadzone(short int deadzone);

        /**
         * @brief Set the straight drive scale for left and right side
         * @todo Add an option to autoscale using test run
         * 
         * @param l_scale Left side scale
         * @param r_scale Right side scale
         */
        drive_builder &add_straight_drive_scale(double l_scale, double r_scale);

        /**
         * @brief Set the exponential drive scale
         * NOTE: Cannot be used with sin drive scale
         * 
         * @param scale Scale to set
         */
        drive_builder &add_exponetial_drive_scale(double scale);

        /**
         * @brief Set the sin drive scale
         * NOTE: Cannot be used with exponential drive scale
         * 
         * @param scale Scale to set
         */
        drive_builder &add_sin_drive_scale(double scale);

        /**
         * @brief Add acceleration limiting
         * 
         * @param limitFactor Factor to limit acceleration by
         * @param ignoredDomain Range to ignore acceleration limiting
         *  The domain will be [-ignoredDomain, ignoredDomain]
         */
        drive_builder &add_acceleration_limiting(double limitFactor, double ignoredDomain);

        /**
         * @brief Set the odom config
         * @todo Need Odom class
         */
        drive_builder &add_odom_config(/** @todo Need Odom class */);

        /**
         * @brief Add uniform holonomic limiting
         */
        drive_builder &add_uniform_holomoic_limiting();

        /**
         * @brief Build the drive object
         * 
         * @return Built drive object
         */
        drive *build();

    private:
        uint8_t checkSum[2];
        drive *drive_;
};
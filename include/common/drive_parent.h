#pragma once
#include "main.h"
#include "common/pid.hpp"
#include "common/odom.h"
#include "common/util.hpp"

// Aliases
using motor = pros::Motor;
using controller = pros::Controller;
using motor_g = pros::MotorGroup;

class Drive {
    public:
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


        enum drive_error {
            NO_ERROR = 0,
            // Long followed by # of short below
            DRIVE_LOOP_FAIL = 1,
            UPDATE_AXIS_BAD_DRIVE_MODE_TANK = 2,
            UPDATE_AXIS_BAD_DRIVE_MODE_HOLONOMIC = 3,
            BUILDER_UNBALANCED_MOTOR_ARRAYS = 4,
            BUILDER_LARGE_CTRLER_DEADZONE = 5,
            BUILDER_OUT_OF_BOUNDS_EXP_SCALE = 6,
            BUILDER_OUT_OF_BOUNDS_SIN_SCALE = 7,
            BUILDER_OUT_OF_BOUNDS_STRAIGHT_SCALE = 8,
            // Two long followed by #-7 of short below
            BUILDER_MISSING_REQUIRED_PARAMS = 9,
            BUILDER_MULTIPLE_DRIVE_SCALES = 10,
            MULTIPLE_DRIVE_OBJECTS = 11
        };

        enum drive_config_e {
            TANK_c = 1,
            HOLONOMIC,
            CUSTOM_c
        };

        enum DriveSide {
            LEFT = 0,
            RIGHT = 1
        };
        
        /**
         * @brief Used by driver control to move the robot
         * 
         * @param thread If true, will run in a separate thread
         *  @todo Need to add a way to stop the thread
         */
        static void loop(bool thread=false);

        /**
         * @brief Stops the drive loop if its in thread mode
         *  To restart, call loop() again
         */
        static void stopLoop();

        ////////////////////////////////////
        ///// Motion Related Functions /////
        ////////////////////////////////////
        
        /**
         * @brief Moves the robot at a set RPM. The RPM will be scaled
         *  to a voltage value and applied to the motors
         * NOTE: The maxRPM must be set for this to work 
         */
        static void moveAtRPM(int rpm);

        /**
         * @brief Turns the robot at a set RPM. The RPM will be scaled
         *  to a voltage value and applied to the motors
         * NOTE: The maxRPM must be set for this to work 
         */
        static void turnAtRPM(int rpm);

        /**
         * @brief Gets the current RPM of the robot
         * 
         * @return A vector of doubles containing the left and right RPM
         */
        static std::pair<double, double> getRPM();

        /**
         * @brief Turns the robot to a target angle using PID.
         * 
         * @param angle Target angle in degrees [-360, 360]
         * @param timeout Timeout in milliseconds
         * @param async If true, will run in a separate thread
         */
        static void turnToAngle(double angle, int timeout = 2500, double turn_max_voltage = 127, double turn_settle_error = .25, bool async=true);

        /**
         * @brief Turns the robot to a target angle using PID.
         * 
         * @param angle Target angle in degrees [-360, 360]
         * @param timeout Timeout in milliseconds
         * @param async If true, will run in a separate thread
         */
        static void swingToAngle(double angle, DriveSide lockedSide, int timeout = 2500, double turn_max_voltage = 127, double turn_settle_error = .25, bool async=true);


        /**
         * @brief Turns the robot to a target angle using PID.
         * 
         * @param angle Target angle in degrees [-360, 360]
         * @param timeout Timeout in milliseconds
         * @param async If true, will run in a separate thread
         */
        static void turnToPoint(double x, double y, int timeout = 2500, double turn_max_voltage = 127, double turn_settle_error = .25, bool async=true);

        /**
         * @brief Turns the robot to a target angle using PID.
         * 
         * @param angle Target angle in degrees [-360, 360]
         * @param timeout Timeout in milliseconds
         * @param async If true, will run in a separate thread
         */
        static void swingToPoint(double x, double y, DriveSide lockedSide, int timeout = 2500, double turn_max_voltage = 127, double turn_settle_error = .25, bool async=true);

        /**
         * @brief Moves the robot given a target position and angle using PID. 
         * By default, we go forward to the target position while maintaining the current angle.
         * 
         * @param target Target position in inches
         * @param angle Target angle in degrees [-360, 360], default is current angle
         * @param timeout Timeout in milliseconds
         * @param async If true, will run in a separate thread
         */
        static void translateBy(double distance, int timeout=5000, double drive_settle_error = .25, double drive_max_voltage = 127, bool async=true);

        /**
         * @brief Turns the robot to a target pose using PID.
         * 
         * @param x Target x position in inches
         * @param y Target y position in inches
         * @param theta Target angle in degrees [-360, 360]
         * @param lead Lead distance in inches
         * @param setback Setback distance in inches
         * @param drive_min_voltage Minimum drive voltage
         * @param drive_max_voltage Maximum drive voltage
         * @param heading_max_voltage Maximum heading voltage
         * @param timeout Timeout in milliseconds
         * @param async If true, will run in a separate thread
         */
        static void moveToPose(double x, double y, double theta=999, bool reverse=false, int timeout=5000, double drive_min_voltage = 0, double drive_max_voltage = 127, double heading_max_voltage = 110, double drive_settle_error = .5, double turn_settle_error = .5, double lead = 1.5, double position_threshold=8.2, bool async=false);
        /**
         * @brief Determines values to move the robot given a vector. This will compute 
         *  the vector from the current position to the target position and move the robot
         * 
         * @param x X component of the vector [0,144] inches
         * @param y Y component of the vector [0,144] inches
         */
        static void moveByVector(double x, double y, double atSpeed=65);

        /**
         * @brief Moves the robot given a normalized left and right drive power value.
         *  Converts the values to voltage and applies them to the motors
         * 
         * @param left Left side power value [-1, 1]
         * @param right Right side power value [-1, 1]
         */
        static void moveDrive(double left, double right);

        /**
         * @brief Stops drive motion given current motor brake mode
         */
        static void brake();

        /**
         * * @brief Sets the brake mode of the drive motors
         */
        static void setBrakeMode(pros::motor_brake_mode_e mode);

        ///////////////////////////////////////////
        /// Temporary Functions For High Stakes ///
        ///////////////////////////////////////////

        /**
         * @brief Changes the drive mode of the robot
         * 
         * @param drive_mode new drive mode to set
         * NOTE: Unique to Vex High Stakes
         */
        // [[deprecated("ONLY FOR VEX HIGH STAKES")]]
        static void changeDriveMode(drive_mode_e drive_mode);

        /**
         * @brief Changes the motors of the robot's drive
         * 
         * @param  new drive configuration to set
         * NOTE: Unique to Vex High Stakes
         */
        // [[deprecated("ONLY FOR VEX HIGH STAKES")]]
        static void changeDriveMotors(motor_g &motor_g_1, motor_g &motor_g_2);


    private:
        Drive() = default;

        static Drive *instance_;

        // Device pointers
        static controller *drive_ctrler_;
        static motor *driveMotors[8];
        static motor_g *left_side_;
        static motor_g *right_side_;

        static PID *drive_pid;
        static PID *turn_pid;

        static odom *odom_;

        // Drive configuration
        inline static short int drive_motor_count_ = 0;
        inline static short int drive_deadzone_ = 0;
        inline static double straight_l_scale_ = 1;
        inline static double straight_r_scale_ = 1;
        inline static double drive_exponential_scale_ = 0;
        inline static double drive_sin_scale_ = 0;
        inline static int max_rpm_ = 0;
        static drive_mode_e drive_mode_;
        static drive_config_e drive_config_;

        static pros::Mutex multiDrive_mutex;
        static bool motionInProgress;
        static bool motionQueued;

        // Drive control variables
        struct ctrler_axis_s {
            int l_x, l_y, r_x, r_y;
        };
        static ctrler_axis_s raw_axis;
        static ctrler_axis_s calc_axis;
        inline static int right=0, left=0, strafe=0, turn=0, fwd=0;

        static pros::Task *drive_task;
        inline static bool isThread = false;

        // struct driveMoveConstants {
        //     double kV;
        //     double kA;
        //     double a_des;
        //     double mVps_decrease;
        //     double mVps_increase;
        //     double ignore_slew;
        //     double maintain_angle_voltage;
        // };

        // static driveMoveConstants *dConsts;

        static void errorMsg(int err_num);

        // Private functions
        /**
         * @brief Drive loop for control for all drive modes.
         * Built to run with or without a thread
         */
        static void driveLoop();

        /**
         * @brief Updates the controller inputs to match the drive mode
         * and scaling configuration
         */
        static void updateAxis();

        /**
         * @brief A helper function that returns a normalized axis 
         *  value [-1, 1] to ease drive scaling calculations
         */
        static double normalizeAxis(double axis);

        // For Scaling see https://www.desmos.com/calculator/82gzfspsei 
        /**
         * @brief Returns an exponential scaled axis value
         * 
         * @param axis Axis value to scale
         * NOTE: Normalization will happen in the function
         */
        static int exponentialScale(int axis);

        /**
         * @brief Returns a sin scaled axis value
         * 
         * @param axis Axis value to scale
         * NOTE: Normalization will happen in the function
         */
        static int sinScale(int axis);

        static void requestMotionStart();
        
        static void endMotion();
        
        static void cancelMotion();
        
        static void cancelAllMotions();

        static bool isInMotion();

    // Allow drive_builder to access private members
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
        drive_builder(controller &ctrler_1);

        /**
         * @brief Set the drive configuration
         * NOTE: Required to build a drive object
         * 
         * @param drive_config Drive configuration to use
         */
        drive_builder &with_drive_config(Drive::drive_config_e drive_config);

        /**
         * @brief Set the drive motors
         * NOTE: Required to build a drive object
         * 
         * @param l_motors Left side motor ports
         * @param r_motors Right side motors ports
         */
        drive_builder &with_drive_motors(vector<int8_t> l_motors, vector<int8_t> r_motors, pros::MotorGears gearing);

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
        drive_builder &with_drive_mode(Drive::drive_mode_e drive_mode);

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
         * @brief Set the odom config
         * @todo Need Odom class
         */
        drive_builder &add_odom_config(odom &robot_odom);

        /**
         * @brief Set the drive PID
         * 
         * @param lat_pid the lateral PID object
         */
        drive_builder &add_lat_pid(PID &lat_pid);

        /**
         * @brief Set the turn PID
         * 
         * @param turn_pid the turn PID object
         */
        drive_builder &add_turn_pid(PID &turn_pid);

        // /**
        //  * @brief Set the drive move constants
        //  * 
        //  * @param constants Drive move constants to set
        //  */
        // drive_builder &add_drive_move_constants(Drive::driveMoveConstants constants);

        /**
         * @brief Build the drive object
         * 
         * @return Built drive object
         */
        Drive *build();

    private:
        uint8_t checkSum[2];
        Drive *drive_;
};
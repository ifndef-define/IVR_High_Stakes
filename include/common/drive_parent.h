#pragma once
#include "main.h"

using motor = pros::Motor;
using ctrler = pros::Controller;
using motor_g = pros::MotorGroup;

class drive {
    public:
        void loop(bool thread=false);
        void pauseLoop();
        void moveAtRpm(int rpm);
        void turnAtRpm(int rpm);
        void stop();

        typedef enum {
            TANK = 1,
            SINGLE_STICK_ARCADE_R,
            SINGLE_STICK_ARCADE_L,
            SPLIT_ARCADE_PR,
            SPLIT_ARCADE_PL,
            HOLONOMIC_SR,
            HOLONOMIC_SL,
            FIELD_CENTRIC_SR,
            FIELD_CENTRIC_SL,
            CUSTOM
        } drive_mode_e;

        typedef enum {
            TANK = 1,
            HOLONOMIC,
            CUSTOM
        } drive_config_e;

    private:
        drive() = default;

        // Device pointers
        ctrler *drive_ctrler_;
        motor *driveMotors[8];
        motor_g *left_side_;
        motor_g *right_side_;

        // Drive configuration
        short int drive_motor_count_;
        short int drive_deadzone_;
        double straight_l_scale_;
        double straight_r_scale_;
        double drive_exponential_scale_;
        double drive_sin_scale_;
        int max_rpm_;
        drive_mode_e drive_mode_;
        drive_config_e drive_config_;

        // Drive control variables
        struct ctrler_axis_s {
            int l_x, l_y, r_x, r_y;
        };
        ctrler_axis_s raw_axis;
        ctrler_axis_s calc_axis;
        int right, left, strafe, turn, fwd;

        // Private functions
        void updateAxis();
        double normalizeAxis(double axis);
        double exponentialScale(double axis);
        double sinScale(double axis);

    friend class drive_builder;
};

class drive_builder {
    public:
        drive_builder(ctrler &ctrler_1);
        drive_builder &with_drive_config(drive::drive_config_e drive_config);
        drive_builder &with_drive_motors(initializer_list<motor&> l_motors, initializer_list<motor&> r_motors);
        drive_builder &with_drive_motors(motor_g &motor_g_1, motor_g &motor_g_2);
        drive_builder &with_drive_mode(drive::drive_mode_e drive_mode);

        drive_builder &add_max_rpm(int rpm);
        drive_builder &add_ctrler_deadzone(short int deadzone);
        drive_builder &add_straight_drive_scale(double l_scale, double r_scale);
        drive_builder &add_exponetial_drive_scale(double scale);
        drive_builder &add_sin_drive_scale(double scale);
        drive_builder &add_odom_config(/** @todo Need Odom class */);

        drive *build();
    private:
        uint8_t checkSum[2];
        drive *drive_;
};
#include "main.h"

using namespace std;
using motor = pros::Motor;
using ctrler = pros::Controller;
using motor_g = pros::MotorGroup;

class drive {
    public:
        void move();

        typedef enum {
            TANK = 1,
            SINGLE_STICK_ARCADE_R,
            SINGLE_STICK_ARCADE_L,
            SPLIT_ARCADE_PR,
            SPLIT_ARCADE_PL,
            HOLONOMIC_SR,
            HOLONOMIC_SL,
            CUSTOM
        } drive_mode_e;

        typedef enum {
            TANK = 1,
            HDRIVE,
            MECANUM,
            XDRIVE,
            CUSTOM
        } drive_config_e;

    private:
        ctrler *drive_ctrler_;
        motor *driveMotors[8];
        motor_g *left_side_;
        motor_g *right_side_;
        short int drive_motor_count_;
        short int drive_deadzone_;
        double straight_l_scale_;
        double straight_r_scale_;
        double drive_exponential_scale_;
        double drive_sin_scale_;

    friend class drive_builder;
};

class drive_builder {
    public:
        drive_builder(ctrler &ctrler_1);
        drive_builder &with_drive_config(drive::drive_config_e drive_config);
        // drive_builder &with_drive_motors(motor &motor_1, motor &motor_2, motor &motor_3, motor &motor_4);
        // drive_builder &with_drive_motors(motor &motor_1, motor &motor_2, motor &motor_3, motor &motor_4, motor &motor_5, motor &motor_6);
        // drive_builder &with_drive_motors(motor &motor_1, motor &motor_2, motor &motor_3, motor &motor_4, motor &motor_5, motor &motor_6, motor &motor_7, motor &motor_8);
        drive_builder &with_drive_motors(initializer_list<motor*> l_motors, initializer_list<motor*> r_motors) {
            if (l_motors.size() != r_motors.size()) {
                throw invalid_argument("Left and right motor arrays must be the same size");
            }
            drive_->drive_motor_count_ = l_motors.size();
            auto l_it = l_motors.begin();
            auto r_it = r_motors.begin();
            for (int i = 0; i < l_motors.size(); i++, l_it++, r_it++) {
                drive_->driveMotors[i] = *l_it;
                drive_->driveMotors[i + l_motors.size()] = *r_it;
            }
            return *this;
        }
        drive_builder &with_drive_motors(motor_g &motor_g_1, motor_g &motor_g_2);
        drive_builder &with_drive_mode(drive::drive_mode_e drive_mode);
        drive_builder &add_ctrler_deadzone(short int deadzone);
        drive_builder &add_straight_drive_scale(double l_scale, double r_scale);
        drive_builder &add_exponetial_drive_scale(double scale);
        drive_builder &add_sin_drive_scale(double scale);
        drive_builder &add_odom_config(/** @todo Need Odom class */);

        drive *build();
    private:
        drive *drive_;
};


//==========================================================
Controller master(E_CONTROLLER_MASTER);

MotorGroup left_side({1, 2, 3, 4}, MotorGears::green);
MotorGroup right_side({5, 6, 7, 8}, MotorGears::green);
Motor left_front(1, MotorGears::green);
Motor left_back(2, MotorGears::green);
Motor right_front(3, MotorGears::green);
Motor right_back(4, MotorGears::green);
int main() {
    drive *drive_1 = drive_builder(master)
        .with_drive_config(drive::MECANUM)
        // .with_drive_motors(left_side, right_side)
        .with_drive_motors({&left_front, &left_back}, {&right_front, &right_back})
        .with_drive_mode(drive::SPLIT_ARCADE_PR)
        .add_ctrler_deadzone(10)
        .add_straight_drive_scale(1.0, 0.98)
        .add_exponetial_drive_scale(1.5)
        // .add_sin_drive_scale(1.6)
        .build();

    // drive_1->

    return 0;
}
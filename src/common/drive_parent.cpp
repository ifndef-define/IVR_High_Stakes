#include "common/drive_parent.h"

drive::drive(ctrler &ctrler_1, motor_g &motor_g_1, motor_g &motor_g_2, 
    drive::drive_config_e drive_config, drive::drive_mode_e drive_mode, 
    short int drive_deadzone, double straight_l_scale, double straight_r_scale, 
    double drive_exponential_scale, double drive_sin_scale, int max_rpm) : 
    drive_ctrler_(&ctrler_1), left_side_(&left_side_), right_side_(&right_side_),
    drive_config_(drive_config), drive_mode_(drive_mode), drive_deadzone_(drive_deadzone),
    straight_l_scale_(straight_l_scale), straight_r_scale_(straight_r_scale), 
    drive_exponential_scale_(drive_exponential_scale), drive_sin_scale_(drive_sin_scale), 
    max_rpm_(max_rpm) {
        left_side_ = &motor_g_1;
        right_side_ = &motor_g_2;
        vector<int8_t> ports_temp = left_side_->get_port_all();
        drive_motor_count_ = ports_temp.size();
    }
drive::drive(ctrler &ctrler_1, initializer_list<motor*> l_motors, 
    initializer_list<motor*> r_motors, drive::drive_config_e drive_config, 
    drive::drive_mode_e drive_mode, short int drive_deadzone, double straight_l_scale, 
    double straight_r_scale, double drive_exponential_scale, double drive_sin_scale, int max_rpm) : 
    drive_ctrler_(&ctrler_1), drive_config_(drive_config), drive_mode_(drive_mode),
    drive_deadzone_(drive_deadzone), straight_l_scale_(straight_l_scale), 
    straight_r_scale_(straight_r_scale), drive_exponential_scale_(drive_exponential_scale), 
    drive_sin_scale_(drive_sin_scale), max_rpm_(max_rpm) {
        if (l_motors.size() != r_motors.size()) {
            throw invalid_argument("Left and right motor arrays must be the same size");
        }
        drive_motor_count_ = l_motors.size();
        auto l_it = l_motors.begin();
        auto r_it = r_motors.begin();
        for (int i = 0; i < l_motors.size(); i++, l_it++, r_it++) {
            driveMotors[i] = *l_it;
            driveMotors[i + l_motors.size()] = *r_it;
        }
    }

void drive::loop(bool thread=false) {

}

void drive::pauseLoop() {

}

void drive::moveAtRpm(int rpm) {

}

void drive::turnAtRpm(int rpm) {

}

void drive::stop() {

}


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
        // Device pointers
        ctrler *drive_ctrler_;
        motor *driveMotors[8];
        motor_g *left_side_;
        motor_g *right_side_;

        // Drive configuration
        uint8_t checkSum;
        short int drive_motor_count_;
        short int drive_deadzone_;
        double straight_l_scale_;
        double straight_r_scale_;
        double drive_exponential_scale_;
        double drive_sin_scale_;
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
        drive *drive_;
};
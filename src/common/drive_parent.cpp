#include "common/drive_parent.h"

void drive::loop(bool thread) {
    left_side_->move(drive_ctrler_->get_analog(LEFT_Y_AXIS));
    right_side_->move(drive_ctrler_->get_analog(RIGHT_Y_AXIS));
}

void drive::pauseLoop() {

}

void drive::moveAtRpm(int rpm) {

}

void drive::turnAtRpm(int rpm) {

}

void drive::stop() {

}

void drive::updateAxis() {

}

double drive::normalizeAxis(double axis) {
    return axis/127.0;
}

double drive::exponentialScale(double axis) {
    return 0;
}

double drive::sinScale(double axis) {
    return 0;
}

drive_builder::drive_builder(ctrler &ctrler_1) {
    checkSum[0] = 0b00000001; // Used for required parameters 
    checkSum[1] = 0b00000001; // Used for selected drive scale parameters

    drive_->drive_ctrler_ = &ctrler_1;    
}
drive_builder &drive_builder::with_drive_config(drive::drive_config_e drive_config) {
    drive_->drive_config_ = drive_config;
    checkSum[0]<<=1; // Shift left by 1    

    return *this;
}
drive_builder &drive_builder::with_drive_motors(initializer_list<motor*> l_motors, initializer_list<motor*> r_motors) {
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
    checkSum[0]<<=1; // Shift left by 1

    return *this;
}
drive_builder &drive_builder::with_drive_motors(motor_g &motor_g_1, motor_g &motor_g_2) {
    drive_->left_side_ = &motor_g_1;
    drive_->right_side_ = &motor_g_2;
    vector<int8_t> l_motors = motor_g_1.get_port_all();
    vector<int8_t> r_motors = motor_g_2.get_port_all();
    if (l_motors.size() != r_motors.size()) {
        throw invalid_argument("Left and right motor arrays must be the same size");
    }
    drive_->drive_motor_count_ = l_motors.size();
    for (int i = 0; i < l_motors.size(); i++) {
        drive_->driveMotors[i] = new motor(l_motors[i]);
        drive_->driveMotors[i + l_motors.size()] = new motor(r_motors[i]);
    }
    checkSum[0]<<=1; // Shift left by 1   

    return *this;
}
drive_builder &drive_builder::with_drive_mode(drive::drive_mode_e drive_mode) {
    drive_->drive_mode_ = drive_mode;
    checkSum[0]<<=1; // Shift left by 1

    return *this;
}

drive_builder &drive_builder::add_max_rpm(int rpm) {
    drive_->max_rpm_ = rpm;

    return *this;
}
drive_builder &drive_builder::add_ctrler_deadzone(short int deadzone) {
    drive_->drive_deadzone_ = deadzone;

    return *this;
}
drive_builder &drive_builder::add_straight_drive_scale(double l_scale, double r_scale) {
    drive_->straight_l_scale_ = l_scale;
    drive_->straight_r_scale_ = r_scale;

    return *this;
}
drive_builder &drive_builder::add_exponetial_drive_scale(double scale) {
    drive_->drive_exponential_scale_ = scale;
    checkSum[1]<<=1; // Shift left by 1

    return *this;
}
drive_builder &drive_builder::add_sin_drive_scale(double scale) {
    drive_->drive_sin_scale_ = scale;
    checkSum[1]<<=1; // Shift left by 1

    return *this;
}
drive_builder &drive_builder::add_odom_config(/** @todo Need Odom class */) {
    // Awaiting Odom class

    return *this;
}

drive *drive_builder::build() {
    if (checkSum[0] != 0b00001000) {
        throw invalid_argument("Missing required parameters");
    }
    if (checkSum[1] != 0b00000001 || checkSum[1] != 0b00000010) {
        throw invalid_argument("Multiple drive scale parameters set, only select one");
    }

    return drive_;
}
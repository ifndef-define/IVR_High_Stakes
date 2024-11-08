#include "common/drive_parent.h"

void drive::driveLoop() {
    switch 
}

void drive::loop(bool thread) {
    if (thread) {
        pros::Task inlineDrive{[&] {
            driveLoop();
            pros::delay(15);  
        }};
    } else {
        driveLoop();
    }
}

void drive::pauseLoop() {

}

void drive::moveAtRpm(int rpm) {

}

void drive::turnAtRpm(int rpm) {

}

void drive::stop() {
    if (left_side_ != nullptr) { left_side_->brake(); }
    if (right_side_ != nullptr) { right_side_->brake(); }
    
    for (int i = 0; i < drive_motor_count_; i++) {
        if (driveMotors[i] != nullptr)
            driveMotors[i]->brake();
    }
}

void drive::updateAxis() {
    // Get raw axis values
    raw_axis.l_x = drive_ctrler_->get_analog(LEFT_X_AXIS);
    raw_axis.l_y = drive_ctrler_->get_analog(LEFT_Y_AXIS);
    raw_axis.r_x = drive_ctrler_->get_analog(RIGHT_X_AXIS);
    raw_axis.r_y = drive_ctrler_->get_analog(RIGHT_Y_AXIS);

    // Apply deadzone
    calc_axis.l_x = abs(raw_axis.l_x) > drive_deadzone_ ? raw_axis.l_x : 0;
    calc_axis.l_y = abs(raw_axis.l_y) > drive_deadzone_ ? raw_axis.l_y : 0;
    calc_axis.r_x = abs(raw_axis.r_x) > drive_deadzone_ ? raw_axis.r_x : 0;
    calc_axis.r_y = abs(raw_axis.r_y) > drive_deadzone_ ? raw_axis.r_y : 0;

    if (drive_config_ == drive_config_e::TANK_c) {
        switch (drive_mode_) {
            case drive_mode_e::TANK_m:
                right = drive_exponential_scale_ ? exponentialScale(calc_axis.r_y) : drive_sin_scale_ ? sinScale(calc_axis.r_y) : calc_axis.r_y;
                left = drive_exponential_scale_ ? exponentialScale(calc_axis.l_y) : drive_sin_scale_ ? sinScale(calc_axis.l_y) : calc_axis.l_y;
            case drive_mode_e::SINGLE_STICK_ARCADE_R:
                fwd = drive_exponential_scale_ ? exponentialScale(calc_axis.r_y) : drive_sin_scale_ ? sinScale(calc_axis.r_y) : calc_axis.r_y;
                turn = drive_exponential_scale_ ? exponentialScale(calc_axis.r_x) : drive_sin_scale_ ? sinScale(calc_axis.r_x) : calc_axis.r_x;
                right = fwd + turn;
                left = fwd - turn;
            case drive_mode_e::SINGLE_STICK_ARCADE_L:
                fwd = drive_exponential_scale_ ? exponentialScale(calc_axis.l_y) : drive_sin_scale_ ? sinScale(calc_axis.l_y) : calc_axis.l_y;
                turn = drive_exponential_scale_ ? exponentialScale(calc_axis.l_x) : drive_sin_scale_ ? sinScale(calc_axis.l_x) : calc_axis.l_x;
                right = fwd + turn;
                left = fwd - turn;
            case drive_mode_e::SPLIT_ARCADE_PR:
                fwd = drive_exponential_scale_ ? exponentialScale(calc_axis.r_y) : drive_sin_scale_ ? sinScale(calc_axis.r_y) : calc_axis.r_y;
                turn = drive_exponential_scale_ ? exponentialScale(calc_axis.l_x) : drive_sin_scale_ ? sinScale(calc_axis.l_x) : calc_axis.l_x;
                right = fwd + turn;
                left = fwd;
            case drive_mode_e::SPLIT_ARCADE_PL:
                fwd = drive_exponential_scale_ ? exponentialScale(calc_axis.l_y) : drive_sin_scale_ ? sinScale(calc_axis.l_y) : calc_axis.l_y;
                turn = drive_exponential_scale_ ? exponentialScale(calc_axis.r_x) : drive_sin_scale_ ? sinScale(calc_axis.r_x) : calc_axis.r_x;
                right = fwd;
                left = fwd + turn;
            case drive_mode_e::CUSTOM_m:
                // Add custom drive configuration here
            default:
                throw invalid_argument("Invalid drive mode for tank drive config");
                break;
        }

        // Apply straight drive scale
        right = right * straight_r_scale_;
        left = left * straight_l_scale_;
    } else if (drive_config_ == drive_config_e::HOLONOMIC) {
        switch (drive_mode_) {
            case drive_mode_e::HOLONOMIC_SR:
                // Add holonomic drive configuration
            case drive_mode_e::HOLONOMIC_SL:
                // Add holonomic drive configuration
            case drive_mode_e::FIELD_CENTRIC_SR:
                // Add field centric drive configuration
            case drive_mode_e::FIELD_CENTRIC_SL:
                // Add field centric drive configuration
            case drive_mode_e::CUSTOM_m:
                // Add custom drive configuration here
            default:
                throw invalid_argument("Invalid drive mode for holonomic drive config");
                break;
        }
    } else if (drive_config_ == drive_config_e::CUSTOM_c) {
        // Add custom drive configuration
    }
}

double drive::normalizeAxis(double axis) {
    return axis/127.0;
}

double drive::exponentialScale(double axis) {
    return (axis/fabs(axis)) * pow(fabs(normalizeAxis(axis)), drive_exponential_scale_) * 127.0;
}

double drive::sinScale(double axis) {
    return (axis/fabs(axis)) * pow(sin(M_PI_2 * fabs(normalizeAxis(axis))), drive_sin_scale_) * 127.0;
}

drive_builder::drive_builder(ctrler &ctrler_1) {
    checkSum[0] = 0b00000001; // Used for required parameters 
    checkSum[1] = 0b00000001; // Used for selected drive scale parameters

    drive_.drive_ctrler_ = &ctrler_1;    
}
drive_builder &drive_builder::with_drive_config(drive::drive_config_e drive_config) {
    drive_.drive_config_ = drive_config;
    checkSum[0]<<=1; // Shift left by 1    

    return *this;
}
drive_builder &drive_builder::with_drive_motors(initializer_list<motor*> l_motors, initializer_list<motor*> r_motors) {
    if (l_motors.size() != r_motors.size()) {
        throw invalid_argument("Left and right motor arrays must be the same size");
    }
    drive_.drive_motor_count_ = l_motors.size();
    auto l_it = l_motors.begin(); 
    auto r_it = r_motors.begin();
    for (int i = 0; i < l_motors.size(); i++, l_it++, r_it++) {
        drive_.driveMotors[i] = *l_it;
        drive_.driveMotors[i + l_motors.size()] = *r_it;
    }
    checkSum[0]<<=1; // Shift left by 1

    return *this;
}
drive_builder &drive_builder::with_drive_motors(motor_g &motor_g_1, motor_g &motor_g_2) {
    drive_.left_side_ = &motor_g_1;
    drive_.right_side_ = &motor_g_2;
    vector<int8_t> l_motors = motor_g_1.get_port_all();
    vector<int8_t> r_motors = motor_g_2.get_port_all();
    if (l_motors.size() != r_motors.size()) {
        throw invalid_argument("Left and right motor arrays must be the same size");
    }
    drive_.drive_motor_count_ = l_motors.size();
    for (int i = 0; i < l_motors.size(); i++) {
        drive_.driveMotors[i] = new motor(l_motors[i]);
        drive_.driveMotors[i + l_motors.size()] = new motor(r_motors[i]);
    }
    checkSum[0]<<=1; // Shift left by 1   

    return *this;
}
drive_builder &drive_builder::with_drive_mode(drive::drive_mode_e drive_mode) {
    drive_.drive_mode_ = drive_mode;
    checkSum[0]<<=1; // Shift left by 1

    return *this;
}

drive_builder &drive_builder::add_max_rpm(int rpm) {
    drive_.max_rpm_ = abs(rpm);

    return *this;
}
drive_builder &drive_builder::add_ctrler_deadzone(short int deadzone) {
    if (deadzone < 0 || deadzone > 10) {
        throw domain_error("Controller deadzone must be between 0 and 10");
    }
    
    drive_.drive_deadzone_ = abs(deadzone);

    return *this;
}
drive_builder &drive_builder::add_straight_drive_scale(double l_scale, double r_scale) {
    drive_.straight_l_scale_ = fabs(l_scale);
    drive_.straight_r_scale_ = fabs(r_scale);

    return *this;
}
drive_builder &drive_builder::add_exponetial_drive_scale(double scale) {
    if (scale < 0.3 || scale > 3) {
        throw domain_error("Exponential scale must be between 0.3 and 3");
    }
    drive_.drive_exponential_scale_ = fabs(scale);

    checkSum[1]<<=1; // Shift left by 1

    return *this;
}
drive_builder &drive_builder::add_sin_drive_scale(double scale) {
    if (scale < 0.3 || scale > 3) {
        throw domain_error("Sin scale must be between 0.3 and 3");
    }
    drive_.drive_sin_scale_ = fabs(scale);
    checkSum[1]<<=1; // Shift left by 1

    return *this;
}
drive_builder &drive_builder::add_odom_config(/** @todo Need Odom class */) {
    // Awaiting Odom class

    return *this;
}

drive &drive_builder::build() {
    if (checkSum[0] != 0b00001000) {
        throw invalid_argument("Missing required parameters");
    }
    if (checkSum[1] != 0b00000001 || checkSum[1] != 0b00000010) {
        throw invalid_argument("Multiple drive scale parameters set, only select one");
    }

    // If in tank config, create motor_g objects to ease control of left and right side
    if (drive_.drive_config_ == drive::drive_config_e::TANK_c) {
        drive_.left_side_ = new motor_g({drive_.driveMotors[0]->get_port(), drive_.driveMotors[1]->get_port(), 
            drive_.driveMotors[2]->get_port(), drive_.driveMotors[3]->get_port()}, drive_.driveMotors[0]->get_gearing());
        drive_.right_side_ = new motor_g({drive_.driveMotors[4]->get_port(), drive_.driveMotors[5]->get_port(),
            drive_.driveMotors[6]->get_port(), drive_.driveMotors[7]->get_port()}, drive_.driveMotors[4]->get_gearing());
    }

    return drive_;
}
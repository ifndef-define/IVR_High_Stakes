#include "common/drive_parent.h"

// Static variables //
Drive *Drive::instance_;
motor_g *Drive::left_side_;
motor_g *Drive::right_side_;
odom *Drive::odom_;
PID *Drive::drive_pid;
PID *Drive::turn_pid;
bool Drive::motionInProgress;
bool Drive::motionQueued;
Drive::drive_mode_e Drive::drive_mode_;
Drive::drive_config_e Drive::drive_config_;
Drive::ctrler_axis_s Drive::raw_axis;
Drive::ctrler_axis_s Drive::calc_axis;
controller *Drive::drive_ctrler_;
motor *Drive::driveMotors[8];
pros::Task *Drive::drive_task;
pros::Mutex Drive::multiDrive_mutex;

void Drive::driveLoop() {
    // High Stakes Specific, prevent multiple threads from running
    Drive::multiDrive_mutex.take();
    do {
        updateAxis();

        switch (drive_config_) {
            case drive_config_e::TANK_c:
                right_side_->move_voltage((int)((right/127.0) * 12000.0));
                left_side_->move_voltage((int)((left/127.0) * 12000.0));
                break;
            case drive_config_e::HOLONOMIC:
                // Add holonomic drive control
                break;
            case drive_config_e::CUSTOM_c:
                // Add custom drive control
                break;
            default:
                errorMsg(DRIVE_LOOP_FAIL);
                break;
        }

        delay(10);
    } while (isThread);
    Drive::multiDrive_mutex.give();
}

void Drive::loop(bool thread) {
    isThread = thread;
    if (isThread) {
        if (Drive::drive_task == nullptr)
            drive_task = new pros::Task(driveLoop);
    } else {
        if (Drive::drive_task == nullptr)
            driveLoop();
    }
}

void Drive::errorMsg(int err_num) {
    int i = 0;
    string error_msg;
    if (err_num <= 7)
        error_msg = "-";
    else
        error_msg = "--";
    
        for (int dash = 0; dash < err_num-7; dash++) {
        error_msg.append(".");
    }
    pros::Task errorLoop([&]() {
        while (1) {
            if (i%2==0)
                drive_ctrler_->rumble(error_msg.c_str());
            i++;

            delay(2500);
        }
    });
}

void Drive::stopLoop() {
    brake();

    if (Drive::drive_task != nullptr) {
        Drive::drive_task->remove();
        Drive::drive_task = nullptr;
        isThread = false;
    }
}

void Drive::requestMotionStart() {
    if (motionInProgress) 
        motionQueued = true; // indicate a motion is queued
    else 
        motionInProgress = true; // indicate a motion is running

    // effectivly makes a queue
    multiDrive_mutex.take();
}

void Drive::endMotion() {
    motionInProgress = motionQueued;
    motionQueued = false;   

    multiDrive_mutex.give();
}

void Drive::cancelMotion() {
    motionInProgress = false;
    pros::delay(10);
}

void Drive::cancelAllMotions() {
    motionInProgress = false;
    motionQueued = false;
    pros::delay(10);
}

void Drive::moveAtRPM(int rpm) {
    left_side_->move_velocity(rpm);
    right_side_->move_velocity(rpm);
}

void Drive::turnByPID(double angle, int timeout, double turn_settle_error, double turn_max_voltage, bool async) {
    if(async) {
        pros::Task task([&](){ turnByPID(angle, timeout, turn_settle_error, turn_max_voltage, false); });
        pros::delay(10); // delay to give the task time to start
        return;
    }
    requestMotionStart();
    if (!motionInProgress) return;
    int start_time = pros::millis();
    
    double turnError = reduce_negative_180_to_180(angle - odom::getPos().theta);
    double output = 9;
    std::pair<double, double> rpm = getRPM();
    while(motionInProgress && !isDone(start_time, timeout)) {
        if(abs(output) < 5 && abs(turnError) < turn_settle_error && ((rpm.first+rpm.second)/2) < 10) 
            break;

        turnError = reduce_negative_180_to_180(angle - odom::getPos().theta);
        // lcd::print(0, "od: %f", odom::getPos().theta);
        // lcd::print(1, "TE: %f", turnError);
        output = turn_pid->update(turnError);
        // lcd::print(2, "TOi: %f", output);
        output = clamp(output, -turn_max_voltage, turn_max_voltage);
        // lcd::print(3, "TOf: %f", output);
        left_side_->move(-output);
        right_side_->move(output);
        pros::delay(10);
    }
    brake();
    endMotion();
}

void Drive::moveByPID(double distance, double angle, int timeout, double drive_settle_error, double turn_settle_error, double drive_max_voltage, double heading_max_voltage, bool async) {
    requestMotionStart();
    if (!motionInProgress) return;
    if(async) {
        pros::Task task([&](){ moveByPID(distance, angle, timeout, drive_settle_error, turn_settle_error, drive_max_voltage, heading_max_voltage, false); });
        endMotion();
        pros::delay(10); // delay to give the task time to start
        return;
    }
    int start_time = pros::millis();
    
    double heading_error = reduce_negative_180_to_180(angle - odom::getPos().theta);
    double drive_error = distance;
    double start_position = odom_->getPos().x * cos(to_rad(angle)) + 
                            odom_->getPos().y * sin(to_rad(angle));
lcd::print(0, "sp: %f", start_position);
    float drive_output = 6;
    float heading_output = 6;
    float cur_position = 0;
    std::pair<double, double> rpm = getRPM();
    while (motionInProgress && !isDone(start_time, timeout)) {
        if(abs(drive_output) < 5 && abs(heading_error) < turn_settle_error && 
        abs(drive_error) < drive_settle_error && ((rpm.first+rpm.second)/2) < 10){
            break;
        }
        
        cur_position = odom_->getPos().x * cos(to_rad(angle)) + 
                        odom_->getPos().y * sin(to_rad(angle));
lcd::print(1, "cp: %f", cur_position);
        drive_error = distance+start_position-cur_position;
lcd::print(2, "de: %f", drive_error);
        heading_error = reduce_negative_180_to_180(angle - odom::getPos().theta);
// lcd::print(3, "he: %f", heading_error);
        drive_output = drive_pid->update(drive_error);
        heading_output = turn_pid->update(heading_error);
lcd::print(4, "do: %f", drive_output);
// lcd::print(5, "ho: %f", heading_output);
        drive_output = clamp(drive_output, -drive_max_voltage, drive_max_voltage);
        heading_output = clamp(heading_output, -heading_max_voltage, heading_max_voltage);
    
        // left_side_->move(drive_output-heading_output);
        // right_side_->move(drive_output+heading_output);
        pros::delay(10);
    }
    brake();
    endMotion();
}

void Drive::moveByPID(double x, double y, double angle, int timeout, double drive_settle_error, double turn_settle_error, double lead, double setback, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage, bool async) {
    requestMotionStart();
    if (!motionInProgress) return;
    if(async) {
        pros::Task task([&](){ moveByPID(x, y, angle, timeout, drive_settle_error, turn_settle_error, lead, setback, drive_min_voltage, drive_max_voltage, heading_max_voltage, false); });
        endMotion();
        pros::delay(10); // delay to give the task time to start
        return;
    }
    bool line_settled = 0;
    bool prev_line_settled = is_line_settled(x, y, angle, odom::getPos().x, odom::getPos().y);
    bool crossed_center_line = false;
    bool center_line_side = is_line_settled(x, y, angle+90, odom::getPos().x, odom::getPos().y);
    bool prev_center_line_side = center_line_side;
    float target_distance = hypot(x-odom::getPos().x,y-odom::getPos().y);
    float carrot_X = x - sin(to_rad(angle)) * (lead * target_distance + setback);
    float carrot_Y = y - cos(to_rad(angle)) * (lead * target_distance + setback);
    float drive_error = hypot(carrot_X-odom::getPos().x,carrot_Y-odom::getPos().y);
    float heading_error = reduce_negative_180_to_180(to_deg(atan2(carrot_X-odom::getPos().x,carrot_Y-odom::getPos().y))-odom::getPos().theta);
    float drive_output = 0;
    float heading_output = 0;
    float heading_scale_factor = 0;
    int start_time = pros::millis();
    while(motionInProgress && !isDone(start_time, timeout)) {
        if(abs(drive_output) < 5 && abs(heading_error) && abs(heading_output) < 5 
        && abs(drive_error) < 0.25 && ((getRPM().first+getRPM().second)/2) < 10){
            break;
        }

        line_settled = is_line_settled(x, y, angle, odom::getPos().x, odom::getPos().y);
        if(line_settled && !prev_line_settled){ break; }
        prev_line_settled = line_settled;

        center_line_side = is_line_settled(x, y, angle+90, odom::getPos().x, odom::getPos().y);
        if(center_line_side != prev_center_line_side){
            crossed_center_line = true;
        }

        target_distance = hypot(x-odom::getPos().x,y-odom::getPos().y);

        carrot_X = x - sin(to_rad(angle)) * (lead * target_distance + setback);
        carrot_Y = y - cos(to_rad(angle)) * (lead * target_distance + setback);

        drive_error = hypot(carrot_X-odom::getPos().x,carrot_Y-odom::getPos().y);
        heading_error = reduce_negative_180_to_180(to_deg(atan2(carrot_X-odom::getPos().x,carrot_Y-odom::getPos().y))-odom::getPos().theta);

        if (drive_error<.25 || crossed_center_line || drive_error < setback) { 
            heading_error = reduce_negative_180_to_180(angle-odom::getPos().theta); 
            drive_error = target_distance;
        }
        
        drive_output = drive_pid->update(drive_error);

        heading_scale_factor = cos(to_rad(heading_error));
        drive_output*=heading_scale_factor;
        heading_error = reduce_negative_90_to_90(heading_error);
        heading_output = turn_pid->update(heading_error);

        drive_output = clamp(drive_output, -fabs(heading_scale_factor)*drive_max_voltage, fabs(heading_scale_factor)*drive_max_voltage);
        heading_output = clamp(heading_output, -heading_max_voltage, heading_max_voltage);

        drive_output = clamp_min_voltage(drive_output, drive_min_voltage);

        left_side_->move(left_voltage_scaling(drive_output, heading_output));
        right_side_->move(right_voltage_scaling(drive_output, heading_output));
        pros::delay(10);
    }
    brake();
    endMotion();
}

void Drive::turnAtRPM(int rpm) {
    left_side_->move_velocity(rpm);
    right_side_->move_velocity(-rpm);
}

std::pair<double, double> Drive::getRPM() {
    std::pair<double, double> rpm = {0, 0};
    double avg_rpm = 0;
    for (auto motor : left_side_->get_actual_velocity_all()) {
        avg_rpm += fabs(motor);
        avg_rpm /= drive_motor_count_;
    }
    rpm.first = avg_rpm;
    for (auto motor : right_side_->get_actual_velocity_all()) {
        avg_rpm += fabs(motor);
        avg_rpm /= drive_motor_count_;
    }
    rpm.second = avg_rpm;
    return rpm;
}

void Drive::brake() {    
    if (left_side_ != nullptr && right_side_ != nullptr) {
        left_side_->brake();
        right_side_->brake();

        return;
    }

    // for (int i = 0; i < drive_motor_count_; i++) {
    //     if (driveMotors[i] != nullptr)
    //         driveMotors[i]->brake();
    // }
}

void Drive::changeDriveMotors(motor_g &motor_g_1, motor_g &motor_g_2) {
    left_side_ = &motor_g_1;
    right_side_ = &motor_g_2;
}

void Drive::setBrakeMode(pros::motor_brake_mode_e mode) {
    left_side_->set_brake_mode_all(mode);
    right_side_->set_brake_mode_all(mode);
}

void Drive::updateAxis() {
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
                if (drive_exponential_scale_ != 0 && drive_sin_scale_ == 0) {
                    right = exponentialScale(calc_axis.r_y);
                    left = exponentialScale(calc_axis.l_y);
                } else if (drive_exponential_scale_ == 0 && drive_sin_scale_ != 0) {
                    right = sinScale(calc_axis.r_y);
                    left = sinScale(calc_axis.l_y);
                } else {
                    right = calc_axis.r_y;
                    left = calc_axis.l_y;
                }

                break;
            case drive_mode_e::SINGLE_STICK_ARCADE_R:
                if (drive_exponential_scale_ != 0 && drive_sin_scale_ == 0) {
                    fwd = exponentialScale(calc_axis.r_y);
                    turn = exponentialScale(calc_axis.r_x);
                } else if (drive_exponential_scale_ == 0 && drive_sin_scale_ != 0) {
                    fwd = sinScale(calc_axis.r_y);
                    turn = sinScale(calc_axis.r_x);
                } else {
                    fwd = calc_axis.r_y;
                    turn = calc_axis.r_x;
                }

                right = (straight_r_scale_ * fwd) - turn;
                left = (straight_l_scale_ * fwd) + turn;
                break;
            case drive_mode_e::SINGLE_STICK_ARCADE_L:
                if (drive_exponential_scale_ != 0 && drive_sin_scale_ == 0) {
                    fwd = exponentialScale(calc_axis.l_y);
                    turn = exponentialScale(calc_axis.l_x);
                } else if (drive_exponential_scale_ == 0 && drive_sin_scale_ != 0) {
                    fwd = sinScale(calc_axis.l_y);
                    turn = sinScale(calc_axis.l_x);
                } else {
                    fwd = calc_axis.l_y;
                    turn = calc_axis.l_x;
                }

                right = (straight_r_scale_ * fwd) - turn;
                left = (straight_l_scale_ * fwd) + turn;
                break;
            case drive_mode_e::SPLIT_ARCADE_PR:
                if (drive_exponential_scale_ != 0 && drive_sin_scale_ == 0) {
                    fwd = exponentialScale(calc_axis.r_y);
                    turn = exponentialScale(calc_axis.l_x);
                } else if (drive_exponential_scale_ == 0 && drive_sin_scale_ != 0) {
                    fwd = sinScale(calc_axis.r_y);
                    turn = sinScale(calc_axis.l_x);
                } else {
                    fwd = calc_axis.r_y;
                    turn = calc_axis.l_x;
                }

                right = (straight_r_scale_ * fwd) - turn;
                left = (straight_l_scale_ * fwd) + turn;
                break;
            case drive_mode_e::SPLIT_ARCADE_PL:
                if (drive_exponential_scale_ != 0 && drive_sin_scale_ == 0) {
                    fwd = exponentialScale(calc_axis.l_y);
                    turn = exponentialScale(calc_axis.r_x);
                } else if (drive_exponential_scale_ == 0 && drive_sin_scale_ != 0) {
                    fwd = sinScale(calc_axis.l_y);
                    turn = sinScale(calc_axis.r_x);
                } else {
                    fwd = calc_axis.l_y;
                    turn = calc_axis.r_x;
                }

                right = (straight_r_scale_ * fwd) - turn;
                left = (straight_l_scale_ * fwd) + turn;
                break;
            case drive_mode_e::CUSTOM_m:
                fwd = calc_axis.l_y;
                turn = 0;

                right = left = fwd;
                break;
            default:
                errorMsg(UPDATE_AXIS_BAD_DRIVE_MODE_TANK);
                break;
        }
    } else if (drive_config_ == drive_config_e::HOLONOMIC) {
        switch (drive_mode_) {
            case drive_mode_e::HOLONOMIC_SR:
                // Add holonomic drive configuration
                break;
            case drive_mode_e::HOLONOMIC_SL:
                // Add holonomic drive configuration
                break;
            case drive_mode_e::FIELD_CENTRIC_SR:
                // Add field centric drive configuration
                break;
            case drive_mode_e::FIELD_CENTRIC_SL:
                // Add field centric drive configuration
                break;
            case drive_mode_e::CUSTOM_m:
                // Add custom drive configuration here
                break;
            default:
                errorMsg(UPDATE_AXIS_BAD_DRIVE_MODE_HOLONOMIC);
                break;
        }
    } else if (drive_config_ == drive_config_e::CUSTOM_c) {
        // Add custom drive configuration
    }

    // lcd::print(0, "Raw Axis: %d %d %d %d", raw_axis.l_x, raw_axis.l_y, raw_axis.r_x, raw_axis.r_y);
    // lcd::print(1, "Calc Axis: %d %d %d %d", calc_axis.l_x, calc_axis.l_y, calc_axis.r_x, calc_axis.r_y);
    // lcd::print(2, "Left: %d Right: %d", left, right);
}

double Drive::normalizeAxis(double axis) {
    return axis/127.0;
}

int Drive::exponentialScale(int axis) {
    return (axis/abs(axis)) * pow(fabs(normalizeAxis(axis)), drive_exponential_scale_) * 127.0;
}

int Drive::sinScale(int axis) {
    return (axis/abs(axis)) * pow(sin(M_PI_2 * fabs(normalizeAxis(axis))), drive_sin_scale_) * 127.0;
}

void Drive::changeDriveMode(drive_mode_e drive_mode) {
    drive_mode_ = drive_mode;
}

drive_builder::drive_builder(controller &ctrler_1) {
    checkSum[0] = 0b00000001; // Used for required parameters 
    checkSum[1] = 0b00000001; // Used for selected drive scale parameters

    drive_ = new Drive();

    drive_->drive_ctrler_ = &ctrler_1;    
}
drive_builder &drive_builder::with_drive_config(Drive::drive_config_e drive_config) {
    drive_->drive_config_ = drive_config;

    checkSum[0]<<=1; // Shift left by 1    

    return *this;
}
drive_builder &drive_builder::with_drive_motors(vector<int8_t> l_motors, vector<int8_t> r_motors, pros::MotorGears gearing) {
    if (l_motors.size() != r_motors.size()) {
        drive_->errorMsg(Drive::drive_error::BUILDER_UNBALANCED_MOTOR_ARRAYS);
    }
    drive_->drive_motor_count_ = l_motors.size();

    auto l_it = l_motors.begin();
    auto r_it = r_motors.begin();

    drive_->left_side_ = new motor_g(l_motors, gearing);
    drive_->right_side_ = new motor_g(r_motors, gearing);

    checkSum[0]<<=1; // Shift left by 1

    return *this;
}
drive_builder &drive_builder::with_drive_motors(motor_g &motor_g_1, motor_g &motor_g_2) {
    drive_->left_side_ = &motor_g_1;
    drive_->right_side_ = &motor_g_2;
    vector<int8_t> l_motors = motor_g_1.get_port_all();
    vector<int8_t> r_motors = motor_g_2.get_port_all();
    if (l_motors.size() != r_motors.size()) {
        drive_->errorMsg(Drive::drive_error::BUILDER_UNBALANCED_MOTOR_ARRAYS);
    }
    drive_->drive_motor_count_ = l_motors.size();
    // for (int i = 0; i < l_motors.size(); i++) {
    //     drive_->driveMotors[i] = new motor(l_motors[i]);
    //     drive_->driveMotors[i + l_motors.size()] = new motor(r_motors[i]);
    // }
    checkSum[0]<<=1; // Shift left by 1   

    return *this;
}
drive_builder &drive_builder::with_drive_mode(Drive::drive_mode_e drive_mode) {
    drive_->drive_mode_ = drive_mode;
    checkSum[0]<<=1; // Shift left by 1

    return *this;
}

drive_builder &drive_builder::add_max_rpm(int rpm) {
    drive_->max_rpm_ = abs(rpm);

    return *this;
}
drive_builder &drive_builder::add_ctrler_deadzone(short int deadzone) {
    if (deadzone < 0 || deadzone > 10) {
        drive_->errorMsg(Drive::drive_error::BUILDER_LARGE_CTRLER_DEADZONE);
    }
    
    drive_->drive_deadzone_ = abs(deadzone);

    return *this;
}
drive_builder &drive_builder::add_straight_drive_scale(double l_scale, double r_scale) {
    drive_->straight_l_scale_ = fabs(l_scale);
    drive_->straight_r_scale_ = fabs(r_scale);

    if (drive_->straight_l_scale_ < 0.7 || drive_->straight_r_scale_ < 0.7) {
        drive_->errorMsg(Drive::drive_error::BUILDER_OUT_OF_BOUNDS_STRAIGHT_SCALE);
    }

    return *this;
}
drive_builder &drive_builder::add_exponetial_drive_scale(double scale) {
    if (scale < 0.3 || scale > 3) {
        drive_->errorMsg(Drive::drive_error::BUILDER_OUT_OF_BOUNDS_EXP_SCALE);
    }
    drive_->drive_exponential_scale_ = fabs(scale);
    drive_->drive_sin_scale_ = 0;

    checkSum[1]<<=1; // Shift left by 1

    return *this;
}
drive_builder &drive_builder::add_sin_drive_scale(double scale) {
    if (scale < 0.3 || scale > 3) {
        drive_->errorMsg(Drive::drive_error::BUILDER_OUT_OF_BOUNDS_SIN_SCALE);
    }
    drive_->drive_sin_scale_ = fabs(scale);
    drive_->drive_exponential_scale_ = 0;

    checkSum[1]<<=1; // Shift left by 1

    return *this;
}
drive_builder &drive_builder::add_odom_config(odom &robot_odom) {
    drive_->odom_ = &robot_odom;

    return *this;
}

drive_builder &drive_builder::add_lat_pid(PID &lat_pid) {
    drive_->drive_pid = &lat_pid;

    return *this;
}

drive_builder &drive_builder::add_turn_pid(PID &turn_pid) {
    drive_->turn_pid = &turn_pid;

    return *this;
}

Drive *drive_builder::build() {
    if (checkSum[0] != 0b00001000) {
        drive_->errorMsg(Drive::drive_error::BUILDER_MISSING_REQUIRED_PARAMS);
    }
    if (!(checkSum[1] == 0b00000001 || checkSum[1] == 0b00000010)) {
        drive_->errorMsg(Drive::drive_error::BUILDER_MULTIPLE_DRIVE_SCALES);
    }

    /** @todo Need to allow for varying number of drive motors to be correctly assigned to the group */
    // If in tank config, create motor_g objects to ease control of left and right side
    // if (drive_->drive_config_ == Drive::drive_config_e::TANK_c) {
    //     drive_->left_side_ = new motor_g({drive_->driveMotors[0]->get_port(), drive_->driveMotors[1]->get_port(), 
    //         drive_->driveMotors[2]->get_port(), drive_->driveMotors[3]->get_port()}, drive_->driveMotors[0]->get_gearing());
    //     drive_->right_side_ = new motor_g({drive_->driveMotors[4]->get_port(), drive_->driveMotors[5]->get_port(),
    //         drive_->driveMotors[6]->get_port(), drive_->driveMotors[7]->get_port()}, drive_->driveMotors[4]->get_gearing());
    // }

    if(drive_->instance_ != nullptr) {
        drive_->errorMsg(Drive::drive_error::MULTIPLE_DRIVE_OBJECTS);
    }

    drive_->instance_ = drive_;

    return drive_;
}
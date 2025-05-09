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
    isThread = false;
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

// void Drive::cancelMotion() {
//     motionInProgress = false;
//     pros::delay(10);
// }

void Drive::cancelAllMotions() {
    motionInProgress = false;
    motionQueued = false;
    pros::delay(10);
}

bool Drive::isInMotion() { return motionInProgress; }

void Drive::moveAtRPM(int rpm) {
    left_side_->move_velocity(rpm);
    right_side_->move_velocity(rpm);
}

const bool debug_turnToAngle = false;
void Drive::turnToAngle(double angle, int timeout, bool async, double turn_max_voltage, double turn_settle_error) {
    requestMotionStart();
    if (!motionInProgress) return;
    if(async) {
        pros::Task task([&](){ turnToAngle(angle, timeout, false, turn_max_voltage, turn_settle_error); });
        endMotion();
        pros::delay(10); // delay to give the task time to start
        return;
    }
    int start_time = pros::millis();
    
    double turn_error = reduce_negative_180_to_180(angle - odom::getPos().theta);
    double output = 9;
    std::pair<double, double> rpm = getRPM();

    while(motionInProgress && !isDone(start_time, timeout)) {
        if(abs(output) < 5 && abs(turn_error) < turn_settle_error) 
            break;

                    if (debug_turnToAngle) { lcd::print(0, "od: %.2f", odom::getPos().theta); }
        turn_error = reduce_negative_180_to_180(angle - odom::getPos().theta);
                    if (debug_turnToAngle) { lcd::print(1, "TE: %.2f", turn_error); } 
        output = turn_pid->update(turn_error);
                    if (debug_turnToAngle) { lcd::print(2, "TOi: %.2f", output); }
        output = clamp(output, -turn_max_voltage, turn_max_voltage);
                    if (debug_turnToAngle) { lcd::print(3, "TOf: %.2f", output); }
        left_side_->move(-output);
        right_side_->move(output);
                    if (debug_turnToAngle) { lcd::print(4, "L: %.2f | R: %.2f", -output, output); }
        pros::delay(10);
    }
    brake();
    endMotion();
}

const bool debug_swingToAngle = false;
void Drive::swingToAngle(double angle, Drive::DriveSide lockedSide, int timeout, bool async, double turn_max_voltage, double turn_settle_error) {
    requestMotionStart();
    if (!motionInProgress) return;
    if(async) {
        pros::Task task([&](){ swingToAngle(angle, lockedSide, timeout, false, turn_max_voltage, turn_settle_error); });
        endMotion();
        pros::delay(10); // delay to give the task time to start
        return;
    }
    int start_time = pros::millis();

    pros::MotorBrake og_brake_mode = (lockedSide == DriveSide::LEFT) ? left_side_->get_brake_mode() : right_side_->get_brake_mode();
    if (lockedSide == DriveSide::LEFT) {
        left_side_->set_brake_mode(BRAKE_HOLD);
    } else {
        right_side_->set_brake_mode(BRAKE_HOLD);
    }
                if (debug_swingToAngle) { lcd::print(0, "locked: %s", lockedSide == DriveSide::LEFT ? "Left" : "Right"); }
    double heading_error = reduce_negative_180_to_180(angle - odom::getPos().theta);
    double output = 6;
    std::pair<double, double> rpm = getRPM();
    
    while(motionInProgress && !isDone(start_time, timeout)) {
        if(abs(output) < 10 && abs(heading_error) < turn_settle_error && ((rpm.first+rpm.second)/2) < 10) 
            break;

        heading_error = reduce_negative_180_to_180(angle - odom::getPos().theta);
                    if (debug_swingToAngle) { lcd::print(1, "TE: %.2f", heading_error); }
        output = turn_pid->update(heading_error);
                    if (debug_swingToAngle) { lcd::print(2, "TOi: %.2f", output); }
        output = clamp(output, -turn_max_voltage, turn_max_voltage);
                    if (debug_swingToAngle) { lcd::print(3, "TOf: %.2f", output); }
        if(lockedSide == DriveSide::LEFT) {
            left_side_->brake();
            right_side_->move(output);
                    if (debug_swingToAngle) { lcd::print(4, "L: %.2f | R: %.2f", 0.0, output); }
        } else {
            left_side_->move(-output);
            right_side_->brake();
                    if (debug_swingToAngle) { lcd::print(4, "L: %.2f | R: %.2f", -output, 0.0); }
        }
        pros::delay(10);
    }
    brake();

    if (lockedSide == DriveSide::LEFT) {
        left_side_->set_brake_mode(og_brake_mode);
    } else {
        right_side_->set_brake_mode(og_brake_mode);
    }

    brake(); // cycle through the brake mode to set it back to the original mode
    endMotion();
}

void Drive::turnToPoint(double x, double y, int timeout, bool async, double turn_max_voltage, double turn_settle_error) {
    turnToAngle(to_deg(atan2(x-odom_->getPos().x, y-odom_->getPos().y)), timeout, async, turn_max_voltage, turn_settle_error);
}

void Drive::swingToPoint(double x, double y, Drive::DriveSide lockedSide, int timeout, bool async, double turn_max_voltage, double turn_settle_error) {
    swingToAngle(to_deg(atan2(x-odom_->getPos().x, y-odom_->getPos().y)), lockedSide, timeout, async, turn_max_voltage, turn_settle_error);
}

const double tb_kV = 0.15;
const double tb_kA = 0.002;
const double tb_a_des = 20;
const double tb_mVps_decrease = 15;
const double tb_mVps_increase = 35;
const double tb_ignore_slew = 1.5;
const double tb_maintain_angle_voltage = 60;
const double tb_turn_settle_error = 0.25;
const bool debug_translateBy = false;
void Drive::translateBy(double distance, int timeout, bool async, double drive_min_voltage, double drive_max_voltage, double drive_settle_error, double turn_settle_error) {
    requestMotionStart();
    if (!motionInProgress) return;
    if(async) {
        pros::Task task([&](){ translateBy(distance, timeout, false, drive_min_voltage, drive_max_voltage, drive_settle_error, turn_settle_error); });
        endMotion();
        pros::delay(10); // delay to give the task time to start
        return;
    }
    int start_time = pros::millis();

    double initial_heading = odom::getPos().theta;
    double heading_error = 0;
    double drive_error = distance;
    double start_position = odom_->getPos().x * cos(to_rad(initial_heading)) + 
                            odom_->getPos().y * sin(to_rad(initial_heading));
                if (debug_translateBy) { lcd::print(1, "start_pos: %f", start_position); }
    float drive_output = 0;
    float heading_output = 0;
    float cur_position = 0;
    std::pair<double, double> rpm = getRPM();
    double dt, previous_time, v_des, drive_ff_output, drive_cmd, prev_drive_cmd = 0;
    
    while (motionInProgress && !isDone(start_time, timeout)) {
        if(abs(drive_output) < 13 /*&& abs(heading_error) < turn_settle_error*/ && abs(drive_error) < drive_settle_error) {
            break;
        }
                    if (debug_translateBy) { lcd::print(0, "X: %.2f | Y: %.2f | T: %.2f", odom_->getPos().x, odom_->getPos().y, odom_->getPos().theta); }
        dt = (pros::millis() - previous_time) / 1000;
        previous_time = pros::millis();
        v_des = copysign( std::sqrt(2 * tb_a_des * std::fabs(drive_error)), drive_error);
        
        cur_position = odom_->getPos().x * cos(to_rad(initial_heading)) + 
                        odom_->getPos().y * sin(to_rad(initial_heading));
                    if (debug_translateBy) { lcd::print(2, "CPos: %f", cur_position); }
        drive_error = distance+start_position-cur_position;
        heading_error = reduce_negative_180_to_180(initial_heading - odom::getPos().theta);
                    if (debug_translateBy) { lcd::print(3, "DErr: %f | HErr: %f", drive_error, heading_error); }
        drive_output = drive_pid->update(drive_error);
        heading_output = turn_pid->update(heading_error);
        heading_output *= 1.5;
        if (abs(drive_error) > tb_ignore_slew) {
            drive_ff_output = tb_kV * v_des + tb_kA * (v_des - prev_drive_cmd/tb_kV) / dt;
            drive_cmd = drive_output + drive_ff_output;
            drive_cmd = std::clamp(drive_cmd,prev_drive_cmd - tb_mVps_decrease, prev_drive_cmd + tb_mVps_increase);
        } else {
            drive_cmd = drive_output;
        }
        
        // Apply minimum voltage ONLY when far from target (> 2.0 units)
        if (abs(drive_error) > 6 && abs(drive_cmd) > 0) {
            drive_cmd = copysign(std::max(abs(drive_cmd), drive_min_voltage), drive_cmd);
        }
        
        prev_drive_cmd = drive_cmd;
        heading_output = clamp(heading_output, -tb_maintain_angle_voltage, tb_maintain_angle_voltage);
                    if (debug_translateBy) { lcd::print(4, "DO: %f | HO: %f", drive_cmd, heading_output); }
        double left_side_output = drive_cmd - heading_output;
        double right_side_output = drive_cmd + heading_output;
        left_side_output = clamp(left_side_output, -drive_max_voltage, drive_max_voltage);
        right_side_output = clamp(right_side_output, -drive_max_voltage, drive_max_voltage);
                    if (debug_translateBy) { lcd::print(5, "L: %f | R: %f", left_side_output, right_side_output); }
        left_side_->move(left_side_output);
        right_side_->move(right_side_output);
        pros::delay(10);
    }
    brake();
    endMotion();
}

const double mp_kV = 0.2;
const double mp_kA = 0.0025;
const double mp_a_des = 20;
const double mp_mVps_decrease = 15;
const double mp_mVps_increase = 35;
const double mp_ignore_slew = 1.5;
const double end_angle_voltage = 60;
const bool debug_moveToPose = false;
void Drive::moveToPose(double x, double y, double theta, bool reverse, int timeout, bool async, double drive_min_voltage, double drive_max_voltage, double heading_max_voltage, double drive_settle_error, double turn_settle_error, double lead, double position_threshold) {
    requestMotionStart();
    if (!motionInProgress) return;
    if(async) {
        pros::Task task([&](){ moveToPose(x, y, theta, reverse, timeout, false, drive_min_voltage, drive_max_voltage, heading_max_voltage, drive_settle_error, turn_settle_error, lead, position_threshold); });
        endMotion();
        pros::delay(10); // delay to give the task time to start
        return;
    }

    int start_time = pros::millis();

    double straight_angle = to_deg(atan2(y - odom_->getPos().y, x - odom_->getPos().x));
    if (reverse) { straight_angle += 180; }
    double initial_distance = hypot(x - odom_->getPos().x, y - odom_->getPos().y);
    double target_distance = initial_distance;
    double heading1_Error = reduce_negative_180_to_180(straight_angle - odom_->getPos().theta);
    double heading2_Error = reduce_negative_180_to_180(theta - odom_->getPos().theta); // Final heading error
    double x_error = x - odom_->getPos().x;
    double y_error = y - odom_->getPos().y;
    double x_pid, y_pid = 0;
    double drive_output = 0;
    double heading_output = 0;
    double distance = 0;
    
    // Motion tuning parameters
    double first_turn_end = 0.37;
    double second_turn_start = 0.75;
    double dt, previous_time, v_des, drive_ff_output, drive_cmd, prev_drive_cmd, progress = 0;
    double carrotX = 0, carrotY = 0;
    bool in_final_angle_phase = false;
    while (motionInProgress && !isDone(start_time, timeout)) {
        if (abs(drive_output) < 13 && abs(heading_output) < 13 && abs(heading1_Error) < turn_settle_error && abs(target_distance) < drive_settle_error) {
            break;
        }
                    if (debug_moveToPose) { lcd::print(0, "X: %.2f, Y: %.2f, T: %.2f", odom_->getPos().x, odom_->getPos().y, odom_->getPos().theta); }
        // Update target calculations
        distance = hypot(x - odom_->getPos().x, y - odom_->getPos().y);
        x_error = x - odom_->getPos().x;
        y_error = y - odom_->getPos().y;

        // Slew rate limit
        dt = (pros::millis() - previous_time) / 1000;
        previous_time = pros::millis();
        v_des = copysign(std::sqrt(2 * mp_a_des * std::fabs(distance)), distance);

        // Calculate progress percentage (0 to 1)
        if (initial_distance > 0.1) {
            progress = 1.0 - (distance / initial_distance);
        } else {
            progress = 1.0;
        }
        progress = std::min(1.0, std::max(0.0, progress)); // Clamp between 0 and 1
                    if (debug_moveToPose) { lcd::print(2, "Dis: %.2f | Prog: %.2f", distance, progress); }

        // During initial phase: use carrot follower
        if (progress < first_turn_end) {
            double curX = odom_->getPos().x;
            double curY = odom_->getPos().y;
            carrotX = curX + cos(to_rad(straight_angle)) * lead;
            carrotY = curY + sin(to_rad(straight_angle)) * lead;
            heading1_Error = reduce_negative_180_to_180(to_deg(atan2(carrotY - curY, carrotX - curX)) - odom_->getPos().theta);
                    if (debug_moveToPose) { lcd::print(1, "Mode: Carrot"); }
        } else { // After reaching the threshold: focus on the target position headin
            heading1_Error = reduce_negative_180_to_180(straight_angle - odom_->getPos().theta);
                    if (debug_moveToPose) { lcd::print(1, "Mode: Target"); }
        }
        
        // Update heading2_Error if ending angle is provided
        if (theta != 999) {
            heading2_Error = reduce_negative_180_to_180(theta - odom_->getPos().theta);
        } else {
            heading2_Error = heading1_Error;
        }

                    if (debug_moveToPose) { lcd::print(3, "XErr: %.2f, YErr: %.2f, HErr: %.1f", x_error, y_error, in_final_angle_phase ? heading2_Error : heading1_Error); }
                    if (debug_moveToPose) { lcd::print(4, "SAng: %.1f, FAng: %.1f", straight_angle, theta); }
        
        // calculate drive and heading outputs
        x_pid = drive_pid->update(x_error);
        y_pid = drive_pid->update(y_error*1.3);
        drive_output = x_pid * cos(to_rad(straight_angle)) + y_pid * sin(to_rad(straight_angle));
        drive_output = clamp(drive_output, -drive_max_voltage, drive_max_voltage);        
        
        // Position threshold that triggers final angle transition
        bool start_early_turn = progress > 0.55 && distance < position_threshold * 2 && theta != 999;
        
        // If we're close to final position OR we've already started the final angle phase
        if ((start_early_turn && abs(x_error) < 6.0) || in_final_angle_phase) {
            in_final_angle_phase = true;

            // Closer to target = more angle priority
            double angle_priority = distance < 2.0 ? 1.0 : 0.8;
            
            // Use heading2_Error for the final heading angle
            heading_output = turn_pid->update(heading2_Error);
            
            // Reduce drive output gradually as we approach the target to avoid overshooting
            double position_factor = std::min(1.0, distance / 2.0);
            drive_output *= position_factor * (1.0 - angle_priority * 0.6);
            
            if (debug_moveToPose) { lcd::print(5, "APri: %.2f | PFact: %.2f", angle_priority, position_factor); }
        } else {
            // Use heading1_Error for path following
            heading_output = turn_pid->update(heading1_Error);
            if (debug_moveToPose) { lcd::print(5, "Target Mode"); }
        }
        
        if (abs(heading_output) < 1.5 && distance < 3.0) {
            heading_output = 0;
        }

        if (progress > first_turn_end) {
            // Apply feed-forward when not close to target
            drive_ff_output = mp_kV * v_des + mp_kA * (v_des - prev_drive_cmd/mp_kV) / dt;
            drive_cmd = drive_output + drive_ff_output;
            
            // Apply slew rate limits
            drive_cmd = std::clamp(drive_cmd, prev_drive_cmd - mp_mVps_decrease, prev_drive_cmd + mp_mVps_increase);
        } else {
            drive_cmd = drive_output;
        }

        prev_drive_cmd = drive_cmd;
        
        double left_output = drive_cmd - heading_output;
        double right_output = drive_cmd + heading_output;
        left_output = clamp(left_output, -drive_max_voltage, drive_max_voltage);
        right_output = clamp(right_output, -drive_max_voltage, drive_max_voltage);
                    if (debug_moveToPose) { lcd::print(6, "DOut: %.1f, HOut: %.1f", drive_output, heading_output); }
                    if (debug_moveToPose) { lcd::print(7, "L: %.1f | R: %.1f", left_output, right_output); }


        // Move robot
        left_side_->move(left_output);
        right_side_->move(right_output);

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

void Drive::moveByVector(double x, double y, double atSpeed) {
    // Get angle to target in radians (atan2 handles quadrant correctly)
    double angle = atan2(x, y);

    // Convert angle to unit vector components
    double unitX = cos(angle);
    double unitY = sin(angle);

    // Scale by desired speed
    double scaledX = unitX * atSpeed;
    double scaledY = unitY * atSpeed;

    // Convert to arcade drive style inputs
    // Y component controls forward/backward
    // X component controls turning
    left = scaledY - scaledX; 
    right = scaledY + scaledX;

    // Clamp values to valid range
    left = clamp(left, -127.0, 127.0);
    right = clamp(right, -127.0, 127.0);

    // Move motors
    left_side_->move_voltage((int)((left/127.0) * 12000.0));
    right_side_->move_voltage((int)((right/127.0) * 12000.0));
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
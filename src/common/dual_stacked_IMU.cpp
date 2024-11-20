#include "dual_stacked_IMU.h"
dualStackedIMU::dualStackedIMU(pros::Imu &imu1, pros::Imu &imu2){
    imu1_= imu1; imu1_active = true;
    imu2_ = imu2; imu2_active = true;
}
void dualStackedIMU::reset(bool blocking = true){
    imu1_.reset(true);
    imu2_.reset(true);
}

void dualStackedIMU::set_heading(double heading){
    imu1_.set_heading(heading);
    imu2_.set_heading(heading);
}
void dualStackedIMU::_check_active(){
    std::string active_message = "";
    active_message += (imu1_active)? "IMU1 ACTIVE, " : "IMU1 INACTIVE, ";
    active_message += (imu2_active)? "IMU2 ACTIVE" : "IMU2 INACTIVE";
    pros::lcd::print(5, active_message);
}

orientation dualStackedIMU::get(){
    double heading_ = 0;
    double pitch_ =0;
    double roll_ = 0;
    double &yaw_ = heading_;
    double count = 0;
    if(imu1_active){
        heading_ += imu1_.get_heading();
        pitch_ += imu1_.get_pitch();
        roll_+= imu1_.get_roll();
        ++count;
    }
    if(imu2_active){
        heading_ += imu2_.get_heading();
        pitch_ += imu2_.get_pitch();
        roll_+= imu2_.get_roll();
        ++count;
    }
    if(count==0){
        pros::lcd::print(6, "IMU1 INACTIVE, IMU2 INACTIVE");
        orientation result(heading_,pitch_,roll_,yaw_);
        return result;
    }
    orientation result(heading_/count,pitch_/count,roll_/count,yaw_/count);
    return result;
}

void check_IMU_difference(double heading){
    
}
// #include "common/odom.h"

// /* Measurements:
// Track Length: 8 in
// Track Width: 12 in
//  Tracking Center: 6 in, 4 in

// x Enc Offset: 3 5/8  in
// RY Enc Offset: 4.75 in
// LY Enc Offset: 4 7/8 in

// X Enc Calc Offset: 0.375
// RY Enc Calc Offset: 1.25
// LY Enc Calc Offset: 1.125

// */

// // Definitions of static members
// const double odom::WHEEL_DIAMETER = 1.36;
// const double odom::RX_ENC_OFFSET = 1.25;
// const double odom::LX_ENC_OFFSET = 1.125;
// const double odom::Y_ENC_OFFSET = 0.375;
// const double odom::TICKS_PER_ROTATION = 8192.0;
// const double odom::WHEEL_CIRCUMFERENCE = odom::WHEEL_DIAMETER * M_PI;
// const double odom::DISPLACEMENT_CONSTANT = WHEEL_CIRCUMFERENCE / TICKS_PER_ROTATION;
// // const double odom::TICKS_PER_INCH = odom::TICKS_PER_ROTATION / odom::WHEEL_CIRCUMFERENCE;
// bool odom::debugMode = true;

// pros::adi::Encoder* odom::_r_x_enc;
// pros::adi::Encoder* odom::_l_x_enc;
// pros::adi::Encoder* odom::_y_enc;
// pros::IMU* odom::_imu;

// odom::enc_coord odom::currentEnc;
// odom::enc_coord odom::lastEnc;
// odom::enc_coord odom::deltaEnc;
// odom::r_coord odom::deltaDisp;
// odom::r_coord odom::currentPos;
// odom::r_coord odom::lastPos;

// odom::odom() {
    
// }

// void odom::init(pros::adi::Encoder *r_x_enc, pros::adi::Encoder *l_x_enc, pros::adi::Encoder *y_enc, pros::IMU *imu, r_coord startPos) {
//     _r_x_enc = r_x_enc;
//     _l_x_enc = l_x_enc;
//     _y_enc = y_enc;
//     _imu = imu;

//     // Data rate of 5ms guarantees most recent position but doesn't mean it updates every 5ms
//     // Rotation sensor value only
//     // mx_enc.set_data_rate(5);
//     // my_enc.set_data_rate(5);

//     _r_x_enc->reset();
//     _l_x_enc->reset();
//     _y_enc->reset();

//     // Clear all values
//     currentEnc = {0, 0, 0, 0};
//     lastEnc = {0, 0, 0, 0};
//     deltaEnc = {0, 0, 0};
//     deltaDisp = {0, 0, 0};
//     currentPos = startPos;
//     lastPos = startPos;
// }

// void odom::update() {
//     // Get the current encoder values and heading
//     currentEnc.r_x = _r_x_enc->get_value();
//     currentEnc.l_x = _l_x_enc->get_value();
//     currentEnc.y = _y_enc->get_value();
//     currentEnc.theta = -convert::degToRad(_imu->get_rotation());

//     // Calculate the encoder value change since the last update
//     deltaEnc = currentEnc - lastEnc;
    
//     // Update last encoder values
//     lastEnc = currentEnc;

//     /**
//      * There are two ways that odom can be calculated:
//      * - Use given IMU heading
//      * - Calculate heading from encoder values
//      */

//     // Assume given IMU heading

//     // Calculate the distance traveled
//     // deltaDisp.x = (DISPLACEMENT_CONSTANT * deltaEnc.x) - X_ENC_OFFSET * convert::degToRad(deltaEnc.theta);
//     // deltaDisp.y = (DISPLACEMENT_CONSTANT * deltaEnc.l_y) - LY_ENC_OFFSET * convert::degToRad(deltaEnc.theta);
//     // deltaDisp.theta = NAN; // This is not used

//     // // Update the global position of the robot
//     // currentPos.x = lastPos.x + (deltaDisp.x * cos(convert::degToRad(deltaEnc.theta)) + (deltaDisp.y * sin(convert::degToRad(deltaEnc.theta))));
//     // currentPos.y = lastPos.y + (deltaDisp.x * sin(convert::degToRad(deltaEnc.theta)) + (deltaDisp.y * cos(convert::degToRad(deltaEnc.theta))));
//     // currentPos.theta = currentEnc.theta; // Raw IMU value is the heading

//     // Calculate the heading in Odom

//     // // Calculate the distance traveled
//     // deltaDisp.x = DISPLACEMENT_CONSTANT * (deltaEnc.l_y+deltaEnc.r_y) / 2;
//     // deltaDisp.y = DISPLACEMENT_CONSTANT * (deltaEnc.x - X_ENC_OFFSET * (deltaEnc.r_y - deltaEnc.l_y) / (1.25*2));
//     // // deltaDisp.theta = DISPLACEMENT_CONSTANT * (deltaEnc.r_y - deltaEnc.l_y) / (1.25*2);
//     // deltaDisp.theta = deltaEnc.theta;


//     // deltaDisp.x = (DISPLACEMENT_CONSTANT * (deltaEnc.l_x + deltaEnc.r_x) - (RX_ENC_OFFSET - LX_ENC_OFFSET) * deltaEnc.theta) / 2.0;
//     // deltaDisp.y = (DISPLACEMENT_CONSTANT * deltaEnc.y - Y_ENC_OFFSET * deltaEnc.theta);

//     // double delta_x = E_c * delta_n1 + L1 * delta_theta;
//     // double delta_y = E_c * delta_n3 - L3 * delta_theta;

//     deltaDisp.x = (DISPLACEMENT_CONSTANT * deltaEnc.l_x) + (LX_ENC_OFFSET * deltaEnc.theta);
//     deltaDisp.y = (DISPLACEMENT_CONSTANT * deltaEnc.y) - (Y_ENC_OFFSET * deltaEnc.theta);
//     deltaDisp.theta = deltaEnc.theta;
    
//     // Update the global position of the robot
//     currentPos.x = lastPos.x + (deltaDisp.x * cos(lastPos.theta) - (deltaDisp.y * sin(lastPos.theta)));
//     currentPos.y = lastPos.y + (deltaDisp.x * sin(lastPos.theta) + (deltaDisp.y * cos(lastPos.theta)));
//     currentPos.theta = lastPos.theta + deltaDisp.theta;

//     // deltaDist.x = ((M_PI * WHEEL_DIAMETER) / TICKS_PER_ROTATION) * deltaEnc.x;
//     // deltaDist.y = ((M_PI * WHEEL_DIAMETER) / TICKS_PER_ROTATION) * deltaEnc.y;
//     // deltaDist.theta = NAN; // This is not used

//     // Calculate the displacement of the robot since the last update
//     // displacement.x = deltaDist.x - (HORIZONTAL_OFFSET * convert::degToRad(deltaEnc.theta));
//     // displacement.y = deltaDist.y - (VERTICAL_OFFSET * convert::degToRad(deltaEnc.theta));
//     // displacement.theta = deltaEnc.theta; // These changes are the same

//     // Find field position given the displacement
//     // currentPos.x = lastPos.x + (displacement.x * cos(convert::degToRad(lastPos.theta)) - (displacement.y * sin(convert::degToRad(lastPos.theta))));
//     // currentPos.y = lastPos.y + (displacement.x * sin(convert::degToRad(lastPos.theta)) + (displacement.y * cos(convert::degToRad(lastPos.theta))));
//     // currentPos.theta = currentEnc.theta; // Raw IMU value is the heading


//     // Update last position
//     lastPos = currentPos;
// }
// /*
// 21800


// 0.785

// */

// odom::r_coord odom::getPos() {
//     return currentPos;
// }

// void odom::debug() {
//     lcd::print(0, "Odom Info:");
//     lcd::print(1, "X: %f", currentPos.x);
//     lcd::print(2, "Y: %f", currentPos.y);
//     lcd::print(3, "Theta: %f", convert::radToDeg(currentPos.theta));
//     lcd::print(4, "RX Enc: %d", _r_x_enc->get_value());
//     lcd::print(5, "LX Enc: %d", _l_x_enc->get_value());
//     lcd::print(6, "Y Enc: %d", _y_enc->get_value());
//     lcd::print(7, "Theta Enc: %f", _imu->get_rotation());
// }

// void odom::start() {
//     // pros::Task odmo{[=] {
//     // lcd::print(7, "Hello World");
//     // while(pros::millis() < 30000) {
//     while(true) {
//             update();
//             // if (debugMode) {debug();}
//             debug();
//             delay(10);
//     }
//     // }};
// }

// // void odom::stop() {
// //     _odom_task->remove();
// // }

// odom::r_coord odom::r_coord::operator+(const r_coord& inp) const {
//     return {this->x + inp.x, this->y + inp.y, this->theta + inp.theta};
// }

// odom::r_coord odom::r_coord::operator-(const r_coord& inp) const {
//     return {this->x - inp.x, this->y - inp.y, this->theta - inp.theta};
// }

// odom::enc_coord odom::enc_coord::operator+(const enc_coord& inp) const {
//     return {this->r_x + inp.r_x, this->l_x + inp.l_x, this->y + inp.y, this->theta + inp.theta};
// }

// odom::enc_coord odom::enc_coord::operator-(const enc_coord& inp) const {
//     return {this->r_x - inp.r_x, this->l_x - inp.l_x, this->y - inp.y, this->theta - inp.theta};
// }
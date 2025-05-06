#include "common/odom.h"

/* Measurements:
Track Length: 8 in
Track Width: 11 1/8 in
Tracking Center: 5 9/16 in, 4 in

Y Enc Offset: 5/8  in
X Enc Offset: 1.25 in

LY Enc Offset: 4 7/8 in

X Enc Calc Offset: -1.25
Y Enc Calc Offset: -0.625

*/

// Definitions of static members
const double odom::WHEEL_DIAMETER = 1.36;
const double odom::TICKS_PER_ROTATION = 8192.0;
const double odom::WHEEL_CIRCUMFERENCE = odom::WHEEL_DIAMETER * M_PI;
double odom::X_DISPLACEMENT_CONSTANT = WHEEL_CIRCUMFERENCE / TICKS_PER_ROTATION;
double odom::Y_DISPLACEMENT_CONSTANT = WHEEL_CIRCUMFERENCE / TICKS_PER_ROTATION;
// const double odom::TICKS_PER_INCH = odom::TICKS_PER_ROTATION / odom::WHEEL_CIRCUMFERENCE;
bool odom::debugMode = true;
bool odom::isThread = false;
pros::Task* odom::odom_task = nullptr;

double odom::X_ENC_OFFSET = 0.0;
double odom::Y_ENC_OFFSET = 0.0;

pros::adi::Encoder* odom::_x_enc;
pros::adi::Encoder* odom::_y_enc;
pros::IMU* odom::_imu;
DualIMU* odom::_dual_imu;

odom::enc_coord odom::currentEnc;
odom::enc_coord odom::lastEnc;
odom::enc_coord odom::deltaEnc;
odom::r_coord odom::deltaDisp;
odom::r_coord odom::currentPos;
odom::r_coord odom::lastPos;

odom::odom(pros::adi::Encoder *x_enc, pros::adi::Encoder *y_enc, pros::IMU *imu, r_coord startPos, double x_off, double x_tune, double y_off, double y_tune) {
    _x_enc = x_enc;
    _y_enc = y_enc;
    _imu = imu;
    _dual_imu = nullptr;
    
    if (_x_enc != nullptr && _y_enc != nullptr) {
        _x_enc->reset();
        _y_enc->reset();
    } else {
        pros::lcd::print(0, "Odom: Encoder not found!");
        debugMode = false;
        return;
    }

    X_ENC_OFFSET = x_off;
    Y_ENC_OFFSET = y_off;
    X_DISPLACEMENT_CONSTANT *= x_tune;
    Y_DISPLACEMENT_CONSTANT *= y_tune;
    
    // Clear all values
    currentEnc = {0, 0, 0};
    lastEnc = {0, 0, 0};
    deltaEnc = {0, 0, 0};
    deltaDisp = {0, 0, 0};
    currentPos = {startPos.x, startPos.y, convert::degToRad(startPos.theta)};
    lastPos = {startPos.x, startPos.y, convert::degToRad(startPos.theta)};
}

odom::odom(pros::adi::Encoder *x_enc, pros::adi::Encoder *y_enc, DualIMU *imu, r_coord startPos, double x_off, double x_tune, double y_off, double y_tune) {
    _x_enc = x_enc;
    _y_enc = y_enc;
    _imu = nullptr;
    _dual_imu = imu;
    
    if (_x_enc != nullptr && _y_enc != nullptr) {
        _x_enc->reset();
        _y_enc->reset();
    } else {
        pros::lcd::print(0, "Odom: Encoder not found!");
        debugMode = false;
        return;
    }

    X_ENC_OFFSET = x_off;
    Y_ENC_OFFSET = y_off;
    X_DISPLACEMENT_CONSTANT *= x_tune;
    Y_DISPLACEMENT_CONSTANT *= y_tune;
    
    // Clear all values
    currentEnc = {0, 0, 0};
    lastEnc = {0, 0, 0};
    deltaEnc = {0, 0, 0};
    deltaDisp = {0, 0, 0};
    currentPos = {startPos.x, startPos.y, convert::degToRad(startPos.theta)};
    lastPos = {startPos.x, startPos.y, convert::degToRad(startPos.theta)};
}

void odom::update() {
    do {
        if (_dual_imu != nullptr) {
            _dual_imu->update();
        }

        // Get the current encoder values and heading
        currentEnc.x = _x_enc->get_value();
        currentEnc.y = _y_enc->get_value();
        currentEnc.theta = -convert::degToRad(_dual_imu ? _dual_imu->get_rotation() : _imu->get_rotation());
    
        // Calculate the encoder value change since the last update
        deltaEnc = currentEnc - lastEnc;
        
        // Update last encoder values
        lastEnc = currentEnc;
    
        // Assume given IMU heading 
        // Calculate the distance traveled
        deltaDisp.x = (X_DISPLACEMENT_CONSTANT * deltaEnc.x) - X_ENC_OFFSET * deltaEnc.theta;
        deltaDisp.y = (Y_DISPLACEMENT_CONSTANT * deltaEnc.y) - Y_ENC_OFFSET * deltaEnc.theta;
        deltaDisp.theta = NAN; // This is not used
    
        // Update the global position of the robot
        currentPos.x = lastPos.x + (deltaDisp.x * cos(currentPos.theta) - (deltaDisp.y * sin(currentPos.theta)));
        currentPos.y = lastPos.y + (deltaDisp.x * sin(currentPos.theta) + (deltaDisp.y * cos(currentPos.theta)));
        currentPos.theta += deltaEnc.theta;
    
        // Update last position
        lastPos = currentPos;
        
        if (debugMode)
            debug();

        if (isThread)
            pros::delay(10); // Delay to save resources
    } while (isThread);
}

odom::r_coord odom::getPos() {
    return {currentPos.x, currentPos.y, convert::radToDeg(currentPos.theta)};
}

void odom::setPos(r_coord pos) {
    currentPos = {pos.x, pos.y, convert::degToRad(pos.theta)};
    lastPos = {pos.x, pos.y, convert::degToRad(pos.theta)};
    currentEnc = {0, 0, 0};
    lastEnc = {0, 0, 0};
    deltaEnc = {0, 0, 0};
    deltaDisp = {0, 0, 0};
}

void odom::debug() {
    // lcd::print(0, "Odom Info:");
    lcd::print(1, "X Enc: %d", _x_enc->get_value());
    lcd::print(2, "Y Enc: %d", _y_enc->get_value());
    lcd::print(3, "IMU Theta: %f", _imu == nullptr ? _dual_imu->get_rotation() : _imu->get_rotation());
    lcd::print(4, "X: %f", currentPos.x);
    lcd::print(5, "Y: %f", currentPos.y);
    lcd::print(6, "Theta: %f", currentPos.theta);
}

void odom::start(bool thread) {
    isThread = thread;
    if (isThread) {
        if (odom::odom_task == nullptr)
            odom_task = new pros::Task(update);
    } else {
        if (odom::odom_task == nullptr)
            update();
    }
}

void odom::stop() {
    isThread = false;
}

odom::r_coord odom::r_coord::operator+(const r_coord& inp) const {
    return {this->x + inp.x, this->y + inp.y, this->theta + inp.theta};
}

odom::r_coord odom::r_coord::operator-(const r_coord& inp) const {
    return {this->x - inp.x, this->y - inp.y, this->theta - inp.theta};
}

odom::enc_coord odom::enc_coord::operator+(const enc_coord& inp) const {
    return {this->x + inp.x,  this->y + inp.y, this->theta + inp.theta};
}

odom::enc_coord odom::enc_coord::operator-(const enc_coord& inp) const {
    return {this->x - inp.x, this->y - inp.y, this->theta - inp.theta};
}
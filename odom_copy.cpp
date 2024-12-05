// // The implementation below is mostly based off of
// // the document written by 5225A (Pilons)
// // Here is a link to the original document
// // http://thepilons.ca/wp-content/uploads/2018/10/Tracking.pdf

// #include <math.h>
// #include "pros/rtos.hpp"
// #include "lemlib/util.hpp"
// #include "lemlib/chassis/odom.hpp"
// #include "lemlib/chassis/chassis.hpp"
// #include "lemlib/chassis/trackingWheel.hpp"

// class odom {
//     public:
//         struct r_coord {
//             double x, y, theta;

//             r_coord operator+(const r_coord& inp) const;
//             r_coord operator-(const r_coord& inp) const;
//         }; // Robot Coordinate

//         static void init(pros::adi::Encoder *r_x_enc, pros::adi::Encoder *l_x_enc, pros::adi::Encoder *y_enc, pros::IMU *imu, r_coord startPos);
//         static void start();
//         static void stop();
//         static r_coord getPos();

//         static odom* instance;

//         odom();

//     // private:
//         struct enc_coord {
//             double r_x, l_x, y, theta;

//             enc_coord operator+(const enc_coord& inp) const;
//             enc_coord operator-(const enc_coord& inp) const;
//         }; // Encoder Coordinate

//         static const double WHEEL_DIAMETER;

//         static const double RX_ENC_OFFSET;
//         static const double LX_ENC_OFFSET;
//         static const double Y_ENC_OFFSET;

//         static const double TICKS_PER_ROTATION;
//         static const double WHEEL_CIRCUMFERENCE;
//         static const double TICKS_PER_INCH;
//         static const double DISPLACEMENT_CONSTANT;

//         static void update();
//         static void debug();
//         // static pros::Task *_odom_task;

//         static pros::adi::Encoder* _r_x_enc;
//         static pros::adi::Encoder* _l_x_enc;
//         static pros::adi::Encoder* _y_enc;
//         static pros::IMU* _imu;

//         static enc_coord currentEnc;
//         static enc_coord lastEnc;
//         static enc_coord deltaEnc;
//         static r_coord deltaDisp;
//         static r_coord currentPos;
//         static r_coord lastPos;

//         // To be moved to gui
//         static bool debugMode;
// };

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

// // tracking thread
// pros::Task* trackingTask = nullptr;

// // global variables
// lemlib::OdomSensors odomSensors(nullptr, nullptr, nullptr, nullptr, nullptr); // the sensors to be used for odometry
// lemlib::Drivetrain drive(nullptr, nullptr, 0, 0, 0, 0); // the drivetrain to be used for odometry
// lemlib::Pose odomPose(0, 0, 0); // the pose of the robot
// lemlib::Pose odomSpeed(0, 0, 0); // the speed of the robot
// lemlib::Pose odomLocalSpeed(0, 0, 0); // the local speed of the robot

// float prevVertical = 0;
// float prevVertical1 = 0;
// float prevVertical2 = 0;
// float prevHorizontal = 0;
// float prevHorizontal1 = 0;
// float prevHorizontal2 = 0;
// float prevImu = 0;

// void lemlib::setSensors(lemlib::OdomSensors sensors, lemlib::Drivetrain drivetrain) {
//     odomSensors = sensors;
//     drive = drivetrain;
// }

// lemlib::Pose lemlib::getPose(bool radians) {
//     if (radians) return odomPose;
//     else return lemlib::Pose(odom::currentPos.x, odom::currentPos.y, radToDeg(odom::currentPos.theta));
// }

// void lemlib::setPose(lemlib::Pose pose, bool radians) {

//     if (radians) {
//         odomPose = pose;
//         odom::currentPos = {pose.x, pose.y, pose.theta};
//         odom::lastPos = {pose.x, pose.y, pose.theta};
//     } else {
//         odomPose = lemlib::Pose(pose.x, pose.y, degToRad(pose.theta));
//         odom::currentPos = {pose.x, pose.y, degToRad(pose.theta)};
//         odom::lastPos = {pose.x, pose.y, degToRad(pose.theta)};
//     }
// }

// lemlib::Pose lemlib::getSpeed(bool radians) {
//     if (radians) return odomSpeed;
//     else return lemlib::Pose(odomSpeed.x, odomSpeed.y, radToDeg(odomSpeed.theta));
// }

// lemlib::Pose lemlib::getLocalSpeed(bool radians) {
//     if (radians) return odomLocalSpeed;
//     else return lemlib::Pose(odomLocalSpeed.x, odomLocalSpeed.y, radToDeg(odomLocalSpeed.theta));
// }

// lemlib::Pose lemlib::estimatePose(float time, bool radians) {
//     // get current position and speed
//     Pose curPose = getPose(true);
//     Pose localSpeed = getLocalSpeed(true);
//     // calculate the change in local position
//     Pose deltaLocalPose = localSpeed * time;

//     // calculate the future pose
//     float avgHeading = curPose.theta + deltaLocalPose.theta / 2;
//     Pose futurePose = curPose;
//     futurePose.x += deltaLocalPose.y * sin(avgHeading);
//     futurePose.y += deltaLocalPose.y * cos(avgHeading);
//     futurePose.x += deltaLocalPose.x * -cos(avgHeading);
//     futurePose.y += deltaLocalPose.x * sin(avgHeading);
//     if (!radians) futurePose.theta = radToDeg(futurePose.theta);

//     return futurePose;
// }


// void lemlib::update() {
//     // // TODO: add particle filter
//     // // get the current sensor values
//     // float vertical1Raw = 0;
//     // float vertical2Raw = 0;
//     // float horizontal1Raw = 0;
//     // float horizontal2Raw = 0;
//     // float imuRaw = 0;
//     // if (odomSensors.vertical1 != nullptr) vertical1Raw = odomSensors.vertical1->getDistanceTraveled();
//     // if (odomSensors.vertical2 != nullptr) vertical2Raw = odomSensors.vertical2->getDistanceTraveled();
//     // if (odomSensors.horizontal1 != nullptr) horizontal1Raw = odomSensors.horizontal1->getDistanceTraveled();
//     // if (odomSensors.horizontal2 != nullptr) horizontal2Raw = odomSensors.horizontal2->getDistanceTraveled();
//     // if (odomSensors.imu != nullptr) imuRaw = degToRad(odomSensors.imu->get_rotation());

//     // // calculate the change in sensor values
//     // float deltaVertical1 = vertical1Raw - prevVertical1;
//     // float deltaVertical2 = vertical2Raw - prevVertical2;
//     // float deltaHorizontal1 = horizontal1Raw - prevHorizontal1;
//     // float deltaHorizontal2 = horizontal2Raw - prevHorizontal2;
//     // float deltaImu = imuRaw - prevImu;

//     // // update the previous sensor values
//     // prevVertical1 = vertical1Raw;
//     // prevVertical2 = vertical2Raw;
//     // prevHorizontal1 = horizontal1Raw;
//     // prevHorizontal2 = horizontal2Raw;
//     // prevImu = imuRaw;

//     // // calculate the heading of the robot
//     // // Priority:
//     // // 1. Horizontal tracking wheels
//     // // 2. Vertical tracking wheels
//     // // 3. Inertial Sensor
//     // // 4. Drivetrain
//     // float heading = odomPose.theta;
//     // // calculate the heading using the horizontal tracking wheels
//     // if (odomSensors.horizontal1 != nullptr && odomSensors.horizontal2 != nullptr)
//     //     heading -= (deltaHorizontal1 - deltaHorizontal2) /
//     //                (odomSensors.horizontal1->getOffset() - odomSensors.horizontal2->getOffset());
//     // // else, if both vertical tracking wheels aren't substituted by the drivetrain, use the vertical tracking wheels
//     // else if (!odomSensors.vertical1->getType() && !odomSensors.vertical2->getType())
//     //     heading -= (deltaVertical1 - deltaVertical2) /
//     //                (odomSensors.vertical1->getOffset() - odomSensors.vertical2->getOffset());
//     // // else, if the inertial sensor exists, use it
//     // else if (odomSensors.imu != nullptr) heading += deltaImu;
//     // // else, use the the substituted tracking wheels
//     // else
//     //     heading -= (deltaVertical1 - deltaVertical2) /
//     //                (odomSensors.vertical1->getOffset() - odomSensors.vertical2->getOffset());
//     // float deltaHeading = heading - odomPose.theta;
//     // float avgHeading = odomPose.theta + deltaHeading / 2;

//     // // choose tracking wheels to use
//     // // Prioritize non-powered tracking wheels
//     // lemlib::TrackingWheel* verticalWheel = nullptr;
//     // lemlib::TrackingWheel* horizontalWheel = nullptr;
//     // if (!odomSensors.vertical1->getType()) verticalWheel = odomSensors.vertical1;
//     // else if (!odomSensors.vertical2->getType()) verticalWheel = odomSensors.vertical2;
//     // else verticalWheel = odomSensors.vertical1;
//     // if (odomSensors.horizontal1 != nullptr) horizontalWheel = odomSensors.horizontal1;
//     // else if (odomSensors.horizontal2 != nullptr) horizontalWheel = odomSensors.horizontal2;
//     // float rawVertical = 0;
//     // float rawHorizontal = 0;
//     // if (verticalWheel != nullptr) rawVertical = verticalWheel->getDistanceTraveled();
//     // if (horizontalWheel != nullptr) rawHorizontal = horizontalWheel->getDistanceTraveled();
//     // float horizontalOffset = 0;
//     // float verticalOffset = 0;
//     // if (verticalWheel != nullptr) verticalOffset = verticalWheel->getOffset();
//     // if (horizontalWheel != nullptr) horizontalOffset = horizontalWheel->getOffset();

//     // // calculate change in x and y
//     // float deltaX = 0;
//     // float deltaY = 0;
//     // if (verticalWheel != nullptr) deltaY = rawVertical - prevVertical;
//     // if (horizontalWheel != nullptr) deltaX = rawHorizontal - prevHorizontal;
//     // prevVertical = rawVertical;
//     // prevHorizontal = rawHorizontal;

//     // // calculate local x and y
//     // float localX = 0;
//     // float localY = 0;
//     // if (deltaHeading == 0) { // prevent divide by 0
//     //     localX = deltaX;
//     //     localY = deltaY;
//     // } else {
//     //     localX = 2 * sin(deltaHeading / 2) * (deltaX / deltaHeading + horizontalOffset);
//     //     localY = 2 * sin(deltaHeading / 2) * (deltaY / deltaHeading + verticalOffset);
//     // }

//     // Get the current encoder values and heading
//     currentEnc.r_x = _r_x_enc->get_value();
//     currentEnc.l_x = _l_x_enc->get_value();
//     currentEnc.y = _y_enc->get_value();
//     currentEnc.theta = -convert::degToRad(_imu->get_rotation());

//     // Calculate the encoder value change since the last update
//     deltaEnc = currentEnc - lastEnc;
    
//     // Update last encoder values
//     lastEnc = currentEnc;

//     odom::deltaDisp.x = (odom::DISPLACEMENT_CONSTANT * odom::deltaEnc.l_x) + (odom::LX_ENC_OFFSET * odom::deltaEnc.theta);
//     odom::deltaDisp.y = (odom::DISPLACEMENT_CONSTANT * odom::deltaEnc.y) - (odom::Y_ENC_OFFSET * odom::deltaEnc.theta);
//     odom::deltaDisp.theta = odom::deltaEnc.theta;
    
//     // Update the global position of the robot
//     odom::currentPos.x = odom::lastPos.x + (odom::deltaDisp.x * cos(odom::lastPos.theta) - (odom::deltaDisp.y * sin(odom::lastPos.theta)));
//     odom::currentPos.y = odom::lastPos.y + (odom::deltaDisp.x * sin(odom::lastPos.theta) + (odom::deltaDisp.y * cos(odom::lastPos.theta)));
//     odom::currentPos.theta = odom::lastPos.theta + odom::deltaDisp.theta;

//     lemlib::Pose prevPose = {odom::lastPos.x, odom::lastPos.y, odom::lastPos.theta};

//     odom::lastPos = odom::currentPos;

//     // save previous pose
//     // lemlib::Pose prevPose = odomPose;

//     // Update the lemlib global pose
//     odomPose = {odom::currentPos.x, odom::currentPos.y, odom::currentPos.theta};

//     // calculate global x and y
//     // odomPose.x += localY * sin(avgHeading);
//     // odomPose.y += localY * cos(avgHeading);
//     // odomPose.x += localX * -cos(avgHeading);
//     // odomPose.y += localX * sin(avgHeading);
//     // odomPose.theta = heading;

//     // calculate speed
//     // odomSpeed.x = ema((odomPose.x - prevPose.x) / 0.01, odomSpeed.x, 0.95);
//     // odomSpeed.y = ema((odomPose.y - prevPose.y) / 0.01, odomSpeed.y, 0.95);
//     // odomSpeed.theta = ema((odomPose.theta - prevPose.theta) / 0.01, odomSpeed.theta, 0.95);

//     // // calculate local speed
//     // odomLocalSpeed.x = ema(localX / 0.01, odomLocalSpeed.x, 0.95);
//     // odomLocalSpeed.y = ema(localY / 0.01, odomLocalSpeed.y, 0.95);
//     // odomLocalSpeed.theta = ema(deltaHeading / 0.01, odomLocalSpeed.theta, 0.95);
// }

// void lemlib::init() {

//     odom::currentEnc = {0, 0, 0, 0};
//     odom::lastEnc = {0, 0, 0, 0};
//     odom::deltaEnc = {0, 0, 0};
//     odom::deltaDisp = {0, 0, 0};

//     odom::init(odomSensors.)

//     // odom::currentPos = startPos;
//     // odom::lastPos = startPos;

//     if (trackingTask == nullptr) {
//         trackingTask = new pros::Task {[=] {
//             while (true) {
//                 update();
//                 pros::delay(10);
//             }
//         }};
//     }
// }

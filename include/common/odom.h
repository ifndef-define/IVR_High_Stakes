// #include "main.h"

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

//     private:
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


// // Post riverbots
// // class odomBuilder {
// //     public:
// //         typedef enum {
// //             THREE_WHEEL_IMU,
// //             THREE_WHEEL,
// //             TWO_WHEEL_IMU
// //         } odomConfig;

// //         odomBuilder(odomConfig config);
// //         odomBuilder& with_encoders(pros::adi::Encoder *x_enc, pros::adi::Encoder *y_enc);
// //         odomBuilder& with_encoders(pros::adi::Encoder *r_x_enc, pros::adi::Encoder *l_x_enc, pros::adi::Encoder *y_enc);
// //         odomBuilder& with_imu(pros::IMU *imu);
// //         odomBuilder
// // }


#pragma once
#include "main.h"

class odom {
    public:
        struct r_coord {
            double x, y, theta;

            r_coord operator+(const r_coord& inp) const;
            r_coord operator-(const r_coord& inp) const;
        }; // Robot Coordinate

        static void start(bool thread=true);
        static void stop();
        static r_coord getPos();

        static odom* instance;

        odom(pros::adi::Encoder *x_enc, pros::adi::Encoder *y_enc, pros::IMU *imu, r_coord startPos, double x_off, double x_tune, double y_off, double y_tune);

    private:
        struct enc_coord {
            double x, y, theta;

            enc_coord operator+(const enc_coord& inp) const;
            enc_coord operator-(const enc_coord& inp) const;
        }; // Encoder Coordinate

        static const double WHEEL_DIAMETER;
        static const double TICKS_PER_ROTATION;
        static const double WHEEL_CIRCUMFERENCE;
        static const double TICKS_PER_INCH;
        static const double DISPLACEMENT_CONSTANT;
        
        static double X_ENC_OFFSET;
        static double Y_ENC_OFFSET;

        static void update();
        static void debug();
        // static pros::Task *_odom_task;

        static pros::adi::Encoder* _x_enc;
        static pros::adi::Encoder* _y_enc;
        static pros::IMU* _imu;
        static pros::Task *odom_task;

        static enc_coord currentEnc;
        static enc_coord lastEnc;
        static enc_coord deltaEnc;
        static r_coord deltaDisp;
        static r_coord currentPos;
        static r_coord lastPos;

        static bool debugMode;
        static bool isThread;
};
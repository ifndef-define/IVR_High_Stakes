#pragma once
#include "common/includeList.h"

class Arm {
    public:
        enum class State : int {
            DOWN = 0,   // index 0
            READY = 1,  // index 1
            SCORE = 2,  // index 2
            DESCORE = 3,  // index 3
            NUM_ARM_STATES = 4 // count of states
        };
        private:
        pros::Motor armMotor;
        pros::Rotation armRot;
        // pros::adi::DigitalIn armLimit;
        int currentLimitState;

        struct PIDconfig {
            float kP;
            float kI;
            float kD;
        };
        PIDconfig smallConfig;
        PIDconfig largeConfig;

        float error = 0;
        PID small;
        PID large;

        // Add variable to track previous limit switch state
        int prevLimitState = 0;

        State curArmState = State::DOWN;
        const double armStateAngles[(int)(Arm::State::NUM_ARM_STATES)] = { 
            5,   // Angle for DOWN
            28,  // Angle for READY 
            200,   // Angle for SCORE
            350  // Angle for DESCORE
        };

        bool override = false;

    public:
        Arm(float kP, float kI, float kD, float kP2, float kI2, float kD2);

        /**
         * @brief Updates arm position using PID
         * 
         */
        void update();

        /**
         * @brief Move to the next state
         * 
         */
        void nextState();

        /**
         * @brief Move to the previous state
         * 
         */
        void prevState();
        
        /**
         * @brief Set the arm to a specific state
         * 
         * @param newState The state to set the arm to
         */
        void setState(State newState);

        /**
         * @brief Set speed of arm
         * 
         * @param power Speed to move arm [0,1]
         */
        void setSpeed(double speed);
        
        /**
         * @brief Returns the current arm state position in degrees
         *
         */
        double getAngle();
        
        /**
         * @brief Returns the current arm state
         * 
         */
        State getState();
        void setBrakeMode(pros::motor_brake_mode_e_t mode);
        void setClimb(bool climb, int scale = .65);
        PID getPID(bool type);
};
#pragma once
#include "common/includeList.h"

class Arm {
    public:
        enum class State : int {
            DOWN = 0,   // index 0
            READY = 1,  // index 1
            SCORE = 2,  // index 2
            NUM_ARM_STATES = 3 // count of states
        };
    private:
        pros::Motor armMotor;
        pros::Rotation armRot;

        lemlib::PID armPID;

        State curArmState = State::DOWN;
        const double armStateAngles[(int)(Arm::State::NUM_ARM_STATES)] = { 
            0,   // Angle for DOWN
            18,  // Angle for READY 
            100   // Angle for SCORE
        };

        bool override = false;

    public:
        Arm(float kP, float kI, float kD): armMotor(-14, pros::MotorGears::red), armRot(-21), armPID(kP, kI, kD, 10, 0) {};

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
        void setSpeed(int speed);
        
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
};
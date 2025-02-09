#pragma once
#include "common/includeList.h"

class Arm {
    public:
        enum class State {
            DOWN,   // index 0
            READY,  // index 1
            SCORE,  // index 2
            NUM_ARM_STATES = 3 // count of states
        };
    private:
        pros::MotorGroup armMotor;
        pros::Rotation armRot;

        State curArmState = State::DOWN;
        static constexpr double armStateAngles[int(State::NUM_ARM_STATES)] = { 
            0,   // Angle for DOWN
            25,  // Angle for READY 
            145   // Angle for SCORE
        };

        lemlib::PID armPID;

        bool override = false;

    public:
        Arm(float kP, float kI, float kD);

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
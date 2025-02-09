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
        pros::Motor armMotor;
        pros::Rotation armRot;

        State curArmState = State::DOWN;
        static constexpr double armStateAngles[int(State::NUM_ARM_STATES)] = { 
            0,   // Angle for DOWN
            25,  // Angle for READY 
            145   // Angle for SCORE
        };

        lemlib::PID armPID;

    public:

        Arm(float kP, float kI, float kD): armMotor(3), armRot(-17), armPID(kP, kI, kD, 0, false){};

        /**
         * @brief Updates arm position using PID
         * 
         */
        void update();
        
        /**
         * @brief Set the arm to a specific state
         * 
         * @param newState The state to set the arm to
         */
        void setState(State newState);
        
        /**
         * @brief Moves arm to next state
         * 
         */
        void nextState();
        
        /**
         * @brief Moves arm to previous state
         * 
         */
        void prevState();

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
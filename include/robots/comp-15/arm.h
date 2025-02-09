#pragma once
#include "common/includeList.h"

/**
 * @brief The arm for scoring rings on wall stake
 * 
 */
class Arm {
    private:
        pros::MotorGroup armMotor;
        pros::Rotation armRot;

        enum armState_t {
            DOWN = 0,   // index 0
            READY = 1,  // index 1
            SCORE = 2,  // index 2
            NUM_ARM_STATES = 3 // count of states
        };
        armState_t armState = DOWN;
        static constexpr double armStateAngles[NUM_ARM_STATES] = { 
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
        void setState(armState_t newState);
        
        /**
         * @brief Returns the current arm state position in degrees
         *
         */
        double getAngle();
        
        /**
         * @brief Returns the current arm state
         * 
         */
        armState_t getState();

};
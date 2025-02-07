#pragma once
#include "common/includeList.h"

/**
 * @brief The arm for scoring rings
 * 
 */
class Arm{
    private:
        pros::Motor armMotor;
        pros::Rotation armRot;

        typedef enum {
            DOWN = 0,
            READY = 123,
            SCORE = 123
        } armState_t;
        armState_t armState;

        struct armGains {
            double kP = 0;
            double kI = 0;
            double kD = 0;
        };

    public:
        Arm(): armMotor(-11), armRot(-17), armState(DOWN) {}

        /**
         * @brief Updates arm to new state
         * 
         * @param newState The new state to move to
         */
        void update(armState_t newState);
        
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
         * @brief Moves arm using motor power
         * 
            * @param speed The power to move the arm [0, 1]
         */
        void armPower(double speed);
        
        /**
         * @brief Moves arm to a specific position
         *
         */
        double getPosition();
        
        /**
         * @brief Moves arm to a specific position
         * 
         */
        armState_t getState();

};
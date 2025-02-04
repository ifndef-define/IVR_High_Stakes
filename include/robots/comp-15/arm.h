#pragma once
#include "common/includeList.h"

/**
 * @brief The arm for scoring rings
 * 
 */
class Arm{
    private:
        pros::Motor* armMotor;
        pros::Rotation* armRot;

        typedef enum {
            DOWN = 123,
            READY = 123,
            SCORE = 123
        } armState_t;
    public:
        Arm();

        /**
         * @brief Updates arm to new state
         * 
         * @param newState The new state to move to
         */
        void moveToState(armState_t newState);
        
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
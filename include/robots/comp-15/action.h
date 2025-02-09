#pragma once
#include "common/includeList.h"
#include "robots/comp-15/includeList.h"

enum class ActionState {
    IDLE, // Robot is doing normal actions
    SORTING,   // Intake is sorting rings by color
};

class Action {
    private:
        ActionState currentState;
        IntakeManager intakeManager;
        Arm arm;
        bool isAuton;
        bool override = false;
    public:
        Action(bool isAuton, Ring::Color ringToKeep);
        void runSubsystemFSM();
        void stateControl();
        /**
         * @brief Sets arm to override state
         *
         */
        void setOverride(bool override);

        /**
         * @brief Returns if the arm is in an override state
         *
         */
        bool isOverride();

        /**
         * @brief Sets the speed of the intake
         *
         * @param speed The speed of the intake [0, 1]
         */
        void setIntakeSpeed(double speed);

        /**
         * @brief Set the Arm Speed object
         * 
         * @param speed The speed of the arm
         */
        void setArmSpeed(int speed);

        /**
         * @brief Sets the color to keep in the filter
         *
         */
        void setRingColor(Ring::Color ringToKeep);

        /**
         
        /**
         * @brief Move to the next state
         * 
         */
        void nextArmState();
        
        /**
         * @brief Move to the previous state
         * 
         */
        void prevArmState();
        
        /**
         * @brief Set the arm to a specific state
         * 
         * @param newState The state to set the arm to
         */
        void setArmState(Arm::State newState);

        /**
         * @brief Get the Arm State object
         * 
         * @return Arm::State 
         */
        Arm::State getArmState();

        /**
        * @brief Get the State object
        * 
        * @return ActionState 
        */
       ActionState getState();
};


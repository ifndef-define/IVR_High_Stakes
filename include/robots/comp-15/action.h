#pragma once
#include "robots/comp-15/robot.hpp"
#include "common/includeList.h"
// #include "robots/comp-24/includeList.h"

enum class ActionState {
    IDLE, // Robot is doing normal actions
    SORTING,   // Intake is sorting rings by color
    PULLBACK // Intake is pulling back to avoid hook-ring collision
};

enum class EjectPhase { IDLE, FORWARD, PAUSE, REVERSE };

class Action {
    private:
        ActionState currentState;
        IntakeManager intakeManager;
        Distance *mogoSensor_;
        Arm arm;
        // Climb climb;
        bool isAuton;
        bool override = false;
        Arm::State lastArmState = Arm::State::DOWN;
        
        int pauseCounter = 0;
        int ejectCounter = 23;
        bool pullbackFlag = false;
        bool ejectFlag = false;
        bool runColorSort = true;
        bool autoResumeFlag = false;
        bool isClimbing = false;
        bool runAutoMogoClamp = false;
        bool ringDetected = false;
        bool runArm = false;
        bool reclamp = false;

        EjectPhase ejectPhase = EjectPhase::IDLE;
        double ejectStartPos = 0;
        uint32_t pauseStartTime = 0;
        double currentRotation = 0;

        // Climb state tracking
        // Climb::Tier lastTier = Climb::Tier::IDLE;  
        // bool tierStateInitialized = false;
        // int tierSubstate = 0;  // For tracking progress within each tier

    public:
        Action(bool isAuton, Ring::Color ringToKeep, int mogoSensorPort);
        void runSubsystemFSM();
        void stateControl();
        void climbControl();
        void autoMogo();

        void releaseIntake(bool inv=false);
        void intakeState();
        void climbState();

        /**
         * @brief Sets arm to override state
         *
         */
        void setOverride(bool override);


        /**
         * @brief Enables auto resume for the intake during auton
         *
         * @param flag The flag to set
         */
        void setAutonControlFlag(bool flag);

        bool getAutonControlFlag();

        /**
         * @brief Returns if the arm is in an override state
         *
         */
        bool getOverride();

        /**
         * @brief Sets the speed of the intake
         *
         * @param speed The speed of the intake [0, 1]
         * @param autonResume If the intake should automatically resume during auton after 
         *  a ring is ejected
         */
        void setIntakeSpeed(double speed);

        /**
         * @brief Set the Arm Speed object
         * 
         * @param speed The speed of the arm
         */
        void setArmSpeed(double speed);

        /**
         * @brief Sets the color to keep in the filter
         *
         */
        void setRingColor(Ring::Color ringToKeep);

        /**
         * @brief Toggle the color sorting
         * 
         */
        void setRunColorSort(bool colorSort);
         
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
         * @brief Set the climb state
         * 
         * @param newState The state to set the climb to
         */
        // void setClimbState(Climb::State newState);

        /**
         * @brief Set the arm to a specific state
         * 
         * @param newState The state to set the arm to
         */
        void setArmState(Arm::State newState);

        /**
         * @brief Get the Pullback Flag bool
         * 
         * @return bool 
         */
        void setPullbackFlag(bool flag);


        /**
         * @brief Set the Eject Flag bool
         * 
         * @param flag 
         */
        void setEjectFlag(bool flag);

        /**
         * @brief Get the Arm State object
         * 
         * @return Arm::State 
         */
        Arm::State getArmState();

        /**
         * @brief Get the Arm Angle
         * 
         * @return double 
         */
        double getArmAngle();

        /**
        * @brief Get the State object
        * 
        * @return ActionState 
        */
        ActionState getActionState();

        /**
         * @brief Get the Pullback Flag bool
         * 
         * @return true 
         * @return false 
         */
        bool getPullbackFlag();

        /**
         * @brief Get the Eject Flag bool
         * 
         * @return true 
         * @return false 
         */
        bool getEjectFlag();

        /**
         * @brief Get the Run Color Sort object
         * 
         * @return true 
         * @return false 
         */
        bool getRunColorSort();

        void setClimbing(bool flag);
        bool getClimbing();
        void setRunAutoMogoClamp(bool flag);
        void setRunArm(bool flag);
        void setArmBrakeMode(pros::motor_brake_mode_e mode);
};

extern Action actions;
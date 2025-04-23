#pragma once
#include "robots/comp-24/pneumatics.h"
#include "common/includeList.h"
#include <queue>
// #include <functional>

class Climb {
    public:
        enum class State {
            IDLE = 0,   // index 0
            UP = 1,  // index 1
            DOWN = 2,  // index 2
            NUM_CLIMB_STATES = 3 // count of states
        };

        enum class Tier {
            IDLE,
            ZERO,
            ONE,
            TWO,
            THREE
        };

        private:

        struct TaskWithDelay {
            std::function<void()> task;
            uint32_t delayMs;
        };

        PneumaticsGroup* pneumatics;
        State curClimbState = State::UP;
        Tier curTier = Tier::IDLE;
        State lastClimbState = curClimbState;
        PID climbPID;
        float error;
        double curPos = 0;

        std::queue<TaskWithDelay> taskQueue;
        bool isWaitingAfterTask = false;
        uint32_t lastTaskExecutionTime = 0;
        uint32_t currentTaskDelayMs = 0;

        bool override = false;
        
        double climbStateAngles[(int)(State::NUM_CLIMB_STATES)] = { 
            curPos,   // Angle for IDLE
            900,  // Angle for DOWN
            0   // Angle for UP
        };

    public:
        Climb(PneumaticsGroup& p, float kP, float kI, float kD);
        
        void increment();
        void decrement();
        void setState(State newState);

        void extendPto();
        void retractPto();
        void extendInnerArm();
        void extendOuterArm();
        void retractInnerArm();
        void retractOuterArm();

        bool isPtoExtended();
        bool isInnerArmExtended();
        bool isOuterArmExtended();

        void addEvent(std::function<void()> task, uint32_t delayMs = 0);
        void update(int curPos);

        void setBrakeMode(pros::motor_brake_mode_e_t mode);
        
        Tier getTier();
        void setTier(Tier newTier);
        State getState();
        State getLastState();
        double getAngle(int pos);
        
        void setOverride(bool override);
        bool isOverride();
        
        bool isAtTargetPosition();  // New method to check if PID has reached target
};
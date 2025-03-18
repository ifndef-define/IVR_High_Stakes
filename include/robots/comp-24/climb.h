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
        };;

        pros::Rotation climbRot;
        PneumaticsGroup* pneumatics;
        State curClimbState = State::UP;
        Tier curTier = Tier::ONE;
        State lastClimbState = curClimbState;
        PID climbPID;

        std::queue<TaskWithDelay> taskQueue;
        bool isWaitingAfterTask = false;
        uint32_t lastTaskExecutionTime = 0;
        uint32_t currentTaskDelayMs = 0;

        bool override = false;
        bool climbing = false;
        
        const double climbStateAngles[(int)(State::NUM_CLIMB_STATES)] = { 
            0,   // Angle for IDLE
            90,  // Angle for DOWN
            900   // Angle for UP
        };

    public:
        Climb(PneumaticsGroup& p, float kP, float kI, float kD);
        
        void increment();
        void decrement();
        void setState(State newState);

        void extendPto();
        void retractPto();
        void extendPusher();
        void retractPusher();

        void addEvent(std::function<void()> task, uint32_t delayMs = 0);
        void update();

        void setBrakeMode(pros::motor_brake_mode_e_t mode);
        
        Tier getTier();
        void setTier(Tier newTier);
        State getState();
        State getLastState();
        double getAngle();

        bool isClimbing();
        void setClimbing(bool climbing);
        void setOverride(bool override);
        bool isOverride();
};;
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
        Controller *controller;
        bool isAuton;
    public:
        Action(bool isAuton, Ring::Color ringToKeep, Controller &controller);
        void runSubsystemFSM();
        void stateControl();
};


#include "robots/comp-15/action.h"

Action::Action(bool isAuton, Ring::Color ringToKeep, Controller &controller): currentState(ActionState::IDLE), intakeManager(), arm(0.1, 0, 0){ 
    intakeManager.setFilterColor(ringToKeep);
    this->controller = &controller;
    this->isAuton = isAuton;
}

void Action::runSubsystemFSM() {
        // Update intake manager FSM and check ring color
        intakeManager.update();
        
        // Branch based on intakeManager's decision (e.g. eject set true if wrong color)
        if (intakeManager.getEject()) {
            currentState = ActionState::SORTING;
        } else {
            currentState = ActionState::IDLE;
        }
        // Run state control
        stateControl();
        
        // Update arm with new state
        arm.update();
        
}

void Action::stateControl() {
    switch(currentState) {
        case ActionState::IDLE:
            break;
        case ActionState::SORTING:
            if(int(arm.getState()) > int(Arm::State::READY)){
                arm.setState(Arm::State::SCORE); // position SCORE to avoid prevent intake collision
            } else {
                arm.setState(Arm::State::DOWN);  // For example, position DOWN to avoid intake
            }
            intakeManager.ejectDisc();
            break;
    }
}

bool Action::isOverride(){ return override; }

void Action::setOverride(bool override){ this->override = override; }

void Action::setIntakeSpeed(double speed) {
    if(currentState!=ActionState::SORTING)
        intakeManager.setIntakeSpeed(speed);
        if(speed>0)
            intakeManager.startIntake();
        else
            intakeManager.stopIntake();
}

void Action::nextArmState() {
    arm.nextState();
}

void Action::prevArmState() {
    arm.prevState();
}

void Action::setArmState(Arm::State newState) {
    arm.setState(newState);
}

void Action::setArmSpeed(int speed) {
    arm.setSpeed(speed);
}

void Action::setRingColor(Ring::Color ringToKeep) {
    intakeManager.setFilterColor(ringToKeep);
}

ActionState Action::getState() {
    return currentState;
}


#include "robots/comp-15/action.h"

Action::Action(bool isAuton, Ring::Color ringToKeep): arm(.5, 0, 0), currentState(ActionState::IDLE), intakeManager(){ 
    intakeManager.setFilterColor(ringToKeep);
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
            if(!override){
                if(int(arm.getState()) > int(Arm::State::READY)){
                    arm.setState(Arm::State::SCORE); // position SCORE to avoid prevent intake collision
                } else {
                    arm.setState(Arm::State::DOWN);  // For example, position DOWN to avoid intake
                }
                intakeManager.ejectDisc();
            }
            break;
    }
}

bool Action::isOverride(){ return override; }

void Action::setOverride(bool override){ this->override = override; }

void Action::setIntakeSpeed(double speed) {
    if(currentState!=ActionState::SORTING || override){
        intakeManager.setIntakeSpeed(speed);
        if(speed!=0){
            intakeManager.startIntake();
        } else {
            intakeManager.setIntakeSpeed(1);
            intakeManager.stopIntake();
        }
    }
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

Arm::State Action::getArmState() {
    return arm.getState();
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

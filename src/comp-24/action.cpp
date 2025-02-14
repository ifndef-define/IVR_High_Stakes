#include "robots/comp-24/action.h"

Action::Action(bool isAuton, Ring::Color ringToKeep): arm(3.2,0,0, 3.85,0,0), currentState(ActionState::IDLE), intakeManager(){ 
    intakeManager.setFilterColor(ringToKeep);
    this->isAuton = isAuton;
}

void Action::runSubsystemFSM() {
        // Update intake manager FSM and check ring color
        intakeManager.update();
        
        // Branch based on intakeManager's decision (e.g. eject set true if wrong color)
        if (intakeManager.getEject() && intakeManager.getColorSort()) {
            currentState = ActionState::SORTING;
        }else if((arm.getState() == Arm::State::SCORE && lastArmState == Arm::State::READY) || intakeManager.getPullbackFlag()){
            currentState = ActionState::PULLBACK;
            intakeManager.setPullbackFlag(true);
        } else {
            currentState = ActionState::IDLE;
            intakeManager.setPullbackFlag(false);
        }
        // Run state control
        stateControl();
        
        // Update arm with new state
        arm.update();
        lastArmState = arm.getState();
}

void Action::stateControl() {
    switch(currentState) {
        case ActionState::IDLE:
            break;
        case ActionState::SORTING:
            if(!override){
                if((int)(arm.getState()) > (int)(Arm::State::READY)){
                    arm.setState(Arm::State::SCORE); // position SCORE to avoid prevent intake collision
                } else {
                    arm.setState(Arm::State::DOWN);  // For example, position DOWN to avoid intake
                }
                intakeManager.ejectDisc();
            }
            break;
        case ActionState::PULLBACK:
            intakeManager.setIntakeSpeed(-1);
            intakeManager.startIntake();
            if (pauseCounter < 7){ // 7*15 = 105ms
                pauseCounter++;
            } else {
                pauseCounter = 0;
                intakeManager.stopIntake();
                intakeManager.setIntakeSpeed(1);
                intakeManager.setPullbackFlag(false);
            }
            break;
    }
}

bool Action::getOverride(){ return override; }

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

double Action::getArmAngle() {
    return arm.getAngle();
}

void Action::setArmSpeed(int speed) {
    arm.setSpeed(speed);
}

void Action::setRunColorSort(bool colorSort){
    intakeManager.setColorSort(colorSort);
}

void Action::setRingColor(Ring::Color ringToKeep) {
    intakeManager.setFilterColor(ringToKeep);
}

ActionState Action::getState() {
    return currentState;
}

bool Action::getPullbackFlag(){
    return intakeManager.getPullbackFlag();
}
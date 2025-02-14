#include "robots/comp-24/action.h"

Action::Action(bool isAuton, Ring::Color ringToKeep): arm(3.2,0,0, 3.85,0,0), currentState(ActionState::IDLE), intakeManager(){ 
    intakeManager.setFilterColor(ringToKeep);
    this->isAuton = isAuton;
}

void Action::runSubsystemFSM() {
    // Update intake manager FSM and check ring color
    intakeManager.update();
    
    // Branch based on intakeManager's decision (e.g. eject set true if wrong color)
    if ((intakeManager.getEject() && getRunColorSort()) || getEjectFlag()) {
        currentState = ActionState::SORTING;
    } else if((arm.getAngle() > 17 && arm.getAngle() < 20) || getPullbackFlag()){
        currentState = ActionState::PULLBACK;
    } else {
        currentState = ActionState::IDLE;
    }
    // Run state control
    stateControl();
    
    // Update arm with new state
    lastArmState = arm.getState();
    arm.update();
}

void Action::stateControl() {
    switch(currentState) {
        case ActionState::IDLE:
            setEjectFlag(false);
            setPullbackFlag(false);
            break;
        case ActionState::SORTING:
            if(!override){
                setEjectFlag(true);
                if((int)(arm.getState()) > (int)(Arm::State::READY)){
                    // arm.setState(Arm::State::SCORE); // position SCORE to avoid prevent intake collision
                } else {
                    arm.setState(Arm::State::DOWN);  // For example, position DOWN to avoid intake
                }
                ejectDisc();
            }
            break;
        case ActionState::PULLBACK:
            setEjectFlag(true);
            intakeManager.setIntakeSpeed(-1);
            intakeManager.startIntake();
            if (pauseCounter < 10){ // 7*15 = 105ms
                pauseCounter++;
            } else {
                pauseCounter = 0;
                intakeManager.stopIntake();
                setPullbackFlag(false);
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
            intakeManager.stopIntake();
        }
    }
}

void Action::ejectDisc(){
    if(ejectCounter >= 12){
        ejectCounter--;
    } else if(ejectCounter > 0){
        intakeManager.setIntakeSpeed(-1);
        intakeManager.startIntake();
        ejectCounter--;
    } else {
        ejectCounter = 17;
        intakeManager.stopIntake();
        setEjectFlag(false);
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

void Action::setRingColor(Ring::Color ringToKeep) {
    intakeManager.setFilterColor(ringToKeep);
}

ActionState Action::getState() {
    return currentState;
}

void Action::setEjectFlag(bool flag){
    ejectFlag = flag;
}

void Action::setPullbackFlag(bool flag){
    pullbackFlag = flag;
}

void Action::setRunColorSort(bool colorSort){
    runColorSort = colorSort;
}

bool Action::getRunColorSort(){
    return runColorSort;
}

bool Action::getPullbackFlag(){
    return pullbackFlag;
}

bool Action::getEjectFlag(){
    return ejectFlag;
}

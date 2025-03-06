#include "robots/comp-24/action.h"

/**
 * @brief Constructs an Action object
 * @param isAuton Whether this is running in autonomous mode
 * @param ringToKeep The color to keep in the filter
 * 
 * Sets the initial state of the arm to DOWN and the initial state of the Action to IDLE.
 */
Action::Action(bool isAuton, Ring::Color ringToKeep): arm(2.5,0,0, 4,0,0), currentState(ActionState::IDLE), intakeManager(){ 
    intakeManager.setFilterColor(ringToKeep);
    this->isAuton = isAuton;
}

void Action::runSubsystemFSM() {
    // Update intake manager FSM and check ring color
    intakeManager.update();
    
    // Branch based on intakeManager's decision (e.g. eject set true if wrong color)
    if ((intakeManager.getEject() && getRunColorSort()) || getEjectFlag()) {
        currentState = ActionState::SORTING;
    } else if((arm.getAngle() > 60 && arm.getAngle() < 70 && ((int)lastArmState > (int)Arm::State::READY)) || getPullbackFlag()){
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
            ejectPhase = EjectPhase::IDLE;
            ejectStartPos = intakeManager.getIntakeAngle();
            break;
        case ActionState::SORTING:
            if(!override){
                setEjectFlag(true);
                if((int)(arm.getState()) > (int)(Arm::State::READY)){
                    arm.setState(Arm::State::SCORE); // position SCORE to avoid prevent intake collision
                } else {
                    arm.setState(Arm::State::DOWN);  // For example, position DOWN to avoid intake
                }
                intakeState();
            }
            break;
        case ActionState::PULLBACK:
            setPullbackFlag(true);
            intakeManager.setIntakeSpeed(-1);
            intakeManager.startIntake();
            intakeState();
            break;
    }
}

bool Action::getOverride(){ return override; }

void Action::setOverride(bool override){ this->override = override; }

void Action::setAutonControlFlag(bool flag){ autoResumeFlag = flag; }

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

void Action::releaseIntake(bool inv){
    intakeManager.setIntakeSpeed(inv ? 1 : -1);
    intakeManager.startIntake();
    delay(100);  
    intakeManager.stopIntake();
}

// void Action::ejectDisc(){
    // if(ejectCounter >= 12){
    //     ejectCounter--;
    // } else if(ejectCounter > 0){
    //     intakeManager.setIntakeSpeed(-1);
    //     intakeManager.startIntake();
    //     ejectCounter--;
    // } else {
    //     ejectCounter = 17;
    //     if (this->autoResumeFlag){
    //         intakeManager.setIntakeSpeed(1);
    //         intakeManager.startIntake();
    //     } else {
    //         intakeManager.stopIntake();
    //     }
    //     setEjectFlag(false);
    // }

// }

void Action::intakeState() {
    if(ejectFlag){
        ejectPhase = EjectPhase::FORWARD;
    } else if(pullbackFlag){
        ejectPhase = EjectPhase::REVERSE;
    } else {
        ejectPhase = EjectPhase::IDLE;
    }
    // This function should be called each cycle when in SORTING state
    switch(ejectPhase) {
        case EjectPhase::IDLE:
            // Clear the eject flag so the FSM resumes normal operation
            setEjectFlag(false);
            setPullbackFlag(false);
            break;
        case EjectPhase::FORWARD: 
            // Calculate rotation taking into account possible wrap-around
            currentRotation = intakeManager.getIntakeAngle();
        
            pros::lcd::print(4, "D: %f, C: %f, S: %f", ejectStartPos - currentRotation, currentRotation, ejectStartPos);

            // Check if we've rotated the required amount
            if (std::abs(ejectStartPos - currentRotation) >= 150) {
                ejectStartPos = currentRotation;
                setEjectFlag(false);
                setPullbackFlag(true);
                ejectPhase = EjectPhase::REVERSE;
            } else {
                intakeManager.setIntakeSpeed(1);
                intakeManager.startIntake();
            }
            break;
        
        case EjectPhase::REVERSE: 
            // Similar fix for reverse phase
            currentRotation = intakeManager.getIntakeAngle();
            if (std::abs(ejectStartPos - currentRotation) >= 150) {
                intakeManager.stopIntake();
                intakeManager.setIntakeSpeed(1);
                setEjectFlag(false);
                setPullbackFlag(false);
                ejectPhase = EjectPhase::IDLE;
            } else {
                intakeManager.setIntakeSpeed(-1);
                intakeManager.startIntake();
            }
            break;
    }
}

// void Action::pullbackIntake(){
    // if (pauseCounter < 10){ // 7*15 = 105ms
    //     pauseCounter++;
    // } else {
    //     pauseCounter = 0;
    //     intakeManager.stopIntake();
    //     setPullbackFlag(false);
    // }

// }

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

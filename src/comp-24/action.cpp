#include "robots/comp-24/action.h"

/**
 * @brief Constructs an Action object
 * @param isAuton Whether this is running in autonomous mode
 * @param ringToKeep The color to keep in the filter
 * 
 * Sets the initial state of the arm to DOWN and the initial state of the Action to IDLE.
 */
Action::Action(bool isAuton, Ring::Color ringToKeep): arm(3.2,0,0, 4.7,0,0), currentState(ActionState::IDLE), intakeManager(){ 
    intakeManager.setFilterColor(ringToKeep);
    this->isAuton = isAuton;
}

void Action::runSubsystemFSM() {
    // Update intake manager FSM and check ring color
    intakeManager.update();
    
    // Branch based on intakeManager's decision (e.g. eject set true if wrong color)
    if ((intakeManager.getEject() && getRunColorSort()) || getEjectFlag()) {
        currentState = ActionState::SORTING;
    } else if((arm.getAngle() > 14 && arm.getAngle() < 16) || getPullbackFlag()){
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

/**
 * @brief Updates the state of the Action class based on the currentState
 * 
 * @details
 * This function will run the appropriate code based on the currentState.
 * If the currentState is ActionState::IDLE, it will set ejectFlag and pullbackFlag to false.
 * If the currentState is ActionState::SORTING, it will check if the override is on. If it's not, 
 * it will set ejectFlag to true and change the arm's state to DOWN or SCORE depending on its current state. 
 * It will then eject the disc.
 * If the currentState is ActionState::PULLBACK, it will set ejectFlag to true and set the intake speed to -1, 
 * then start the intake. It will then pause for 105ms and then stop the intake and set pullbackFlag to false.
 */
void Action::stateControl() {
    switch(currentState) {
        case ActionState::IDLE:
            setEjectFlag(false);
            setPullbackFlag(false);
            break;
        case ActionState::SORTING: // Intake is sorting rings by color
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
        case ActionState::PULLBACK: // Intake is pulling back to avoid hook-ring collision
            setEjectFlag(true);
            intakeManager.setIntakeSpeed(-1);
            intakeManager.startIntake();
            if (pauseCounter < 10){ // 10*10 = 100 ms
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

void Action::releaseIntake(bool inv){
    intakeManager.setIntakeSpeed(inv ? 1 : -1);
    intakeManager.startIntake();
    delay(80);  
    intakeManager.stopIntake();
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
        if (this->autoResumeFlag){
            intakeManager.setIntakeSpeed(1);
            intakeManager.startIntake();
        } else {
            intakeManager.stopIntake();
        }
        setEjectFlag(false);
    }
}

void Action::setAutonControlFlag(bool flag){ autoResumeFlag = flag; }

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

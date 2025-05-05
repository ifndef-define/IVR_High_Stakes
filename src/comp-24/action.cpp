#include "robots/comp-24/action.h"

/**
 * @brief Constructs an Action object
 * @param isAuton Whether this is running in autonomous mode
 * @param ringToKeep The color to keep in the filter
 * 
 * Sets the initial state of the arm to DOWN and the initial state of the Action to IDLE.
 */
Action::Action(bool isAuton, Ring::Color ringToKeep, int mogoSensorPort): arm(2.5,0,0, 4,0,0), currentState(ActionState::IDLE), intakeManager()/*, climb(p, 1,0,0)*/ { 
    intakeManager.setFilterColor(ringToKeep);
    this->isAuton = isAuton;
    mogoSensor_ = new pros::Distance(mogoSensorPort);
}

void Action::runSubsystemFSM() {
    // Update intake manager FSM and check ring color
    intakeManager.update();
    
    // Branch based on intakeManager's decision (e.g. eject set true if wrong color)
    if(!getOverride()){
        if ((intakeManager.getEject() && getRunColorSort()) || getEjectFlag()) {
            currentState = ActionState::SORTING;
        } else if((arm.getAngle() > 50 && arm.getAngle() < 60 && ((int)lastArmState > (int)Arm::State::READY)) || getPullbackFlag()){
            currentState = ActionState::PULLBACK;
        } else {
            currentState = ActionState::IDLE;
        }
    } else {
        currentState = ActionState::IDLE;
    }
    // Run state control
    stateControl();

    autoMogo();
    
    // // Check if tier changed - reset state initialization flag
    // if (lastTier != climb.getTier()) {
    //     tierStateInitialized = false;
    //     tierSubstate = 0;
    //     lastTier = climb.getTier();
    // }
    
    // Update climb
    // if(runClimb){
    //     climbControl();
    //     climb.update(intakeManager.getIntakeAngle());
    // } else {
    //     climb.setState(Climb::State::IDLE);
    // }
    
    // Update arm with new state
    if (runArm) {
        lastArmState = arm.getState();
        arm.update();
    }
}

void Action::autoMogo() {
    if(runAutoMogoClamp){
        if(!pneumatics.mogoClamp.is_extended() && (mogoSensor_->get_distance() < 13) && !reclamp) {
            pneumatics.mogoClamp.extend();
            reclamp = true;
        }

        if (!pneumatics.mogoClamp.is_extended() && reclamp) {
            if (mogoSensor_->get_distance() > 100) {
                reclamp = false;
            }
        }
    }
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
            // if(!override){
                setEjectFlag(true);
                if((int)(arm.getState()) > (int)(Arm::State::READY)){
                    arm.setState(Arm::State::SCORE); // position SCORE to avoid prevent intake collision
                } else {
                    arm.setState(Arm::State::DOWN);  // For example, position DOWN to avoid intake
                }
                intakeState();
            // }
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
            if (std::abs(ejectStartPos - currentRotation) >= 20) {
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
                // intakeManager.stopIntake();
                intakeManager.setIntakeSpeed(1);
                intakeManager.startIntake();
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

// void Action::climbState() {
//     climb.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
//     switch(climb.getState()) {
//         case Climb::State::IDLE:
//             break;
//         case Climb::State::UP:
//             climb.addEvent([&](){climb.retractOuterArm();});
//             break;
//         case Climb::State::DOWN:
//             climb.addEvent([&](){climb.retractInnerArm();});
//             break;
//     }
// }

// void Action::climbControl() {
//     if(climb.isOverride()){ 
//         climb.setTier(Climb::Tier::IDLE); 
//         return;
//     }
    
//     switch(climb.getTier()) {
//         case Climb::Tier::IDLE:
//             tierStateInitialized = false;
//             tierSubstate = 0;
//             break;
            
//         case Climb::Tier::ZERO:
//             if (!tierStateInitialized) {
//                 tierStateInitialized = true;
//                 intakeManager.resetIntakeAngle();
//                 climb.addEvent([&](){climb.extendPto();}, 100);
//                 climb.addEvent([&](){climb.retractPto();});
//                 climb.addEvent([&](){climb.extendInnerArm();}, 100);
//                 climb.setTier(Climb::Tier::IDLE);
//             }
//             break;
            
//         case Climb::Tier::ONE:
//             // Only initialize actions once when entering this state
//             if (!tierStateInitialized) {
//                 // Pull up to tier one
//                 climb.setState(Climb::State::DOWN);
//                 tierStateInitialized = true;
//             }
            
//             // When PID target is reached, proceed to next tier
//             if (climb.isAtTargetPosition()) {
//                 tierStateInitialized = false;
//                 climb.setTier(Climb::Tier::TWO);
//             }
//             break;
            
//         case Climb::Tier::TWO:
//             // Use substates to track progress through this tier
//             if (!tierStateInitialized) {
//                 climb.addEvent([&](){climb.setState(Climb::State::UP);});
//                 tierStateInitialized = true;
//             }
            
//             // First wait for UP movement to complete
//             if (tierSubstate == 0 && climb.isAtTargetPosition()) {
//                 climb.addEvent([&](){climb.setState(Climb::State::DOWN);});
//                 tierSubstate = 1;
//             }
            
//             // Then wait for DOWN movement to complete
//             if (tierSubstate == 1 && climb.isAtTargetPosition()) {
//                 tierSubstate = 0;
//                 tierStateInitialized = false;
//                 climb.setTier(Climb::Tier::THREE);
//             }
//             break;
            
//         case Climb::Tier::THREE:
//             // Similar substate approach for the final tier
//             if (!tierStateInitialized) {
//                 climb.setState(Climb::State::UP);
//                 tierStateInitialized = true;
//             }
            
//             if (climb.isAtTargetPosition()) {
//                 climb.setState(Climb::State::DOWN);
//                 arm.setState(Arm::State::SCORE);
//             }
//             break;
//     }
// }

void Action::nextArmState() {
    arm.nextState();
}

void Action::prevArmState() {
    arm.prevState();
}

// void Action::setClimbState(Climb::State newState) {
//     climb.setState(newState);
// }

void Action::setArmState(Arm::State newState) {
    arm.setState(newState);
}

Arm::State Action::getArmState() {
    return arm.getState();
}

double Action::getArmAngle() {
    return arm.getAngle();
}

void Action::setArmSpeed(double speed) {
    arm.setSpeed(speed);
}

void Action::setRingColor(Ring::Color ringToKeep) {
    intakeManager.setFilterColor(ringToKeep);
}

ActionState Action::getActionState() {
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

bool Action::getAutonControlFlag(){
    return autoResumeFlag;
}

bool Action::getPullbackFlag(){
    return pullbackFlag;
}

bool Action::getEjectFlag(){
    return ejectFlag;
}


void Action::setRunClimb(bool runClimb){
    this->runClimb = runClimb;
}

bool Action::getRunClimb(){
    return runClimb;
}

void Action::setRunAutoMogoClamp(bool runAutoMogoClamp){
    this->runAutoMogoClamp = runAutoMogoClamp;
}

void Action::setRunArm(bool runArm){
    this->runArm = runArm;
}

void Action::setArmBrakeMode(pros::motor_brake_mode_e_t mode){
    arm.setBrakeMode(mode);
}

void Action::setArmClimb(bool climb){
    arm.setClimb(climb);
}

Action actions(0, Ring::Color::NONE, 13); // Initialize Action object with default values
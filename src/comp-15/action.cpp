#include "robots/comp-15/action.h"

Action::Action(Ring::Color ringToKeep, Controller &controller): currentState(ActionState::IDLE), intakeManager(), arm(0.1, 0, 0){ 
    intakeManager.setFilterColor(ringToKeep);
    this->controller = &controller;
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
        
        // Update arm with new state
        arm.update();
        stateControl();
}

void Action::stateControl() {
    switch(currentState) {
        case ActionState::IDLE:
            if (controller->get_digital(controller_digital_e_t::E_CONTROLLER_DIGITAL_R1)) {
                intakeManager.setIntakeSpeed(1);
            } else if (controller->get_digital(controller_digital_e_t::E_CONTROLLER_DIGITAL_R2)) {
                intakeManager.setIntakeSpeed(-1);
            } else {
                intakeManager.setIntakeSpeed(0);
            }

            if (controller->get_digital(controller_digital_e_t::E_CONTROLLER_DIGITAL_L1)) {
                arm.nextState();
            } else if (controller->get_digital(controller_digital_e_t::E_CONTROLLER_DIGITAL_L2)) {
                arm.prevState();
            }
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
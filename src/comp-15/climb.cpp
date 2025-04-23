#include "robots/comp-15/climb.h"

Climb::Climb(PneumaticsGroup& p, float kP, float kI, float kD): climbPID(kP, kI, kD) {}

void Climb::increment() { curClimbState = (State)(std::min(int(curClimbState) + 1, int(State::NUM_CLIMB_STATES) - 1)); }
void Climb::decrement() { curClimbState = (State)(std::max(int(curClimbState) - 1, 0)); }
void Climb::setState(State newState){ curClimbState = newState; }
void Climb::setTier(Tier newTier){ curTier = newTier; }

void Climb::extendPto(){ pneumatics->climbPTO.extend(); }
void Climb::retractPto(){ pneumatics->climbPTO.retract(); }
void Climb::extendInnerArm(){ pneumatics->innerClimbArms.extend(); }
void Climb::retractInnerArm(){ pneumatics->innerClimbArms.retract(); }
void Climb::extendOuterArm(){ pneumatics->outerClimbArms.extend(); }
void Climb::retractOuterArm(){ pneumatics->outerClimbArms.retract(); }

void Climb::addEvent(std::function<void()> task, uint32_t delayMs){ 
    taskQueue.push({task, delayMs}); 
}

void Climb::update(int pos) {
    lastClimbState = curClimbState; 
    // Task queue handling with non-blocking delays
    if(!override){
        if (!taskQueue.empty()) {
            if (!isWaitingAfterTask) {
                // Execute the next task
                taskQueue.front().task();
                
                // Set up the delay
                lastTaskExecutionTime = pros::millis();
                currentTaskDelayMs = taskQueue.front().delayMs;
                
                if (currentTaskDelayMs > 0) {
                    isWaitingAfterTask = true;
                } else {
                    // No delay needed, remove the task immediately
                    taskQueue.pop();
                }
            } else {
                // Check if we've waited long enough
                if (pros::millis() - lastTaskExecutionTime >= currentTaskDelayMs) {
                    // Delay completed, move to next task
                    taskQueue.pop();
                    isWaitingAfterTask = false;
                }
            }
        }
        // Calculate PID error
        curPos = pos;
        error = climbStateAngles[(int)curClimbState] - getAngle(curPos);
        // Apply PID output to drivetrain
        // drivetrain.move(climbPID.update(error));
    }
}

void Climb::setBrakeMode(pros::motor_brake_mode_e_t mode){
    // drivetrain.set_brake_mode(mode);
}

Climb::Tier Climb::getTier(){ return curTier; }
Climb::State Climb::getState(){ return curClimbState; }
Climb::State Climb::getLastState(){ return lastClimbState; }
double Climb::getAngle(int pos){ return pos/100.0; }

void Climb::setOverride(bool override){ this->override = override; }
bool Climb::isOverride(){ return override; }

bool Climb::isAtTargetPosition() {
    return std::abs(error) <= 2.0; // Consider position reached when error is less than 1.0
}

bool Climb::isPtoExtended() {
    return pneumatics->climbPTO.is_extended();
}
bool Climb::isInnerArmExtended() {
    return pneumatics->innerClimbArms.is_extended();
}

bool Climb::isOuterArmExtended() {
    return pneumatics->outerClimbArms.is_extended();
}
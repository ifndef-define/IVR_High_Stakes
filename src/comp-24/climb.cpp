#include "robots/comp-24/climb.h"

Climb::Climb(PneumaticsGroup& p, float kP, float kI, float kD): climbRot(17), climbPID(kP, kI, kD) {}

void Climb::increment() { curClimbState = (State)(std::min(int(curClimbState) + 1, int(State::NUM_CLIMB_STATES) - 1)); }
void Climb::decrement() { curClimbState = (State)(std::max(int(curClimbState) - 1, 0)); }
void Climb::setState(State newState){ curClimbState = newState; }
void Climb::setTier(Tier newTier){  }

void Climb::extendPto(){ pneumatics->climbPto.extend(); }
void Climb::retractPto(){ pneumatics->climbPto.retract(); }
void Climb::extendPusher(){ pneumatics->climbPusher.extend(); }
void Climb::retractPusher(){ pneumatics->climbPusher.retract(); }

void Climb::addEvent(std::function<void()> task, uint32_t delayMs){ 
    taskQueue.push({task, delayMs}); 
}

void Climb::update(){
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

        // drivetrain.move(climbPID.update(climbStateAngles[(int)curClimbState] - climbRot.get_position()));
    }
}

void Climb::setBrakeMode(pros::motor_brake_mode_e_t mode){
    // drivetrain.set_brake_mode(mode);
}

Climb::Tier Climb::getTier(){ return Tier::ONE; }
Climb::State Climb::getState(){ return curClimbState; }
Climb::State Climb::getLastState(){ return lastClimbState; }
double Climb::getAngle(){ return climbRot.get_position()/100; }

void Climb::setClimbing(bool climbing){ this->climbing = climbing; }
bool Climb::isClimbing(){ return climbing; }
void Climb::setOverride(bool override){ this->override = override; }
bool Climb::isOverride(){ return override; }
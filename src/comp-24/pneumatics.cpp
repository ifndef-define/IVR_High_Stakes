#include "robots/comp-24/pneumatics.h"

PneumaticsGroup::PneumaticsGroup(): 
    mogoClamp(pros::adi::ext_adi_port_pair_t(8, 1), false), // start retracted, 1 is extended
    leftMogoRushArm(pros::adi::ext_adi_port_pair_t(8, 2), false), // start retracted, 1 is extended
    rightMogoRushArm(pros::adi::ext_adi_port_pair_t(8, 6), false), // start retracted, 1 is extended
    mogoRushTeeth(pros::adi::ext_adi_port_pair_t(8, 3), true, true), // start extended, 1 is retracted
    intakeLock(pros::adi::ext_adi_port_pair_t(8, 5), true), // start retracted, 1 is extended
    intakeLift(7, true, true), // start extended, 1 is retract
    innerClimbArms(8, false), // start retracted, 1 is extended
    outerClimbArms(5, false), // start retracted, 1 is extended
    climbPTO(6, false), // start retracted, 1 is extended
    paperLeft(99, false), // start retracted, 1 is extended
    paperRight(99, false) {} // start retracted, 1 is extended
// No other methods, just a grouping class
PneumaticsGroup pneumatics;
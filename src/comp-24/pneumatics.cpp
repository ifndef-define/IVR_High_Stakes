#include "robots/comp-24/pneumatics.h"

PneumaticsGroup::PneumaticsGroup(): 
    mogoClamp({8, 1}, false), // start retracted, 1 is extended
    leftMogoRushArm({8, 2}, false), // start retracted, 1 is extended
    rightMogoRushArm({8, 6}, false), // start retracted, 1 is extended
    mogoRushTeeth({8, 3}, true, true), // start extended, 1 is retracted
    // intakeLock({8, 5}, true), // start retracted, 1 is extended
    intakeLift({8, 4}, true, true), // start extended, 1 is retract
    innerClimbArms(5, false), // start retracted, 1 is extended
    outerClimbArms(6, false), // start retracted, 1 is extended
    climbPTO(8, false), // start retracted, 1 is extended
    paperLeft(99, false), // start retracted, 1 is extended
    paperRight(99, false) {} // start retracted, 1 is extended
// No other methods, just a grouping class
PneumaticsGroup pneumatics;
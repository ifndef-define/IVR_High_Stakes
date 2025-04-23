#include "robots/comp-15/pneumatics.h"

PneumaticsGroup::PneumaticsGroup(): 
    mogoClamp({8, 1}, false),
    leftMogoRushArm({8, 2}, false),
    mogoRushTeeth({8, 3}, false),
    intakeLift({8, 4}, false),
    intakeLock({8, 5}, true),
    rightMogoRushArm({8, 6}, false),
    climbPto({8, 7}, false),
    climbPusher({8, 8}, false) {}
// No other methods, just a grouping class
PneumaticsGroup pneumatics;
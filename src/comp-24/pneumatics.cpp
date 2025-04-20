#include "robots/comp-24/pneumatics.h"

PneumaticsGroup::PneumaticsGroup(): 
    mogoRushLeftArm({8,'F'}, false),
    mogoRushRightArm({8,'D'}, false),
    mogoRushClamp({8,'E'}, false),
    mogoClamp({8,'A'}, false),
    climbPto({8,'B'}, false),
    climbPusher({8,'C'}, false) {}

// No other methods, just a grouping class
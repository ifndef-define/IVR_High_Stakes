#include "robots/comp-24/pneumatics.h"

PneumaticsGroup::PneumaticsGroup(): 
    mogoRushArm('F', false),
    mogoRushClamp('C', false),
    mogoClamp('H', false),
    doinker('G', false), 
    intakeLift('E', false) {}

// No other methods, just a grouping class
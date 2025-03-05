#include "robots/comp-24/pneumatics.h"

PneumaticsGroup::PneumaticsGroup(): 
    mogoRushLeftArm('E', false),
    mogoRushRightArm('H', false),
    mogoRushClamp('F', false),
    mogoClamp('G', false) {}

// No other methods, just a grouping class
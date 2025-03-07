#include "robots/comp-24/pneumatics.h"

PneumaticsGroup::PneumaticsGroup(): 
    mogoRushLeftArm('G', false),
    mogoRushRightArm('H', false),
    mogoRushClamp('E', false),
    mogoClamp('F', false) {}

// No other methods, just a grouping classa
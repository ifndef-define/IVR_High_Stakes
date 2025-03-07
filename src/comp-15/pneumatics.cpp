#include "robots/comp-15/pneumatics.h"

PneumaticsGroup::PneumaticsGroup(): 
    mogoRushLeftArm('H', false),
    mogoRushRightArm('G', false),
    mogoRushClamp('E', false),
    mogoClamp('F', false) {}

// No other methods, just a grouping classa
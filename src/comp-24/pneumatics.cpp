#include "robots/comp-24/pnuematics.h"

PneumaticsGroup::PneumaticsGroup(): 
    mogoRushArm('G', false), 
    mogoRushClamp('H', false), 
    mogoClamp('I', false) {}

// No other methods, just a grouping class
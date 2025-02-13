#include "robots/comp-24/pnuematics.h"

PneumaticsGroup::PneumaticsGroup(): 
    mogoRushArm('F', false), 
    mogoRushClamp('C', false), 
    mogoClamp('H', false), 
    doinker('G', false) {}
    

// No other methods, just a grouping class
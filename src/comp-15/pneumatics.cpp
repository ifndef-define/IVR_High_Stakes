#include "robots/comp-15/pnuematics.h"

PneumaticsGroup::PneumaticsGroup(): 
    mogoRushArm('A', false), 
    mogoRushClamp('B', false), 
    mogoClamp('G', false), 
    doinker('H', false) {}
    

// No other methods, just a grouping class
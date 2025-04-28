#include "robots/comp-15/pneumatics.h"

PneumaticsGroup::PneumaticsGroup(): 
    mogoClamp(pros::adi::ext_adi_port_pair_t(9, 6), false),
    leftMogoRushArm(pros::adi::ext_adi_port_pair_t(9, 7), false),
    mogoRushTeeth(pros::adi::ext_adi_port_pair_t(9, 3), false, true),
    intakeLift(7, false),
    rightMogoRushArm(pros::adi::ext_adi_port_pair_t(9, 99), false),
    leftClimbWing(99, false),
    rightClimbWing(99, false),
    innerClimbArms(8, false),
    outerClimbArms(5, false),
    climbPTO(6, false) {}
// No other methods, just a grouping class
PneumaticsGroup pneumatics;
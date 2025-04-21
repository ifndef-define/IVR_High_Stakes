#include "robots/comp-15/pneumatics.h"

PneumaticsGroup::PneumaticsGroup(): 
    mogoClamp(pros::adi::ext_adi_port_pair_t(8, 1), false),
    leftMogoRushArm(pros::adi::ext_adi_port_pair_t(8, 2), false),
    mogoRushTeeth(pros::adi::ext_adi_port_pair_t(8, 3), false),
    intakeLift(pros::adi::ext_adi_port_pair_t(8, 4), false),
    intakeLock(pros::adi::ext_adi_port_pair_t(8, 5), true),
    rightMogoRushArm(pros::adi::ext_adi_port_pair_t(8, 6), false) {}
// No other methods, just a grouping class

PneumaticsGroup pneumatics;
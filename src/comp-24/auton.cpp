#include "robots/comp-24/auton.h"

void auton1(bool isRed) {
    chassis.setPose(0, 0, 0);
    // turn to face heading 90 with a very long timeout
    chassis.turnToHeading(90, 100000);
}
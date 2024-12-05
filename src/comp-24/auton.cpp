#include "robots/comp-24/auton.h"

void auton1() {
    while(1){
        lcd::print(0, "X: %f", chassis.getPose().x);
        lcd::print(1, "Y: %f", chassis.getPose().y);
        delay(15);
    }
    // chassis.moveToPoint(0,72,100000);
    // turn to face heading 90 with a very long timeout
    // chassis.turnToHeading(90, 2000);
    // chassis.turnToHeading(180, 2000);
    // chassis.turnToHeading(270, 2000);
    // chassis.turnToHeading(90, 2000);
    // chassis.turnToHeading(0, 2000);
}
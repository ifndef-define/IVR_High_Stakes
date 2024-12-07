#include "robots/comp-24/auton.h"

// ASSET(mogoRushMid_txt);
ASSET(comp_black_1_txt);
ASSET(comp_black_2_txt);
void auton1() {
    // while(1){
    //     lcd::print(0, "X: %f", chassis.getPose().x);
    //     lcd::print(1, "Y: %f", chassis.getPose().y);
    //     delay(15);
    // }
    // chassis.moveToPoint(0,72,100000);
    // turn to face heading 90 with a very long timeout
    // chassis.turnToHeading(90, 10000);
    // chassis.turnToHeading(180, 2000);
    // chassis.turnToHeading(270, 2000);
    // chassis.turnToHeading(90, 2000);
    // chassis.turnToHeading(0, 2000);
    // chassis.moveToPoint(0, 24*2, 10000);
    // chassis.moveToPose(0, 48, 0, 10000);
    chassis.follow(comp_black_1_txt, 15, 3000, true, false);
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    delay(500);
    chassis.follow(comp_black_2_txt, 15, 3000, false, false);
    // clampPiston.extend();
    // chassis.moveToPose(-58.531, -23.912, 90, 4000);
}
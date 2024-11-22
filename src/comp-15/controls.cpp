#include "robots/comp-15/controls.h"

void teleOp(){
    int z, y, L1, L2, R1, R2, buttonA, buttonB, buttonX, buttonY;
    while(true){
        L1 = ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
		L2 = ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
        R1 = ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
		R2 = ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
		buttonA = ctrl_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A);
		buttonB = ctrl_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B);
		buttonX = ctrl_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X);
		buttonY = ctrl_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y);
		
		y = ctrl_master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		z = ctrl_master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)/1.5;

        leftDrive.move(y + z);
		rightDrive.move(y - z);

        if(buttonB){
            mogoRush.toggle();
        }
        if(buttonX){
            clampPiston.toggle();
        }
        if(buttonY){
            doinker.toggle();
        }

        intake.manualControl();
        arm.manualControl();

        pros::delay(15);
    }
}
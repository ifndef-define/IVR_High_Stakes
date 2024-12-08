#include "robots/comp-24/controls.h"
#include "robots/comp-24/arm.h"

void teleOp(){
    int z, y, L1, L2, R1, R2, buttonA, buttonB, buttonX, buttonY, buttonUp, buttonDown, buttonLeft, buttonRight;
    bool bruh = true;
    mogoRushClamp.retract();
    mogoRushReach.retract();
    // intake.setAutonControlFlag(false);
    while(true){
        L1 = ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
		L2 = ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
        R1 = ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
		R2 = ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_R2);

		buttonA = ctrl_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A);
		buttonB = ctrl_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B);
		buttonX = ctrl_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X);
		buttonY = ctrl_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y);

        buttonUp = ctrl_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP);
        buttonDown = ctrl_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN);
        buttonLeft = ctrl_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT);
        buttonRight = ctrl_master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT);

		// y = ctrl_master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		// z = ctrl_master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)/1.5;
        // leftDrive.move(y + z);
		// rightDrive.move(y - z);

        chassis.arcade(y, z);

        if(buttonB){
            intake.toggleColorSort();
        }
        if(buttonY){
            mogoClamp.toggle();
        }
        if(buttonX){
            doinker.toggle();
        }

        if(buttonUp){
            bruh = !bruh;
            intake.setAutonControlFlag(bruh);
        }
        
        // intake.manualControl();
        arm.manualControl();
        delay(15);
    }
}

void debug(){
    while(1){
    // TELEMETRY //
        // lcd::print(2, "X: %f", chassis.getPose().x);
        // lcd::print(3, "Y: %f", chassis.getPose().y);
        // lcd::print(4, "Theta: %f", chassis.getPose().theta);
        // lcd::print(1, "Arm: %f", arm.normalizeAngle(armRot.get_position()));

        pros::lcd::print(1, "Arm State: %d", arm.getState());
        pros::lcd::print(2, "Arm Pos: %f", arm.getNormalizedAngle());
        // pros::lcd::print(3, "PID Output: %f", armPID.update(targetPosition[armState], armRot->get_position()));
        pros::lcd::print(3, "Intake: %s", intake.getAutonControlFlag());

        pros::delay(15);
    }
}
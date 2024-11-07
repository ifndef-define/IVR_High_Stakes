#include "pookie/controls.h"

// Will add driver profiles later
void controls() {
    // for debugging
	// pros::Task odom_task{[=] {
	// 	while (1) {
	// 		tank_drive_15.getOdom().updatePosition();
	// 		pros::delay(50);
	// 	}
	// }};

    pros::lcd::set_text(1, "Running Controls");
    tank_drive_15.change_drive_mode(0);

    while(1) {
        //ACTIVATE DRIVE
        
        tank_drive_15.toggle_drive_mode();

        //INTAKE CONTROLS
        if (ctrl_master.get_digital(BUTTON_R1)){
            Intake::getInstance()->set_power(-12000);
            Pneumatics::getInstance()->getIntake()->off();
        }
        else if (ctrl_master.get_digital(BUTTON_R2)){
            Intake::getInstance()->set_power(12000);
            Pneumatics::getInstance()->getIntake()->on();
        } else {
            Intake::getInstance()->set_power(0);
        }

        //PNEUMATICS CONTROLS
        if(ctrl_master.get_digital_new_press(BUTTON_L2)) {
            Pneumatics::getInstance()->getWings()->toggle();
        }

        if(ctrl_master.get_digital_new_press(BUTTON_L1)) {
            Pneumatics::getInstance()->getIntake()->toggle();
        }   

        if(ctrl_master.get_digital_new_press(BUTTON_UP)) {
            Pneumatics::getInstance()->getIntake()->on();

            Pneumatics::getInstance()->getTopHang()->toggle();
            // pros::delay(25);
        }

        if(ctrl_master.get_digital_new_press(BUTTON_DOWN)) {
            Pneumatics::getInstance()->getSideHang()->toggle();
            // pros::delay(25);
        }

        // if(ctrl_master.get_digital_new_press(BUTTON_X)) {
        //     // tank_drive_15. = 200;

        // }
        
        pros::delay(15);
    }
}

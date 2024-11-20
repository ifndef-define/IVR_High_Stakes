#include "main.h"
#include "robots/skills-15/auton.h"
#include "robots/skills-15/controls.h"
#define i_waitUntil(condition)          \
  do {                                \
    pros::delay(5);                   \
  } while (!(condition))

IMU imu1(11); 
IMU imu2(20);
dualStackedIMU dual(imu1,imu2); 
/* First method to run. Should last only a few seconds max. */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::print(0, "Skills 15 Bot");
	dualStackedIMU.reset(true);
}

/* Runs when robot is disabled from competition controller after driver/auton */
void disabled() {}

/* If connected to competition controller, this runs after initialize */
void competition_initialize() {}

/* Autonmous Method */
void autonomous() {}

/* Driver Control. Runs default if not connected to field controler */
void opcontrol() {
	dual._check_active();
	pros::lcd::print(4,dual.get()._heading);
}
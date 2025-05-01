/**
 * \file main.h
 *
 * Contains common definitions and header files used throughout your PROS
 * project.                            Hahaha
 *                                      vvvv
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

#pragma GCC diagnostic ignored "-Wdeprecated-enum-enum-conversion"

/**
 * Add PROS header files here;
 * NOTE: There shouldn't be anything else here
 */
#include "api.h" // DO NOT REMOVE //
#include "pros/apix.h"
#include "liblvgl/lvgl.h"
///////////////////////////////////////

/**
 * Add C++ headers here
 */
#include <iostream>
#include <chrono>
#include <string>
#include <math.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <initializer_list>
#include <functional>
///////////////////////////////////////

/**
 * Add global defines/constants here;
 */
#ifndef M_PI // PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PI_2 // PI/2
#define M_PI_2 1.57079632679489661923
#endif
#ifndef M_PI_4 // PI/4
#define M_PI_4 0.78539816339744830962
#endif
#ifndef M_E // e
#define M_E 2.71828182845904523536
#endif
#ifndef M_G // Acceleration due to gravity
#define G 9.81
#endif
///////////////////////////////////////

/**
 * Add other header files here;
 * NOTE: Only add imported libraries' headers here
 */
// #include "dualEnable.h"
///////////////////////////////////////

/**
 * When multitasking, these values indicate the priority levels of tasks, and
 * how often they will run relative to other tasks.
 * 
 * NOTE: Default priority level is 8 on scale of 0-16
 *  0 means task will not run at all
 *  16 means task will be the only task running
*/
#define TASK_PRIORITY_LOW 6
#define TASK_PRIORITY_HIGH 11
///////////////////////////////////////

// Vex defined function, might as well have just in case
#define i_waitUntil(condition)          \
  do {                                \
    pros::delay(5);                   \
  } while (!(condition))
///////////////////////////////////////

/**
 * Custom namespace for converting between units. Please note that we want all our 
 * units to be in imperial units (inches, feet, etc.). This is the easiest way to
 * visualize locations and distances on the field. Angles will be in degrees, but 
 * can be converted to radians when performing trigonometric calculations.
 */
#ifndef CONVERT_NAMESPACE

#define CONVERT_NAMESPACE
namespace convert {
  // Convert degrees to radians
  static double degToRad(double degrees) {
    return degrees * (M_PI / 180);
  }

  // Convert radians to degrees
  static double radToDeg(double radians) {
    return radians * (180 / M_PI);
  }

  static double normalizeAngle(double angle) {
    return (angle/100 - (std::floor((angle/100 + 180.0)/360.0))*360.0);
  }

  // Convert inches to meters
  [[deprecated("Prefer to calculate in inches/feet only")]]
  static double inToM(double inches) {
    return inches * 0.0254;
  }
  
  // Convert meters to inches
  [[deprecated("Prefer to calculate in inches/feet only")]]
  static double mToIn(double meters) {
    return meters * 39.3701;
  }
  
  // Convert inches to feet
  static double inToFt(double inches) {
    return inches * 0.0833333;
  }

  // Convert feet to inches
  static double ftToIn(double feet) {
    return feet * 12;
  }

  // Convert feet to meters
  [[deprecated("Prefer to calculate in inches/feet only")]]
  static double ftToM(double feet) {
    return feet * 0.3048;
  }

  // Convert meters to feet
  [[deprecated("Prefer to calculate in inches/feet only")]]
  static double mToFt(double meters) {
    return meters * 3.28084;
  }

  // Convert tile coordinates to meters
  [[deprecated("Prefer to calculate in inches/feet only")]]
  static double tileToM(double tiles) {
    return tiles * 0.61538;
  }

  // Convert meters to tile coordinates
  [[deprecated("Prefer to calculate in inches/feet only")]]
  static double mToTile(double meters) {
    return meters * 1.625;
  }

  // Convert tile coordinates to inches
  [[deprecated("Prefer to calculate in inches/feet only")]]
  static double tileToIn(double tiles) {
    return tiles * 24;
  }

  // Convert inches to tile coordinates
  [[deprecated("Prefer to calculate in inches/feet only")]]
  static double inToTile(double inches) {
    return inches * 0.0416667;
  }
} // namespace convert
#endif

/**
 * If defined, some commonly used enums will have preprocessor macros which give
 * a shorter, more convenient naming pattern. If this isn't desired, simply
 * comment the following line out.
 */
#define ILLINI_SIMPLE_NAMES

#ifdef ILLINI_SIMPLE_NAMES
#define CONTROLLER_MASTER   pros::E_CONTROLLER_MASTER
#define CONTROLLER_PARTNER  pros::E_CONTROLLER_PARTNER
#define GEARSET_06          pros::v5::MotorGears::blue
#define GEARSET_18          pros::v5::MotorGears::green
#define GEARSET_36          pros::v5::MotorGears::red
#define BRAKE_BRAKE         pros::E_MOTOR_BRAKE_BRAKE
#define BRAKE_COAST         pros::E_MOTOR_BRAKE_COAST
#define BRAKE_HOLD          pros::E_MOTOR_BRAKE_HOLD
#define RIGHT_X_AXIS        pros::E_CONTROLLER_ANALOG_RIGHT_X
#define RIGHT_Y_AXIS        pros::E_CONTROLLER_ANALOG_RIGHT_Y
#define LEFT_X_AXIS         pros::E_CONTROLLER_ANALOG_LEFT_X
#define LEFT_Y_AXIS         pros::E_CONTROLLER_ANALOG_LEFT_Y
#define BUTTON_A            pros::E_CONTROLLER_DIGITAL_A
#define BUTTON_B            pros::E_CONTROLLER_DIGITAL_B
#define BUTTON_X            pros::E_CONTROLLER_DIGITAL_X
#define BUTTON_Y            pros::E_CONTROLLER_DIGITAL_Y
#define BUTTON_UP           pros::E_CONTROLLER_DIGITAL_UP
#define BUTTON_DOWN         pros::E_CONTROLLER_DIGITAL_DOWN
#define BUTTON_LEFT         pros::E_CONTROLLER_DIGITAL_LEFT
#define BUTTON_RIGHT        pros::E_CONTROLLER_DIGITAL_RIGHT
#define BUTTON_R1           pros::E_CONTROLLER_DIGITAL_R1
#define BUTTON_R2           pros::E_CONTROLLER_DIGITAL_R2
#define BUTTON_L1           pros::E_CONTROLLER_DIGITAL_L1
#define BUTTON_L2           pros::E_CONTROLLER_DIGITAL_L2
#define LINK_TRANSMIT       pros::E_LINK_TRANSMITTER
#define LINK_RECEIVE        pros::E_LINK_RECIEVER
#else
// Use PROS Default Simple Name List //
#define PROS_USE_SIMPLE_NAMES
#endif

/**
 * If you find doing pros::Motor() to be tedious and you'd prefer just to do
 * Motor, you can use the namespace with the following commented out line.
 *
 * IMPORTANT: Only the okapi or pros namespace may be used, not both
 * concurrently! The okapi namespace will export all symbols inside the pros
 * namespace.
 */
using namespace pros;
using namespace std;

/**
 * Prototypes for the competition control tasks are redefined here to ensure
 * that they can be called from user code (i.e. calling autonomous from a
 * button press in opcontrol() for testing purposes).
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 * 
 * Runs everytime program is called
 */
void initialize(void);

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 * 
 * Runs only if connected to Field Network or Competition Switch
 */
void competition_initialize(void);

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 * 
 * Runs if field controller or competition switch sets mode to disable
 */
void disabled(void);

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 * 
 * Runs if Competition Switch or Field Network start autonomous
 */
void autonomous(void);

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 * 
 * Runs if Competition Switch or Field Network start driver control
 */
void opcontrol(void);

#ifdef __cplusplus
}
#endif

#endif  // _PROS_MAIN_H_

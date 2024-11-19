#include "main.h"
// #include <deque>
#include <queue>
using namespace std;
using namespace pros;
#include <iostream>
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

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
 */
void autonomous() {
}

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
 */


/*
Motor Ports
=============
Right Front: 3
Right Mid: 4
Right Back Top: -1
Right Back Bottom: 5
Left Front: -10
Left Mid: -9
Left Back Top: 7
Left Back Bottom: -8
Left Intake: 17
Right Intake: -13
Wall Arm: -14
Distance: 18
Optical: 15
IMU Left: 20
IMU Right: 11
Wall Arm Rotation: 21
X Enc: A-B
rY Enc: C-D
lY Enc: E-F
Mogo Mech: G
*/

MotorGroup rightDrive({3, 4, -1, 5}, MotorGears::blue);
MotorGroup leftDrive({-10, -9, 7, -8}, MotorGears::blue);

MotorGroup intake{-13, 17};
Motor arm(-14, MotorGears::red);

Rotation armRot(-21);

Distance dist(18);
Optical color(15);
IMU lIMU(20);
IMU rIMU(11);

// adi::Encoder xEnc('A', 'B', true);
// adi::Encoder rYEnc('C', 'D', true);
// adi::Encoder lYEnc('E', 'F');
adi::Pneumatics mogoRush('F', false);
adi::Pneumatics clampPist('G', false);
adi::Pneumatics doinker('H', false);


bool arcadeDrive = false;

int armState = 0;
int lastArmState = armState;

pros::Controller master(pros::E_CONTROLLER_MASTER);

////////// PID ////////////

// double const kU = .03; // Increase until steady continuous oscillations.
// double const pU = 1.0; // Time to do a full cycle from a point back to itself.
// double const kP = 0.6*kU
// double const kD = 0.075*kU*pU
// double const kI = 1.2*kU/pU

double const kP = 0.045;
double const kD = 0.11;
double const kI = 0.0;

double error = 0;
double integralI = 0;
double lastError = 0;
double derivative = 0;
double output = 0;



void armMoveToPosition(int target){ //move arm to positions
	error = target - armRot.get_position();
	integralI += error;
	if (error == 0 || error > target)
		integralI = 0;
	derivative = error - lastError;
	lastError = error;
	output = kP*error + integralI*kI + kD*derivative;
	if (output > 127) output = 127;
        else if (output < -127) output = -127;
	arm.move(output);
}
//////////////////////////////////

////////// Arm ////////////

void setArmState(){
	switch(armState) {
		case 0:
			armMoveToPosition(0);
			break;
		case 1:
			armMoveToPosition(1600);
			break;
		case 2:
			armMoveToPosition(11000);
			break;
		case 3:
			armMoveToPosition(14000);
			break;
		case 4:
			if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
				if(armRot.get_position() >= 20000){
					arm.move(-20);
				} else {
					arm.move(127);
				}
			} else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
				if(armRot.get_position() <= 0){
					arm.move(0);
				} else {
					arm.move(-127);
				}
			} else {
				arm.move(0);
			}
			break;
	}
}
bool intakeArmFlag = false;
void incrementArmState(){
	if (armState == 1) {
        armState = 2;
        intakeArmFlag = true;
	} else {
		armState++;
		if(armState>3){
			armState = 0;
		}
	}
}

void decrementArmState(){
	armState--;
	if(armState<0){
		armState = 3;
	}
}
//////////////////////////////////

bool colorToKeep = false; // 0 for red, 1 for blue
bool isEjecting = false;
void toggleColor() {
	colorToKeep = !colorToKeep;
}	

void ringTask() {
	typedef enum {
		NONE,
		RED,
		BLUE
	} RingColor;
	queue<RingColor> ringQueue1;
	vector<int> blueRange = {200, 240};
	vector<int> redRange = {0, 30};
	RingColor detectedRing = NONE;
	int tolerance = 10;

	bool ringTop = false;
	bool waitRingFlag = false;
	int pos = 0;

	// short int ringFlag = 0b00000000;

	delay(1000);

	while(true) {
		// Detect Ring //
		static bool colorFlag = false;
		if (color.get_hue() >= blueRange[0] && color.get_hue() <= blueRange[1])
			detectedRing = BLUE;
		else if (color.get_hue() >= redRange[0] && color.get_hue() <= redRange[1])
			detectedRing = RED;
		else { detectedRing = NONE; }

		if(!waitRingFlag && detectedRing != NONE) {
			ringQueue1.push(detectedRing);
			waitRingFlag = true;
		} else if (detectedRing == NONE) {
			waitRingFlag = false;
		}
		// Sort //
		static bool ringProcessed = false;

		if (!ringProcessed && dist.get() <= 100) {
			ringTop = true;
			ringProcessed = true;
		} else if (ringProcessed && dist.get() > 100) {
			ringProcessed = false;
		}

		if (ringTop) {
			if ((ringQueue1.front() == RED && !colorToKeep) || (ringQueue1.front() == BLUE && colorToKeep)) {
				isEjecting = true;
			}
			if (!ringQueue1.empty()) {
				ringQueue1.pop();
			}
			ringTop = false;
		}		

		pros::lcd::print(5, "Hue: %f | Ring: %d", color.get_hue(), (int)detectedRing);
		if (!ringQueue1.empty())
			pros::lcd::print(6, "RQ: 1: %d, 2: %d | RT: %d", (int)ringQueue1.front(), (int)ringQueue1.back(), ringTop);
		else 
			pros::lcd::print(6, "RQ: Empty | RT: %d", ringTop);
		pros::lcd::print(7, "Dist: %d", dist.get());
		pros::delay(15);
	}
}

void opcontrol() {

	// pros::Task ringThread(ringTask);
	color.set_led_pwm(100);

	arm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	intake.set_gearing_all(MotorGears::blue);
	rIMU.reset();
	lIMU.reset();
	armRot.set_position(0); //-15 degrees default position

	int z, y, L1, L2, R1, R2, buttonA, buttonB, buttonX, buttonY;
	bool intakeToggle = false;
	bool toggle1 = false;
	bool toggle2 = false;
	bool toggle3 = false;
	float xEncVal;
	float rYEncVal;
	float lYEncVal;

	// Variables to store previous motor speeds
	int prevLeftSpeed = 0;
	int prevRightSpeed = 0;

	int pauseCounter1 = 0;
	int pauseCounter2 = 0;

	while(1){
		L1 = master.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
		L2 = master.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
		R1 = master.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
		R2 = master.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
		buttonA = master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A);
		buttonB = master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B);
		buttonX = master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X);
		buttonY = master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y);
		
		y = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		z = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)/1.5;

		// if(master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)){
		// 	y /= 2;
		// 	z/= 1.5;
		// }
		// if(((rIMU.get_roll()+lIMU.get_roll())/2)<-15){
		// 	y = -127;
		// 	z = 0;
		// }

		// Desired speeds
		int targetLeftSpeed = y + z;
		int targetRightSpeed = y - z;
		
		leftDrive.move(targetLeftSpeed);
		rightDrive.move(targetRightSpeed);

		// Update previous speeds
		prevLeftSpeed = targetLeftSpeed;
		prevRightSpeed = targetRightSpeed;

		/////// Pneumatics ////////

		if(buttonB)
			toggle1 = !toggle1;
		if(buttonX)
			toggle2 = !toggle2;
		if(buttonY)
			toggle3 = !toggle3;

		if(toggle1){
			mogoRush.extend();
		} else {
			mogoRush.retract();
		}

		if(toggle2){
			clampPist.extend();
		} else {
			clampPist.retract();
		}

		if(toggle3){
			doinker.extend();
		} else {
			doinker.retract();
		}
		///////////////////////////////////

		/////// INTAKE ////////
		// if(R1)
		// 	intakeToggle = !intakeToggle;
		// if(intakeToggle){
		if(!isEjecting){
			if(R1){
				// sift(1, intakeToggle);
				if(master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)){
					intake.move(127/2);
				} else {
					intake.move(127);
				}
			} else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
				// intakeToggle = false;
				if(master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)){
					intake.move(-127/3);
				} else {
					intake.move(-127);
				}
			} else {
				intake.brake();
			}
		} else {
			if(armState <= 1){
				armState = 0;
			}
			intake.move(127);
			if(pauseCounter2<10){ //10*15 = 150ms
				pauseCounter2++;
			} else {
				pauseCounter2 = 0;
				intake.brake();
				isEjecting = false;
			}
		}

		if(intakeArmFlag){
			intake.move(-127);
			if(pauseCounter1<7){ //7*15 = 105ms
				pauseCounter1++;
			} else {
				pauseCounter1 = 0;
				intake.brake();
				intakeArmFlag = false;
			}
		}
		///////////////////////////////////////////

		////// ARM //////
		if(!isEjecting){
			if(master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)){
				armState=4;
			} else if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
				incrementArmState();
			} else if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
				decrementArmState();
			}
		}

		setArmState();
		//////////////////////

		/////////// Telemetry ///////////

		// xEncVal = xEnc.get_value()*(M_PI*1.36)/8192; //8192 ticks per revolution 
		// rYEncVal = rYEnc.get_value()*(M_PI*1.36)/8192; //8192 ticks per revolution 
		// lYEncVal = lYEnc.get_value()*(M_PI*1.36)/8192; //8192 ticks per revolution 
		pros::lcd::print(0, "X: %f | rY: %f | lY: %f", xEncVal, rYEncVal, lYEncVal);
		pros::lcd::print(1, "Arm Rot %d", armRot.get_position());
		pros::lcd::print(2, "Color Keep: %s", colorToKeep ? "Blue" : "Red");
		// pros::lcd::print(2, "Arm: %f | Prx: %d | Dis: %d", armRot.get_position(), color.get_proximity(), dist.get_distance());
		// pros::lcd::print(3, "Arm State: %d", armState);
		pros::lcd::print(3, "Intake pos: %d", intake.get_position());
		
		// pros::lcd::print(4, "Heading: %f", (rIMU.get_rotation()+lIMU.get_rotation())/2);
		// pros::lcd::print(5, "Ejecting: %d", isEjecting);
		// pros::lcd::print(4, "Color: %f", color.get_hue());
		////////////////
		
		pros::delay(15);
	}
}

// bool colorFlag = false;
// bool distFlag = false;
// std::deque<bool> ringQueue;  // Queue to store the color of the detected rings
// int pos = 0;
// int pauseCounter = 0;
// // Sift function
// void sift(bool allianceColor, bool intakeToggle) {  // 0 for red, 1 for blue
// 	if(!isEjecting){
// 		//Reset flags when no ring is detected
// 		if (color.get_proximity() < 180) {
// 			colorFlag = false;
// 		}
// 		if (dist.get_distance() > 70) {
// 			distFlag = false;
// 			intake.move(127);
// 		}

// 		// Detect ring color once when proximity is within range
// 		if (color.get_proximity() >= 180 && !colorFlag) {
// 			if (color.get_hue() > 180 && color.get_hue() < 270) {  // Detect Blue
// 				pros::lcd::print(7, "Blue Detected");
// 				ringQueue.push_back(1);  // Push Blue to queue
// 			} else {  // Detect Red
// 				pros::lcd::print(7, "Red Detected");
// 				ringQueue.push_back(0);  // Push Red to queue
// 			}
// 			colorFlag = true;  // Mark color as detected
// 		}

// 		// Handle distance detection and act accordingly when a ring is at the top
// 		if (dist.get_distance() <= 70 && !distFlag) {
// 			if (ringQueue.front() != allianceColor) {
// 				pros::lcd::print(5, "Ejecting");
// 				isEjecting = true;  // Mark ejection in progress
// 				pauseCounter = 7; // Pause for 15*7=105ms
// 				pos = intake.get_position();
// 			} else {
// 				pros::lcd::print(5, "Scoring");
// 				if(!ringQueue.empty()){
// 					ringQueue.pop_front(); // Remove the ring from queue
// 				}
// 			}  
// 			distFlag = true;  // Mark distance detection
// 		}
// 	} else {
// 		if(intake.get_position()<pos+65){
// 			intake.move(127);
// 		} else {
// 			intake.move(-127);
// 			if(pauseCounter>0){
// 				pauseCounter--;
// 			} else {
// 				isEjecting = false;
// 				if(!ringQueue.empty()){
// 					ringQueue.pop_front(); // Remove the ring from queue
// 				}
// 			}
// 		}
// 	}
// }
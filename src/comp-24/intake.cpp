#include "robots/comp-24/intake.h"
#include "robots/comp-24/arm.h"

pros::MotorGroup *Intake::intake;
bool Intake::isEjecting;
int Intake::pauseCounter1;
int Intake::pauseCounter2;

Intake::Intake(pros::MotorGroup *intakeMotor) {
    intake = intakeMotor;
    isEjecting = false;
    pauseCounter1 = 0;
    pauseCounter2 = 0;
    runColorSort = 1;
    colorToKeep = 0;
    autonControlFlag = 0;
}

void Intake::setVoltage(int volt) {
    intake->move(volt);
}

void Intake::setRpm(int rpm) {
    intake->move_velocity(rpm);
}

void Intake::setRelative(double position, int rpm) {
    intake->move_relative(position, rpm);
}

void Intake::brake() {
    intake->brake();
}

void Intake::pullBack() {
    if (arm.getIntakePullBackFlag()){
        intake->move(-127);
        if (pauseCounter1 < 7){ // 7*15 = 105ms
            pauseCounter1++;
        } else {
            pauseCounter1 = 0;
            intake->brake();
            arm.setIntakePullBackFlag(false);
        }
    }
}

void Intake::manualControl(){
    if (!isEjecting){
        if (ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
            if (ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)){
                intake->move(127 / 2);
            } else {
                intake->move(127);
            }
        } else if (ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {

            if (ctrl_master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
                intake->move(-127 / 3);
            } else {
                intake->move(-127);
            }
        } else {
            intake->brake();
        }
    } else {
        // if (arm.getState() <= 1) {
        //     arm.setState(0);
        // }
        intake->move(-127);
        if (pauseCounter2 < 10) { // 10*15 = 150ms
            pauseCounter2++;
        } else {
            pauseCounter2 = 0;
            intake->brake();
            isEjecting = false;
        }
    }
    pullBack();
}

void Intake::autonControl(int speed){
    if(autonControlFlag){
        if (!isEjecting){
            move(speed);
        } else {
            if (arm.getState() <= 1) {
                arm.setState(0);
            }
            intake->move(-speed);
            if (pauseCounter2 < 10) { // 10*15 = 150ms
                pauseCounter2++;
            } else {
                pauseCounter2 = 0;
                intake->brake();
                isEjecting = false;
            }
        }
    }
    pullBack();
}

bool Intake::getIsEjecting(){
    return isEjecting;
}

void Intake::setColorToKeep(bool cToKeep){
	colorToKeep = cToKeep;
}

void Intake::toggleColorSort(){
    runColorSort = !runColorSort;
}

void Intake::setAutonControlFlag(bool flag){
    autonControlFlag = flag;
}

void Intake::ringTask() {
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
        if(runColorSort){
		    // Detect Ring //
            static bool colorFlag = false;
            if (intakeColor.get_hue() >= blueRange[0] && intakeColor.get_hue() <= blueRange[1])
                detectedRing = BLUE;
            else if (intakeColor.get_hue() >= redRange[0] && intakeColor.get_hue() <= redRange[1])
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

            if (!ringProcessed && intakeDist.get() <= 80) {
                ringTop = true;
                ringProcessed = true;
            } else if (ringProcessed && intakeDist.get() > 80) {
                ringProcessed = false;
            }

            if (ringTop) {
                if ((ringQueue1.front() == RED && colorToKeep) || (ringQueue1.front() == BLUE && !colorToKeep)) {
                    delay(60);
                    isEjecting = true;
                }
                if (!ringQueue1.empty()) {
                    ringQueue1.pop();
                }
                ringTop = false;
            }		

            autonControl(127);

            pros::lcd::print(5, "Hue: %f | Ring: %d", intakeColor.get_hue(), (int)detectedRing);
            if (!ringQueue1.empty())
                pros::lcd::print(6, "RQ: 1: %d, 2: %d | RT: %d", (int)ringQueue1.front(), (int)ringQueue1.back(), ringTop);
            else 
                pros::lcd::print(6, "RQ: Empty | RT: %d", ringTop);
            pros::lcd::print(7, "Dist: %d", intakeDist.get());

        }
		pros::delay(15);
	}
}

void Intake::ringTaskNew() {
    typedef enum {
		NONE,
		RED,
		BLUE
	} RingColor;
	vector<int> blueRange = {200, 240};
	vector<int> redRange = {0, 30};
	RingColor detectedRing = NONE;

    while(true) {
        if(runColorSort){
            if(intakeColor.get_proximity() > 200) {
                if (intakeColor.get_hue() >= blueRange[0] && intakeColor.get_hue() <= blueRange[1]) { detectedRing = BLUE; }
                else if (intakeColor.get_hue() >= redRange[0] && intakeColor.get_hue() <= redRange[1]) { detectedRing = RED; }
                else { detectedRing = NONE; }
                
                if((detectedRing == RED && colorToKeep) || (detectedRing == BLUE && !colorToKeep)) { 
                    delay(60);
                    isEjecting = true;
                }
            }

            autonControl(127);
        }
    }
}
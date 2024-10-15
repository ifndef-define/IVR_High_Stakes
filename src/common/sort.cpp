#include <sort.h>

sort::sort(pros::Motor &rIntake, pros::Optical &rColor, pros::Distance &rDist, bool allianceColor): intake(rIntake), color(rColor), dist(rDist){
    sort_consts_.alliance = allianceColor;
}

void sort::update() {
    if (color.get_proximity() < 180) {
		sort_state_.colorFlag = false;
	}
	if (dist.get_distance() > 120) {
		sort_state_.distFlag = false;
		sort_state_.isEjecting = false;
	}
}

int sort::sift(){
	if(!sort_state_.isEjecting){
		if (color.get_proximity() >= 180 && !sort_state_.colorFlag) {
			if (color.get_hue() > 180 && color.get_hue() < 270) {  // Detect Blue
				pros::lcd::print(7, "Blue Detected");
				sort_state_.ringQueue.push_back(1);  // Push Blue to queue
			} else {  // Detect Red
				pros::lcd::print(7, "Red Detected");
				sort_state_.ringQueue.push_back(0);  // Push Red to queue
			}
			sort_state_.colorFlag = true;  // Mark color as detected
		}
		// Handle distance detection and act accordingly when a ring is at the top
		if (dist.get_distance() < sort_consts_.distThreshold && !sort_state_.distFlag && !sort_state_.ringQueue.empty()) {
			if (sort_state_.ringQueue.front() != sort_consts_.alliance) {
				pros::lcd::print(5, "Ejecting");
				sort_state_.isEjecting = true;  // Mark ejection in progress
				sort_state_.delay = 7;
				sort_state_.curPos = intake.get_position();
			} else {
				pros::lcd::print(5, "Scoring");
				sort_state_.ringQueue.pop_front();
			}  // Remove the ring from queue
			sort_state_.distFlag = true;  // Mark distance detection
		}
	} else {
		if(intake.get_position()<sort_state_.curPos+65){
			sort_state_.power=127;
		} else {
			sort_state_.power=-127;
			if(sort_state_.delay>0){
				sort_state_.delay--;
			} else {
				sort_state_.isEjecting = false;
				sort_state_.ringQueue.pop_front();
			}
		}
	}
}

/*
pros::Optical color(15);
pros::Distance ringDist(14);
bool isEjecting = false;
bool colorFlag = false;
bool distFlag = false;
int ejectPos = 12;  // Eject position in inches
int distThreshold = 30;  // Distance threshold for distance sensor
std::deque<bool> ringQueue;  // Queue to store the color of the detected rings
int pos = 0;
int pauseCounter = 0;

// Sift function
void sift(bool allianceColor) {  // 0 for red, 1 for blue
	if(!isEjecting){
		if (color.get_proximity() < distThreshold) {
			colorFlag = false;
		}
		if (ringDist.get_distance() > 120) {
			distFlag = false;
			intake.move(127);
			isEjecting = false;
		}
		// Detect ring color once when proximity is within range
		if (color.get_proximity() >= 180 && !colorFlag) {
			if (color.get_hue() > 180 && color.get_hue() < 270) {  // Detect Blue
				pros::lcd::print(7, "Blue Detected");
				ringQueue.push_back(1);  // Push Blue to queue
			} else {  // Detect Red
				pros::lcd::print(7, "Red Detected");
				ringQueue.push_back(0);  // Push Red to queue
			}
			colorFlag = true;  // Mark color as detected
		}

		// Handle distance detection and act accordingly when a ring is at the top
		if (ringDist.get_distance() < distThreshold && !distFlag && !ringQueue.empty()) {
			if (ringQueue.front() != allianceColor) {
				pros::lcd::print(5, "Ejecting");
				isEjecting = true;  // Mark ejection in progress
				pauseCounter = 7;
				pos = intake.get_position();
			} else {
				pros::lcd::print(5, "Scoring");
				ringQueue.pop_front();
			}  // Remove the ring from queue
			distFlag = true;  // Mark distance detection
		}
	} else {
		if(intake.get_position()<pos+65){
			intake.move(127);
		} else {
			intake.move(-127);
			if(pauseCounter>0){
				pauseCounter--;
			} else {
				isEjecting = false;
				ringQueue.pop_front();
			}
		}
	}
}
*/
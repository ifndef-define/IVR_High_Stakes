#include <sort.h>

sort::sort(pros::Color &rColor, pros::Distance &rDist, bool allianceColor): color(rColor), dist(rDist){
    alliance = allianceColor;
}

int sort::update(int current) {
    
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
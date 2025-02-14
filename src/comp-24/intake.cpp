#include "robots/comp-24/intake.h"

void Intake::startIntake(double speed){ intakeMotor.move(speed * 127); }

void Intake::stopIntake(){ intakeMotor.brake(); }

void Intake::liftIntake() { lift.retract(); }

void Intake::lowerIntake() { lift.extend(); }

Ring::Color ColorDetector::getColor() {
  double hue = colorSensor.get_hue();
  if(colorSensor.get_proximity() >= 220){
    if((hue <= 280) && (hue >= 160)) { 
      return Ring::Color::BLUE;
    } 
    if((hue >= 340) && (hue <= 20)) {
      return Ring::Color::RED; 
    }
  }
  return Ring::Color::NONE;
}


IntakeManager::IntakeManager() : detector() {
  filter = Ring::Color::NONE;
  eject = false;
  intakeSpeed = 1;
  counter = 13; //13 * 10 = 130ms
}

double IntakeManager::getIntakeSpeed() const { return intakeSpeed; }

void IntakeManager::setIntakeSpeed(double speed) { intakeSpeed = speed; }

void IntakeManager::startIntake() { intake.startIntake(intakeSpeed); }

void IntakeManager::stopIntake() { intake.stopIntake(); }

void IntakeManager::setFilterColor(Ring::Color filterColor) { filter = filterColor; }

Ring::Color IntakeManager::getFilterColor() const { return filter; }

void IntakeManager::setLED(int num){ detector.setLED(num); }

void IntakeManager::setPullbackFlag(bool flag) { pullbackFlag = flag; }

bool IntakeManager::getPullbackFlag() { return pullbackFlag; }

void IntakeManager::setColorSort(bool colorSort) { runColorSort=colorSort; }

bool IntakeManager::getColorSort() { return runColorSort; }

bool IntakeManager::getEject() { return eject; }

void IntakeManager::ejectDisc(){
  intake.startIntake(intakeSpeed);
  if(counter > 0){
    counter--;
  } else {
    intake.stopIntake();
    counter = 13;
  }
}

void IntakeManager::update() {
    // Check the current ring color from the detector
    if(filter != Ring::Color::NONE) {
      Ring::Color ringColor = detector.getColor();
      eject = ((ringColor != filter) && (ringColor != Ring::Color::NONE));
    }
}
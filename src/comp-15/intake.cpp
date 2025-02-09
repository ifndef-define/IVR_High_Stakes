#include "robots/comp-15/intake.h"

void Intake::startIntake(double speed){ intakeMotor.move(speed * 127); }

void Intake::stopIntake(){ intakeMotor.brake(); }

void Intake::liftIntake() { lift.retract(); }

void Intake::lowerIntake() { lift.extend(); }

Ring::Color ColorDetector::getColor() {
  double hue = colorSensor.get_hue();
  double prox = colorSensor.get_proximity();
  if(prox >= 220){
    if((hue <= 270) && (hue >= 180)) { 
      return Ring::Color::BLUE;
    } 
    if((hue >= 350) && (hue <= 15)) {
      return Ring::Color::RED; 
    }
  }
  return Ring::Color::NONE;
}

IntakeManager::IntakeManager() : detector() {
  filter = Ring::Color::NONE;
  eject = false;
  intakeSpeed = 1;
  counter = 12; //12 * 10 = 120ms
}

double IntakeManager::getIntakeSpeed() const { return intakeSpeed; }

void IntakeManager::setIntakeSpeed(double speed) { intakeSpeed = speed; }

void IntakeManager::startIntake() { intake.startIntake(intakeSpeed); }

void IntakeManager::stopIntake() { intake.stopIntake(); }

Ring::Color IntakeManager::getFilterColor() const { return filter; }

void IntakeManager::setFilterColor(Ring::Color filterColor) { filter = filterColor; }

bool IntakeManager::getEject() { return eject; }

void IntakeManager::ejectDisc(){
  intake.startIntake(intakeSpeed);
  if(counter > 0){
    counter--;
  } else {
    intake.stopIntake();
    counter = 12;
  }
}

void IntakeManager::update() {
    // Check the current ring color from the detector
    if(filter != Ring::Color::NONE) {
      Ring::Color ringColor = detector.getColor();
      eject = ((ringColor != filter) && (ringColor != Ring::Color::NONE));
    }
}
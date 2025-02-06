#include "robots/comp-15/intake.h"

void Intake::startIntake(double speed) { intakeMotor.move(speed * 127); }

void Intake::stopIntake() { intakeMotor.brake(); }

void Intake::liftGate() { lift.retract(); }

void Intake::lowerGate() { lift.extend(); }

void Intake::toggleGate() { lift.toggle(); }

Ring::Color ColorDetector::getColor() {
  if((colorSensor.get_hue() <= 270) && (colorSensor.get_hue() >= 180)) { return Ring::Color::BLUE; } 
  else { return Ring::Color::RED; }
}

IntakeManager::IntakeManager() {
  filter = Ring::Color::NONE;
  eject = false;
  intakeSpeed = 1;
}

double IntakeManager::getIntakeSpeed() const { return intakeSpeed; }

void IntakeManager::setIntakeSpeed(double speed) { intakeSpeed = speed; }

void IntakeManager::startIntake() { intake.startIntake(intakeSpeed); }

void IntakeManager::stopIntake() { intake.stopIntake(); }

Ring::Color IntakeManager::getFilterColor() const { return filter; }

void IntakeManager::setFilterColor(Ring::Color filterColor) { filter = filterColor; }

bool IntakeManager::getShouldEject() const { return eject; }

void IntakeManager::update() {
  
}
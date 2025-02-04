#include "robots/comp-15/intake.h"

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

void IntakeManager::setFilterColor(Ring::Color filterColor) {
  filter = filterColor;
}

bool IntakeManager::getShouldEject() const { return eject; }

void IntakeManager::update() {}

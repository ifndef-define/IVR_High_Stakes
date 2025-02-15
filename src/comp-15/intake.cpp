#include "robots/comp-15/intake.h"

void Intake::startIntake(double speed){ intakeMotor.move(speed * 127); }

void Intake::stopIntake(){ intakeMotor.brake(); }

void Intake::liftIntake() { lift.retract(); }

void Intake::lowerIntake() { lift.extend(); }

ColorDetector::ColorDetector() : colorSensor(6) {
  colorSensor.set_led_pwm(100);
  colorSensor.set_integration_time(20);
}

Ring::Color ColorDetector::getColor() {
  double hue = colorSensor.get_hue();
  if(colorSensor.get_proximity() >= 230){
    if((hue <= 280) && (hue >= 175)) { 
      return Ring::Color::BLUE;
    } 
    if((hue >= 330) || (hue <= 30)) {
      return Ring::Color::RED; 
    }
  }
  return Ring::Color::NONE;
}

IntakeManager::IntakeManager() : detector() {
  filter = Ring::Color::NONE;
  eject = false;
  intakeSpeed = 1;
  curRing = detector.getColor();
}

double IntakeManager::getIntakeSpeed() const { return intakeSpeed; }

void IntakeManager::setIntakeSpeed(double speed) { intakeSpeed = speed; }

void IntakeManager::startIntake() { intake.startIntake(intakeSpeed); }

void IntakeManager::stopIntake() { intake.stopIntake(); }

Ring::Color IntakeManager::getFilterColor() const { return filter; }

void IntakeManager::setFilterColor(Ring::Color filterColor) { filter = filterColor; }

bool IntakeManager::getEject() { return eject; }



void IntakeManager::update() {
    // Check the current ring color from the detector
    if(filter != Ring::Color::NONE) {
      curRing = detector.getColor();
      eject = ((curRing != filter) && (curRing != Ring::Color::NONE));
      pros::lcd::set_text(5, "Ring Color: " + std::to_string((int)curRing));
      pros::lcd::print(6, "eject: %d", eject);
    }
}
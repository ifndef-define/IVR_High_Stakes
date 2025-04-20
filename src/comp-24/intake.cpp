#include "robots/comp-24/intake.h"

Intake::Intake() : intakeMotor({10, -9}, pros::MotorGears::blue), intakeRot(-5) {
  intakeRot.reset_position();
}

void Intake::startIntake(double speed){ intakeMotor.move(speed * 127); }

void Intake::stopIntake(){ intakeMotor.brake(); }

double Intake::getAngle(){ return intakeRot.get_position()/100; }

void Intake::resetAngle(){ intakeRot.reset_position(); }

ColorDetector::ColorDetector() : colorSensor(11) {
  colorSensor.set_led_pwm(100);
  colorSensor.set_integration_time(10);
}

Ring::Color ColorDetector::getColor() {
  double hue = colorSensor.get_hue();
  if(colorSensor.get_proximity() >= 230){
    if((hue <= 280) && (hue >= 200)) { 
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

double IntakeManager::getIntakeAngle() { return intake.getAngle(); }

void IntakeManager::resetIntakeAngle(){ intake.resetAngle(); }

void IntakeManager::update() {
    // Check the current ring color from the detector
    if(filter != Ring::Color::NONE) {
      curRing = detector.getColor();
      eject = ((curRing != filter) && (curRing != Ring::Color::NONE));
      pros::lcd::set_text(5, "Ring Color: " + std::to_string((int)curRing));
      pros::lcd::print(6, "eject: %d", eject);
      // if(eject) { 
      //   delay(500); 
      //   eject = false;
      // }
    }
}
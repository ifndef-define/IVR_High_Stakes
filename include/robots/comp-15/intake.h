#pragma once
#include "common/includeList.h"
#include "robots/comp-15/devices.h"

/**
 * @class Intake
 * @brief Controls the intake hardware
 *
 */
class Intake {
private:
  pros::MotorGroup intakeMotor;
  pros::adi::Pneumatics lift;

public:
  Intake();
  /**
   * @brief Lifts up the gate
   */
  void liftGate();
  /**
   * @brief Sets the gate back down
   */
  void lowerGate();

  /**
   * @brief Start the intake
   *
   * @param speed The speed of the intake [0, 1]
   */
  void startIntake(double speed);
  /**
   * @brief Stops the intake motors
   */
  void stopIntake();
};

namespace Ring {
enum class Color { BLUE, RED, NONE };
};

/**
 * @class ColorDetector
 * @brief The color detector
 *
 */
class ColorDetector {
private:
  pros::Optical sensor;

public:
  ColorDetector();
  /**
   * @brief Gets the color in the intake
   *
   * @return The color in the take
   */
  Ring::Color getColor() const;
};

/**
 * @class IntakeManager
 * @brief Runs the intake
 *
 */
class IntakeManager {
private:
  Intake intake;
  ColorDetector detector;

  bool eject;
  Ring::Color filter;

  double intakeSpeed;

public:
  IntakeManager();

  double getIntakeSpeed() const;
  void setIntakeSpeed(double speed);

  void startIntake();
  void stopIntake();
  /**
   * @brief Gets the type that is kept in filter
   *
   * @return The color to be kept in a filter, NONE is no filter
   */
  Ring::Color getFilterColor() const;
  /**
   * @brief Sets whether or not the colors are being filtered
   *
   * @param filterColor What color to keep
   */
  void setFilterColor(Ring::Color filterColor);

  /**
   * @brief Gets whether or not should eject
   *
   * @return Whether or not should eject
   */
  bool getShouldEject() const;

  /**
   * @brief Updates the system
   */
  void update();
};

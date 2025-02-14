#pragma once
#include "common/includeList.h"
// #include "robots/comp-24/devices.h"

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
  Intake(): intakeMotor({-13, 17, -18}, pros::MotorGears::blue), lift('E', false) {};
  /**
   * @brief Lifts up the Intake
   */
  void liftIntake();
  /**
   * @brief Sets the Intake back down
   */
  void lowerIntake();

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
  pros::Optical colorSensor;

public:
  ColorDetector(): colorSensor(6) {};
  /**
   * @brief Gets the color in the intake
   *
   * @return The color in the take
   */
  Ring::Color getColor();
  void setLED(int num);
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
  double counter;
  bool pullbackFlag = false;
  bool runColorSort = true;

public:
  IntakeManager();

  double getIntakeSpeed() const;
  void setIntakeSpeed(double speed);

  void startIntake();
  void stopIntake();

  void setPullbackFlag(bool flag);
  bool getPullbackFlag();

  void ejectDisc();
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
   * @brief Toggles color sorting
   *
   * @param colorSort Whether or not to sort colors
   */
  void setColorSort(bool colorSort);

  /**
   * @brief Sets the LED for the color sensor
   *
   * @param num The number of the LED
   */
  void setLED(int num);

  /**
   * @brief Gets whether or not should eject
   *
   * @return Whether or not should eject
   */
  bool getEject();

  /**
   * @brief Gets whether or not to sort colors
   *
   * @return Whether or not to sort colors
   */
  bool getColorSort();

  /**
   * @brief Updates the system
   */
  void update();
};

#include "robots/comp-15/devices.h"

/**
 * @class Intake
 * @brief Controls the intake hardware
 *
 */
class Intake {
private:
  pros::MotorGroup intakeMotor;
  pros::Peumatic gate;

public:
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

enum class Color { BLUE, RED };

/**
 * @class ColorDetector
 * @brief The color detector
 *
 */
class ColorDetector {
private:
  pros::Optical sensor;

public:
  /**
   * @brief Gets the color in the intake
   *
   * @return The color in the take
   */
  Color getColor() const;
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

  bool filterColor;

public:
  /**
   * @brief Gets whether or not to filter the colors
   *
   * @return Whether or not colors are being filtered
   */
  bool getFilterColor() const;
  /**
   * @brief Sets whether or not the colors are being filtered
   *
   * @param filterColor Whether or not colors should be filtered
   */
  void setFilterColor(bool filterColor);

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

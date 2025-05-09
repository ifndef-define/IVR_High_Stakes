#pragma once
#include "common/includeList.h"

/**
 * @brief A simple grouping class for all pneumatics to find them easily
 */
class PneumaticsGroup {
    private:

    public: 
        pros::adi::Pneumatics mogoClamp;
        pros::adi::Pneumatics leftMogoRushArm;
        pros::adi::Pneumatics rightMogoRushArm;
        pros::adi::Pneumatics mogoRushTeeth;
        // pros::adi::Pneumatics intakeLock;
        pros::adi::Pneumatics intakeLift;

        pros::adi::Pneumatics innerClimbArms;
        pros::adi::Pneumatics outerClimbArms;
        pros::adi::Pneumatics climbPTO;
        pros::adi::Pneumatics paperLeft;
        pros::adi::Pneumatics paperRight;
        PneumaticsGroup();
};

extern PneumaticsGroup pneumatics;
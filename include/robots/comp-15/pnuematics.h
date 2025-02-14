#pragma once
#include "common/includeList.h"

/**
 * @brief A simple grouping class for all pneumatics to find them easily
 */
class PneumaticsGroup {
    private:

    public: 
        // Intake lift is managed by the intake class
        pros::adi::Pneumatics mogoRushArm;
        pros::adi::Pneumatics mogoRushClamp;
        pros::adi::Pneumatics mogoClamp;
        pros::adi::Pneumatics doinker;
        PneumaticsGroup();
};

using Air = PneumaticsGroup;
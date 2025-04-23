#pragma once
#include "common/includeList.h"

/**
 * @brief A simple grouping class for all pneumatics to find them easily
 */
class PneumaticsGroup {
    private:

    public: 
        // Intake lift is managed by the intake class
        pros::adi::Pneumatics mogoClamp;
        pros::adi::Pneumatics leftMogoRushArm;
        pros::adi::Pneumatics mogoRushTeeth;
        pros::adi::Pneumatics intakeLift;
        pros::adi::Pneumatics intakeLock;
        pros::adi::Pneumatics rightMogoRushArm;
        
        pros::adi::Pneumatics leftClimbWing;
        pros::adi::Pneumatics rightClimbWing;
        pros::adi::Pneumatics innerClimbArms;
        pros::adi::Pneumatics outerClimbArms;
        pros::adi::Pneumatics climbPTO;
        PneumaticsGroup();
};

extern PneumaticsGroup pneumatics;
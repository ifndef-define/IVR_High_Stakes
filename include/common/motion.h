#pragma once
#include "main.h"
#include "common/drive_parent.h"

class vf_motion {
    public:
        vf_motion(drive *driveObject);
        
        /**
         * @brief Moves the robot using a vector field. This method requires a polynomial function to be
         *  calculated and its x and y equations passed in as a funciton to determine the vector to move the robot.
         * 
         * @param x_comp X component equation of the vector field function
         * @param y_comp Y component equation of the vector field function
         * @param gain Gain of the vector field function [0,1] percent of max speed
         * @param func_degree Degree of the polynomial function
         * @param heading Angle to end at [0,360] degrees
         * @param min_speed Minimum speed to move at [0,1] percent of max speed
         * @param max_speed Maximum speed to move at [0,1] percent of max speed
         *  NOTE: max_speed > min_speed
         * @param timeout Time to wait for the move to complete in milliseconds
         * @param wait If true, will wait for the move to complete before returning
         *  NOTE: Queuing is supported with mutex locks
         * 
         * @example
         * \code{.cpp}
         *   // Move via the equation y = x^3
         *   double x_comp(double x, double y, double k) {
         *    return 1 + 3*k*pow(x, 2) * (y - pow(x,3));
         *   }
         *   double y_comp(double x, double y, double k) {
         *    return 3*pow(x, 2) - k*(y-pow(x,3));
         *   }
         *   move_VF(x_comp, y_comp, 0.5, 3, 100, 0.1, 0.7, 5000, true);
         */
        static void move_VF(std::function<double(double, double, double)> x_comp, std::function<double(double, double, double)> y_comp,
            double gain, double func_degree, double heading, double min_speed=0.1, double max_speed=0.1, int timeout=10000, bool wait=true);
        
        /**
         * @brief Moves the robot to a set pose (x, y, theta). Field (0,0) is the red alliance
         *  positive corner.
         * 
         * @param x X coordinate to move to [0,144] inches
         * @param y Y coordinate to move to [0,144] inches
         * @param theta Angle to move to [0,360] degrees
         * @param min_speed Minimum speed to move at [0,1] percent of max speed
         * @param max_speed Maximum speed to move at [0,1] percent of max speed
         *  NOTE: max_speed > min_speed
         * @param timeout Time to wait for the move to complete in milliseconds
         * @param wait If true, will wait for the move to complete before returning
         *  NOTE: Queuing is supported with mutex locks
         */
        static void moveToPose(double x, double y, double theta, double min_speed=0.1, double max_speed=0.1, int timeout=10000, bool wait=true);
        
        /**
         * @brief Turns the robot to face a set pose. Field (0,0) is the red alliance
         *  positive corner.
         * 
         * @param x X coordinate to face [0,144] inches
         * @param y Y coordinate to face [0,144] inches
         * @param min_speed Minimum speed to move at [0,1] percent of max speed
         * @param max_speed Maximum speed to move at [0,1] percent of max speed
         *  NOTE: max_speed > min_speed
         * @param timeout Time to wait for the move to complete in milliseconds
         * @param wait If true, will wait for the move to complete before returning
         *  NOTE: Queuing is supported with mutex locks
         */
        static void turnToPose(double x, double y, double min_speed=0.1, double max_speed=0.1, int timeout=10000, bool wait=true);
        
        /**
         * @brief Turns the robot to face a set heading. Field (0,0) is the red alliance
         *  positive corner.
         * 
         * @param theta Angle to face [0,360] degrees
         * @param min_speed Minimum speed to move at [0,1] percent of max speed
         * @param max_speed Maximum speed to move at [0,1] percent of max speed
         *  NOTE: max_speed > min_speed
         * @param timeout Time to wait for the move to complete in milliseconds
         * @param wait If true, will wait for the move to complete before returning
         *  NOTE: Queuing is supported with mutex locks  
         */
        static void turnToHeading(double theta, double min_speed=0.1, double max_speed=0.1, int timeout=10000, bool wait=true);
        
        enum drive_side_e {
            LEFT = 0,
            RIGHT = 1
        };
        
        /**
         * @brief Moves the robot to face a set pose (x, y) while keeping one side of the drive
         *  locked in place. Field (0,0) is the red alliance positive corner.
         * 
         * @param x X coordinate to move to [0,144] inches
         * @param y Y coordinate to move to [0,144] inches
         * @param locked_side Side to lock in place
         * @param min_speed Minimum speed to move at [0,1] percent of max speed
         * @param max_speed Maximum speed to move at [0,1] percent of max speed
         *  NOTE: max_speed > min_speed
         * @param timeout Time to wait for the move to complete in milliseconds
         * @param wait If true, will wait for the move to complete before returning
         *  NOTE: Queuing is supported with mutex locks
         */
        static void swingToPose(double x, double y, drive_side_e locked_side, double min_speed=0.1, double max_speed=0.1, int timeout=10000, bool wait=true);
        
        /**
         * @brief Moves the robot to a set heading while keeping one side of the drive
         *  locked in place. Field (0,0) is the red alliance positive corner.  
         * 
         * @param theta Angle to face [0,360] degrees
         * @param locked_side Side to lock in place
         * @param min_speed Minimum speed to move at [0,1] percent of max speed
         * @param max_speed Maximum speed to move at [0,1] percent of max speed
         *  NOTE: max_speed > min_speed
         * @param timeout Time to wait for the move to complete in milliseconds
         * @param wait If true, will wait for the move to complete before returning
         *  NOTE: Queuing is supported with mutex locks 
         */
        static void swingToHeading(double theta, drive_side_e locked_side, double min_speed=0.1, double max_speed=0.1, int timeout=10000, bool wait=true);

    private:
        
};

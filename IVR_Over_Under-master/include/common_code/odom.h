#pragma once
#include "main.h"
#include "common_code/generic_rotation_vex_rot.h"

class Odom {
    public:
    Odom(pros::IMU &theImu, Generic_Rotation* transverseWheel, Generic_Rotation* radialWheel);
    ~Odom();

    void initTracker(double initial_x, double initial_y, double initial_heading);
    double headingCorrection(double currentRotation);
    void updatePosition();
    double getX();
    double getY();
    double getHeading();
    double getTransverseValue();
    double getRadialValue();

    private:
    // wheel radii are stored in the GenericRotation sensor
    double initHeading;
    double x_tracking_offset;
    double y_tracking_offset;
    double last_x_tracking_offset;
    double last_y_tracking_offset;
    double positionX = 0.0;
    double positionY = 0.0;
    double last_heading = 0.0;
    const double FEET_TO_METERS = 0.304;

    double RADIAL_WHEEL_TRANS_OFFSET; // meters. Should be const (and treated as such) but is ideally also initialized in init_tracker() like everything else
    double TRANSVERSE_WHEEL_RAD_OFFSET; // meters. 

    Generic_Rotation* transverseWheel = nullptr;
    Generic_Rotation* radialWheel = nullptr;
    pros::IMU& imu;
};
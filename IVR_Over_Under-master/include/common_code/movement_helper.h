#pragma once
#include "common_code/traditional_drive.h"
#include "main.h"

int sgn(double number);
double optimizeAngle(double angleDeg);
double reverseAngle(double angle);
double lineAndAngleAngularDiff(double dx, double dy, double angleDegrees);
double getRotationalRPM(double curr_heading, double desiredAngleDeg, bool reversed = false, double p = 3.0);
// For pure pursuit
double getTranslationalRPM(double dist_to_goal_meters, double max_translational_rpm, double total_path_distance, double minTransRPM = 40.0, double rpm_per_meter = 580.0); // basically rpm_per_meter * dist left in path (meters) = rpm sent to motors, bounded by min and max rpm
double calculate_distance_two_points(std::vector<double> point_one, std::vector<double> point_two);
std::vector<double> calculate_remaining_dist(std::vector<std::vector<double>>& path, bool ignore_last_point = true);

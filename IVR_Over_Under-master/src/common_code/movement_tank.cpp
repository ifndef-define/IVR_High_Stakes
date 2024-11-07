#include "common_code/movement_tank.h"
#include "common_code/PID.h"

void moveMotors(traditional_drive& drive, double leftRPM, double rightRPM) {
    drive.get_motor_group(0).move_velocity(leftRPM);
    drive.get_motor_group(1).move_velocity(rightRPM);
}

void stopMotors(traditional_drive& drive) {
    // drive.get_motor_group(0).move_velocity(0.0);
    // drive.get_motor_group(1).move_velocity(0.0);
    drive.get_motor_group(0).set_brake_modes(BRAKETYPE_BRAKE);
    drive.get_motor_group(1).set_brake_modes(BRAKETYPE_BRAKE);
    drive.get_motor_group(0).brake();
    drive.get_motor_group(1).brake();
    delay(15);
    drive.get_motor_group(0).set_brake_modes(BRAKETYPE_COAST);
    drive.get_motor_group(1).set_brake_modes(BRAKETYPE_COAST);
}

// to be used exclusively when only turning (no translation)
void turnToAngle(traditional_drive& drive, double desiredAngleDeg, double toleranceDeg, bool debug, double p, int time_in_range) {
    double degFromFinalAngle = desiredAngleDeg - drive.get_imu().get_heading();
    degFromFinalAngle = optimizeAngle(degFromFinalAngle);
    double start_time = pros::millis();
    double in_range_time = 0;
    const int required_time = time_in_range;
    const int delay_time = 20;
    while (in_range_time < required_time) {
        if (std::abs(degFromFinalAngle) <= toleranceDeg) {
            in_range_time += (delay_time);
        } else {
            in_range_time = 0;
        }
        degFromFinalAngle = optimizeAngle(desiredAngleDeg - drive.get_imu().get_heading());
        double rotRPM = degFromFinalAngle * p;
        moveMotors(drive, rotRPM, -rotRPM);
        if (debug) pros::lcd::set_text(4, "Robot angle: " + std::to_string(drive.get_imu().get_heading()));
        if (pros::millis() - start_time > 5000) {
            if (debug) pros::lcd::set_text(6, "Turn to angle timeout reached!!!");
            break;
        } 
        pros::delay(delay_time);
    }
    // need to stop motors in case of break statement
    stopMotors(drive);
}

//PID version of turn to angle
void turnPID(traditional_drive& drive, double desiredAngleDeg, int maxTime, double toleranceDeg, double p, double i, double d) {
    PID t_PID = PID(p, i, d);
    double degFromFinalAngle = 0;
    double turn=0;
    double start_time = pros::millis();
    while(!t_PID.getState().targetReached && pros::millis()-start_time < maxTime) {
        degFromFinalAngle = optimizeAngle(desiredAngleDeg - drive.get_imu().get_heading());
        turn = t_PID.updatePID(desiredAngleDeg, drive.get_imu().get_heading(), toleranceDeg, degFromFinalAngle);
        // output = t_PID.updatePID(desiredAngleDeg, drive.get_imu().get_heading(), toleranceDeg);
        drive.turn_with_power(turn);
        // if(std::abs(turn)<=1){ break; }
        if(std::abs(degFromFinalAngle) <= toleranceDeg) { break; }
        delay(50);
    }
    // need to stop motors in case of break statement
    stopMotors(drive);
}

void latPID(traditional_drive& drive, double target, int maxTime, double tolerance, double p, double i, double d) {
    PID l_PID = PID(p, i, d);
    double lateral=0;
    double initVal = 2 * M_PI * (1.96 * 0.0254 / 2) * (drive.getOdom().getRadialValue() / 5000);
    double deltaVal = 0;
    double start_time = pros::millis();
    while(!l_PID.getState().targetReached && pros::millis()-start_time < maxTime){
        deltaVal = convert::mToIn((2 * M_PI * (1.96 * 0.0254 / 2) * (drive.getOdom().getRadialValue() / 5000))-initVal);
        lcd::set_text(7, "deltaVal: " + std::to_string(deltaVal));
        lateral = l_PID.updatePID(target, deltaVal, tolerance);
        drive.move_with_power(lateral);
        if(std::abs(l_PID.getState().error) <= tolerance) { break; }
        delay(50);
    }
    // need to stop motors in case of break statement
    stopMotors(drive);
}

void movePID(traditional_drive& drive, double target, double desiredAngleDeg, int maxTime, double latTolerance, double turnTolerance, double lP, double lI, double lD,double tP, double tI, double tD) {
    PID l_PID = PID(lP, lI, lD);
    PID t_PID = PID(tP, tI, tD);
    double lateral=0;
    double turn=0;
    double initVal = 2 * M_PI * (1.96 * 0.0254 / 2) * (drive.getOdom().getRadialValue() / 5000);
    double deltaVal = 0;
    double degFromFinalAngle = 0;
    double start_time = pros::millis();
    while(!l_PID.getState().targetReached && pros::millis()-start_time < maxTime){
        degFromFinalAngle = optimizeAngle(desiredAngleDeg - drive.get_imu().get_heading());
        deltaVal = convert::mToIn((2 * M_PI * (1.96 * 0.0254 / 2) * (drive.getOdom().getRadialValue() / 5000))-initVal);
        lateral = l_PID.updatePID(target, deltaVal, latTolerance);
        turn = t_PID.updatePID(desiredAngleDeg, drive.get_imu().get_heading(), turnTolerance, degFromFinalAngle);
        drive.tank_with_power(lateral, turn);
        if(std::abs(l_PID.getState().error) <= latTolerance) { break; }
        delay(20);
    }
    // need to stop motors in case of break statement
    stopMotors(drive);
}

void followPath(std::vector<std::vector<double>>& path, traditional_drive& drive, double finalAngleDeg, bool reversed, bool spinAtEnd, bool goal_at_end, double lookForwardRadius, double final_angle_tolerance_deg, double MAX_TRANSLATIONAL_RPM, double maxRPM, double minTransRPM, bool printMessages, double turnP) {    
    double firstX = path[0][0];
    double firstY = path[0][1];
    double currentIndex = 0;
    double SMALL_NUM = 0.000001;
    // A bonus would be able to calculate ALIGNMENT_HELPER_MULTIPLIER based on robot velocity
    double ALIGNMENT_HELPER_MULTIPLIER = 1.5;
    double ALIGN_HELPER_DIST_AWAY = ALIGNMENT_HELPER_MULTIPLIER * lookForwardRadius;
    double FINAL_LOCATION_TOLERANCE = 0.2; // meters
    bool readyForSpin = false;

    double lastSegDX = path[path.size() - 1][0] - path[path.size() - 2][0];
    double lastSegDY = path[path.size() - 1][1] - path[path.size() - 2][1];
    double lastAngleDiff = lineAndAngleAngularDiff(lastSegDX, lastSegDY, finalAngleDeg);

    // If the dist between the original second to last point & the original final point is less than the 
    // dist between new second to last and original final (which is ALIGN_HELPER_DIST_AWAY) then going to the new
    // second to last point doesn't make sense because the robot would ideally drive forwards, then backwards, then forwards again. 
    // So, we check for that and spin on spot at the end if that's the case .
    // There's also a manual override for spinOnSpot that's an optional argument (spinAtEnd) in case the course is too compact.
    bool spinOnSpot = (std::abs(lastAngleDiff) > 90) || (sqrt(pow(lastSegDX, 2) + pow(lastSegDY, 2)) < ALIGN_HELPER_DIST_AWAY) || spinAtEnd;
    std::vector<double> ORIGINAL_PATH_FINAL = path[path.size() - 1];

    if (!spinOnSpot) {
        if (printMessages) pros::lcd::set_text(1, "No SpinOnSpot");
        // Calculate and add new 2nd to last point that will let the robot drive towards the original final point
        // while facing the desired final angle. To do this, a point is added in the opposite direction of the 
        // desired final angle at a distance away equal to ALIGN_HELPER_DIST_AWAY.
        double finalAngleRad = finalAngleDeg * M_PI / 180.0;
        double dx = std::sin(finalAngleRad) * ALIGN_HELPER_DIST_AWAY;
        double dy = std::cos(finalAngleRad) * ALIGN_HELPER_DIST_AWAY;
        std::vector<double> pathSecondToLast = {ORIGINAL_PATH_FINAL[0] - dx, ORIGINAL_PATH_FINAL[1] - dy};

        // In the future we want to configure the dist between pathSecondToLast and ORIGINAL_PATH_FINAL to be as small
        // as possible with the robot's turning capabilities.

        // If the potential new second to last point is at the same location as the current second to last don't add it.
        if (pathSecondToLast[0] != path[path.size() - 2][0] || pathSecondToLast[1] != path[path.size() - 2][1]) {
            // Delete last element of path (but it's stored above)
            path.pop_back();
            // Add the new second to last and final back
            path.push_back(pathSecondToLast);
            path.push_back(ORIGINAL_PATH_FINAL);
        } 

        // Add an extra point at the end of the path that is in the direction of the desired final angle. The distance
        // of this point from the desired stopping point is the lookForwardRadius, as when the look ahead point "falls off"
        // the end of the path created by this extra point the path following section of this program will stop.
        std::vector<double> extraPoint = {ORIGINAL_PATH_FINAL[0] + (dx / ALIGNMENT_HELPER_MULTIPLIER), ORIGINAL_PATH_FINAL[1] + (dy / ALIGNMENT_HELPER_MULTIPLIER)};
        path.push_back(extraPoint);
        if (printMessages) {
            pros::lcd::set_text(2, "3rd to last: " + std::to_string(std::round(pathSecondToLast[0] * 1000) / 1000.0) + ", " + std::to_string(pathSecondToLast[1]));
            pros::lcd::set_text(3, "Last: " + std::to_string(std::round(extraPoint[0] * 1000) / 1000.0) + ", " + std::to_string(extraPoint[1]));
        }
    } else {
        if (printMessages) pros::lcd::set_text(1, "Yes SpinOnSpot");
        // This appends an extra point to the end of the path (similar to how it's done on line 107 with extraPoint)
        // except the point is in the direction of the line between the original final point and original second to last point
        double finalSegAngle = atan2(lastSegDX, lastSegDY) * 180 / M_PI;
        if (finalSegAngle < 0) finalSegAngle += 360;
        double dx = sin(finalSegAngle * M_PI / 180.0) * lookForwardRadius;
        double dy = cos(finalSegAngle * M_PI / 180.0) * lookForwardRadius;
        std::vector<double> pathNewLast = {ORIGINAL_PATH_FINAL[0] + dx, ORIGINAL_PATH_FINAL[1] + dy};
        path.push_back(pathNewLast);
    }

    std::vector<double> distances_to_end = calculate_remaining_dist(path); //path
    double last_calculated_distance = calculate_distance_two_points({drive.getX(), drive.getY()}, path[1]);
    // last_current_index will reflect actual position of robot instead of position of lookAheadPoint
    double last_current_index = 0;

    while (currentIndex < path.size() - 1) {
        std::vector<double> driveTowards = {path[currentIndex][0], path[currentIndex][1]};
        double positionX = drive.getX();
        double positionY = drive.getY();

        for (int i = currentIndex; i < path.size() - 1; i++) {
            double x1 = path[i][0] - positionX;
            double x2 = path[i + 1][0] - positionX;
            double y1 = path[i][1] - positionY;
            double y2 = path[i + 1][1] - positionY;

            double dx = x2 - x1;
            double dy = y2 - y1;
            double segmentDist = std::sqrt(pow(dx, 2) + pow(dy, 2));
            double D = (x1 * y2) - (x2 * y1);

            // Discriminant gives # of intersections between circle and line
            double discriminant = (pow(lookForwardRadius, 2) * pow(segmentDist, 2)) - pow(D, 2);

            if (discriminant >= 0) {
                // At least one intersection has been found
                bool solution1 = true;
                bool solution2 = true;
                double solutionX1 = (D * dy + sgn(dy) * dx * sqrt(discriminant)) / (pow(segmentDist, 2));
                double solutionX2 = (D * dy - sgn(dy) * dx * sqrt(discriminant)) / (pow(segmentDist, 2));
                double solutionY1 = (-D * dx + std::abs(dy) * sqrt(discriminant)) / (pow(segmentDist, 2));
                double solutionY2 = (-D * dx - std::abs(dy) * sqrt(discriminant)) / (pow(segmentDist, 2));

                // Test for legitness of solutions (find which one we want to drive towards)
                double robotNextPointDist = sqrt(pow(x2, 2) + pow(y2, 2));

                if ((std::min(x1, x2) - SMALL_NUM > solutionX1) || (solutionX1 > std::max(x1, x2) + SMALL_NUM) || (std::min(y1, y2) - SMALL_NUM > solutionY1) || (solutionY1 > std::max(y1, y2) + SMALL_NUM)) {
                    // "solution" is not within the rectangle created by the sooner and later points, so it's invalid
                    solution1 = false;
                }

                if ((std::min(x1, x2) - SMALL_NUM > solutionX2) || (solutionX2 > std::max(x1, x2) + SMALL_NUM) || (std::min(y1, y2) - SMALL_NUM > solutionY2) || (solutionY2 > std::max(y1, y2) + SMALL_NUM)) {
                    solution2 = false;
                }

                if (solution1 && solution2) {
                    // follow one that's closer to next point
                    if (sqrt(pow(solutionX1 - x2, 2) + pow(solutionY1 - y2, 2)) < sqrt(pow(solutionX2 - x2, 2) + pow(solutionY2 - y2, 2))) {
                        driveTowards[0] = solutionX1 + positionX;
                        driveTowards[1] = solutionY1 + positionY;
                        currentIndex = i;
                        break;
                    } else {
                        driveTowards[0] = solutionX2 + positionX;
                        driveTowards[1] = solutionY2 + positionY;
                        currentIndex = i;
                        break;
                    }
                } else if (solution1 && (sqrt(pow(solutionX1 - x2, 2) + pow(solutionY1 - y2, 2)) < robotNextPointDist)) {
                    // Ensure solution1 is closer to next point than the robot. If it isn't, then
                    // the point is an intersection with the back of the look ahead circle and the path, so it's invalid
                    driveTowards[0] = solutionX1 + positionX;
                    driveTowards[1] = solutionY1 + positionY;
                    currentIndex = i;
                    break;
                } else if (solution2 && (sqrt(pow(solutionX2 - x2, 2) + pow(solutionY2 - y2, 2)) < robotNextPointDist)) {
                    driveTowards[0] = solutionX2 + positionX;
                    driveTowards[1] = solutionY2 + positionY;
                    currentIndex = i;
                    break;
                }
            } else {
                // If there are no valid solutions to drive towards by the time you get to the end
                // of the loop you should just go back towards the last point you know you reached.
                driveTowards[0] = path[currentIndex][0];
                driveTowards[1] = path[currentIndex][1];
            }

            // If statement should be true once end of path is reached
            if ((i == currentIndex) && (currentIndex == path.size() - 2) && (sqrt(pow(positionX - ORIGINAL_PATH_FINAL[0], 2) + pow(positionY - ORIGINAL_PATH_FINAL[1], 2)) < FINAL_LOCATION_TOLERANCE)) {
                // look ahead point has (most likely) gone off the edge of the extension on the final point, so the 
                // robot is at the final point and should spin on spot (if applicable) or the program should terminate
                readyForSpin = true;
                break;
            }
        }

        if (printMessages) {
            pros::lcd::set_text(4, "Robot angle: " + std::to_string(drive.get_imu().get_heading()));
            pros::lcd::set_text(5, "Dist from end: " + std::to_string(sqrt(pow(positionX - ORIGINAL_PATH_FINAL[0], 2) + pow(positionY - ORIGINAL_PATH_FINAL[1], 2))));
            pros::lcd::set_text(6, "Y Position: " + std::to_string(positionY));
        }
        
        if (readyForSpin) break;

        double desiredAngle = atan2(driveTowards[0] - positionX, driveTowards[1] - positionY) * 180 / M_PI;


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // IMPORTANT NOTE: If there is a path segment less than 0.05 meters in length this code segment will not work,
        //                 try to avoid defining a path of that distance or less.
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        double remaining_dist = 0.0;
        double distance_to_index = calculate_distance_two_points({positionX, positionY}, path[currentIndex]);
        // check if behind mode needs to be left
        // second part of && conditional is to prevent edge case
        if (distance_to_index > last_calculated_distance && std::abs(distance_to_index - last_calculated_distance) < 0.05) { 
            // switch to normal ahead mode
            last_current_index = currentIndex;
        }
        // check for behind mode
        if (last_current_index != currentIndex) {
            // for behind of currentIndex (bc of funny look ahead angle)
            remaining_dist = distances_to_end[currentIndex] + calculate_distance_two_points({positionX, positionY}, path[currentIndex]);
            if (printMessages) ::lcd::set_text(3, "APP behind mode");
        } else {
            // for ahead of currentIndex (normal for long path segments)
            remaining_dist = distances_to_end[currentIndex] - calculate_distance_two_points({positionX, positionY}, path[currentIndex]);
            if (printMessages) pros::lcd::set_text(3, "APP ahead mode");
        }

        last_calculated_distance = distance_to_index;
        // if current index increments start adding calc_dist_two_points instead of subtracting until calculate_distance_two_points starts increasing again

        if (printMessages) pros::lcd::set_text(2, "remaining dist: " + std::to_string(remaining_dist));
        // pros::lcd::set_text(2, "dist_to_end: " + std::to_string(distances_to_end[currentIndex]));
        double translationalRPM = getTranslationalRPM(remaining_dist, MAX_TRANSLATIONAL_RPM, distances_to_end[0], minTransRPM);
        // pros::lcd::set_text(3, "trans RPM: " + std::to_string(translationalRPM));

        if (desiredAngle < 0) desiredAngle += 360;

        // Positive angular difference -> turn clockwise
        double rotationalRPM = getRotationalRPM(drive.get_imu().get_heading(), desiredAngle, reversed, turnP);

        // ideally in the future pure pursuit is oblivious of the robot's drive train. First step is to
        // use this app_move function from DriveParent
        drive.app_move({translationalRPM, rotationalRPM}, 0, maxRPM, reversed);

        pros::delay(50);
    }

    // moveMotors(drive, 0.0, 0.0);
    stopMotors(drive);
    pros::delay(400); // give the robot time to come to a full stop

    // Turn to face final angle. This runs regardless of spinOnSpot to guarantee we're facing
    // the desired final angle
    if (goal_at_end) {
        turnToPoint(drive, turnP);
    } else {
        turnToAngle(drive, finalAngleDeg, final_angle_tolerance_deg, false, turnP);   
    }
    // moveMotors(drive, 0.0, 0.0);
    stopMotors(drive);


    // Delay and reprint final dist from target location to see how inaccurate this is because this isn't using a trapezoidal profile
    // pros::delay(1500); imagine there being a random 1.5 second delay left lying around from testing
    if (printMessages) {
        pros::lcd::set_text(5, "Dist from end: " + std::to_string(sqrt(pow(drive.getX() - ORIGINAL_PATH_FINAL[0], 2) + pow(drive.getY() - ORIGINAL_PATH_FINAL[1], 2))));
        pros::lcd::set_text(6, "DONE");
    }
}

// default x, y coords are the goal/net
void turnToPoint(traditional_drive& drive, double pointX, double pointY, double turnP) {
    double FINAL_ANGLE_TOLERANCE = 3.0;
    double desiredAngle = atan2(pointX - drive.getX(), pointY - drive.getY()) * 180 / M_PI;
    if (desiredAngle < 0) desiredAngle += 360;

    while (std::abs(optimizeAngle(desiredAngle - drive.get_imu().get_heading())) > FINAL_ANGLE_TOLERANCE) {
        desiredAngle = atan2(pointX - drive.getX(), pointY - drive.getY()) * 180 / M_PI;
        if (desiredAngle < 0) desiredAngle += 360;
        double rotationalRPM = getRotationalRPM(drive.get_imu().get_heading(), desiredAngle, false, turnP);
        moveMotors(drive, rotationalRPM, -rotationalRPM);

        if (drive.get_controller().get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
            break;
        }

        pros::delay(50);
    }
    stopMotors(drive);
}
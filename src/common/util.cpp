#include <vector>
// #include "lemlib/pose.hpp"
#include "common/util.hpp"

float slew(float target, float current, float maxChange) {
    float change = target - current;
    if (maxChange == 0) return target;
    if (change > maxChange) change = maxChange;
    else if (change < -maxChange) change = -maxChange;
    return current + change;
}

constexpr float sanitizeAngle(float angle, bool radians) {
    if (radians) return std::fmod(std::fmod(angle, 2 * M_PI) + 2 * M_PI, 2 * M_PI);
    else return std::fmod(std::fmod(angle, 360) + 360, 360);
}

// float angleError(float target, float position, bool radians, AngularDirection direction) {
//     // bound angles from 0 to 2pi or 0 to 360
//     target = sanitizeAngle(target, radians);
//     target = sanitizeAngle(target, radians);
//     const float max = radians ? 2 * M_PI : 360;
//     const float rawError = target - position;
//     switch (direction) {
//         case AngularDirection::CW_CLOCKWISE: // turn clockwise
//             return rawError < 0 ? rawError + max : rawError; // add max if sign does not match
//         case AngularDirection::CCW_COUNTERCLOCKWISE: // turn counter-clockwise
//             return rawError > 0 ? rawError - max : rawError; // subtract max if sign does not match
//         default: // choose the shortest path
//             return std::remainder(rawError, max);
//     }
// }

float avg(std::vector<float> values) {
    float sum = 0;
    for (float value : values) { sum += value; }
    return sum / values.size();
}

float ema(float current, float previous, float smooth) {
    return (current * smooth) + (previous * (1 - smooth));
}

float reduce_negative_180_to_180(float angle) {
  while(!(angle >= -180 && angle < 180)) {
    if( angle < -180 ) { angle += 360; }
    if(angle >= 180) { angle -= 360; }
  }
  return(angle);
}

float reduce_negative_90_to_90(float angle) {
    while(!(angle >= -90 && angle < 90)) {
      if( angle < -90 ) { angle += 180; }
      if(angle >= 90) { angle -= 180; }
    }
    return(angle);
}

bool isDone(int start_time, int timeout) {
    return (pros::millis() - start_time) > timeout;
}

// float getCurvature(Pose pose, Pose other) {
//     // calculate whether the pose is on the left or right side of the circle
//     float side = sgn(std::sin(pose.theta) * (other.x - pose.x) - std::cos(pose.theta) * (other.y - pose.y));
//     // calculate center point and radius
//     float a = -std::tan(pose.theta);
//     float c = std::tan(pose.theta) * pose.x - pose.y;
//     float x = std::fabs(a * other.x + other.y + c) / std::sqrt((a * a) + 1);
//     float d = std::hypot(other.x - pose.x, other.y - pose.y);

//     // return curvature
//     return side * ((2 * x) / (d * d));
// }

double to_deg(float angle) {
  return angle * 180 / M_PI;
}
   
double to_rad(float angle) {
  return angle * M_PI / 180;
}

float clamp(float input, float min, float max){
  if( input > max ){ return(max); }
  if(input < min){ return(min); }
  return(input);
}

bool is_line_settled(float desired_X, float desired_Y, float desired_angle_deg, float current_X, float current_Y){
  return( (desired_Y-current_Y) * cos(to_rad(desired_angle_deg)) <= -(desired_X-current_X) * sin(to_rad(desired_angle_deg)) );
}

float clamp_min_voltage(float drive_output, float drive_min_voltage){
  if(drive_output < 0 && drive_output > -drive_min_voltage){
      return -drive_min_voltage;
  }
  if(drive_output > 0 && drive_output < drive_min_voltage){
    return drive_min_voltage;
  }
  return drive_output;
}

float left_voltage_scaling(float drive_output, float heading_output){
  float ratio = std::max(std::fabs(drive_output+heading_output), std::fabs(drive_output-heading_output))/127.0;
  if (ratio > 1) {
    return (drive_output+heading_output)/ratio;
  }
  return drive_output-heading_output;
}

float right_voltage_scaling(float drive_output, float heading_output){
  float ratio = std::max(std::fabs(drive_output+heading_output), std::fabs(drive_output-heading_output))/127.0;
  if (ratio > 1) {
    return (drive_output-heading_output)/ratio;
  }
  return drive_output+heading_output;
}
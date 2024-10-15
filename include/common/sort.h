#pragma once
#include <main.h>
#include <deque>

class sort {
    private:
        struct sort_consts_s {
            bool alliance;
            int distThreshold = 30;  // Distance threshold for distance sensor
        } sort_consts_;
        struct sort_state_s{
            std::deque<bool> ringQueue;
            bool isEjecting = false;
            bool colorFlag = false;
            bool distFlag = false;
            int delay = 7; //7 frames = 7*15ms = 105ms
            int curPos = 0;
            int power = 127;
        } sort_state_;

    public:
        sort(pros::Motor &rIntake, pros::Optical &rColor, pros::Distance &rDist, bool allianceColor);

        void sort::update();
        int sort::sift(); //0 for forward, 1 for reverse

        pros::Optical &color;
        pros::Distance &dist;
        pros::Motor &intake;
};
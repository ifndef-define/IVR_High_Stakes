#pragma once
#include <main.h>
#include <deque>

class sort {
    private:
        bool alliance;
        std::deque<bool> ringQueue;
        bool isEjecting = false;
        bool colorFlag = false;
        bool distFlag = false;
        int distThreshold = 30;  // Distance threshold for distance sensor
        int delay = 100; //ms
    public:
        sort(pros::Color &rColor, pros::Distance &rDist, bool allianceColor);

        int sort::update(int current);

        pros::Color &color;
        pros::Distance &dist;
};
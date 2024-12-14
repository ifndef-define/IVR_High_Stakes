#pragma once

#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
#include "math.h"
#include "main.h"

using namespace std;

#define SCREEN_WIDTH_MAX 480
#define SCREEN_HEIGHT_MAX 240
// #define RED 1
// #define BLUE 2
// #define SKILLS 3

namespace ui {
    typedef enum {
        COLOR_RED,
        COLOR_BLUE,
        COLOR_GREEN
    } auton_color_t;
    // typedef enum {
    //     RUN_OPTION_1,
    //     RUN_OPTION_2,
    //     RUN_OPTION_3
    // } auton_option_t;
    // typedef enum {
    //     AUTON_MATCH,
    //     AUTON_SKILLS
    // } auton_type_t;
    // struct autonSelection {
    //     auton_color_t color;
    //     auton_option_t option;
    //     auton_type_t type;
    // };

    static auton_color_t selection;

    void init();
    void shutdown();
}
void loadingScreen(void);
void screensaver(void);

// For loop iterator
typedef unsigned short int fli;
#pragma warning(disable: 4244) // Disable warning for possible loss of data of the loop iterator

LV_IMG_DECLARE(illini_logo_full);

LV_IMG_DECLARE(frame);
LV_IMG_DECLARE(blue_bkrd);
LV_IMG_DECLARE(red_bkrd);
LV_IMG_DECLARE(green_bkrd);
LV_IMG_DECLARE(default_bothOpen);
LV_IMG_DECLARE(default_bothClosed);
LV_IMG_DECLARE(default_leftOpen);
LV_IMG_DECLARE(default_rightOpen);
LV_IMG_DECLARE(angry);
LV_IMG_DECLARE(standby_smrik);
LV_IMG_DECLARE(standby_ready);
LV_IMG_DECLARE(standby_eyebrows);

// Define colors
#define MY_COLOR_RED LV_COLOR_MAKE(255, 0, 0)
#define MY_COLOR_BLUE LV_COLOR_MAKE(0, 0, 255)
#define MY_COLOR_GREEN LV_COLOR_MAKE(0, 255, 0)
#define MY_COLOR_YELLOW LV_COLOR_MAKE(255, 255, 0)
#define MY_COLOR_ORANGE LV_COLOR_MAKE(255, 165, 0)
#define MY_COLOR_WHITE LV_COLOR_MAKE(255, 255, 255)
#define MY_COLOR_BLACK LV_COLOR_MAKE(0, 0, 0)
#define MY_COLOR_GRAY LV_COLOR_MAKE(128, 128, 128)
#define MY_COLOR_PURPLE LV_COLOR_MAKE(128, 0, 128)
#define MY_COLOR_CYAN LV_COLOR_MAKE(0, 255, 255)
#define MY_COLOR_PINK LV_COLOR_MAKE(255, 192, 203)
#define MY_COLOR_BROWN LV_COLOR_MAKE(165, 42, 42)
#define MY_COLOR_TURQUOISE LV_COLOR_MAKE(64, 224, 208)
#define MY_COLOR_SILVER LV_COLOR_MAKE(192, 192, 192)
#define MY_COLOR_GOLD LV_COLOR_MAKE(255, 215, 0)
#define MY_COLOR_NAVY LV_COLOR_MAKE(0, 0, 128)
#define MY_COLOR_TEAL LV_COLOR_MAKE(0, 128, 128)
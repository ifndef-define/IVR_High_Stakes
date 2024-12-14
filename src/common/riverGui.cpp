#include "main.h"
#include "common/gui.h"

static lv_obj_t *ss_screen;
static lv_obj_t *frame_obj;
static lv_obj_t *bkrd;
static lv_obj_t *face;
static lv_obj_t *eyebrows;

void ui::init() {
    lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(lv_scr_act(), MY_COLOR_BLACK, 0);

    loadingScreen();
    ss_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(ss_screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(ss_screen, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(ss_screen, MY_COLOR_BLACK, 0);
    lv_scr_load(ss_screen);
    screensaver();
}

double razeBlickDelay = 2.0;
void defaultMode() {
    lv_obj_align(face, LV_ALIGN_CENTER, 0, 10);
    lv_obj_add_flag(eyebrows, LV_OBJ_FLAG_HIDDEN);

    if(rand()%2 == 0)
        lv_img_set_src(face, &default_leftOpen);
    else
        lv_img_set_src(face, &default_rightOpen);
    pros::delay(100);
    lv_img_set_src(face, &default_bothClosed);
    pros::delay(100);
    lv_img_set_src(face, &default_bothOpen);
    razeBlickDelay = ((double)((rand()%4) + 1.5) + (double)((rand()%8)/10.0));  // Blinking (1.5-5.5)
    pros::delay(razeBlickDelay * 1000);
}

void anim_x_cb(lv_obj_t *var, int32_t v)
{
    lv_obj_set_y(var, v);
}

double tauntDelay = 3.0;
bool once = true;
static lv_anim_t a;
void tauntMode() {
    if (once) {
        // Animation
        lv_anim_init(&a);

        lv_anim_set_var(&a, eyebrows);
        lv_anim_set_time(&a, 250);
        lv_anim_set_values(&a, -35, -55);
        lv_anim_set_playback_time(&a, 250);
        lv_anim_set_playback_delay(&a, 15);
        lv_anim_set_repeat_count(&a, 2);
        lv_anim_set_repeat_delay(&a, 125);
        lv_anim_set_early_apply(&a, true);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)anim_x_cb);

        once = false;
    }
    lv_obj_clear_flag(eyebrows, LV_OBJ_FLAG_HIDDEN);
    lv_img_set_src(face, &standby_ready);
    lv_obj_align(face, LV_ALIGN_CENTER, 0, 25);
    
    lv_img_set_src(face, &standby_smrik);
    delay(800);
    lv_anim_start(&a);
    delay(1700);
    lv_img_set_src(face, &standby_ready);

    tauntDelay = ((double)((rand()%4) + 3) + (double)((rand()%8)/10.0)); // Taunting (3-6)
    pros::delay(tauntDelay * 1000);
}

void glitchMode() {
    lv_img_set_src(face, &angry);
    lv_obj_add_flag(eyebrows, LV_OBJ_FLAG_HIDDEN);
    // Glitch +- 5 pixels in x and y
    lv_obj_align(face, LV_ALIGN_CENTER, (rand() % 21 + (-5)), (rand() % 21 + (-5)));
}

void displayRunner() {
    /**
     * State 0: Default
     * State 1: Comp Disabled
     * State 2: Comp Enabled
     */
    pros::screen_touch_status_s_t touch;
    ui::selection = ui::COLOR_BLUE; 

    pros::Task touchTask([&]{
        while (true) {
            touch = pros::c::screen_touch_status();
            if(touch.touch_status == pros::E_TOUCH_HELD) {
                if (ui::selection == ui::COLOR_BLUE) {
                    ui::selection = ui::COLOR_RED;
                    lv_img_set_src(bkrd, &red_bkrd);
                } else if (ui::selection == ui::COLOR_RED) {
                    ui::selection = ui::COLOR_GREEN;
                    lv_img_set_src(bkrd, &green_bkrd);
                } else if (ui::selection == ui::COLOR_GREEN) {
                    ui::selection = ui::COLOR_BLUE;
                    lv_img_set_src(bkrd, &blue_bkrd);
                }

                delay(2000);
            }

            delay(20);
        }
    });

    while(true) {
        if (!pros::competition::is_connected()) {
            defaultMode();
        } else {
            if (pros::competition::is_disabled()) {
                tauntMode();
            } else {
                glitchMode();
            }
        }

        delay(50);
    }
}

void screensaver(void) {
    bkrd = lv_img_create(lv_scr_act());
    lv_obj_align(bkrd, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_src(bkrd, &blue_bkrd);

    frame_obj = lv_img_create(lv_scr_act());
    lv_img_set_src(frame_obj, &frame);
    lv_obj_align(frame_obj, LV_ALIGN_CENTER, 0, 0);

    face = lv_img_create(lv_scr_act());
    lv_obj_align(face, LV_ALIGN_CENTER, 0, 10);

    eyebrows = lv_img_create(lv_scr_act());
    lv_img_set_src(eyebrows, &standby_eyebrows);
    lv_obj_align(eyebrows, LV_ALIGN_CENTER, 0, -35);
    lv_obj_add_flag(eyebrows, LV_OBJ_FLAG_HIDDEN);

    displayRunner();
}   

/**
 * TODO: Fade into main screen immeditely after
 */
void loadingScreen(void) {
    // Load Illini Logo Full
    lv_obj_t *illini_logo_img = lv_img_create(lv_scr_act());
    lv_img_set_src(illini_logo_img, &illini_logo_full);
    lv_obj_align(illini_logo_img, LV_ALIGN_CENTER, 0, -25);

    // Load Bar Style
    static lv_style_t lb_style;
    static lv_style_t lb_style_indic;

    lv_style_init(&lb_style);
    lv_style_set_radius(&lb_style, 1);
    lv_style_set_pad_all(&lb_style, 6); /*To make the indicator smaller*/
    lv_style_set_border_color(&lb_style, LV_COLOR_MAKE32(133, 133, 133));
    lv_style_set_border_width(&lb_style, 6);
    lv_style_set_anim_time(&lb_style, 4000);

    lv_style_init(&lb_style_indic);
    lv_style_set_bg_opa(&lb_style_indic, LV_OPA_COVER); // Set transparency to zero (255 is fully opaque)
    lv_style_set_bg_color(&lb_style_indic, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_radius(&lb_style_indic, 0);

    // Load Bar Object
    lv_obj_t * load_bar = lv_bar_create(lv_scr_act());
    lv_obj_remove_style_all(load_bar);  /*To have a clean start*/
    lv_obj_add_style(load_bar, &lb_style, 0);
    lv_obj_add_style(load_bar, &lb_style_indic, LV_PART_INDICATOR);

    lv_obj_set_size(load_bar, 400, 45);
    lv_obj_center(load_bar);
    lv_bar_set_value(load_bar, 100, LV_ANIM_ON);
    lv_obj_align(load_bar, LV_ALIGN_BOTTOM_MID, 0, -10);
    delay(4000);
}

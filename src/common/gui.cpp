// #include "main.h"
// #include "liblvgl/lvgl.h"
// #include "gui.h"

// // static lv_obj_t *ss_screen;
// // static lv_obj_t *frame_obj;
// // static lv_obj_t *bkrd;
// // static lv_obj_t *face;
// // static lv_obj_t *eyebrows;

// static lv_style_t blue_button_style;
// static lv_style_t blue_selected_button_style;
// static lv_style_t red_button_style;
// static lv_style_t red_selected_button_style;
// static lv_style_t orange_button_style;
// static lv_style_t white_mod_button_style;
// static lv_style_t white_text_style;
// static lv_style_t auto_option_style;
// static lv_style_t auto_option_selected_style;
// static lv_style_t auto_option_disabled_style;

// #define SCR_LOAD 0
// static lv_obj_t *scr_load;
// #define SCR_AUTON 1
// static lv_obj_t *scr_auton;
// #define SCR_SCREEN_SAVER 2
// static lv_obj_t *scr_screen_saver;
// #define NUM_SCREENS 3

// lv_obj_t *screens[3] = {
//     scr_load,
//     scr_auton,
//     scr_screen_saver
// };

// #define SCREEN_WIDTH_MAX 480
// #define SCREEN_HEIGHT_MAX 240

// // For loop iterator
// typedef unsigned short int fli;
// #pragma warning(disable: 4244) // Disable warning for possible loss of data of the loop iterator

// static string robotName = "";
// bool running = true;
// int screenSaverReset = 0;
// short int currentAutoOption = -1;
// bool colorSort = true;
// bool forceCompMode = false;
// bool ringColor = false; // 0 = Red, 1 = Blue

// // Define colors
// #define MY_COLOR_RED LV_COLOR_MAKE(255, 0, 0)
// #define MY_COLOR_LIGHT_RED LV_COLOR_MAKE(255, 85, 85)
// #define MY_COLOR_BLUE LV_COLOR_MAKE(0, 0, 255)
// #define MY_COLOR_LIGHT_BLUE LV_COLOR_MAKE(0, 150, 255)
// #define MY_COLOR_GREEN LV_COLOR_MAKE(0, 255, 0)
// #define MY_COLOR_LIGHT_GREEN LV_COLOR_MAKE(115, 255, 115)
// #define MY_COLOR_YELLOW LV_COLOR_MAKE(255, 255, 0)
// #define MY_COLOR_ORANGE LV_COLOR_MAKE(255, 150, 0)
// #define MY_COLOR_DARK_ORANGE LV_COLOR_MAKE(165, 60, 0)
// #define MY_COLOR_WHITE LV_COLOR_MAKE(255, 255, 255)
// #define MY_COLOR_BLACK LV_COLOR_MAKE(0, 0, 0)
// #define MY_COLOR_GRAY LV_COLOR_MAKE(70, 70, 70)
// #define MY_COLOR_PURPLE LV_COLOR_MAKE(128, 0, 128)
// #define MY_COLOR_CYAN LV_COLOR_MAKE(0, 255, 255)
// #define MY_COLOR_PINK LV_COLOR_MAKE(255, 192, 203)
// #define MY_COLOR_BROWN LV_COLOR_MAKE(165, 42, 42)
// #define MY_COLOR_TURQUOISE LV_COLOR_MAKE(64, 224, 208)
// #define MY_COLOR_SILVER LV_COLOR_MAKE(192, 192, 192)
// #define MY_COLOR_GOLD LV_COLOR_MAKE(255, 215, 0)
// #define MY_COLOR_NAVY LV_COLOR_MAKE(0, 0, 128)
// #define MY_COLOR_TEAL LV_COLOR_MAKE(0, 128, 128)

// LV_IMG_DECLARE(illini_logo_full);
// // LV_IMG_DECLARE(jonah_frfr_sized);
// // LV_IMG_DECLARE(frame);
// // LV_IMG_DECLARE(blue_bkrd);
// // LV_IMG_DECLARE(red_bkrd);
// // LV_IMG_DECLARE(green_bkrd);
// // LV_IMG_DECLARE(default_bothOpen);
// // LV_IMG_DECLARE(default_bothClosed);
// // LV_IMG_DECLARE(default_leftOpen);
// // LV_IMG_DECLARE(default_rightOpen);
// // LV_IMG_DECLARE(angry);
// // LV_IMG_DECLARE(standby_smrik);
// // LV_IMG_DECLARE(standby_ready);
// // LV_IMG_DECLARE(standby_eyebrows);

// void loadingScreen(void);
// void screensaver(void);
// void autoSelector(void);

// void load_global_styles(void) {
//     lv_style_init(&blue_button_style);
//     lv_style_set_bg_color(&blue_button_style, MY_COLOR_BLACK);
//     lv_style_set_radius(&blue_button_style, 4);
//     lv_style_set_border_color(&blue_button_style, MY_COLOR_BLUE);
//     lv_style_set_border_width(&blue_button_style, 4);
//     lv_style_set_text_color(&blue_button_style, MY_COLOR_BLUE);
//     lv_style_set_text_align(&blue_button_style, LV_TEXT_ALIGN_CENTER);

//     lv_style_init(&blue_selected_button_style);
//     lv_style_set_bg_color(&blue_selected_button_style, MY_COLOR_LIGHT_BLUE);
//     lv_style_set_radius(&blue_button_style, 4);
//     lv_style_set_border_color(&blue_button_style, MY_COLOR_BLUE);
//     lv_style_set_border_width(&blue_button_style, 4);
//     lv_style_set_text_color(&blue_button_style, MY_COLOR_BLUE);
//     lv_style_set_text_align(&blue_button_style, LV_TEXT_ALIGN_CENTER);

//     lv_style_init(&red_button_style);
//     lv_style_set_bg_color(&red_button_style, MY_COLOR_BLACK);
//     lv_style_set_radius(&red_button_style, 4);
//     lv_style_set_border_color(&red_button_style, MY_COLOR_RED);
//     lv_style_set_border_width(&red_button_style, 4);
//     lv_style_set_text_color(&red_button_style, MY_COLOR_RED);
//     lv_style_set_text_align(&red_button_style, LV_TEXT_ALIGN_CENTER);   

//     lv_style_init(&red_selected_button_style);
//     lv_style_set_bg_color(&red_selected_button_style, MY_COLOR_LIGHT_RED);
//     lv_style_set_radius(&red_button_style, 4);
//     lv_style_set_border_color(&red_button_style, MY_COLOR_RED);
//     lv_style_set_border_width(&red_button_style, 4);
//     lv_style_set_text_color(&red_button_style, MY_COLOR_RED);
//     lv_style_set_text_align(&red_button_style, LV_TEXT_ALIGN_CENTER);
    
//     lv_style_init(&orange_button_style);
//     lv_style_set_bg_color(&orange_button_style, MY_COLOR_BLACK);
//     lv_style_set_radius(&orange_button_style, 4);
//     lv_style_set_border_color(&orange_button_style, MY_COLOR_ORANGE);
//     lv_style_set_border_width(&orange_button_style, 4);
//     lv_style_set_text_color(&orange_button_style, MY_COLOR_ORANGE);
//     lv_style_set_text_align(&orange_button_style, LV_TEXT_ALIGN_CENTER);

//     lv_style_init(&auto_option_style);
//     lv_style_set_bg_color(&auto_option_style, MY_COLOR_DARK_ORANGE);
//     lv_style_set_radius(&auto_option_style, 4);
//     lv_style_set_border_color(&auto_option_style, MY_COLOR_DARK_ORANGE);
//     lv_style_set_border_width(&auto_option_style, 4);
//     lv_style_set_text_color(&auto_option_style, MY_COLOR_WHITE);
//     lv_style_set_text_align(&auto_option_style, LV_TEXT_ALIGN_CENTER);

//     lv_style_init(&auto_option_selected_style);
//     lv_style_set_bg_color(&auto_option_selected_style, MY_COLOR_ORANGE);
//     lv_style_set_radius(&auto_option_selected_style, 4);
//     lv_style_set_border_color(&auto_option_selected_style, MY_COLOR_ORANGE);
//     lv_style_set_border_width(&auto_option_selected_style, 4);
//     lv_style_set_text_color(&auto_option_selected_style, MY_COLOR_WHITE);
//     lv_style_set_text_align(&auto_option_selected_style, LV_TEXT_ALIGN_CENTER);

//     lv_style_init(&auto_option_disabled_style);
//     lv_style_set_bg_color(&auto_option_disabled_style, MY_COLOR_GRAY);
//     lv_style_set_radius(&auto_option_disabled_style, 4);
//     lv_style_set_border_color(&auto_option_disabled_style, MY_COLOR_GRAY);
//     lv_style_set_border_width(&auto_option_disabled_style, 4);
//     lv_style_set_text_color(&auto_option_disabled_style, MY_COLOR_WHITE);
//     lv_style_set_text_align(&auto_option_disabled_style, LV_TEXT_ALIGN_CENTER);

//     lv_style_init(&white_mod_button_style);
//     lv_style_set_bg_color(&white_mod_button_style, MY_COLOR_BLACK);
//     lv_style_set_radius(&white_mod_button_style, 4);
//     lv_style_set_border_color(&white_mod_button_style, MY_COLOR_WHITE);
//     lv_style_set_border_width(&white_mod_button_style, 4);
//     lv_style_set_text_color(&white_mod_button_style, MY_COLOR_WHITE);
//     lv_style_set_text_align(&white_mod_button_style, LV_TEXT_ALIGN_CENTER);

//     lv_style_init(&white_text_style);
//     lv_style_set_text_color(&white_text_style, MY_COLOR_WHITE);
//     lv_style_set_text_align(&white_text_style, LV_TEXT_ALIGN_CENTER);
//     lv_style_set_border_opa(&white_text_style, LV_OPA_TRANSP);
//     lv_style_set_bg_opa(&white_text_style, LV_OPA_TRANSP);
// }

// void ui::init(string robot) {
//     robotName = robot;

//     for(fli i=0;i<NUM_SCREENS;i++) {
//         screens[i] = lv_obj_create(NULL);
//         lv_obj_clear_flag(screens[i], LV_OBJ_FLAG_SCROLLABLE);
//         lv_obj_set_scrollbar_mode(screens[i], LV_SCROLLBAR_MODE_OFF);
//         lv_obj_set_style_bg_color(screens[i], LV_COLOR_MAKE(0,0,0), LV_PART_MAIN);
//     }

//     load_global_styles();

//     lv_scr_load(screens[SCR_LOAD]);
//     loadingScreen();

//     lv_scr_load(screens[SCR_AUTON]);
//     autoSelector();

//     // screensaver();
// }

// // double razeBlickDelay = 2.0;
// // void defaultMode() {
// //     lv_obj_align(face, LV_ALIGN_CENTER, 0, 10);
// //     lv_obj_add_flag(eyebrows, LV_OBJ_FLAG_HIDDEN);

// //     if(rand()%2 == 0)
// //         lv_img_set_src(face, &default_leftOpen);
// //     else
// //         lv_img_set_src(face, &default_rightOpen);
// //     pros::delay(100);
// //     lv_img_set_src(face, &default_bothClosed);
// //     pros::delay(100);
// //     lv_img_set_src(face, &default_bothOpen);
// //     razeBlickDelay = ((double)((rand()%4) + 1.5) + (double)((rand()%8)/10.0));  // Blinking (1.5-5.5)
// //     pros::delay(razeBlickDelay * 1000);
// // }

// // void anim_x_cb(lv_obj_t *var, int32_t v)
// // {
// //     lv_obj_set_y(var, v);
// // }

// // double tauntDelay = 3.0;
// // bool once = true;
// // static lv_anim_t a;
// // void tauntMode() {
// //     if (once) {
// //         // Animation
// //         lv_anim_init(&a);

// //         lv_anim_set_var(&a, eyebrows);
// //         lv_anim_set_time(&a, 250);
// //         lv_anim_set_values(&a, -35, -55);
// //         lv_anim_set_playback_time(&a, 250);
// //         lv_anim_set_playback_delay(&a, 15);
// //         lv_anim_set_repeat_count(&a, 2);
// //         lv_anim_set_repeat_delay(&a, 125);
// //         lv_anim_set_early_apply(&a, true);
// //         lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)anim_x_cb);

// //         once = false;
// //     }
// //     lv_obj_clear_flag(eyebrows, LV_OBJ_FLAG_HIDDEN);
// //     lv_img_set_src(face, &standby_ready);
// //     lv_obj_align(face, LV_ALIGN_CENTER, 0, 25);
    
// //     lv_img_set_src(face, &standby_smrik);
// //     delay(800);
// //     lv_anim_start(&a);
// //     delay(1700);
// //     lv_img_set_src(face, &standby_ready);

// //     tauntDelay = ((double)((rand()%4) + 3) + (double)((rand()%8)/10.0)); // Taunting (3-6)
// //     pros::delay(tauntDelay * 1000);
// // }

// // void glitchMode() {
// //     lv_img_set_src(face, &angry);
// //     lv_obj_add_flag(eyebrows, LV_OBJ_FLAG_HIDDEN);
// //     // Glitch +- 5 pixels in x and y
// //     lv_obj_align(face, LV_ALIGN_CENTER, (rand() % 21 + (-5)), (rand() % 21 + (-5)));
// // }

// // void displayRunner() {
// //     /**
// //      * State 0: Default
// //      * State 1: Comp Disabled
// //      * State 2: Comp Enabled
// //      */
// //     pros::screen_touch_status_s_t touch;
// //     pros::Task touchTask([&]{
// //         while (running) {
// //             touch = pros::c::screen_touch_status();
            
// //             if(touch.touch_status == pros::E_TOUCH_HELD) {
// //                 screenSaverReset = 0;
// //                 lv_scr_load(screens[SCR_AUTON]);
// //             }

// //             if (screenSaverReset > 25) {
// //                 lv_scr_load(screens[SCR_SCREEN_SAVER]);
// //                 if (!pros::competition::is_connected()) {
// //                     defaultMode();
// //                 } else {
// //                     if (pros::competition::is_disabled()) {
// //                         tauntMode();
// //                     } else {
// //                         glitchMode();
// //                     }
// //                 }
// //             } else {
// //                 screenSaverReset++;
// //                 delay(1000);
// //             }
        
// //             delay(50);
// //         }
// //     });
// // }

// // void screensaver(void) {
// //     bkrd = lv_img_create(screens[SCR_SCREEN_SAVER]);
// //     lv_obj_align(bkrd, LV_ALIGN_CENTER, 0, 0);
// //     lv_img_set_src(bkrd, &blue_bkrd);

// //     frame_obj = lv_img_create(screens[SCR_SCREEN_SAVER]);
// //     lv_img_set_src(frame_obj, &frame);
// //     lv_obj_align(frame_obj, LV_ALIGN_CENTER, 0, 0);

// //     face = lv_img_create(screens[SCR_SCREEN_SAVER]);
// //     lv_obj_align(face, LV_ALIGN_CENTER, 0, 10);

// //     eyebrows = lv_img_create(screens[SCR_SCREEN_SAVER]);
// //     lv_img_set_src(eyebrows, &standby_eyebrows);
// //     lv_obj_align(eyebrows, LV_ALIGN_CENTER, 0, -35);
// //     lv_obj_add_flag(eyebrows, LV_OBJ_FLAG_HIDDEN);

// //     displayRunner();
// // }   

// void loadingScreen(void) {
//     // Load Illini Logo Full
//     static lv_obj_t *illini_logo_img = lv_img_create(screens[SCR_LOAD]);
//     lv_img_set_src(illini_logo_img, &illini_logo_full);
//     lv_obj_align(illini_logo_img, LV_ALIGN_CENTER, 0, -25);

//     // Load Bar Style
//     static lv_style_t lb_style;
//     static lv_style_t lb_style_indic;

//     lv_style_init(&lb_style);
//     lv_style_set_radius(&lb_style, 1);
//     lv_style_set_pad_all(&lb_style, 6); /*To make the indicator smaller*/
//     lv_style_set_border_color(&lb_style, LV_COLOR_MAKE32(133, 133, 133));
//     lv_style_set_border_width(&lb_style, 6);
//     lv_style_set_anim_time(&lb_style, 4000);

//     lv_style_init(&lb_style_indic);
//     lv_style_set_bg_opa(&lb_style_indic, LV_OPA_COVER); // Set transparency to zero (255 is fully opaque)
//     lv_style_set_bg_color(&lb_style_indic, lv_palette_main(LV_PALETTE_RED));
//     lv_style_set_radius(&lb_style_indic, 0);

//     // Load Bar Object
//     static lv_obj_t * load_bar = lv_bar_create(screens[SCR_LOAD]);
//     lv_obj_remove_style_all(load_bar);  /*To have a clean start*/
//     lv_obj_add_style(load_bar, &lb_style, 0);
//     lv_obj_add_style(load_bar, &lb_style_indic, LV_PART_INDICATOR);

//     lv_obj_set_size(load_bar, 400, 45);
//     lv_obj_center(load_bar);
//     lv_bar_set_value(load_bar, 100, LV_ANIM_ON);
//     lv_obj_align(load_bar, LV_ALIGN_BOTTOM_MID, 0, -10);
//     delay(4000);
// }

// static lv_obj_t *red_auto_1 = nullptr;
// static lv_obj_t *red_auto_2 = nullptr;
// static lv_obj_t *blue_auto_1 = nullptr;
// static lv_obj_t *blue_auto_2 = nullptr;
// static lv_obj_t *no_auton_btn = nullptr;
// static lv_obj_t *color_sort_switch = nullptr;
// static lv_obj_t *force_comp_mode_switch = nullptr;
// static lv_obj_t *ring_switch = nullptr;

// void cb_autoMenu(lv_event_t * e) {
//     lv_obj_t * btn = lv_event_get_target(e);
//     int btn_id = (int)btn->user_data;
//     screenSaverReset = 0; // Reset screensaver timer
    
//     switch (btn_id) {
//         case -1: // No Auto Button
//             currentAutoOption = -1;
//             // cout << "No Auto" << endl;
//             break;
//         case 0: // Red Auto 1 Button
//             currentAutoOption = 0;
//             // cout << "Red Auto 1" << endl;
//             break;
//         case 1: // Red Auto 2 Button
//             currentAutoOption = 1;
//             // cout << "Red Auto 2" << endl;
//             break;
//         case 2: // Blue Auto 1 Button
//             currentAutoOption = 2;
//             // cout << "Blue Auto 1" << endl;
//             break;
//         case 3: // Blue Auto 2 Button
//             currentAutoOption = 3;
//             // cout << "Blue Auto 2" << endl;
//             break;
//         case 4: // Color Sort Change
//             colorSort = color_sort_switch->state & LV_STATE_CHECKED;
//             // cout << "Color Sort Change -> " << ((color_sort_switch->state & LV_STATE_CHECKED) ? "On" : "Off") << endl;
//             return;
//             break;
//         case 5: // Force Comp Mode Change
//             forceCompMode = force_comp_mode_switch->state & LV_STATE_CHECKED;
//             // cout << "Force Comp Mode Change -> " << ((force_comp_mode_switch->state & LV_STATE_CHECKED) ? "On" : "Off") << endl;
//             return;
//             break;
//         case 6: // Ring Switch Change
//             ringColor = ring_switch->state & LV_STATE_CHECKED;
//             // cout << "Ring Switch Change -> " << ((ring_switch->state & LV_STATE_CHECKED) ? "Blue" : "Red") << endl;
//             return;
//             break;
//     }

//     if (currentAutoOption == -1) {
//         no_auton_btn->state = LV_STATE_CHECKED;
//         lv_obj_clear_state(red_auto_1, LV_STATE_CHECKED);
//         lv_obj_clear_state(red_auto_2, LV_STATE_CHECKED);
//         lv_obj_clear_state(blue_auto_1, LV_STATE_CHECKED);
//         lv_obj_clear_state(blue_auto_2, LV_STATE_CHECKED);
//         return;
//     } else if (currentAutoOption == 0) {
//         red_auto_1->state = LV_STATE_CHECKED;
//         lv_obj_clear_state(red_auto_2, LV_STATE_CHECKED);
//         lv_obj_clear_state(blue_auto_1, LV_STATE_CHECKED);
//         lv_obj_clear_state(blue_auto_2, LV_STATE_CHECKED);
//         lv_obj_clear_state(no_auton_btn, LV_STATE_CHECKED);
//         return;
//     } else if (currentAutoOption == 1) {
//         red_auto_2->state = LV_STATE_CHECKED;
//         lv_obj_clear_state(red_auto_1, LV_STATE_CHECKED);
//         lv_obj_clear_state(blue_auto_1, LV_STATE_CHECKED);
//         lv_obj_clear_state(blue_auto_2, LV_STATE_CHECKED);
//         lv_obj_clear_state(no_auton_btn, LV_STATE_CHECKED);
//         return;
//     } else if (currentAutoOption == 2) {
//         blue_auto_1->state = LV_STATE_CHECKED;
//         lv_obj_clear_state(red_auto_1, LV_STATE_CHECKED);
//         lv_obj_clear_state(red_auto_2, LV_STATE_CHECKED);
//         lv_obj_clear_state(blue_auto_2, LV_STATE_CHECKED);
//         lv_obj_clear_state(no_auton_btn, LV_STATE_CHECKED);
//         return;
//     } else if (currentAutoOption == 3) {
//         blue_auto_2->state = LV_STATE_CHECKED;
//         lv_obj_clear_state(red_auto_1, LV_STATE_CHECKED);
//         lv_obj_clear_state(red_auto_2, LV_STATE_CHECKED);
//         lv_obj_clear_state(blue_auto_1, LV_STATE_CHECKED);
//         lv_obj_clear_state(no_auton_btn, LV_STATE_CHECKED);
//         return;
//     }
// }

// /**
//  * TODO: Add a customizable auto option method to configure the auto names
//  */
// // void autoSelector(short int currentAuto, short int currentAutoOption) {
// void autoSelector() {
//     // Load Background
//     // static lv_obj_t *boner = lv_img_create(screens[SCR_AUTON]);
//     // lv_img_set_src(boner, &jonah_frfr_sized);
//     // lv_obj_align(boner, LV_ALIGN_LEFT_MID, 0, 0);

//     // Load Watermarks
//     static lv_obj_t *robot_label = lv_label_create(screens[SCR_AUTON]);
//     lv_label_set_text(robot_label, robotName.c_str());
//     lv_obj_add_style(robot_label, &white_text_style, 0);
//     lv_obj_align(robot_label, LV_ALIGN_TOP_LEFT, 5, 5);

//     static lv_obj_t *team_label = lv_label_create(screens[SCR_AUTON]);
//     lv_label_set_text(team_label, "ILLINI VEX ROBOTICS");
//     lv_obj_add_style(team_label, &white_text_style, 0);
//     lv_obj_align(team_label, LV_ALIGN_BOTTOM_LEFT, 5, -5);
    
//     // Choose Auton Label
//     lv_obj_t *choose_auton_label = lv_label_create(screens[SCR_AUTON]);
//     lv_label_set_text(choose_auton_label, "Auto ???");
//     lv_obj_add_style(choose_auton_label, &white_text_style, 0);
//     lv_obj_align(choose_auton_label, LV_ALIGN_TOP_RIGHT, -65, 10);

//     // Red Auto 1 Button
//     red_auto_1 = lv_btn_create(screens[SCR_AUTON]);
//     lv_obj_add_style(red_auto_1, &red_button_style, 0);
//     lv_obj_add_style(red_auto_1, &red_selected_button_style, 1);
//     lv_obj_set_size(red_auto_1, 65, 30);
//     lv_obj_align(red_auto_1, LV_ALIGN_TOP_RIGHT, -20-65-25, 30);
//     lv_obj_add_flag(red_auto_1, LV_OBJ_FLAG_CHECKABLE);
//     red_auto_1->user_data = (void *)(0);
//     lv_obj_add_event_cb(red_auto_1, cb_autoMenu, LV_EVENT_CLICKED, NULL);
//     lv_obj_t *red_label = lv_label_create(red_auto_1);
//     lv_label_set_text(red_label, "Red");
//     lv_obj_center(red_label); 

//     // Red Auto 2 Button
//     red_auto_2 = lv_btn_create(screens[SCR_AUTON]);
//     lv_obj_add_style(red_auto_2, &red_button_style, 0);
//     lv_obj_add_style(red_auto_2, &red_selected_button_style, 1);
//     lv_obj_set_size(red_auto_2, 65, 30);
//     lv_obj_align(red_auto_2, LV_ALIGN_TOP_RIGHT, -20, 30);
//     lv_obj_add_flag(red_auto_2, LV_OBJ_FLAG_CHECKABLE);
//     red_auto_2->user_data = (void *)(1);
//     lv_obj_add_event_cb(red_auto_2, cb_autoMenu, LV_EVENT_CLICKED, NULL);
//     lv_obj_t *red_label_2 = lv_label_create(red_auto_2);
//     lv_label_set_text(red_label_2, "Red 2");
//     lv_obj_center(red_label_2);

//     // Blue Auto 1 Button
//     blue_auto_1 = lv_btn_create(screens[SCR_AUTON]);
//     lv_obj_add_style(blue_auto_1, &blue_button_style, 0);
//     lv_obj_add_style(blue_auto_1, &blue_selected_button_style, 1);
//     lv_obj_set_size(blue_auto_1, 65, 30);
//     lv_obj_align(blue_auto_1, LV_ALIGN_TOP_RIGHT, -20-65-25, 30+10+30);
//     lv_obj_add_flag(blue_auto_1, LV_OBJ_FLAG_CHECKABLE);
//     blue_auto_1->user_data = (void *)(2);
//     lv_obj_add_event_cb(blue_auto_1, cb_autoMenu, LV_EVENT_CLICKED, NULL);
//     lv_obj_t *blue_label = lv_label_create(blue_auto_1);
//     lv_label_set_text(blue_label, "Blue");
//     lv_obj_center(blue_label);

//     // Blue Auto 2 Button
//     blue_auto_2 = lv_btn_create(screens[SCR_AUTON]);
//     lv_obj_add_style(blue_auto_2, &blue_button_style, 0);
//     lv_obj_add_style(blue_auto_2, &blue_selected_button_style, 1);
//     lv_obj_set_size(blue_auto_2, 65, 30);
//     lv_obj_align(blue_auto_2, LV_ALIGN_TOP_RIGHT, -20, 30+10+30);
//     lv_obj_add_flag(blue_auto_2, LV_OBJ_FLAG_CHECKABLE);
//     blue_auto_2->user_data = (void *)(3);
//     lv_obj_add_event_cb(blue_auto_2, cb_autoMenu, LV_EVENT_CLICKED, NULL);
//     lv_obj_t *blue_label_2 = lv_label_create(blue_auto_2);
//     lv_label_set_text(blue_label_2, "Blue 2");
//     lv_obj_center(blue_label_2);

//     // No Auto Option
//     no_auton_btn = lv_btn_create(screens[SCR_AUTON]);
//     lv_obj_add_style(no_auton_btn, &auto_option_style, 0);
//     lv_obj_add_style(no_auton_btn, &auto_option_selected_style, 1);
//     no_auton_btn->state = LV_STATE_CHECKED;
//     lv_obj_set_size(no_auton_btn, 80, 30);
//     lv_obj_align(no_auton_btn, LV_ALIGN_BOTTOM_RIGHT, -55, -5);
//     lv_obj_add_flag(no_auton_btn, LV_OBJ_FLAG_CHECKABLE);
//     no_auton_btn->user_data = (void *)(-1);
//     lv_obj_add_event_cb(no_auton_btn, cb_autoMenu, LV_EVENT_CLICKED, NULL);
//     lv_obj_t *no_auton_label = lv_label_create(no_auton_btn);
//     lv_label_set_text(no_auton_label, "No Auton");
//     lv_obj_center(no_auton_label);

//     static lv_obj_t *color_sort_label = lv_label_create(screens[SCR_AUTON]);
//     lv_label_set_text(color_sort_label, "Color Sort");
//     lv_obj_add_style(color_sort_label, &white_text_style, 0);
//     lv_obj_align(color_sort_label, LV_ALIGN_RIGHT_MID, -20, -8);

//     static lv_obj_t *force_comp_mode_label = lv_label_create(screens[SCR_AUTON]);
//     lv_label_set_text(force_comp_mode_label, "Force Comp");
//     lv_obj_add_style(force_comp_mode_label, &white_text_style, 0);
//     lv_obj_align(force_comp_mode_label, LV_ALIGN_RIGHT_MID, -12, 42);
    
//     color_sort_switch = lv_switch_create(screens[SCR_AUTON]);
//     color_sort_switch->user_data = (void *)(4);
//     lv_obj_add_event_cb(color_sort_switch, cb_autoMenu, LV_EVENT_VALUE_CHANGED, NULL);
//     color_sort_switch->state = LV_STATE_CHECKED; // Default to checked
//     lv_obj_align(color_sort_switch, LV_ALIGN_RIGHT_MID, -30, 15);
    
//     force_comp_mode_switch = lv_switch_create(screens[SCR_AUTON]);
//     force_comp_mode_switch->user_data = (void *)(5);
//     lv_obj_add_event_cb(force_comp_mode_switch, cb_autoMenu, LV_EVENT_VALUE_CHANGED, NULL);
//     lv_obj_align(force_comp_mode_switch, LV_ALIGN_RIGHT_MID, -30, 65);
    
//     static lv_style_t ring_switch_style;
//     lv_style_init(&ring_switch_style);
//     lv_style_set_bg_color(&ring_switch_style, MY_COLOR_RED);
    
//     static lv_obj_t *ring_switch_label = lv_label_create(screens[SCR_AUTON]);
//     lv_label_set_text(ring_switch_label, "Ring Color");
//     lv_obj_add_style(ring_switch_label, &white_text_style, 0);
//     lv_obj_align(ring_switch_label, LV_ALIGN_RIGHT_MID, -115, 15);

//     ring_switch = lv_switch_create(screens[SCR_AUTON]);
//     ring_switch->user_data = (void *)(6);
//     lv_obj_add_style(ring_switch, &ring_switch_style, LV_PART_MAIN);
//     lv_obj_add_event_cb(ring_switch, cb_autoMenu, LV_EVENT_VALUE_CHANGED, NULL);
    
//     lv_obj_align(ring_switch, LV_ALIGN_RIGHT_MID, -125, 42);

//     if(robotName == "Comp-24") {
//         lv_obj_add_flag(red_auto_2, LV_OBJ_FLAG_HIDDEN);
//         lv_obj_add_flag(blue_auto_2, LV_OBJ_FLAG_HIDDEN);
//     }
// }
  
// int ui::getCurrentAuto() {
//     return currentAutoOption;
// }

// bool ui::getRunColorSort() {
//     return colorSort;
// }

// bool ui::getRunForceCompMode() {
//     return forceCompMode;
// }

// bool ui::getRingColor() {
//     return ringColor;
// }

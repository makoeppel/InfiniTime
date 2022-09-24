#include "displayapp/screens/Boule.h"
#include "displayapp/DisplayApp.h"

using namespace Pinetime::Applications::Screens;

static void btnEventHandler(lv_obj_t* obj, lv_event_t event) {
  auto* screen = static_cast<Boule*>(obj->user_data);
  if (event == LV_EVENT_PRESSED) {
    //screen->ButtonPressed();
  } else if (event == LV_EVENT_RELEASED || event == LV_EVENT_PRESS_LOST) {
    //screen->MaskReset();
  } else if (event == LV_EVENT_SHORT_CLICKED) {
    screen->ToggleRunning();
  }
}

Boule::Boule(DisplayApp* app, Controllers::TimerController& timerController) : Screen(app), timerController {timerController} {

    lv_obj_t* title_time1 = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(title_time1, "Team 1");
    lv_label_set_align(title_time1, LV_LABEL_ALIGN_CENTER);
    lv_obj_set_style_local_text_color(title_time1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    lv_obj_align(title_time1, lv_scr_act(), LV_ALIGN_CENTER, -70, -60);

    lv_obj_t* title_time2 = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(title_time2, "Team 2");
    lv_label_set_align(title_time2, LV_LABEL_ALIGN_CENTER);
    lv_obj_set_style_local_text_color(title_time2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_obj_align(title_time2, lv_scr_act(), LV_ALIGN_CENTER, +70, -60);

    btnObjectMask = lv_objmask_create(lv_scr_act(), nullptr);
    lv_obj_set_size(btnObjectMask, 50, 100);
    lv_obj_align(btnObjectMask, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 0, 0);

    btnPlayPause = lv_btn_create(btnObjectMask, nullptr);
    btnPlayPause->user_data = this;
    lv_obj_set_style_local_radius(btnPlayPause, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
    lv_obj_set_style_local_bg_color(btnPlayPause, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
    lv_obj_set_event_cb(btnPlayPause, btnEventHandler);
    lv_obj_set_size(btnPlayPause, LV_HOR_RES, 50);

    txtPlayPause = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_align(txtPlayPause, btnPlayPause, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    txtThrowCount = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_set_style_local_text_font(txtThrowCount, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_42);
    lv_label_set_text_fmt(txtThrowCount, "%li", secondCounterThrow);
    lv_obj_align(txtThrowCount, nullptr, LV_ALIGN_IN_TOP_LEFT, +70, 0);

    time1_counter.Create();
    time2_counter.Create();
    lv_obj_set_size(time1_counter.GetObject(), 80, 170);
    lv_obj_set_size(time2_counter.GetObject(), 80, 170);
    lv_obj_align(time1_counter.GetObject(), nullptr, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    lv_obj_align(time2_counter.GetObject(), nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);

    if (timerController.IsRunning()) {
        SetTimerRunning();
    } else {
        SetTimerStopped();
    }

    taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
}

Boule::~Boule() {
    lv_task_del(taskRefresh);
    lv_obj_clean(lv_scr_act());
}

void Boule::Refresh() {
    if (timerController.IsRunning()) {
        uint32_t seconds = timerController.GetTimeRemaining() / 1000;
        secondCounterThrow = seconds % 60;
        lv_label_set_text_fmt(txtThrowCount, "%li", secondCounterThrow);
    }
}

void Boule::SetTimerRunning() {
  lv_label_set_text_static(txtPlayPause, "Stop");
}

void Boule::SetTimerStopped() {
  lv_label_set_text_static(txtPlayPause, "Start");
}

void Boule::ToggleRunning() {
    if (timerController.IsRunning()) {
        uint32_t seconds = timerController.GetTimeRemaining() / 1000;
        secondCounterThrow = seconds % 60;
        lv_label_set_text_fmt(txtThrowCount, "%li", secondCounterThrow);
        timerController.StopTimer();
        SetTimerStopped();
    } else {
        timerController.StartTimer(60 * 1000);
        SetTimerRunning();
    }
}

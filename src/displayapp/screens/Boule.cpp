#include "displayapp/screens/Boule.h"
#include "displayapp/DisplayApp.h"
#include "displayapp/screens/Symbols.h"

using namespace Pinetime::Applications::Screens;

static void btnThrowHandler(lv_obj_t* obj, lv_event_t event) {
  auto* screen = static_cast<Boule*>(obj->user_data);
  if (event == LV_EVENT_PRESSED) {
    //screen->ButtonPressed();
  } else if (event == LV_EVENT_RELEASED || event == LV_EVENT_PRESS_LOST) {
    //screen->MaskReset();
  } else if (event == LV_EVENT_SHORT_CLICKED) {
    screen->ToggleThrow();
  }
}

static void btnGameHandler(lv_obj_t* obj, lv_event_t event) {
  auto* screen = static_cast<Boule*>(obj->user_data);
  if (event == LV_EVENT_PRESSED) {
    //screen->ButtonPressed();
  } else if (event == LV_EVENT_RELEASED || event == LV_EVENT_PRESS_LOST) {
    //screen->MaskReset();
  } else if (event == LV_EVENT_SHORT_CLICKED) {
    screen->ToggleGame();
  }
}


Boule::Boule(DisplayApp* app,
             Controllers::TimerController& throwTimer,
             Pinetime::Controllers::DateTime& dateTimeController)
: Screen(app), throwTimer {throwTimer}, dateTimeController {dateTimeController} {

    // Time
    label = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_recolor(label, true);
    lv_label_set_text_fmt(label,
                          "#808080 %02d:%02d\n",
                          dateTimeController.Hours(),
                          dateTimeController.Minutes());
    lv_obj_align(label, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 0);


    // Team title 1
    lv_obj_t* title_time1 = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(title_time1, "Team 1");
    lv_label_set_align(title_time1, LV_LABEL_ALIGN_CENTER);
    lv_obj_set_style_local_text_color(title_time1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_LIGHTBLUE);
    lv_obj_align(title_time1, lv_scr_act(), LV_ALIGN_CENTER, -70, -60);


    // Time title 2
    lv_obj_t* title_time2 = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(title_time2, "Team 2");
    lv_label_set_align(title_time2, LV_LABEL_ALIGN_CENTER);
    lv_obj_set_style_local_text_color(title_time2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_LIGHTRED);
    lv_obj_align(title_time2, lv_scr_act(), LV_ALIGN_CENTER, +70, -60);


    // Start the throw timer
    btnObjectPlayPause = lv_objmask_create(lv_scr_act(), nullptr);
    lv_obj_set_size(btnObjectPlayPause, 50, 100);
    lv_obj_align(btnObjectPlayPause, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 0, 0);

    btnPlayPause = lv_btn_create(btnObjectPlayPause, nullptr);
    btnPlayPause->user_data = this;
    lv_obj_set_style_local_bg_color(btnPlayPause, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_LIGHTORANGE);
    lv_obj_set_event_cb(btnPlayPause, btnThrowHandler);
    lv_obj_set_size(btnPlayPause, LV_HOR_RES, 50);

    txtPlayPause = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_align(txtPlayPause, btnPlayPause, LV_ALIGN_IN_TOP_LEFT, 15, 5);

    txtThrowCount = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_recolor(txtThrowCount, true);
    lv_label_set_text_fmt(txtThrowCount, "%li", secondCounterThrow);
    lv_obj_align(txtThrowCount, nullptr, LV_ALIGN_IN_TOP_LEFT, 15, 25);

    if (throwTimer.IsRunning()) {
      ThrowCounterRunning();
    } else {
      ThrowCounterStopped();
    }


    // Start the game timer
    btnObjectStartGame = lv_objmask_create(lv_scr_act(), nullptr);
    lv_obj_set_size(btnObjectStartGame, 60, 100);
    lv_obj_align(btnObjectStartGame, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, 0, 0);

    btnStartGame = lv_btn_create(btnObjectStartGame, nullptr);
    btnStartGame->user_data = this;
    lv_obj_set_style_local_bg_color(btnStartGame, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_LIGHTORANGE);
    lv_obj_set_event_cb(btnStartGame, btnGameHandler);
    lv_obj_set_size(btnStartGame, LV_HOR_RES, 50);

    txtStartGame = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_align(txtStartGame, btnStartGame, LV_ALIGN_IN_TOP_LEFT, 0, 10);
    lv_label_set_text_static(txtStartGame, "Start");

    txtGameCount = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_recolor(txtGameCount, true);
    lv_label_set_text_fmt(txtGameCount, "#808080 --:--:--");
    lv_obj_align(txtGameCount, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 20);

    gameRunning = false;


    // Text middle
    lv_obj_t* txtTree = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(txtTree, " *\n /.\\\n /..\'\\\n /\'.\'\\\n /\'\'.\\\n ``||``\n");
    lv_label_set_align(txtTree, LV_LABEL_ALIGN_CENTER);
    lv_obj_set_style_local_text_color(txtTree, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_LIGHTGREEN);
    lv_obj_align(txtTree, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);

    txtBoule = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(txtBoule, "");
    lv_label_set_align(txtBoule, LV_LABEL_ALIGN_CENTER);
    lv_obj_set_style_local_text_color(txtBoule, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_LIGHTGREEN);
    lv_obj_align(txtBoule, lv_scr_act(), LV_ALIGN_CENTER, -40, 80);


    // Points counter
    point_counter1.Create();
    point_counter2.Create();
    lv_obj_set_size(point_counter1.GetObject(), 80, 170);
    lv_obj_set_size(point_counter2.GetObject(), 80, 170);
    lv_obj_align(point_counter1.GetObject(), nullptr, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    lv_obj_align(point_counter2.GetObject(), nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);


    // Refresh screen
    taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
}

Boule::~Boule() {
    lv_task_del(taskRefresh);
    lv_obj_clean(lv_scr_act());
}

void Boule::Refresh() {
    if (throwTimer.IsRunning()) {
      uint32_t seconds = throwTimer.GetTimeRemaining() / 1000;
      secondCounterThrow = seconds % 60;
      lv_label_set_text_fmt(txtThrowCount, "%li", secondCounterThrow);
    }
    if (gameRunning) {
      int curSec = dateTimeController.Hours() * 3600 + dateTimeController.Minutes() * 60 + dateTimeController.Seconds() - gameStartSec;
      lv_label_set_text_fmt(txtGameCount, 
                            "#808080 %02d:%02d:%2d\n",
                            curSec / 3600,
                            (curSec % 3600) / 60,
                            curSec % 60
                            );
      if ((curSec % 2 == 0) & curSec != lastSec) {
        if ( txtBouleState == 0 ) {
          lv_label_set_text_static(txtBoule, "   .");
          txtBouleState = 1;
        } else if (txtBouleState == 1) {
          lv_label_set_text_static(txtBoule, "   .o");
          txtBouleState = 2;
        } else if (txtBouleState == 2) {
          lv_label_set_text_static(txtBoule, "   .oo");
          txtBouleState = 3;
        } else if (txtBouleState == 3) {
          lv_label_set_text_static(txtBoule, "   . o");
          txtBouleState = 4;
        } else if (txtBouleState == 4) {
          lv_label_set_text_static(txtBoule, "  o. o");
          txtBouleState = 5;
        } else if (txtBouleState == 5) {
          lv_label_set_text_static(txtBoule, "");
          txtBouleState = 0;
        }
      }
      lastSec = curSec;
    }
    lv_label_set_text_fmt(label,
                          "#808080 %02d:%02d\n",
                          dateTimeController.Hours(),
                          dateTimeController.Minutes());
}

void Boule::ThrowCounterRunning() {
  lv_label_set_text_static(txtPlayPause, Symbols::pause);
}

void Boule::ThrowCounterStopped() {
  lv_label_set_text_static(txtPlayPause, Symbols::play);
}

void Boule::ToggleThrow() {
    if (throwTimer.IsRunning()) {
      uint32_t seconds = throwTimer.GetTimeRemaining() / 1000;
      secondCounterThrow = 0;
      lv_label_set_text_fmt(txtThrowCount, "%li", secondCounterThrow);
      throwTimer.StopTimer();
      ThrowCounterStopped();
    } else {
      throwTimer.StartTimer(60 * 1000);
      ThrowCounterRunning();
    }
}

void Boule::GameCounterRunning() {
  lv_label_set_text_static(txtStartGame, "End");
}

void Boule::GameCounterStopped() {
  lv_label_set_text_static(txtStartGame, "Start");
}

void Boule::ToggleGame() {
    if (gameRunning) {
      gameRunning = false;
      int curSec = dateTimeController.Hours() * 3600 + dateTimeController.Minutes() * 60 + dateTimeController.Seconds() - gameStartSec;
      lv_label_set_text_fmt(txtGameCount,
                            "%02d:%02d:%02d\n",
                            curSec / 3600,
                            (curSec % 3600) / 60,
                            curSec % 60
                            );
      GameCounterStopped();
    } else {
      gameRunning = true;
      txtBouleState = 0;
      gameStartSec = dateTimeController.Hours() * 3600 + dateTimeController.Minutes() * 60 + dateTimeController.Seconds();
      GameCounterRunning();
    }
}

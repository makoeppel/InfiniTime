#pragma once

#include "displayapp/screens/Screen.h"
#include "components/timer/TimerController.h"
#include "components/datetime/DateTimeController.h"
#include "displayapp/widgets/Counter.h"
#include <lvgl/lvgl.h>

namespace Pinetime {
    namespace Applications {
        namespace Screens {
            class Boule : public Screen {

            public:
                Boule(DisplayApp* app,
                    Controllers::TimerController& throwTimer,
                    Pinetime::Controllers::DateTime& dateTimeController);
                ~Boule() override;
                void Refresh() override;
                void ToggleThrow();
                void ToggleGame();

                void ThrowCounterRunning();
                void ThrowCounterStopped();

                void GameCounterRunning();
                void GameCounterStopped();

            private:
                Controllers::TimerController& throwTimer;
                
                lv_obj_t* txtPlayPause;
                lv_obj_t* txtThrowCount;
                lv_obj_t* btnObjectPlayPause;
                lv_obj_t* btnPlayPause;

                lv_obj_t* txtStartGame;
                lv_obj_t* txtGameCount;
                lv_obj_t* btnObjectStartGame;
                lv_obj_t* btnStartGame;
                bool gameRunning = false;
                uint32_t gameStartSec;

                lv_obj_t* txtBoule;
                uint8_t txtBouleState;
                int lastSec;

                lv_obj_t* label;

                uint32_t secondCounterThrow = 0;
                uint32_t secondCounterGame = 0;

                lv_task_t* taskRefresh;
                Widgets::Counter point_counter1 = Widgets::Counter(0, 13, jetbrains_mono_76);
                Widgets::Counter point_counter2 = Widgets::Counter(0, 13, jetbrains_mono_76);

                Pinetime::Controllers::DateTime& dateTimeController;
                
            };
        }
    }
}

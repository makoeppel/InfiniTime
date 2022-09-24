#pragma once

#include "displayapp/screens/Screen.h"
#include "components/timer/TimerController.h"
#include "displayapp/widgets/Counter.h"
#include <lvgl/lvgl.h>

namespace Pinetime {
    namespace Applications {
        namespace Screens {
            class Boule : public Screen {

            public:
                Boule(DisplayApp* app, Controllers::TimerController& timerController);
                ~Boule() override;
                void Refresh() override;
                void ToggleRunning();

            private:
                Controllers::TimerController& timerController;
                
                void SetTimerRunning();
                void SetTimerStopped();
                
                lv_obj_t* txtPlayPause;
                lv_obj_t* txtThrowCount;
                lv_obj_t* btnObjectMask;
                lv_obj_t* btnPlayPause;
                uint32_t minuteCounter = 0;
                uint32_t secondCounter = 0;
                uint32_t secondCounterThrow = 0;

                lv_task_t* taskRefresh;
                Widgets::Counter time1_counter = Widgets::Counter(0, 13, jetbrains_mono_76);
                Widgets::Counter time2_counter = Widgets::Counter(0, 13, jetbrains_mono_76);
                
            };
        }
    }
}

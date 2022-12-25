#pragma once

#include <FreeRTOS.h>
#include <timers.h>
#include "systemtask/Messages.h"

namespace Pinetime {
  namespace System {
    class SystemTask;
  }
  namespace Controllers {

    class TimerController {
    public:
      TimerController() = default;

      void Init(System::SystemTask* systemTask, System::Messages systemMessage);

      void StartTimer(uint32_t duration);

      void StopTimer();

      uint32_t GetTimeRemaining();

      bool IsRunning();

      void OnTimerEnd();

    private:
      System::Messages systemMessage;
      System::SystemTask* systemTask = nullptr;
      TimerHandle_t timer;
    };
  }
}

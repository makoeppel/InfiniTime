#pragma once
#include <cstdint>
namespace Pinetime {
  namespace Applications {
    namespace Display {
      enum class Messages : uint8_t {
        GoToSleep,
        GoToRunning,
        UpdateDateTime,
        UpdateBleConnection,
        TouchEvent,
        ButtonPushed,
        ButtonLongPressed,
        ButtonLongerPressed,
        ButtonDoubleClicked,
        NewNotification,
        TimerDone,
        ThrowOver,
        BleFirmwareUpdateStarted,
        UpdateTimeOut,
        DimScreen,
        RestoreBrightness,
        ShowPairingKey,
        AlarmTriggered,
        Clock,
        BleRadioEnableToggle
      };
    }
  }
}

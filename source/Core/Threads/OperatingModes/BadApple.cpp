#include "BadApple.hpp"

#include "OperatingModes.h"
extern osThreadId    GUITaskHandle;
extern osThreadId    MOVTaskHandle;
extern osThreadId    PIDTaskHandle;
extern OperatingMode currentMode;

void showBadApple(void) {
  currentMode        = OperatingMode::badapple;

  TickType_t Millis = xTaskGetTickCount() / (TICKS_10MS / 10);
  uint32_t     fnum = 0;
  uint32_t missedframes = 0;


  TickType_t CurrentMillis, OldMillis = 0;
  uint32_t     screen = 0;
  uint32_t MillisDiv;

  ButtonState b;
  OLED::clearScreen();                                     // Ensure the buffer starts clean
  while (fnum < FRAMES) {
    CurrentMillis = xTaskGetTickCount() / (TICKS_10MS / 10);
    MillisDiv = CurrentMillis - OldMillis;
    TickType_t fnew = (CurrentMillis - Millis)/(1000/FPS);
    if (fnew > fnum) {
      if (fnew - fnum > 1) {
	      missedframes += fnew - fnum -1;
      }
      OLED::drawArea(38, 0, 21, 16, BadAppleFrame[fnew]);
      OldMillis = xTaskGetTickCount() / (TICKS_10MS / 10);
      fnum = fnew;
      //screen++;
    }
    /*
    if (MillisDiv >= 1000/FPS) {
      OLED::drawArea(38, 0, 21, 16, BadAppleFrame[screen]);
      OldMillis = xTaskGetTickCount() / (TICKS_10MS / 10);
      screen++;
    }
    */
    OLED::refresh();
    
    b = getButtonState();
    if (b == BUTTON_F_SHORT) break;
    /*
    switch (screen) {
    case 0: // Build Date
      OLED::drawArea(38, 0, 21, 16, BadAppleFrame[0]);
      break;
    case 1: // Device ID
    {
      OLED::drawArea(38, 0, 21, 16, BadAppleFrame[1]);
    } break;
    case 2: // ACC Type
      break;
    default:
      break;
    }
    */
    /*
    b = getButtonState();
    if (b == BUTTON_B_SHORT) {
      return;
    } else if (b == BUTTON_F_LONG || b == BUTTON_F_SHORT) {
      screen++;
      screen = screen % 1095;
    }

    GUIDelay(); */
  }
  for(;;) {
    OLED::clearScreen();
    OLED::setCursor(0, 0);
    OLED::printNumber(fnum, 8, FontStyle::SMALL);
    OLED::setCursor(0, 8);
    //missedframes = 334;
    OLED::printNumber(missedframes, 8, FontStyle::SMALL, false);
    OLED::refresh();
    b = getButtonState();
    if (b == BUTTON_F_SHORT) return;
  }
  return;
}

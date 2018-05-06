#ifndef _BUTTONTEST_H_
#define _BUTTONTEST_H_

#include <Arduino.h>
#include "GameBoard.h"

class ButtonTest {
  public:
    void begin(Adafruit_RGBLCDShield _lcd);
    void RunTest();
    
  private:
    Adafruit_RGBLCDShield lcd;
    volatile bool bg_ButtonHit;
};

// Interrupt
void bt_ButtonInterrupt();


#endif    // _BUTTONTEST_H_

#ifndef _REACTION_H_
#define _REACTION_H_

#include <Arduino.h>
#include "GameBoard.h"

/////////////////////////////////////////
// Message Strings (pad to 16 chars)
const String r_Ready    = "Ready...";
const String r_Aim      = "Aim...";
const String r_Fire     = "Fire.";
const String r_xPasses  = "Running x  times";
const String r_BestTime = "Best Reaction   ";

#define REACT_PASSES  10

class ReactionTime {
  public:
    void begin(Adafruit_RGBLCDShield _lcd);
    void SelectGame();

  private:
    void RunGame();
    uint8_t LevelUp();
    uint8_t LevelDown();
    Adafruit_RGBLCDShield lcd;
    uint8_t rLevel;    
};

// Interrupt
void rt_ButtonInterrupt();

#endif    // _REACTION_H_

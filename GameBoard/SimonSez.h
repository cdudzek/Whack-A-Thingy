#ifndef _SIMONSEZ_H_
#define _SIMONSEZ_H_

#include <Arduino.h>
#include "GameBoard.h"

/////////////////////////////////////////
// Game Levels
#define MaxMoves 64

/////////////////////////////////////////
// Message Strings (pad to 16 chars)
const String s_Listen     = "Listen to me:   ";
const String s_Playback   = "Play back to me ";
const String s_Sorry      = "Sorry!          ";
const String s_Perfect    = "Perfect Game!   ";
const String s_Matched    = "Matched ";         // Not full length, partial message
const String s_Aborted    = "Stopped Game    ";

class SimonSez {
  public:
    void begin(Adafruit_RGBLCDShield _lcd);
    void SelectGame();

  private:
    void RunGame();
    uint8_t LevelUp();
    uint8_t LevelDown();
    void DumpSequence();
    void FlashLight(uint8_t pin);
    bool GetSteps(uint8_t steps, long timeout);
    Adafruit_RGBLCDShield lcd;
    uint8_t sLevel;
    uint8_t Sequence[MaxMoves];
    unsigned long flashtime;              // time to flash each button in sequence
    unsigned long flashgap;               // time between flashing each button
    unsigned long steptimeout;            // timeout to hit each button in sequence
    bool quitgame;
};

#endif    // _SIMONSEZ_H_

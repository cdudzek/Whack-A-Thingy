#ifndef _WHACKER_H_
#define _WHACKER_H_

#include <Arduino.h>
#include "GameBoard.h"

/////////////////////////////////////////
// Game Levels
struct WhackerLevel_T {
  String label;             // Display name
  unsigned long timeout;    // Max time to leave a target lit
  uint8_t maxbuttons;       // Max # target buttons to use during game
  uint8_t maxspawns;        // Max # buttons to spawn at once
  int     totalspawns;      // Total spawns for the game
};

// Minimum time target must be off before reusing
#define MIN_TIME_OFF  750
// Do not worry about checking for cheats in too fast of a game, too many false positives
#define NOCHEAT_TIME  750

class Whacker {
  public:
    //Whacker();
    void begin(Adafruit_RGBLCDShield _lcd);
    void SelectGame();

  private:
    unsigned int spawns;
    unsigned int score;
    void RunGame();
    bool SpawnNew();
    uint8_t LevelUp();
    uint8_t LevelDown();
    Adafruit_RGBLCDShield lcd;
    uint8_t whLevel;
    unsigned long spawntime[8]; // Array size must be <= NumTargets
    uint8_t Active();
    long LastTimeOff[8]; // Array size must be <= NumTargets
};

// Interrupt
void w_ButtonInterrupt();

#endif    // _WHACKER_H_

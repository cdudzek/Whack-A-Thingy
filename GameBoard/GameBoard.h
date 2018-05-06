#ifndef _GAMEBOARD_H_
#define _GAMEBOARD_H_

#include <Arduino.h>
#include <Adafruit_RGBLCDShield.h>
#include "ButtonTest.h"
#include "Reaction.h"
#include "SimonSez.h"
#include "Whacker.h"

/////////////////////////////////////////
// Message Strings (pad to 16 chars)
//                          
const String s_Blank        = "                ";
const String s_AppTitle     = "Game Board v0.2 ";
const String s_SelectGame   = "Select Game:    ";
const String s_SelectLevel  = "Select Level:   ";
const String s_Exit         = "Exit to MainMenu";
const String s_AnyKey       = "Press any button";
const String s_Ready        = "Ready           ";
const String s_NumButtons   = "# of Buttons:   ";
const String s_UsingButtons = "Using x buttons ";
const String s_Cheater      = "CHEATER!        ";
const String s_ButtonDown   = "Button is held  ";
const String s_CountDown    = "Ready in        ";
const String s_Score        = "Score           ";
// Games
const String sg_Simon       = "Simon Sez       ";
const String sg_Whacker     = "Whack-A-Thingy  ";
const String sg_Test        = "Button Test     ";
const String sg_React       = "Reaction Time   ";
// List of games (match order to enum GameType below)
const String NameOfTheGame[] = {
  sg_Whacker,
  sg_Simon,
  sg_React,
  sg_Test,
  s_Blank         // No game (gINVALID / wrap end of list)
};

/////////////////////////////////////////
// Game Options
enum GameType { WHACKER, SIMON, REACTION, HITTEST, gINVALID };      // Match order to NameOfTheGame[] above
GameType NextGame(GameType gt);   // Scroll game list
GameType PrevGame(GameType gt);   // Scroll game list
bool LaunchGame(GameType gt, Adafruit_RGBLCDShield _lcd);     // Launch the selected game

/////////////////////////////////////////
// Backlight and LED colors
#define BLACK 0x0
#define RED 0x1
#define GREEN 0x2
#define YELLOW 0x3
#define BLUE 0x4
#define VIOLET 0x5
#define TEAL 0x6
#define WHITE 0x7

/////////////////////////////////////////
// Scroll Backlight Colors
uint8_t ColorScrollUp(uint8_t color);
uint8_t ColorScrollDown(uint8_t color);

/////////////////////////////////////////
// Buttons and Interrupts
//
// Mega Interrupts: 2, 3, 18, 19, 20, 21
// Uno Interrupts: 2, 3
//
#define DEBOUNCE   200
#define INT_PIN1   2
#define INT_PIN2   3
#define LED_START  5
#define LED_SELECT 6
uint8_t ReadAllButtons(void);     // Return bitmask of pressed target buttons
String LcdButtons(Adafruit_RGBLCDShield _lcd);  // Return string of pressed buttons on LCD
void PressAnyKey(Adafruit_RGBLCDShield _lcd);   // Pause until button is pressed
void SweepLights(void);     // Light up each button (typically on boot)
void Countdown(unsigned long ms, Adafruit_RGBLCDShield _lcd);   // Countdown timer

struct TargetPins_T {
  uint8_t LedEnable;
  uint8_t SwitchEnable;
  uint8_t SwitchRead;
  uint8_t LedColor;       // use #defined colors above
};

// Mega pinouts, lower connector:
// Even side: 22-52
// Odd side:  23-53
const TargetPins_T Targets[] = 
{
  { 29, 31, 33, YELLOW },     // Yellow 2
  { 34, 36, 38, GREEN  },     // Green 1
  { 41, 43, 45, BLUE   },     // Blue 2
  { 22, 24, 26, RED    },     // Red 1  
  { 23, 25, 27, RED    },     // Red 2  
  { 40, 42, 44, BLUE   },     // Blue 1
  { 35, 37, 39, GREEN  },     // Green 2
  { 28, 30, 32, YELLOW }      // Yellow 1
};
const size_t NumTargets = sizeof(Targets) / sizeof(TargetPins_T);

/////////////////////////////////////////
// Custom LCD symbols
#define SMILEY 0
#define FROWNIE 1
const uint8_t smiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};
const uint8_t frownie[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b10001
};


#endif // _GAMEBOARD_H_

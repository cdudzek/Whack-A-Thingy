#include "GameBoard.h"

/////////////////////////////////////////
// Buttons and Interrupts
uint8_t ReadAllButtons(void) {
  uint8_t reply = 0;
  // initialize all bits in use
  for (int t = 0; t < NumTargets; t++) {
    if (digitalRead(Targets[t].SwitchRead) == LOW)
      bitSet(reply, t);
  }
  return reply;
}

String LcdButtons(Adafruit_RGBLCDShield _lcd) {
  String output = "";
  uint8_t btns = _lcd.readButtons();
  if (btns & BUTTON_SELECT)
    output += "S";
  else
    output += " ";
  if (btns & BUTTON_UP)
    output += "U";
  else
    output += " ";
  if (btns & BUTTON_DOWN)
    output += "D";
  else
    output += " ";
  if (btns & BUTTON_LEFT)
    output += "L";
  else
    output += " ";
  if (btns & BUTTON_RIGHT)
    output += "R";
  else
    output += " ";

  return output;
}

void PressAnyKey(Adafruit_RGBLCDShield _lcd) {
  // debounce any previous button press that got us here
  delay(DEBOUNCE);
  
  // Set timeout so lights don't stay on forever
  long timeout = 30 * 1000;
  long start = millis();
  
  // Turn on all the lights
  for (int t = 0; t < NumTargets; t++) {
    digitalWrite(Targets[t].SwitchEnable, LOW);
    digitalWrite(Targets[t].LedEnable, LOW); 
  }
  digitalWrite(LED_START, LOW);
  digitalWrite(LED_SELECT, LOW);

  // Wait for hit
  while ((_lcd.readButtons() == 0) 
          && (ReadAllButtons() == 0)
          && (millis() - start) < timeout) {
    delay(5);
  }
  
  // Turn off all the lights when you leave
  for (int t = 0; t < NumTargets; t++) {
    digitalWrite(Targets[t].SwitchEnable, HIGH);
    digitalWrite(Targets[t].LedEnable, HIGH); 
  }
  digitalWrite(LED_START, HIGH);
  digitalWrite(LED_SELECT, HIGH);
}

void SweepLights(void) {
  long ontime = 200;
  // Sweep
  digitalWrite(LED_START, LOW);
  delay(ontime);
  digitalWrite(LED_START, HIGH);
  digitalWrite(LED_SELECT, LOW);
  delay(ontime);
  digitalWrite(LED_SELECT, HIGH);
  for (int t = 0; t < NumTargets; t++) {
    digitalWrite(Targets[t].LedEnable, LOW);
    delay(ontime);
    digitalWrite(Targets[t].LedEnable, HIGH);
  }
  delay(ontime);
  
  // Flash All
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_START, LOW);
    digitalWrite(LED_SELECT, LOW);
    for (int t = 0; t < NumTargets; t++) {
      digitalWrite(Targets[t].LedEnable, LOW);
    }
    delay(ontime);
    digitalWrite(LED_START, HIGH);
    digitalWrite(LED_SELECT, HIGH);
    for (int t = 0; t < NumTargets; t++) {
      digitalWrite(Targets[t].LedEnable, HIGH);
    }
    delay(ontime);
  }
}

void Countdown(unsigned long ms, Adafruit_RGBLCDShield _lcd) {
  unsigned long start = millis();
  long remains = 0;
  _lcd.clear();
  _lcd.home();
  _lcd.setBacklight(GREEN);
  _lcd.print(s_CountDown);
  while ((remains = (start + ms) - millis()) > 0) {
    if (remains < 2000)
      _lcd.setBacklight(RED);
    else if (remains < 4000)
      _lcd.setBacklight(YELLOW);
    _lcd.setCursor(9, 0);
    _lcd.print((float)remains/1000.0, 1);
  }
}


/////////////////////////////////////////
// Scroll Backlight Colors
uint8_t ColorScrollUp(uint8_t color) {
  return ((color == 7) ? 0 : ++color);
}

uint8_t ColorScrollDown(uint8_t color) {
  return ((color == 0) ? 0x7 : --color);
}


/////////////////////////////////////////
// Game Options
GameType NextGame(GameType gt) {
  int gi = (int)gt;
  gi++;
  if (gi == GameType::gINVALID)
    gi = 0;   // reset to beginning of list
  return (GameType)gi;
}

GameType PrevGame(GameType gt) {
  int gi = (int)gt;
  if (gi == 0)
    gi = GameType::gINVALID;  // wrap to end of list
  gi--;
  return (GameType)gi;
}

bool LaunchGame(GameType gt, Adafruit_RGBLCDShield _lcd) {
  bool success = true;
  ButtonTest bt;
  ReactionTime rt;
  SimonSez ss;
  Whacker wh;

  switch (gt) {
    case HITTEST:
      bt.begin(_lcd);
      bt.RunTest();
      success = true;
      break;
      
    case REACTION:
      rt.begin(_lcd);
      rt.SelectGame();
      success = true;
      break;
      
    case SIMON:
      ss.begin(_lcd);
      ss.SelectGame();
      success = true;
      break;
      
    case WHACKER:
      wh.begin(_lcd);
      wh.SelectGame();
      success = true;
      break;
      
    default:
      _lcd.clear();
      _lcd.home();
      _lcd.print("Game not yet    ");
      _lcd.setCursor(0, 1);
      _lcd.print("implemented.    ");
      delay(3000);
      success = false;
      break;
  }
  
  return success;
}


#include "SimonSez.h"

void SimonSez::begin(Adafruit_RGBLCDShield _lcd) {
  this->lcd = _lcd;
  this->sLevel = NumTargets;
  for (int i = 0; i < MaxMoves; i++) {
    Sequence[i] = 0;
  }
  quitgame = false;
}

void SimonSez::SelectGame() {
  lcd.clear();
  lcd.home();
  lcd.print(sg_Simon);
  delay(1000);
  bool done = false;
  uint8_t buttons = 0;
  
  while (!done) {
    lcd.home();
    if (sLevel == 1)
      lcd.print(s_Exit);
    else {
      lcd.print(s_NumButtons);
      lcd.setCursor(14, 0);
      lcd.print((int)sLevel);
    }
  
    if ((buttons = lcd.readButtons())) {
      if (buttons & BUTTON_SELECT) {
        if (sLevel == 1) {
          done = true;
          continue;
        }
        else {
          digitalWrite(LED_START, HIGH);
          digitalWrite(LED_SELECT, HIGH);
          RunGame();
          sLevel = 1;
          lcd.setBacklight(WHITE);
          digitalWrite(LED_START, LOW);
          digitalWrite(LED_SELECT, LOW);
          continue;
        }
      }  
      // Choose Level
      else if (buttons & BUTTON_UP) {
        LevelUp();
        delay(DEBOUNCE);
        lcd.home();
        lcd.print(s_NumButtons);
        lcd.setCursor(14, 0);
        lcd.print((int)sLevel);
      }
      else if (buttons & BUTTON_DOWN) {
        LevelDown();
        delay(DEBOUNCE);
        lcd.home();
        lcd.print(s_NumButtons);
        lcd.setCursor(14, 0);
        lcd.print((int)sLevel);
      }
    }
  }
}

void SimonSez::RunGame() {
  lcd.clear();
  lcd.home();
  lcd.print(s_UsingButtons);
  lcd.setCursor(6, 0);
  lcd.print(sLevel);
  delay(1000);
  randomSeed(millis());
  long rnd;
  Serial.print("SimonSez::RunGame() random sequence: ");
  for (int i = 0; i < MaxMoves; i++) {
    rnd = random(sLevel);
    Serial.print(rnd);
    Sequence[i] = rnd;
  }
  Serial.println();
  //DumpSequence();   // For debug/testing only

  lcd.clear();
  lcd.home();
  lcd.print("Panning sequence");
  lcd.setCursor(0, 1);
  lcd.print("Step ");
  flashtime = (1500 / sLevel);  // time to flash each button in sequence
  flashgap = 250;               // time between flashing each button
  steptimeout = 5000;           // timeout to hit each button in sequence
  uint8_t steps;
  for (steps = 1; steps <= MaxMoves; steps++) {              
    // Display a round, get responses, for each level
    lcd.setBacklight(YELLOW);
    lcd.setCursor(5, 1);
    lcd.print(steps);
    lcd.print("/");
    for (uint8_t f = 0; f < steps; f++) {
      lcd.setCursor(7, 1);
      lcd.print(f+1);
      lcd.print(" = ");
      lcd.print(Sequence[f]);
      FlashLight(Targets[Sequence[f]].LedEnable);
    }

    lcd.setBacklight(GREEN);
    lcd.clear();
    lcd.home();
    lcd.print(s_Playback);
    if (!GetSteps(steps, steptimeout)) {
      break;    // didn't answer right - game over
    }

    delay(1000);
  }

  // Game is over
  lcd.clear();
  lcd.home();
  steps--;  
  if (steps == MaxMoves) {
    lcd.setBacklight(WHITE);
    lcd.print(s_Perfect);
    lcd.setCursor(0, 1);
  }
  else if (quitgame == true) {
    lcd.setBacklight(TEAL);
    lcd.print(s_Aborted);
    lcd.setCursor(0, 1);
  }
  else
    lcd.setBacklight(RED);
  lcd.print(s_Matched);
  lcd.print(steps);
  lcd.print("/");
  lcd.print(MaxMoves);
  //delay(5000);
  PressAnyKey(lcd);
  lcd.setBacklight(WHITE);
  lcd.clear();
  lcd.home();
}

void SimonSez::FlashLight(uint8_t pin) {
  digitalWrite(pin, LOW);
  delay(flashtime);
  digitalWrite(pin, HIGH);
  delay(flashgap);
}

bool SimonSez::GetSteps(uint8_t steps, long timeout) {
  Serial.println("SimonSez::GetSteps()");
  for (int b = 0; b < NumTargets; b++) {
    digitalWrite(Targets[b].SwitchEnable, LOW);
  }

  long start;
  uint8_t buttons;
  for (int s = 0; s < steps; s++) {
    Serial.println("Expecting button " + String(Sequence[s]));
    start = millis();
    buttons = 0;
    do {
      if ((millis() - start) > timeout) {
        Serial.println("\nTimed out");
        return false;   // didn't answer in time
      }
      if (lcd.readButtons() & BUTTON_SELECT) {
        Serial.println("\nPlayer quit the game");
        quitgame = true;
        return false;   
      }
      
      buttons = ReadAllButtons();
      delay(5);        
    } while (buttons == 0);
    
    if (buttons > 0) {
      Serial.println("ReadAllButtons() = " + String(buttons));
      delay(200);
    }
      
    if (bitRead(buttons, Sequence[s]) == 0)
      return false;   // wrong answer
    else
      delay(DEBOUNCE);
  }
 
  return true;  // no wrong answers
}

void SimonSez::DumpSequence() {
  lcd.clear();
  lcd.home();
  Serial.print("DumpSequence(): ");
  for (int c = 0; c < MaxMoves; c++) {
    lcd.print(Sequence[c]);
    Serial.print(Sequence[c]);
  }
  Serial.println();
  delay(3000);
}
/////////////////////////////////////////
// Game Levels
uint8_t SimonSez::LevelUp() {
  sLevel++;
  if (sLevel > NumTargets)
    sLevel = 1;
  return sLevel;
}
uint8_t SimonSez::LevelDown() {
  sLevel--;
  if (sLevel == 0)
    sLevel = NumTargets;
  return sLevel;
}



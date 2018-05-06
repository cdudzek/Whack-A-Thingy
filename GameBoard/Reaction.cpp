#include "Reaction.h"

volatile bool rt_ButtonHit;

void rt_ButtonInterrupt() {
  rt_ButtonHit = true;
}

void ReactionTime::begin(Adafruit_RGBLCDShield _lcd) {
  this->lcd = _lcd;
  this->rLevel = NumTargets;
  rt_ButtonHit = false;
}

void ReactionTime::SelectGame() {
  lcd.clear();
  lcd.home();
  lcd.print(sg_React);
  delay(1000);
  bool done = false;
  uint8_t buttons = 0;
  
  while (!done) {
    lcd.home();
    if (rLevel == 1)
      lcd.print(s_Exit);
    else {
      lcd.print(s_NumButtons);
      lcd.setCursor(14, 0);
      lcd.print((int)rLevel);
    }
  
    if ((buttons = lcd.readButtons())) {
      if (buttons & BUTTON_SELECT) {
        if (rLevel == 1) {
          done = true;
          continue;
        }
        else {
          digitalWrite(LED_START, HIGH);
          digitalWrite(LED_SELECT, HIGH);
          RunGame();
          rLevel = 1;
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
        lcd.print((int)rLevel);
      }
      else if (buttons & BUTTON_DOWN) {
        LevelDown();
        delay(DEBOUNCE);
        lcd.home();
        lcd.print(s_NumButtons);
        lcd.setCursor(14, 0);
        lcd.print((int)rLevel);
      }
    }
  }
}

void ReactionTime::RunGame() {
  long Button_Wait = 3000;
  uint8_t target = 0;
  long start = 0;
  long trigger = 0;
  long hittime = 0;
  long BestTime = 99999;
  uint8_t btns = 0;
  bool cheat = false;
  lcd.clear();
  lcd.home();
  lcd.print(s_UsingButtons);
  lcd.setCursor(6, 0);
  lcd.print(rLevel);
  lcd.setCursor(0, 1);
  lcd.print(r_xPasses);
  lcd.setCursor(8, 1);
  lcd.print(REACT_PASSES);
  delay(1000);
  randomSeed(millis());

  for (int p = 0; p < REACT_PASSES; p++) {
    lcd.clear();
    lcd.home();
    lcd.setBacklight(GREEN);
    lcd.print(r_Ready + " ");
    delay(500);
    lcd.setBacklight(YELLOW);
    lcd.print(r_Aim);
    delay(random(250, 2000));
    
    target = random(0, rLevel);
    digitalWrite(Targets[target].SwitchEnable, LOW);
    if (digitalRead(Targets[target].SwitchRead) == LOW) {
      // CHEATER!!!   Holding the button before we enable it
      cheat = true;
      lcd.clear();
      lcd.home();
      lcd.setBacklight(VIOLET);
      lcd.print(s_Cheater);
      lcd.setCursor(0, 1);
      lcd.print(s_ButtonDown);
      delay(2000);
      lcd.clear();
      break;
    }
    attachInterrupt(digitalPinToInterrupt(INT_PIN1), rt_ButtonInterrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(INT_PIN2), rt_ButtonInterrupt, FALLING);
    delay(50);
    rt_ButtonHit = false;
    lcd.setCursor(0, 1);
    lcd.setBacklight(RED);
    lcd.print(r_Fire);
    digitalWrite(Targets[target].LedEnable, LOW);
    start = millis();
      
    while ((rt_ButtonHit == false) 
            && ((millis() - start) < Button_Wait)
            && !((btns = lcd.readButtons()) & BUTTON_SELECT)) {
      // do nothing, wait for break condition or timeout
    }
    trigger = millis();
    
    detachInterrupt(digitalPinToInterrupt(INT_PIN1));
    detachInterrupt(digitalPinToInterrupt(INT_PIN2));
    digitalWrite(Targets[target].SwitchEnable, HIGH);
    digitalWrite(Targets[target].LedEnable, HIGH);
    lcd.clear();
    lcd.home();
    
    if (rt_ButtonHit) {
      lcd.setBacklight(YELLOW);
      hittime = trigger - start;
      lcd.print(sg_React);
      lcd.setCursor(0, 1);
      lcd.print(hittime);
      lcd.print(" ms");
      if (hittime < BestTime) {
        lcd.setBacklight(GREEN);
        BestTime = hittime;
      }
      delay(2000);
    }
    else if (btns & BUTTON_SELECT) {
      lcd.print("Aborted");
      delay(1000); 
      break;
    }
    else {
      lcd.print("Timed out");
      delay(1500);
    }
  }

  if (!cheat && (BestTime < 60000)) {
    lcd.clear();
    lcd.home();
    lcd.setBacklight(GREEN);
    lcd.print(r_BestTime);
    lcd.setCursor(0, 1);
    lcd.print(BestTime);
    lcd.print(" ms");
    //delay(3000);
    PressAnyKey(lcd);
    lcd.clear();
  }
}

/////////////////////////////////////////
// Game Levels
uint8_t ReactionTime::LevelUp() {
  rLevel++;
  if (rLevel > NumTargets)
    rLevel = 1;
  return rLevel;
}
uint8_t ReactionTime::LevelDown() {
  rLevel--;
  if (rLevel == 0)
    rLevel = NumTargets;
  return rLevel;
}


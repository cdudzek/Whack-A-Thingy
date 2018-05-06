#include "ButtonTest.h"

volatile bool bt_ButtonHit;

void bt_ButtonInterrupt() {
  bt_ButtonHit = true;
}

void ButtonTest::begin(Adafruit_RGBLCDShield _lcd) {
  this->lcd = _lcd;
  bt_ButtonHit = false;
}

void ButtonTest::RunTest() {
  int Button_Wait = 10000;
  long timer = 0;
  uint8_t btns = 0;

  digitalWrite(LED_START, HIGH);
  digitalWrite(LED_SELECT, HIGH);
  
  lcd.clear();
  lcd.home();
  lcd.print("Test all buttons");
  delay(2000);  

  lcd.home();
  lcd.print("Press lit button");
  lcd.setCursor(0, 1);
  lcd.print("Select to skip  ");
  delay(1000);

  /////////////////////////////////////////
  // Test Interrupts
  /////////////////////////////////////////
  for (int b = 0; b < NumTargets; b++) {
    attachInterrupt(digitalPinToInterrupt(INT_PIN1), bt_ButtonInterrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(INT_PIN2), bt_ButtonInterrupt, FALLING);
    delay(100);
    btns = 0;
    lcd.setCursor(0, 1);
    lcd.print("Enable Button ");
    lcd.print(Targets[b].LedEnable);
    lcd.print("  ");
    timer = millis();
    digitalWrite(Targets[b].LedEnable, LOW);
    digitalWrite(Targets[b].SwitchEnable, LOW);
    bt_ButtonHit = false;
    while ((bt_ButtonHit == false) 
            && ((millis() - timer) < Button_Wait)
            && !((btns = lcd.readButtons()) & BUTTON_SELECT)) {
      delay(5);
    }
    detachInterrupt(digitalPinToInterrupt(INT_PIN1));
    detachInterrupt(digitalPinToInterrupt(INT_PIN2));
    delay(50);
    lcd.setCursor(0, 1);
    if (btns & BUTTON_SELECT) {
      lcd.print("Skipped         ");
      delay(1000);
    }
    else if (bt_ButtonHit) {
      lcd.print("Button hit!    ");
      lcd.write(SMILEY);
      delay(1000);
    }
    else {
      lcd.print("No button hit  ");
      lcd.write(FROWNIE);
      delay(1000);
    }
    bt_ButtonHit = false;
    delay(DEBOUNCE);
    digitalWrite(Targets[b].LedEnable, HIGH);    
    digitalWrite(Targets[b].SwitchEnable, HIGH);    
  }

  /////////////////////////////////////////
  // Test Random Input
  /////////////////////////////////////////
  lcd.clear();
  lcd.home();
  lcd.print(s_AnyKey);
  for (int b = 0; b < NumTargets; b++) {
    digitalWrite(Targets[b].SwitchEnable, LOW);
  }
  
  while (!((btns = lcd.readButtons()) & BUTTON_SELECT)) {
    lcd.setCursor(0, 1);
    btns = ReadAllButtons();
    for (int t = 0; t < NumTargets; t++) {
      if (bitRead(btns, t))
        lcd.print(t+1);
      else
        lcd.print(" ");
    }
    lcd.print(" ");
    lcd.print(LcdButtons(lcd));
    delay(5);
  }
  lcd.setCursor(0, 1);
  lcd.print("Done.           ");
  delay(1000);

  for (int b = 0; b < NumTargets; b++) {
    digitalWrite(Targets[b].SwitchEnable, HIGH);
  }

  digitalWrite(LED_START, LOW);
  digitalWrite(LED_SELECT, LOW);
}




#include <Adafruit_RGBLCDShield.h>
#include "GameBoard.h"

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
uint8_t lcdcolor;
GameType game;

/////////////////////////////////////////
// Setup
void setup() {
  // Setup Serial
  Serial.begin(9600);

  // Setup LCD display
  lcd.begin(16, 2);
  lcd.createChar(FROWNIE, (uint8_t*)frownie);
  lcd.createChar(SMILEY, (uint8_t*)smiley);
  lcd.noBlink();
  lcd.noCursor();
  lcdcolor = WHITE;
  lcd.setBacklight(lcdcolor);
  lcd.clear();
  lcd.home();
  lcd.print(s_AppTitle);
  //delay(2000);
  lcd.setCursor(0, 1);

  // Setup I/O
  for (int t = 0; t < NumTargets; t++) {
    pinMode(Targets[t].LedEnable, OUTPUT);
    pinMode(Targets[t].SwitchEnable, OUTPUT);
    pinMode(Targets[t].SwitchRead, INPUT_PULLUP);
    digitalWrite(Targets[t].LedEnable, HIGH); 
    digitalWrite(Targets[t].SwitchEnable, HIGH); 
  }
  // Select/Start buttons
  pinMode(LED_START, OUTPUT);
  pinMode(LED_SELECT, OUTPUT);

  // Test all lights
  SweepLights();

  // Control buttons lit
  digitalWrite(LED_START, LOW);
  digitalWrite(LED_SELECT, LOW);

  // Setup Game
  game = (GameType)0;

}

/////////////////////////////////////////
// Loop
void loop() {
  lcd.home();
  lcd.print(s_SelectGame);
  lcd.setCursor(0, 1);
  lcd.print(NameOfTheGame[(int)game]);
      
  uint8_t buttons = lcd.readButtons();

  if (buttons) {
    if (buttons & BUTTON_SELECT) {
      LaunchGame(game, lcd);
      lcd.clear();
      delay(DEBOUNCE);
      digitalWrite(LED_START, LOW);
      digitalWrite(LED_SELECT, LOW);
    }

    // Choose Game
    else if (buttons & BUTTON_UP) {
      game = PrevGame(game);
      delay(DEBOUNCE);
      lcd.setCursor(0, 1);
      lcd.print(NameOfTheGame[(int)game]);
    }
    else if (buttons & BUTTON_DOWN) {
      game = NextGame(game);
      delay(DEBOUNCE);
      lcd.setCursor(0, 1);
      lcd.print(NameOfTheGame[(int)game]);
    }

    // Change Color
    else if (buttons & BUTTON_LEFT) {
      lcdcolor = ColorScrollDown(lcdcolor);
      lcd.setBacklight(lcdcolor);
    }
    else if (buttons & BUTTON_RIGHT) {
      lcdcolor = ColorScrollUp(lcdcolor);
      lcd.setBacklight(lcdcolor);
    }
    
  }
}

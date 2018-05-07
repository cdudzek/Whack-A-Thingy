#include "Whacker.h"

const WhackerLevel_T WhackerLevels[] {
//{ "Name 16 chars   ", time, maxtargets,   maxspawns,    totalspawns }
  { "Toddler Mode    ", 5000, 2,            1,            20 }, 
  { "Lazy Day        ", 3000, NumTargets/2, 1,            30 },
  { "Normal Adult    ", 1000, NumTargets,   2,            50 },
  { "Damn Tough      ", 750,  NumTargets,   NumTargets/2, 50 },
  { "Ludicrous Speed ", 500,  NumTargets,   NumTargets,   75 },
  //{ "Random Speed    ", 0,    NumTargets,   NumTargets/2, 50 },
  { "Exit to MainMenu", 0,    0,            0,            0 }             // keep Main Menu as last entry
};
const size_t wLEVELS = sizeof(WhackerLevels) / sizeof(WhackerLevel_T);

volatile uint8_t w_ButtonsHit;

void w_ButtonInterrupt() {
  uint8_t reply = 0;
  // initialize all bits in use
  for (int t = 0; t < NumTargets; t++) {
    if (digitalRead(Targets[t].SwitchRead) == LOW)
      bitSet(reply, t);
  }
  w_ButtonsHit = reply;
}

void Whacker::begin(Adafruit_RGBLCDShield _lcd) {
  this->lcd = _lcd;
  this->whLevel = 0;
  for (int s = 0; s < NumTargets; s++) {
    digitalWrite(Targets[s].SwitchEnable, HIGH);
    digitalWrite(Targets[s].LedEnable, HIGH);
    spawntime[s] = 0;
    LastTimeOff[s] = 0;
  }
}

void Whacker::SelectGame() {
  lcd.clear();
  lcd.home();
  lcd.print(sg_Whacker);
  delay(1000);
  bool done = false;
  uint8_t buttons = 0;
  
  while (!done) {
    lcd.home();
    lcd.print(s_SelectLevel);
    lcd.setCursor(0, 1);
    lcd.print(WhackerLevels[whLevel].label);
  
    if ((buttons = lcd.readButtons())) {
      if (buttons & BUTTON_SELECT) {
        if (whLevel == (wLEVELS - 1)) {
          done = true;
          continue;
        }
        else {
          digitalWrite(LED_START, HIGH);
          digitalWrite(LED_SELECT, HIGH);
          RunGame();
          whLevel = wLEVELS - 1;    // Main Menu
          lcd.setBacklight(WHITE);
          digitalWrite(LED_START, LOW);
          digitalWrite(LED_SELECT, LOW);
          continue;
        }
      }  
      // Choose Level
      else if (buttons & BUTTON_UP) {
        LevelDown();
        delay(DEBOUNCE);
      }
      else if (buttons & BUTTON_DOWN) {
        LevelUp();      // Yes, move from [0] to [max], not UP==Up
        delay(DEBOUNCE);
      }
    }
  }
}

void Whacker::RunGame() {
  // Debug dump
  Serial.print(millis());
  Serial.println(" - Whacker level \"" + WhackerLevels[whLevel].label + "\": ");
  Serial.println("\ttimeout " + String(WhackerLevels[whLevel].timeout) + " ms, ");
  Serial.println("\tusing " + String(WhackerLevels[whLevel].maxbuttons) + " buttons, ");
  Serial.println("\tmax " + String(WhackerLevels[whLevel].maxspawns) + " at a time, ");
  Serial.println("\trunning up to " + String(WhackerLevels[whLevel].totalspawns) + " lit targets.");

  // lcd begin game
  lcd.clear();
  lcd.home();
  lcd.print(sg_React);
  lcd.setCursor(0, 1);
  if (WhackerLevels[whLevel].timeout == 0)    // Random speed
    lcd.print(WhackerLevels[whLevel].label);
  else {
    lcd.print((float)WhackerLevels[whLevel].timeout / 1000.0, 1);
    lcd.print(" seconds");
  }
  delay(1000);

  // Initialize game play
  randomSeed(millis());
  attachInterrupt(digitalPinToInterrupt(INT_PIN1), w_ButtonInterrupt, FALLING);    
  attachInterrupt(digitalPinToInterrupt(INT_PIN2), w_ButtonInterrupt, FALLING);    
  
  spawns = 0;
  score = 0;
  unsigned int active = 0;
  unsigned long lastspawn = 0;  // last millis() we spawned a target
  unsigned long lasthit = 0;    // last millis() we received a hit
  bool running = true;          // targets still active or waiting
  bool quits = false;
  bool cheat = false;
  Countdown(5000, lcd);
  lcd.clear();
  lcd.home();
  lcd.print(s_Score);
  lcd.setBacklight(GREEN);

  // loop until we have finished all targets
  while (running == true ) {
    // Update running score
    lcd.setCursor(6, 0);
    lcd.print(score);
    lcd.print(s_Blank);
    
    // Check for valid hits, award points
    if (w_ButtonsHit > 0) {
      for (int b = 0; b < 8; b++) {
        if (bitRead(w_ButtonsHit, b)) {    // find out which button
          digitalWrite(Targets[b].SwitchEnable, HIGH);
          digitalWrite(Targets[b].LedEnable, HIGH);
          spawntime[b] = 0;
          LastTimeOff[b] = millis();
          score++;
          lasthit = millis();
          Serial.println("Direct hit on button " + String(b));
        }
      }
      w_ButtonsHit = 0;
    }
    
    // Check if we can spawn a new target
    if ((spawns < WhackerLevels[whLevel].totalspawns)
        && (Active() < WhackerLevels[whLevel].maxspawns)) {
      if (millis() > (lasthit + random(DEBOUNCE, DEBOUNCE*10))) {   // roughly 150-1500ms
        if (SpawnNew()) {
          lastspawn = millis();
          spawns++;
        }
        else {
          cheat = true;
          break;
        }
        lastspawn = millis();
      }
    }
    
    // Check if any targets expired
    for (int t = 0; t < NumTargets; t++) {
      if (spawntime[t] > 0
          && (millis() > (spawntime[t] + WhackerLevels[whLevel].timeout))) {
        digitalWrite(Targets[t].SwitchEnable, HIGH);
        digitalWrite(Targets[t].LedEnable, HIGH);
        spawntime[t] = 0;
        LastTimeOff[t] = millis();
        Serial.println("Timeout on button " + String(t));
      }
    }

    // Check if the game is over
    if ((spawns == WhackerLevels[whLevel].totalspawns) && (Active() == 0)) {
        running = false;
    }

    // User quits
    if (lcd.readButtons() & BUTTON_SELECT) {
      running = false;
      quits = true;
    }
  }   // while loop

  detachInterrupt(digitalPinToInterrupt(INT_PIN1));
  detachInterrupt(digitalPinToInterrupt(INT_PIN2));

  lcd.clear();
  lcd.home();
  if (quits)
    lcd.print(s_AnyKey);
  else if (!cheat) {
    lcd.setBacklight(GREEN);
    lcd.print(s_Score);
    lcd.setCursor(6, 0);
    lcd.print(String(score) + "/" + String(spawns));
    delay(3000);
  }
  PressAnyKey(lcd);
  lcd.clear();
  
}

bool Whacker::SpawnNew() {
  // Pick random target, check if already lit, check if button already pressed (cheat), then turn on
  // Return true = successfully spawned new target
  uint8_t target = 0;
  do {
    target = random(0, WhackerLevels[whLevel].maxbuttons);
  } while ( spawntime[target] != 0  
            && (MIN_TIME_OFF + LastTimeOff[target]) < millis()); // pick a different target until we find one unused, and inactive long enough

  Serial.println("Spawning button " + String(target));

  // check for cheaters, unless we are in an extremely fast game
  if (WhackerLevels[whLevel].timeout >= NOCHEAT_TIME) {
    digitalWrite(Targets[target].SwitchEnable, LOW);
    if (digitalRead(Targets[target].SwitchRead) == LOW) {
      // CHEATER!!!   Holding the button before we enable it
      lcd.clear();
      lcd.home();
      lcd.setBacklight(VIOLET);
      lcd.print(s_Cheater);
      lcd.setCursor(0, 1);
      lcd.print(s_ButtonDown);
      delay(2000);
      lcd.setCursor(0, 1);
      lcd.print(s_Blank);
      lcd.setCursor(0, 1);
      lcd.print("Hit " + String(score) + "/" + String(spawns));
      delay(3000);    
      lcd.clear();
      digitalWrite(Targets[target].SwitchEnable, HIGH);
      return false;
    }
  }

  // Enable target and return good status
  digitalWrite(Targets[target].LedEnable, LOW);
  spawntime[target] = millis();  
  return true;
}

uint8_t Whacker::Active() {
  // Return number of active targets
  uint8_t total = 0;
  for (int t = 0; t < NumTargets; t++) {
    if (spawntime[t] > 0)
      total++;
  }
  return total;
}

/////////////////////////////////////////
// Game Levels
uint8_t Whacker::LevelUp() {
  whLevel++;
  if (whLevel == wLEVELS)
    whLevel = 0;
  return whLevel;
}
uint8_t Whacker::LevelDown() {
  whLevel--;
  if (whLevel == 0)
    whLevel = wLEVELS - 1;
  return whLevel;
}



/*
  Copyright 2019 Thomas Hartmann (@hartmann1301)

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  thanks to the following page for the awesome fast getDegrees code I am using in Help.h
    https://www.romanblack.com/integer_degree.htm

  This is MicroTowers, a game created for Arduboy.

  For lots of information and how to play see:
  https://github.com/hartmann1301/MicroTowers

  Version: 1.0    18.03.2019    release
*/

#include "Globals.h"
#include "Mem.h"
#include "Img.h"
#include "Help.h"
#include "Draw.h"

#include "Font.h"
myFont mF;

#include "Anim.h"
animationManager aM;

#include "Proj.h"
projectileManager pM;

#include "Map.h"
mapMangager mM;

#include "Eeprom.h"

#include "Enemys.h"
enemyManager eM;

#include "Towers.h"
towerManager tM;

#include "Info.h"
#include "Menu.h"
#include "Game.h"
#include "Buttons.h"

void setup() {

#ifdef USE_SERIAL
  Serial.begin(57600);
  Serial.println("Micro Towers");
#endif

#ifdef ESP8266
  ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, false, false);

  // this function must be called or the esp will crash with the first call
  arduboy.setExternalButtonsHandler(getButtons);

  // i think this is only needed for the esp8266 eeprom emulation
  EEPROM.begin(EEPROM_NEEDED_BYTES);
#endif

  arduboy.begin();
  arduboy.setFrameRate(FRAMES_PRO_SEC);

  // initialise the managers
  tM.init();
  eM.init();
  aM.init();

  // check if game was played before
  initEEPROM();

  goToMainMenu();
}

void loop() {

  // if normal speed is false it will be max speed
  if (!(arduboy.nextFrame()) && isNormalSpeed)
    return;

#ifdef DEBUG_MICROS
  uint32_t start = micros();
#endif

  arduboy.pollButtons();
  arduboy.fillScreen(WHITE);

  // calculate this once and store is global
  isFramesMod2 = gameFrames % 2;

  checkButtons();

  if (!inMapsListMode(gameMode) && gameMode != MODE_PLAYING_INFO && gameMode != MODE_ENEMIES && gameMode != MODE_CREDITS)
    updateGame();

  mainScheduler();

#ifdef DEBUG_FRAMES
  arduboy.fillRect(1, 1, 20, 6, WHITE);
  mF.setCursor(2, 1);
  mF.print(getFramesPerSecond());
#endif

#ifdef DEBUG_MICROS
  uint32_t diff = micros() - start;
  Serial.println("took:" + String(diff));
#endif

  arduboy.display();

  gameFrames++;
}

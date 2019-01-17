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

  This is MicroTowers, a game created for Arduboy.

  For lots of information and how to play see:

  Version:
*/

#include "Globals.h"
#include "Sprites.h"
#include "Draw.h"
#include "math.h"

#include "Animations.h"
animationManager aM;

#include "Projectiles.h"
projectileManager pM;

#include "Enemys.h"
enemyManager eM;

#include "Towers.h"

#include "Checks.h"
#include "Buttons.h"

void setup() {

#ifdef USE_SERIAL
  Serial.begin(57600);
  Serial.println(F("MicroTowers!"));

#endif

#ifdef ESP8266
  ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, false, false);

  // this function must be called or the esp will crash with the first call
  arduboy.setExternalButtonsHandler(getButtons);
#endif

  arduboy.begin();
  arduboy.setFrameRate(GAME_FRAMES);

  // some start tone
  //sound.tone(200, 200, 300, 200);

  arduboy.fillScreen(WHITE);
  arduboy.display();
}

void modePlay() {
#ifdef DEBUG_FRAME_TIME
  uint32_t start = millis();
#endif



#ifdef DEBUG_FRAME_TIME
  // pirnt time for a frame to screen
  arduboy.fillRect(0, 0, 26, 7, BLACK);
  drawNumbers(0, 0, millis() - start);
#endif
}

void modeMenu() {

}

void loop() {
  if (!(arduboy.nextFrame()))
    return;

  arduboy.fillScreen(WHITE);




  for (uint16_t i = 0; i < 360; i++) {
    arduboy.fillScreen(WHITE);

    drawRaster();

    drawTower(0, 0, TOWER_MG, i, 0);
    drawTower(0, 2, TOWER_MG, i, 1);
    drawTower(0, 6, TOWER_MG, i, 2);
    drawTower(0, 8, TOWER_MG, i, 3);

    drawTower(3, 0, TOWER_CANON, i, 0);
    drawTower(3, 2, TOWER_CANON, i, 1);
    drawTower(3, 6, TOWER_CANON, i, 2);
    drawTower(3, 8, TOWER_CANON, i, 3);

    drawTower(6, 0, TOWER_LASER, i, 0);
    drawTower(6, 2, TOWER_LASER, i, 1);
    drawTower(6, 6, TOWER_LASER, i, 2);
    drawTower(6, 8, TOWER_LASER, i, 3);

    drawTower(9, 0, TOWER_FLAME, i, 0);
    drawTower(9, 2, TOWER_FLAME, i, 1);
    drawTower(9, 6, TOWER_FLAME, i, 2);
    drawTower(9, 8, TOWER_FLAME, i, 3);

    drawTower(12, 1, TOWER_SHOCK, i, 0);
    drawTower(12, 3, TOWER_FLAK, i, 0);
    drawTower(12, 5, TOWER_SILO, i, 0);
    drawTower(12, 7, TOWER_HELP, i, 0);

    drawTower(14, 1, TOWER_SHOCK, i, 1);
    drawTower(14, 3, TOWER_FLAK, i, 1);
    drawTower(14, 5, TOWER_SILO, i, 1);
    drawTower(14, 7, TOWER_HELP, i, 1);

    drawTower(16, 1, TOWER_SHOCK, i, 2);
    drawTower(16, 3, TOWER_FLAK, i, 2);
    drawTower(16, 5, TOWER_SILO, i, 2);
    drawTower(16, 7, TOWER_HELP, i, 2);

    drawTower(18, 1, TOWER_SHOCK, i, 3);
    drawTower(18, 3, TOWER_FLAK, i, 3);
    drawTower(18, 5, TOWER_SILO, i, 3);
    drawTower(18, 7, TOWER_HELP, i, 3);

    menuButtons();
    drawCursor();

    arduboy.display();
    delay(20);
  }

  arduboy.display();

  gameFrames++;
}

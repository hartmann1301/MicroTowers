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

#include "Map.h"
mapMangager mM;

#include "Enemys.h"
enemyManager eM;

#include "Towers.h"
towerManager tM;

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

  buyTower(3, 1, TOWER_LASER);
  buyTower(11, 3, TOWER_CANON);
  buyTower(14, 0, TOWER_MG);
  buyTower(14, 7, TOWER_FLAME);

  eM.add(0, 18, ENEMY_MG);
  eM.add(40, 18, ENEMY_RUNNING);
  eM.add(60, 10, ENEMY_RAT);

  arduboy.fillScreen(WHITE);

  /*
    arduboy.drawSlowXYBitmap(30, 40, test7x7h, 7, 7, BLACK);
    arduboy.drawBitmap(30, 30, test7x7v, 7, 7, BLACK);
  */

  /*
    drawTower(10, 20, towerRotations, 0, 0);
    drawTower(20, 20, towerRotations, 1, 0);
    drawTower(30, 20, towerRotations, 0, 1);
    drawTower(40, 20, towerRotations, 1, 1);
  */

  drawBitmapSlow(10, 10, towerRotations, 7, 8, 0, 0/*, BLACK*/);
  drawBitmapSlow(20, 10, towerRotations, 7, 8, 1, 1/*, BLACK*/);
  drawBitmapSlow(30, 10, towerRotations, 7, 8, 2, 2/*, BLACK*/);
  drawBitmapSlow(40, 10, towerRotations, 7, 8, 3, 3/*, BLACK*/);
        
  drawBitmapFast(30, 30, towerRotations, 7, 8, 0, false);

  /*
    drawBitmapFast(10, 10, towerRotations, 7, 0, false);
    drawBitmapFast(20, 10, towerRotations, 7, 1, false);
    drawBitmapFast(30, 10, towerRotations, 7, 2, false);
    drawBitmapFast(40, 10, towerRotations, 7, 16, false);
    drawBitmapFast(50, 10, towerRotations, 7, 17, false);
    drawBitmapFast(60, 10, towerRotations, 7, 22, false);


    drawBitmapFast(10, 20, towerRotations, 7, 0, true);
    drawBitmapFast(20, 20, towerRotations, 7, 1, true);
    drawBitmapFast(30, 20, towerRotations, 7, 2, true);
    drawBitmapFast(40, 20, towerRotations, 7, 16, true);
    drawBitmapFast(50, 20, towerRotations, 7, 17, true);
    drawBitmapFast(60, 20, towerRotations, 7, 23, true);
  */

  arduboy.display();
  delay(900000);


  arduboy.fillScreen(WHITE);

  for (uint8_t i = 0; i < 2; i++) {

    uint32_t s = micros();

    if (i == 0) {
      for (uint i = 0; i < 10; i++) {

        //drawBitmapSlow(i * 10, 40, test7x7h, 7, 7, 7, 0, false, BLACK);

        arduboy.drawBitmap(i * 10, 40, test7x7v, 7, 7, BLACK);
      }

    } else {
      for (uint i = 0; i < 10; i++) {
        //arduboy.drawSlowXYBitmap(i * 10, 40, test7x7h, 7, 7, BLACK);

        drawBitmapFast(i * 10, 32, test7x7v, 7, 8, 0, false);
      }
    }

    uint32_t diff = micros() - s;

    drawNumbers(100, 30 + i * 10, diff);

    Serial.println(String(micros()) + " " + String(i) + " took " + String(diff));
  }



  // some start tone
  //sound.tone(200, 200, 300, 200);

  arduboy.display();
  delay(99000);
}

void modeMenu() {

}

void loop() {
  if (!(arduboy.nextFrame()))
    return;

#ifdef DEBUG_FRAME_TIME
  uint32_t start = millis();
#endif

  arduboy.fillScreen(WHITE);

  if (mapChanged) {
    mM.findPath();
    mM.printMap();

    mapChanged = false;
  }

  mM.drawMap();

  tM.update();
  eM.update();

  menuButtons();
  drawCursor();

#ifdef DEBUG_FRAME_TIME
  // pirnt time for a frame to screen
  drawNumbers(0, 0, millis() - start);
#endif

  arduboy.display();

  gameFrames++;
}

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


  for (uint8_t i = 0; i < 12; i++) {
    arduboy.fillScreen(WHITE);
    drawTowerBitmap(5, 5, towerMG, NOROT, NOFLIP, i);
    drawTowerBitmap(15, 5, towerMG, ROTCCW, NOFLIP, i);
    drawTowerBitmap(25, 5, towerMG, ROT180, NOFLIP, i);
    drawTowerBitmap(35, 5, towerMG, ROTCW, NOFLIP, i);

    drawTowerBitmap(5, 15, towerMG, NOROT, NOFLIP, i);
    drawTowerBitmap(15, 15, towerMG, NOROT, FLIPH, i);
    drawTowerBitmap(25, 15, towerMG, NOROT, FLIPV, i);
    drawTowerBitmap(35, 15, towerMG, NOROT, FLIPVH, i);
    
    arduboy.display();
    delay(3000);
  }
 


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


  uint8_t i = 1;
    drawTowerBitmap(5, 5, towerMG, NOROT, NOFLIP, i);
    drawTowerBitmap(15, 5, towerMG, ROTCCW, NOFLIP, i);
    drawTowerBitmap(25, 5, towerMG, ROT180, NOFLIP, i);
    drawTowerBitmap(35, 5, towerMG, ROTCW, NOFLIP, i);

    drawTowerBitmap(5, 15, towerMG, NOROT, NOFLIP, i);
    drawTowerBitmap(15, 15, towerMG, NOROT, FLIPH, i);
    drawTowerBitmap(25, 15, towerMG, NOROT, FLIPV, i);
    drawTowerBitmap(35, 15, towerMG, NOROT, FLIPVH, i);


  /*
    checkButtons();

    if (modePlay) {
    modePlay();

    } else {
    modeMenu();
    }
  */

  arduboy.display();

  gameFrames++;
}

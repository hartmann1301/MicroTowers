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

#include "Font.h"
myFont mF;

#include "Anim.h"
animationManager aM;

#include "Proj.h"
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

  //mM.createMap();
  //mM.generateMapArray();

  mM.loadMap(1);

  buyTower(3, 1, TOWER_LASER);
  buyTower(11, 3, TOWER_CANON);
  buyTower(14, 0, TOWER_MG);
  buyTower(14, 6, TOWER_FLAME);
  buyTower(2, 7, TOWER_FLAK);


  eM.add(0, 18, HUMAN_MG);
  eM.add(40, 18, ANIMAL_RAT);
  eM.add(60, 10, TWOPOD_LVL0);

  // some start tone
  //sound.tone(200, 200, 300, 200);

  //arduboy.display();
  //delay(99000);
}

void modeMenu() {

}

void loop() {

  if (!(arduboy.nextFrame()))
    return;


#ifdef DEBUG_FRAME_TIME
  uint32_t start = micros();
#endif

  arduboy.fillScreen(WHITE);

  if (mapChanged) {
    mM.findPath();

    mapChanged = false;
  }

  mM.drawMap();

  tM.update();
  eM.update();

  menuButtons();
  drawCursor();

#ifdef DEBUG_FRAME_TIME
  // pirnt time for a frame to screen
  mF.setCursor(2, 58);
  mF.print(String(micros() - start));
#endif

  arduboy.display();

  gameFrames++;
}

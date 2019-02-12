#ifndef Towers_h
#define Towers_h

struct tower {
  uint8_t xR;
  uint8_t yR;

  // maybe combine those two like typeRotation
  uint8_t type;
  uint8_t sektor = 1;

  uint8_t range = 20;
  uint8_t lev = 2;

  bool drawRange = false;
  bool active;


  uint8_t getX() {
    return xR * RASTER + RASTER_OFFSET_X + 1;
  }

  uint8_t getY() {
    return yR * RASTER + RASTER_OFFSET_Y + 1;
  }

  int8_t getTarget() {

    int8_t bestTarget = -1;

    for (uint8_t i = 0; i < eM.maximum; i++) {
      // check only active enemys
      if (eM.list[i].active == false)
        continue;

      //Serial.println("Found enemy" + String(i));

      // only if the enemy is in Range
      if (!eM.list[i].isInRange(getX() + 5, getY() + 5, range))
        continue;

      /*
        // set new target if enemy has more map progress
        if (eM.list[i].dijkstraPos > bestTarget)
        bestTarget = i;
      */

      bestTarget = i;
    }

    //Serial.println(bestTarget);

    return bestTarget;
  }

  bool isClockwiseShorter(uint8_t r1, uint8_t r2) {
    int8_t cwTurns, ccwTurns;

    if (r1 > r2) {
      cwTurns = r1 - r2;
      ccwTurns = abs(r1 - r2 - 16);

      return (cwTurns < ccwTurns);

    } else {
      cwTurns = r2 - r1;
      ccwTurns = abs(r2 - r1 - 16);

      return (cwTurns > ccwTurns);
    }
  }

  int16_t getDegree(int8_t x, int8_t y) {
    if (x == 0 && y == 0)
      return -1;

    if (x == 0 && y > 0)
      return 90;

    if (x == 0 && y < 0)
      return 270;

    if (y == 0 && x > 0)
      return 0;

    if (y == 0 && x < 0)
      return 180;

    //Serial.print("getDegree from x:" + String(x) + " y:" + String(y));
    //uint32_t start = micros();

    static const float radToDegree = 180.0 / PI;
    int16_t deg = atanf(float(y) / x) * radToDegree;

    //uint32_t dur = micros() - start;
    //Serial.println(" atan(:" + String(deg) + ") took:" + String(dur));

    if (x < 0) {
      return 180 + deg;

    } else {
      if (deg < 0)
        deg += 360;

      return deg;
    }
  }

  int8_t getSektor(int8_t x, int8_t y) {

    // multiply by 100 to aviod float and add offset 11.25°
    uint16_t deg = getDegree(x, y) * 100 + 1125;

    // calc section of 22.5° circle
    deg /= 2250;

    //Serial.print(" x:" + String(x) + " y:" + String(y));
    //Serial.println(" degree:" + String(getDegree(x, y)) + " ret:" + String(deg % 16));

    // to return number from 0 to 15 use mod
    return deg % 16;
  }

  void update() {
    // look for a target
    int8_t target = getTarget();

    // there is nothing to do if no enemy is near
    if (target == -1)
      return;

    uint8_t x = getX();
    uint8_t y = getY();

    // get the sektor of the prefered target, y-axis is inversed on the display!
    // it is enemyPos - towerPos and tower center is x+5, y+5 - enemy center is x+2, y+4
    int8_t targetSektor = getSektor(int16_t(eM.list[target].x) - 3 - x, 1 + y - int16_t(eM.list[target].y));

    // get the current tower sektor
    int8_t istSektor = sektor;

    // check if turning is necassary
    if (istSektor != targetSektor) {

      // turn the fastest way
      if (isClockwiseShorter(istSektor, targetSektor)) {
        istSektor--;
      } else {
        istSektor++;
      }

      // write rotation back
      sektor = (istSektor + 16) % 16;
    }


  }

  void draw() {
    uint8_t x = getX();
    uint8_t y = getY();

    // draw socket depending on level
    drawTowerSocket(x, y, lev);

    // clear raster mark, if in build menu
    if (false)
      arduboy.fillRect(x + 4, y + 4, 3, 3, WHITE);

    const uint8_t *bitmap;

    // draw the range of this tower
    if (drawRange)
      arduboy.drawCircle(x + 5, y + 5, range, BLACK);

    if (type <= TOWER_FROST) {

      // todo draw those towers fast

      // set offset for correct level and fine rotation
      uint8_t offset = lev % 4 + (sektor % 4) * 4;

      // set tower offset
      offset += type * 16;

      // divide again by 4 because sprite can only rotate every 90 degrees
      uint8_t rotation = sektor / 4;

      // rotate 180° because the sprite looks to the left side
      rotation = (rotation + 2) % 4;

      //  Serial.println("drawSlow:" + String(type) + " offset " + String(offset) + " rotation " + String(rotation));
      drawBitmapSlow(x + 2, y + 2, allTowers, 7, 7, offset, rotation);

    } else {

      // offset is a huge value, because the five other towers are on top
      uint8_t offset = lev % 4 + (type - TOWER_SHOCK) * 4 + 16 * 6;

      // can be drawn fast
      drawBitmapFast(x + 2, y + 2, allTowers, 7, offset, false);
    }
  }
};

struct towerManager {
  static const uint8_t maximum = 10;
  tower list[maximum];

  void add(uint8_t xR, uint8_t yR, uint8_t type) {

    bool foundSlot = false;
    for (uint8_t i = 0; i < maximum; i++) {

      if (list[i].active == true)
        continue;

      list[i].active = true;
      foundSlot = true;

      // set the 4 map notes to tower
      mM.placeTower(xR, yR);

      // set values
      list[i].xR = xR;
      list[i].yR = yR;

      list[i].type = type;
      break;
    }

#ifdef DEBUG_ADD_FUNCTIONS
    if (!foundSlot)
      Serial.println("Warning, no slot for animation!");
#endif
  }

  /*
    bool isBlockedAt(uint8_t xPos, uint8_t yPos) {

     Serial.println("search at x:" + String(xPos) + " y:" + String(yPos));

    for (uint8_t i = 0; i < maximum; i++) {
      // only active
      if (list[i].active == false)
        continue;

      //Serial.println(" tower:" + String(i) + " xR:" + String(list[i].xR) + " yR:" + String(list[i].yR));

      if (list[i].xR < xPos || list[i].xR + 1 > xPos)
        continue;

      if (list[i].yR < yPos || list[i].yR + 1 > yPos)
        continue;

      Serial.println("there is a tower at x:" + String(xPos) + " y:" + String(yPos));

      return true;
    }

    return false;
    }
  */

  void update() {

    for (uint8_t i = 0; i < maximum; i++) {
      // only active
      if (list[i].active == false)
        continue;

      list[i].update();

      list[i].draw();
    }
  }
};


#endif

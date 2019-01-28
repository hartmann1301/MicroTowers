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
    uint8_t socketCorner = 3 - lev + uint8_t(lev < 2);
    arduboy.drawRoundRect(x, y, 11, 11, socketCorner, BLACK);

    // clear raster mark, if in build menu
    if (false)
      arduboy.fillRect(x + 4, y + 4, 3, 3, WHITE);

    const uint8_t *bitmap;

    // draw the range of this tower
    if (drawRange)
      arduboy.drawCircle(x + 5, y + 5, range, BLACK);

    if (type <= TOWER_FLAME) {

      // todo draw those towers fast

      // set offset for correct level and fine rotation
      uint16_t offset = lev % 4 + (sektor % 4) * 4;

      // set tower offset 
      offset += type * 7 * 16;

      // divide again by 4 because sprite can only rotate every 90 degrees
      uint8_t rotation = sektor / 4;

      // rotate 180° because the sprite looks to the left side
      rotation = (rotation + 2) % 4;

      // actual drawing slowly
      drawBitmapSlow(x + 2, y + 2, allTowers, 7, 8, offset, rotation);

    } else {

      // offset is a huge value, because the four other towers are on top
      uint16_t offset = lev % 4 + (type - TOWER_SHOCK) * 4 + 7 * 16 * 4
      
      // can be drawn fast
      drawBitmapFast(x + 2, y + 2, allTowers, 7, 8, offset, false);   
    }
  }
};

struct towerManager {
  static const uint8_t maximum = 20;
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

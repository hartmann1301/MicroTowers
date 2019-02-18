#ifndef Towers_h
#define Towers_h

struct tower {
  // location on the map
  uint8_t index;

  // maybe combine those two like typeSektor
  uint8_t type;
  uint8_t sektor = 8;

  // combine
  bool active;
  uint8_t range = 20;

  // combine
  uint8_t lev = 0;
  uint8_t state = 0;

  uint8_t getX() {
    return getxR(index) * RASTER + RASTER_OFFSET_X + 1;
  }

  uint8_t getY() {
    return getyR(index) * RASTER + RASTER_OFFSET_Y + 1;
  }

  uint8_t getCenterX() {
    return getX() + 5;
  }

  uint8_t getCenterY() {
    return getY() + 5;
  }

  int8_t getTarget(bool isShockTower) {

    int8_t bestTarget = -1;
    uint8_t lowestCosts = 255;

    for (uint8_t i = 0; i < eM.maximum; i++) {
      // check only active enemys
      if (eM.list[i].active == false)
        continue;

      //Serial.println("Found enemy" + String(i));

      // only if the enemy is in Range
      if (!eM.list[i].isInRange(getCenterX(), getCenterY(), range))
        continue;

      // called by the shock towers to do damage to lots of enemys
      if (isShockTower) {
        //
        if (gameFrames % 2)
          arduboy.drawLine(getCenterX(), getCenterY(), eM.list[i].getCenterX(), eM.list[i].getCenterY(), BLACK);

        // TODO:
        eM.list[i].damage(0);
      }

      // set new target if enemy has more map progress
      uint8_t currentCosts = eM.list[i].mapCosts;
      if (currentCosts < lowestCosts) {
        bestTarget = i;

        // set new lowest Cost of the current Target
        lowestCosts = currentCosts;
      }
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

    if (x == 0 && y == 0) {
      return 0;

    } else {
      return fastAtan(y, x);
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
    int8_t target = getTarget(false);

    // reload weapon
    if (state > 0)
      state--;

    // there is nothing to do if no enemy is near
    if (target == -1)
      return;

    // the values to calculate with
    uint8_t xCenter = getCenterX();
    uint8_t yCenter = getCenterY();
    int8_t xTarget = eM.list[target].getCenterX();
    int8_t yTarget = eM.list[target].getCenterY();

    // get the sektor of the prefered target, y-axis is inversed on the display!
    int8_t targetSektor = getSektor(xTarget - xCenter, yCenter - yTarget);

    int8_t istSektor = sektor;

    // check if rotation is necassary
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

    // get positon where to bullet starts
    int8_t xStart = xCenter + getDirectionX(sektor);
    int8_t yStart = yCenter + getDirectionY(sektor);


    if (type == TOWER_SUPPORT) {
      return;

    } else if (type == TOWER_LASER) {

      // do laser dmg
      eM.list[target].damage(2);

      if (gameFrames % 2)
        arduboy.drawLine(xStart, yStart, xTarget, yTarget, BLACK);

      return;

    } else if (type == TOWER_SHOCK) {

      // do shock dmg
      getTarget(true);

      arduboy.drawCircle(xCenter, yCenter, 10 + gameFrames % 3, BLACK);
    }

    // weapon can only shoot if loaded
    if (state != 0)
      return;

    uint8_t reloadTime = 8;

    state += reloadTime;

    // shoot the bullet
    pM.add(xStart, yStart, type, sektor);
  }

  void draw() {
    uint8_t x = getX();
    uint8_t y = getY();

    // draw socket depending on level
    drawTowerSocket(x, y, lev);

    // draw the range of this tower
    if (false)
      arduboy.drawCircle(getCenterX(), getCenterY(), range, BLACK);

    if (type <= TOWER_LASER) {

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

  void sell(uint8_t towerIndex) {

    // delete this tower
    list[towerIndex].active = false;

    uint8_t mapIndex = list[towerIndex].index;

    // set the 4 map notes to tower
    mM.set2x2Nodes(mapIndex, MAP_FREE);
  }

  void init() {
    for (uint8_t i = 0; i < maximum; i++) {
      list[i].active = false;
    }
  }

  uint8_t getTowerAt(uint8_t xRaster, uint8_t yRaster) {
    return getTowerAt(getIndex(xRaster, yRaster));
  }

  uint8_t getTowerAt(uint8_t mapIndex) {

    for (uint8_t i = 0; i < maximum; i++) {

      if (list[i].active == false)
        continue;

      if (list[i].index == mapIndex)
        return i;
    }

    Serial.println("impossible to get here!!!!!!");
    return 0;
  }

  void add(uint8_t xR, uint8_t yR, uint8_t type) {

    bool foundSlot = false;
    for (uint8_t i = 0; i < maximum; i++) {

      if (list[i].active == true)
        continue;

      list[i].active = true;
      foundSlot = true;

      uint8_t towerIndex = getIndex(xR, yR);

      //Serial.println("addTower i:" + String(i) + " at mapIndex:" + String(towerIndex));

      // set the 4 map notes to tower
      mM.set2x2Nodes(xR, yR, MAP_TOWER);

      // set values
      list[i].index = towerIndex;

      list[i].type = type;
      break;
    }

#ifdef DEBUG_ADD_FUNCTIONS
    if (!foundSlot)
      Serial.println("Warning, no slot for tower!");
#endif
  }

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

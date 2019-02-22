#ifndef Towers_h
#define Towers_h

#define BIT_TOWER_ACTIVE    7
#define BIT_TOWER_RELOADED  6

struct tower {
  // location on the map
  uint8_t index;

  // highNibble: type, lowNibble: sektor
  uint8_t typeSektor;

  // highestBit: active, lowNibble: range
  uint8_t activeRange;

  // highNibble: level, lowNibble: state
  uint8_t levelState;

  // getters
  uint8_t getType() {
    return getHighNibble(typeSektor);
  }

  uint8_t getSektor() {
    return getLowNibble(typeSektor);
  }

  uint8_t getRange() {
    return getLowNibble(activeRange) * 2;

    // no need for nibble
    
  }

  uint8_t getLevel() {
    return getHighNibble(levelState);
  }

  uint8_t getState() {
    return getLowNibble(levelState);
  }

  // setters
  void setType(uint8_t value) {
    setHighNibble(typeSektor, value);
  }

  void setSektor(uint8_t value) {
    setLowNibble(typeSektor, value);
  }

  void setRange(uint8_t value) {
    setLowNibble(activeRange, value / 2);
  }

  void setLevel(uint8_t value) {
    setHighNibble(levelState, value);
  }

  void setState(uint8_t value) {
    setLowNibble(levelState, value);
  }


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
      if (eM.isEnemyActive(i) == false)
        continue;

      //Serial.println("Found enemy" + String(i));

      // only if the enemy is in Range
      if (!eM.list[i].isInRange(getCenterX(), getCenterY(), getRange()))
        continue;

      // called by the shock towers to do damage to lots of enemys
      if (isShockTower) {

        // just for debugging
        //if (isFramesMod2)
        //  arduboy.drawLine(getCenterX(), getCenterY(), eM.list[i].getCenterX(), eM.list[i].getCenterY(), BLACK);

        // TODO:
        eM.list[i].damage(1, TOWER_SHOCK);
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

  int8_t calculateSektor(int8_t x, int8_t y) {

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

    // get the type of this tower
    uint8_t type = getType();

    // support tower is only doing something when map changed
    if (type == TOWER_SUPPORT)
      return;

    // look for a target
    int8_t target = getTarget(false);

    // reload weapon but only every second frame
    uint8_t state = getState();
    if (isFramesMod2) {

      // decrement because 0 means is loaded
      if (state > 0)
        state--;

      
          // this is only relevant for the shock and laser tower
          if (type == TOWER_SHOCK /* || type == TOWER_LASER*/) {
            Serial.println("state: " + String(state));
          }
      

      // this is only relevant for the shock and laser tower
      if (state == 0 && (type == TOWER_SHOCK /*|| type == TOWER_LASER*/)) {
        bitSet(activeRange, BIT_TOWER_RELOADED);
        Serial.println("tower is reloaded");
      }
    }

    // there is nothing to do if no enemy is near
    if (target == -1)
      return;

    // the values to calculate with
    uint8_t xCenter = getCenterX();
    uint8_t yCenter = getCenterY();
    int8_t xTarget = eM.list[target].getCenterX();
    int8_t yTarget = eM.list[target].getCenterY();

    // do the rotation
    int8_t istSektor = getSektor();
    if (isRotatingTower(type)) {

      // get the sektor of the prefered target, y-axis is inversed on the display!
      int8_t targetSektor = calculateSektor(xTarget - xCenter, yCenter - yTarget);

      // check if rotation is necassary
      if (istSektor != targetSektor) {

        // turn the fastest way
        if (isClockwiseShorter(istSektor, targetSektor)) {
          istSektor--;
        } else {
          istSektor++;
        }

        // write rotation back
        setSektor((istSektor + 16) % 16);

        // do nothing if tower is still in wrong direction
        if (istSektor != targetSektor)
          return;
      }
    }

    // get positon where the bullet starts
    int8_t xStart = xCenter + getDirectionX(istSektor);
    int8_t yStart = yCenter + getDirectionY(istSektor);

    // every projectile has the sektor on its 4 low bits
    uint8_t projState = istSektor;

    // get the level for different weapon stuff
    uint8_t lvl = getLevel();

    // does direct damage to one enemy
    if (type == TOWER_LASER) {

      // do laser dmg
      eM.list[target].damage(1, TOWER_LASER);

      if (isFramesMod2)
        arduboy.drawLine(xStart, yStart, xTarget, yTarget, BLACK);

      return;

      // does direct damage to any enemy in range
    } else if (type == TOWER_SHOCK && getBit(activeRange, BIT_TOWER_RELOADED)) {

      // add 2 because there was minus 1 on top of this function
      if (isFramesMod2) {

        if (state < 14 /*5 + lvl * 3*/) {
          setState(state + 2);

        } else {
          // start reloading time
          bitClear(activeRange, BIT_TOWER_RELOADED);

          Serial.println("tower needs to reload");
        }
      }

      // do shock dmg in this function call
      getTarget(true);

      // draw the shock wave
      arduboy.drawCircle(xCenter, yCenter, getRange() - gameFrames % 3, BLACK);

      return;

      // does direct damage to any enemy in range
    } else if (type == TOWER_FLAME) {

      // put the current tower level into the high byte of projectile for flame range
      setHighNibble(projState, lvl + 5);
    }

    // write the current state in nibble varialbe for next call
    setState(state);

    // weapon can only shoot if loaded
    if (state != 0)
      return;

    // the state nibble can only count form 0-15 so do it every second frame to get reload timings of 32 is more than a second
    if (isFramesMod2) {
      
      uint8_t reloadTime;
      if (type == TOWER_FLAME) {
        // flame tower is always shoot with max speed
        reloadTime = 1;

      } else {
        // get tower specific reload time
        reloadTime = getProgMem(towerReloadTimes, type);

        // reduce depending on current level
        reloadTime -= lvl;
      }

      // set reload variable
      setState(reloadTime);
    }

    // shoot the bullet
    pM.add(xStart, yStart, type, projState);
  }

  void draw() {
    // get a few variables
    uint8_t x = getX();
    uint8_t y = getY();
    uint8_t lvl = getLevel();
    uint8_t type = getType();

    // draw socket depending on level
    drawTowerSocket(x, y, lvl);

    // draw the range of this tower
    if (false)
      arduboy.drawCircle(getCenterX(), getCenterY(), getRange(), BLACK);

    if (isRotatingTower(type)) {

      // TODO: draw 1/4 of those towers fast

      // get sektor
      uint8_t sektor = getSektor();

      // set offset for correct level and fine rotation
      uint8_t offset = lvl % 4 + (sektor % 4) * 4;

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
      uint8_t offset = lvl % 4 + (type - TOWER_SHOCK) * 4 + 16 * 6;

      // can be drawn fast
      drawBitmapFast(x + 2, y + 2, allTowers, 7, offset, false);
    }
  }
};

struct towerManager {
  static const uint8_t maximum = 20;
  tower list[maximum];

  void clearTower(uint8_t towerIndex) {
    bitClear(list[towerIndex].activeRange, BIT_TOWER_ACTIVE);
  }

  void setTowerActive(uint8_t towerIndex) {
    bitSet(list[towerIndex].activeRange, BIT_TOWER_ACTIVE);
  }

  bool isTowerActive(uint8_t towerIndex) {
    return getBit(list[towerIndex].activeRange, BIT_TOWER_ACTIVE);
  }

  void sell(uint8_t towerIndex) {

    // delete this tower
    clearTower(towerIndex);

    uint8_t mapIndex = list[towerIndex].index;

    // set the 4 map notes to tower
    mM.set2x2Nodes(mapIndex, MAP_FREE);
  }

  void init() {
    for (uint8_t i = 0; i < maximum; i++) {
      clearTower(i);
    }
  }

  uint8_t getTowerAt(uint8_t xRaster, uint8_t yRaster) {
    return getTowerAt(getIndex(xRaster, yRaster));
  }

  uint8_t getTowerAt(uint8_t mapIndex) {

    for (uint8_t i = 0; i < maximum; i++) {

      if (isTowerActive(i) == false)
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

      if (isTowerActive(i))
        continue;

      setTowerActive(i);
      foundSlot = true;

      uint8_t towerIndex = getIndex(xR, yR);

      //Serial.println("addTower i:" + String(i) + " at mapIndex:" + String(towerIndex));

      // set the 4 map notes to tower
      mM.set2x2Nodes(xR, yR, MAP_TOWER);

      // set values
      list[i].index = towerIndex;
      list[i].levelState = 0;
      list[i].setType(type);

      list[i].setRange(20);

      // break after finding a vaild slot to add only one tower
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
      if (isTowerActive(i) == false)
        continue;

      list[i].update();

      list[i].draw();
    }
  }
};


#endif

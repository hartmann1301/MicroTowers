#ifndef Towers_h
#define Towers_h

#define BIT_TOWER_ACTIVE    3
#define BIT_TOWER_RELOADED  2

struct tower {
  // location on the map
  uint8_t index;

  // highNibble: type, lowNibble: sektor     NOTE: highest bit of the type nibble is not used yet!
  uint8_t typeSektor;

  // highNibble: state, Bit3: active, Bit2: reloaded, Bits1&0: level
  uint8_t stateLevel;

  // boost
  uint8_t boost;

  // getters
  uint8_t getType() {
    return getHighNibble(typeSektor);
  }

  uint8_t getSektor() {
    return getLowNibble(typeSektor);
  }

  uint8_t getLevel() {
    return getLow2Bits(stateLevel);
  }

  uint8_t getState() {
    return getHighNibble(stateLevel);
  }

  uint8_t getRange(uint8_t type) {
    return getTowerRange(type, getLevel());
  }

  uint8_t getRange() {
    return getRange(getType());
  }

  void drawRange(uint8_t type) {
    // a circle to show the current range
    arduboy.drawCircle(getCenterX(), getCenterY(), getRange(type), BLACK);
  }

  void drawRange() {
    drawRange(getType());
  }

  // setters
  void setType(uint8_t value) {
    setHighNibble(typeSektor, value);
  }

  void setSektor(uint8_t value) {
    setLowNibble(typeSektor, value);
  }

  void setLevel(uint8_t value) {
    setLow2Bits(stateLevel, value);
  }

  void setState(uint8_t value) {
    setHighNibble(stateLevel, value);
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

        // looks a bit strange but it works
        eM.list[i].damage(getTowerDamage(TOWER_SHOCK, getLevel(), boost), TOWER_SHOCK);
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

  uint8_t getShockwaveMaxTime() {
    return 8 + getLevel() * 2;
  }

  void reloadShockAndLaser() {

    // add 2 because there was minus 1 on top of this function
    if (isFramesMod2)
      return;

    uint8_t state = getState();

    // add a value depending on the current level
    if (state < getShockwaveMaxTime()) {
      setState(state + 2);

    } else {
      // start reloading time
      bitClear(stateLevel, BIT_TOWER_RELOADED);
    }
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

      // this is only relevant for the shock and laser tower they have two phases
      if (state == 0 && (type == TOWER_SHOCK || type == TOWER_LASER)) {

        // mark that the tower is now reloaded and ready to shoot
        bitSet(stateLevel, BIT_TOWER_RELOADED);
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
    if (type == TOWER_LASER && getBit(stateLevel, BIT_TOWER_RELOADED)) {

      // set shooting and loading timings
      reloadShockAndLaser();

      // do laser dmg
      eM.list[target].damage(getTowerDamage(TOWER_LASER, lvl, boost), TOWER_LASER);

      if (isFramesMod2)
        arduboy.drawLine(xStart, yStart, xTarget, yTarget, BLACK);

      return;

      // does direct damage to any enemy in range
    } else if (type == TOWER_SHOCK && getBit(stateLevel, BIT_TOWER_RELOADED)) {

      // set shooting and loading timings
      reloadShockAndLaser();

      // do shock dmg in this function call
      getTarget(true);

      // draw the shock wave
      if (isFramesMod2) {
        // calculate radius of shock, + isFramesMod2 would be a smooth number
        uint8_t shockRadius = getRange() - (getShockwaveMaxTime() - getState());

        arduboy.drawCircle(xCenter, yCenter, shockRadius, BLACK);
      }

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
        reloadTime = getTowerReload(type, lvl);
      }

      // set reload variable
      setState(reloadTime);
    }

    // shoot the bullet
    pM.add(xStart, yStart, type, lvl, projState, boost);
  }

  void draw() {
    // get a few variables
    uint8_t x = getX();
    uint8_t y = getY();
    uint8_t lvl = getLevel();
    uint8_t type = getType();

    // draw socket depending on level
    drawTowerSocket(x, y, lvl);

    //
    if (type == TOWER_PROTOTYPE) {

      // draw range depending on selected tower
      drawRange(indexBuildMenu);

    } else {
      // draw weapon
      drawTowerWeapon(x, y, getType(), getSektor(), lvl);
    }
  }
};

struct towerManager {
  static const uint8_t maximum = 24;
  tower list[maximum];

  void clearTower(uint8_t towerIndex) {
    bitClear(list[towerIndex].stateLevel, BIT_TOWER_ACTIVE);
  }

  void setTowerActive(uint8_t towerIndex) {
    bitSet(list[towerIndex].stateLevel, BIT_TOWER_ACTIVE);
  }

  bool isTowerActive(uint8_t towerIndex) {
    return getBit(list[towerIndex].stateLevel, BIT_TOWER_ACTIVE);
  }

  // carefull with this function it needs the towerIndex not and mapIndex!
  void sell(uint8_t towerIndex) {

    uint8_t mapIndex = list[towerIndex].index;

    // set the 4 map notes to tower
    mM.set2x2Nodes(mapIndex, MAP_FREE);

    //Serial.println("sell Tower:" + String(towerIndex) + " at mapIndex:" + String(mapIndex));

    // delete this tower
    clearTower(towerIndex);
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

    // return did not find a tower
    return NO_INDEX;
  }

  void setBoosts() {

    // delete the last boost values
    for (uint8_t i = 0; i < maximum; i++) {
      list[i].boost = 0;
    }

    for (uint8_t i = 0; i < maximum; i++) {

      if (isTowerActive(i) == false)
        continue;

      if (list[i].getType() != TOWER_SUPPORT)
        continue;

      uint8_t boosterLevel = list[i].getLevel() + 1;

      //Serial.println("found booster i: " + String(i) + " at map index:" + String(list[i].index));

      // check all twelfe postions next to tower, diagonal is not included!
      for (uint8_t p = 0; p < 12; p++) {

        uint8_t mapIndex = list[i].index;

        // get x and y coordinates near
        int8_t xTest = getxR(mapIndex) + pgm_read_byte(xPostionsNear + p);
        int8_t yTest = getyR(mapIndex) + pgm_read_byte(yPostionsNear + p);

        //Serial.println(" look at xTest: " + String(xTest) + " yTest:" + String(yTest));

        // leave because out of map
        if (xTest < 0 || xTest >= ROWS || yTest < 0 || yTest >= COLUMNS)
          continue;

        uint8_t testTowerIndex = getTowerAt(xTest, yTest);

        // leave because there was no tower placed at this positon
        if (testTowerIndex == NO_INDEX)
          continue;

        // leave because support towers can not boost each others
        if (list[testTowerIndex].getType() == TOWER_SUPPORT)
          continue;

        list[testTowerIndex].boost += boosterLevel * BOOST_PRO_LVL;

        //Serial.println(" boost to: " + String(list[testTowerIndex].boost));
      }
    }
  }

  void add(uint8_t xR, uint8_t yR, uint8_t type) {

    bool foundSlot = false;
    for (uint8_t i = 0; i < maximum; i++) {

      if (isTowerActive(i))
        continue;

      setTowerActive(i);
      foundSlot = true;

      uint8_t mapIndex = getIndex(xR, yR);

      //Serial.println("add Tower:" + String(i) + " at mapIndex:" + String(mapIndex));

      // set the 4 map notes to tower
      mM.set2x2Nodes(xR, yR, MAP_TOWER);

      // set values
      list[i].index = mapIndex;
      list[i].boost = 0;
      list[i].setLevel(0);
      list[i].setState(0);
      list[i].setType(type);

      // 8 means tower is looking to the left side
      list[i].setSektor(8);

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

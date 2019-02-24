#ifndef Enemys_h
#define Enemys_h

//#define DEGUG_DMG_ENEMYS

#define BIT_ENEMY_ACTIVE  6
#define BIT_LOOKS_LEFT    7

struct enemy {
  int8_t x;
  int8_t y;

  // here is one nibble free!
  uint8_t type;

  // highNibble: frozen, lowNibble: state
  uint8_t frozenState;

  // this needs to be 16 bits for a good accuracy of all the weapons
  uint16_t health;

  // first 3 * 2 bits are directions, than is active and looks left boolean
  uint8_t pathStorage = 0;

  // stores the cost value, the smaller it is the nearer is the hq
  uint8_t mapCosts;

  uint8_t getState() {
    return getLowNibble(frozenState);
  }

  uint8_t getFrozen() {
    return getHighNibble(frozenState);
  }

  void setState(uint8_t value) {
    setLowNibble(frozenState, value);
  }

  void setFrozen(uint8_t value) {
    setHighNibble(frozenState, value);
  }

  uint8_t getCenterX() {
    return x + 2;
  }

  uint8_t getCenterY() {
    return y + 5;
  }

  void saveDirection(uint8_t dir) {

    //Serial.print("pathStorage was : ");
    //Serial.println(pathStorage, HEX);

    // calc shifts for the current slot
    uint8_t shifts = (getState() % 3) * 2;

    //Serial.println("Save: " + String(dir) + " with shifts:" + String(shifts));

    // create mask for current direction zeros and bits 6 and 7 ones
    uint8_t directionMask = ~(0b00000011 << shifts) | 0xc0;

    // delete old directions
    pathStorage &= directionMask;

    // shift current dir to the right slot
    dir = dir << shifts;

    // save current direction
    pathStorage |= dir;
  }

  uint8_t getSavedDirection() {

    // calc shifts for the current slot
    uint8_t shifts = (getState() % 3) * 2;

    // get the two direction bits
    uint8_t tmp = pathStorage & (0b00000011 << shifts);

    //Serial.println("Get: " + String(tmp >> shifts) + " with shifts:" + String(shifts));

    // return the backshifted direction
    return tmp >> shifts;
  }

  void update() {
    if (isFramesMod2 && type != ENEMY_IS_FAST)
      return;

    // do not walk if enemy is frozen
    uint8_t currentFrozen = getFrozen();
    if (currentFrozen > 0) {
      setFrozen(currentFrozen - 1);

      return;
    }

    // get the state for walking
    uint8_t state = getState();

    uint8_t dir;
    if (state  < 3) {
      // look at the map and get the right direction
      dir = mM.getDirection(getCenterX(), getCenterY());

      // save it
      saveDirection(dir);

    } else {
      // restore the saved dir bytes for some random moves
      dir = getSavedDirection();

    }

    // check if reached a new cost raster
    mapCosts = mM.getCurrentCost(getCenterX(), getCenterY());

    // check if headquarter was reached
    if (dir == NO_DIRECTION) {

      if (currentLifePoints > 0) {
        currentLifePoints--;

      } else {
        Serial.println("End of game");

        currentLifePoints = 100;
      }

      // no need for this enemy anymore
      die();
    }

    // increment state from 0-5
    if (state == 5) {
      setState(0);

    } else {
      setState(state + 1);
    }

    // actual movement of the enemy
    if (dir == GO_RIGHT) {
      x++;

      // clear the is looking Left bit
      bitClear(pathStorage, BIT_LOOKS_LEFT);

    } else if (dir == GO_UP && y > 0) {
      y--;

    } else if (dir == GO_DOWN) {
      y++;

    } else if (dir == GO_LEFT) {
      x--;

      // set the is looking Left bit
      bitSet(pathStorage, BIT_LOOKS_LEFT);
    }
  }

  bool damage(uint16_t dmg, uint8_t dmgType) {

    // freeze the enemys if type was frost tower
    if (dmgType == TOWER_FROST) {
      uint8_t currentFrozen = getFrozen();

      if (currentFrozen < 14)
        setFrozen(currentFrozen + 2);
    }

    if (dmg == 0)
      return false;

#ifdef DEGUG_DMG_ENEMYS
    Serial.print("Enemy got DMG:");
    Serial.print(dmg, DEC);
#endif

#ifdef DEGUG_DMG_ENEMYS
    Serial.print(" health is: ");
    Serial.print(health, DEC);
#endif

    // reduce the damage because of damage resistances
    if (type == ENEMY_RESITS_BULLETS && (dmgType == TOWER_GATLING || dmgType == TOWER_CANNON))
      dmg /= 2;

    if (type == ENEMY_RESITS_LASERS && (dmgType == TOWER_RAILGUN || dmgType == TOWER_LASER))
      dmg /= 2;

    if (type == ENEMY_RESITS_AOES && (dmgType == TOWER_FLAME || dmgType == TOWER_SHOCK))
      dmg /= 2;

#ifdef DEGUG_DMG_ENEMYS
    Serial.print(" resisted: ");
    Serial.print(dmg, DEC);
#endif

    // do the animation


    if (health > dmg) {
      health -= dmg;

#ifdef DEGUG_DMG_ENEMYS
      Serial.print(" health is now: ");
      Serial.println(health, DEC);
#endif

      return false;

    } else {
      //Serial.println("Enemy Died");
      die();

#ifdef DEGUG_DMG_ENEMYS
      Serial.println(" died");
#endif

      return true;
    }
  }

  void die() {

    // delete this enemy for the manager list
    bitClear(pathStorage, BIT_ENEMY_ACTIVE);
  }

  void drawHealthBar() {

    // twopods are a bit to high, so minus one pixel of yPos
    int8_t yPos = y;
    if (currentEnemysType == ENEMY_TYPE_TWOPOD)
      yPos--;

    uint16_t oneHealthPixel = currentWaveHp / 7;

    if (oneHealthPixel == 0)
      return;

    // calculate the length in pixels
    uint8_t hLen = health / oneHealthPixel + 1;

    // draw healt bar
    arduboy.fillRect((8 - hLen) + x, yPos, hLen, 1, BLACK);
  }

  void draw() {
    bool isLookingLeft = getBit(pathStorage, BIT_LOOKS_LEFT);

    uint8_t st = getState() % 3;

    // default width is 6, can be changed later
    uint8_t w = 6;

    const uint8_t *img;
    if (currentEnemysType == ENEMY_TYPE_CYBORG) {
      img = enemyCyborgs;

    } else if (type <= ENEMY_TYPE_TWOPOD) {
      img = enemyTwoPods;

    } else {
      img = enemyMonsters;

      // monsters need a bit more pixels
      w = 9;
    }

    drawHealthBar();

    // draw the spirte fast
    drawBitmapFast(x, y, img, w, type * 3 + st, isLookingLeft);
  }

  bool isInRange(int16_t xTower, int16_t yTower, int16_t range) {

    uint16_t xDist = abs(xTower - getCenterX());
    uint16_t yDist = abs(yTower - getCenterY());

    // check if not in rect around range circle
    if (xDist > range || yDist > range)
      return false;

    // check if in rect in range circle
    uint8_t rectInRange = (range * 10) / 7;
    if (xDist < rectInRange && yDist < rectInRange)
      return true;

    // calculate actual distance with pythagoras
    if (integerSqrt(xDist * xDist + yDist * yDist) < range) {
      return true;
    } else {
      return false;
    }
  }
};

struct enemyManager {
  static const uint8_t maximum = 5;
  enemy list[maximum];

  void clearEnemy(uint8_t index) {
    bitClear(list[index].pathStorage, BIT_ENEMY_ACTIVE);
  }

  void setEnemyActive(uint8_t index) {
    bitSet(list[index].pathStorage, BIT_ENEMY_ACTIVE);
  }

  bool isEnemyActive(uint8_t index) {
    return getBit(list[index].pathStorage, BIT_ENEMY_ACTIVE);
  }

  void init() {
    for (uint8_t i = 0; i < maximum; i++) {
      clearEnemy(i);
    }
  }

  void sendWaves() {

    // return because sending waves is not activated yet or done
    if (sendWaveStatus == WAVE_FINISHED)
      return;

    // return because not frame for next enemy yet
    if (nextEnemyFrame > gameFrames)
      return;

    // is done once when the wave was started
    if (sendWaveStatus == WAVE_START) {

      // sets how many enemys will be sent in this wave
      enemysOfWave = ENEMYS_IN_WAVE;

      // increment the levels waves
      if (waveCounter < MAXIMAL_WAVE)
        waveCounter++;

      // recalculate the hp of the current wave
      currentWaveHp = getEnemyHp(waveCounter, currentMapDifficulty);

      // to skip this in the next call
      sendWaveStatus = WAVE_ACTIVE;
    }

    //  holds what kinds of enemys will be sent, see ENEMYS enum
    uint8_t currentWaveType = waveCounter % TYPES_OF_WAVES;

    // wave if over if no enemy is left to spawn and map is empty
    if (count() == 0 && enemysOfWave == 0) {
      sendWaveStatus = WAVE_FINISHED;

      // if playing show message that next wave can be triggered
      if (inPlayingMode(gameMode))
        setInfoMessage(INFO_SEND_NEXT_WAVE);

      // if 6 waves passed than it is time to change the race
      if (currentWaveType == TYPES_OF_WAVES - 1) {

        // change race, this should only be done if no enemy is on the field
        if (currentEnemysType < 2) {
          currentEnemysType++;

        } else {
          currentEnemysType = 0;
        }
      }
    }

    // return because there is no enemy left to spawn
    if (enemysOfWave == 0)
      return;

    // decrement counter because one enemy will be sent in this call
    enemysOfWave--;

    // get the type of the enemy
    uint8_t enemyType;
    if (currentWaveType == ENEMY_MIX) {

      // add 5 different enemys
      enemyType = enemysOfWave;
    } else {

      // add 5 of the same kind
      enemyType = currentWaveType;
    }

    Serial.println("add Enemy nr: " + String(enemysOfWave) + " type: " + String(enemyType) + " waveCounter " + String(waveCounter));

    // add the enemy left to the entry on screen
    add(rand() % 4 - 8, 22, enemyType);

    // set timeouts for next enemy
    nextEnemyFrame = NEXT_ENEMY_TIMEOUT + gameFrames;
  }

  uint8_t count() {
    uint8_t num = 0;

    for (uint8_t i = 0; i < maximum; i++) {
      if (isEnemyActive(i))
        num++;
    }

    return num;
  }

  void add(uint8_t x, uint8_t y, uint8_t type) {

    bool foundSlot = false;
    for (uint8_t i = 0; i < maximum; i++) {

      // look if this slot is already used
      if (isEnemyActive(i))
        continue;

      setEnemyActive(i);;
      foundSlot = true;

      // set values
      list[i].x = x;
      list[i].y = y;
      list[i].type = type;
      list[i].frozenState = 0;

      // Serial.println("add enemy: " + String(i) + " of type: " + String(type) + " health:" + String(currentWaveHp));

      // get healt depending on global match progress variables
      list[i].health = currentWaveHp;
      break;
    }

#ifdef DEBUG_ADD_FUNCTIONS
    if (!foundSlot)
      Serial.println("Warning, no slot for enemy!");
#endif
  }

  void update() {

    for (uint8_t i = 0; i < maximum; i++) {

      // send new waves
      sendWaves();

      // update only active enemys
      if (isEnemyActive(i) == false)
        continue;

      list[i].update();

      // if they went inactive while updating
      if (isEnemyActive(i) == false)
        continue;

      list[i].draw();
    }
  }
};

#endif

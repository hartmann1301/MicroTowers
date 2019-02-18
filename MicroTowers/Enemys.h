#ifndef Enemys_h
#define Enemys_h

#define STATE_DEAD     250
#define STATE_SPAWN    255

#define BIT_IS_ACTIVE   6 // !!!!!!
#define BIT_LOOKS_LEFT  7

enum {
  ANIMAL_RAT,
  ANIMAL_HYENA,
  ANIMAL_WOLF,
  ANIMAL_PIG,
  ANIMAL_GHOST,
  HUMAN_RUNNING,
  HUMAN_MG,
  HUMAN_SHIELD,
  HUMAN_ROCKET,
  HUMAN_BACKPACK,
  TWOPOD_LVL0,
  TWOPOD_LVL1,
  TWOPOD_LVL2,
  TWOPOD_LVL3,
  TWOPOD_LVL4
};

struct enemy {
  int8_t x;
  int8_t y;

  uint8_t type;
  uint8_t state = 0;

  uint16_t health;

  // first 3 * 2 bits are directions, than is active and looks left boolean
  uint8_t pathStorage = 0;
  bool active = false;

  // stores the cost value, the smaller it is the nearer is the hq
  uint8_t mapCosts;

  uint8_t getCenterX() {
    return x + 2;
  }

  uint8_t getCenterY() {
    return y + 5;
  }

  void saveDirection(uint8_t dir) {

    // calc shifts for the current slot
    uint8_t shifts = (state % 3) * 2;

    //Serial.println("Save: " + String(dir) + " with shifts:" + String(shifts));

    // delete last direction
    pathStorage &= ~(0b00000011 << shifts);

    // shift current dir to the right slot
    dir = dir << shifts;

    // save current direction
    pathStorage |= dir;
  }

  uint8_t getSavedDirection() {

    // calc shifts for the current slot
    uint8_t shifts = (state % 3) * 2;

    // get the two direction bits
    uint8_t tmp = pathStorage & (0b00000011 << shifts);

    //Serial.println("Get: " + String(tmp >> shifts) + " with shifts:" + String(shifts));

    // return the backshifted direction
    return tmp >> shifts;
  }

  void update() {

    if (gameFrames % 2 != 0)
      return;

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
    if (dir == NO_DIRECTION || health == 0) {

      if (type < 14) {
        type++;
      } else {
        type = 0;
      }

      health = 100 * 100;

      // respawn
      x = millis() % 4 - 8;
      y = 22;
    }

    // increment state from 0-5
    if (state == 5) {
      state = 0;

    } else {
      state++;
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

  bool damage(uint8_t dmg) {

#ifdef DEGUG_DMG_ENEMYS
    Serial.print("Enemy got DMG:");
    Serial.print(dmg, DEC);
#endif

    if (dmg == 0)
      return false;

    uint16_t tmpDmg = dmg * 100;

    // reduce the damage because of map progress
    uint16_t reducePercent = 5;

#ifdef DEGUG_DMG_ENEMYS
    Serial.print(" -%:");
    Serial.print(reducePercent, DEC);
#endif

    tmpDmg -= dmg * reducePercent;

#ifdef DEGUG_DMG_ENEMYS
    Serial.print(" is:");
    Serial.println(tmpDmg / 100, 2);
#endif

    if (health > tmpDmg) {
      health -= tmpDmg;

    } else {
      //Serial.println("Enemy Died");
      die();

      return true;
    }

    return false;
  }

  void die() {
    health = 0;
    //state = STATE_DEAD;
  }

  void drawHealthBar() {
    // draw healt bar
    uint8_t hLen = (health / 100) / 14 + 1;
    arduboy.fillRect((8 - hLen) + x, y, hLen, 1, BLACK);
  }

  void draw() {
    bool isLookingLeft = getBit(pathStorage, BIT_LOOKS_LEFT);

    uint8_t st = state % 3;

    uint8_t w;
    if (type <= ANIMAL_GHOST) {
      w = 9;
    } else {
      w = 6;
    }

    const uint8_t *img;
    if (type <= ANIMAL_GHOST) {
      img = enemyAnimals;
    } else if (type <= HUMAN_BACKPACK) {
      img = enemyHumans;
    } else {
      img = enemyTwoPods;
    }

    drawHealthBar();

    // draw the spirte fast
    drawBitmapFast(x, y, img, w, (type % 5) * 3 + st, isLookingLeft);
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
  static const uint8_t maximum = 14;
  enemy list[maximum];

  void add(uint8_t x, uint8_t y, uint8_t type) {

    bool foundSlot = false;
    for (uint8_t i = 0; i < maximum; i++) {

      // look if this slot is already used
      if (list[i].active == true)
        continue;

      list[i].active = true;
      foundSlot = true;

      // set values
      list[i].x = x;
      list[i].y = y;
      list[i].type = type;
      list[i].health = 100 * 100;
      break;
    }

#ifdef DEBUG_ADD_FUNCTIONS
    if (!foundSlot)
      Serial.println("Warning, no slot for enemy!");
#endif
  }

  uint8_t count() {
    uint8_t num = 0;

    for (uint8_t i = 0; i < maximum; i++) {
      if (list[i].active && list[i].health > 0)
        num++;
    }

    return num;
  }

  void update() {

    for (uint8_t i = 0; i < maximum; i++) {

      // update only active enemys
      if (list[i].active == false)
        continue;

      list[i].update();

      // if they went inactive while updating
      if (list[i].active == false)
        continue;

      list[i].draw();
    }
  }
};

#endif

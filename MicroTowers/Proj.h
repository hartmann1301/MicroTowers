#ifndef Projectiles_h
#define Projectiles_h

#define BIT_PROJECTILE_ACTIVE 3
#define BIT_PROJECTILE_IMPACT 2

struct projectile {
  int8_t x;
  int8_t y;

  // highNibble: type, Bit3: active, Bit2: impact, Bits1&0: level  
  uint8_t typeActiveLevel;

  // it is mostly the tower sektor plus some extra info
  uint8_t state;

  // sadly this is a full byte
  uint8_t boost;

  uint8_t getType() {
    return getHighNibble(typeActiveLevel);
  }

  void setType(uint8_t value) {
    setHighNibble(typeActiveLevel, value);
  }

  uint8_t getLevel() {
    getLow2Bits(typeActiveLevel);
  }

  void setLevel(uint8_t value) {
    setLow2Bits(typeActiveLevel, value);
  }  

  void draw() {
    // strange switch thing, variable needs to be initialised here
    int8_t flameSize;

    switch (getType()) {
      case TOWER_GATLING:
        arduboy.drawPixel(x, y, BLACK);
        break;

      case TOWER_CANNON:
        arduboy.fillRect(x, y, 2, 2, BLACK);
        break;

      case TOWER_FROST:
        arduboy.drawCircle(x, y, 1, BLACK);
        break;

      case TOWER_FLAME:
        // get frame size
        flameSize = 5 - getHighNibble(state);

        if (flameSize < 2)
          flameSize = 2;

        fillChessRect(x - 1, y - 1, flameSize, flameSize, BLACK);
        break;

      case TOWER_RAILGUN:
        arduboy.drawLine(x, y, x + getDirectionX(state), y + getDirectionY(state), BLACK);
        break;

      // uses no projectile
      case TOWER_LASER:
      case TOWER_SHOCK:
      case TOWER_SUPPORT:
        break;

    }
  }

  void update() {
    uint8_t type = getType();

    // is needed because the flame tower writes level in high byte of state var
    uint8_t tmpSektor = getLowNibble(state);

    // moves projectile depinding on sektor
    x += getDirectionX(tmpSektor);
    y += getDirectionY(tmpSektor);

    if (type == TOWER_FLAME) {
      // gets the level
      uint8_t levelRange = getHighNibble(state);

      // decrease the levelRange variable
      setHighNibble(state, levelRange - 1);

      // delete this flame if range is over
      if (levelRange == 0)
        bitClear(typeActiveLevel, BIT_PROJECTILE_ACTIVE);
    }
  }

  bool touchesPosition(uint8_t p_x, uint8_t p_y) {
    const uint8_t tolerance = 2;

    // target to far left
    if (p_x + 7 < x + tolerance)
      return false;

    // target to high up
    if (p_y + 7 < y + tolerance)
      return false;

    // target to far right
    if (x + 7 < p_x + tolerance)
      return false;

    // target to far down
    if (y + 7 < p_y + tolerance)
      return false;

    return true;
  }

  bool offScreen() {
    if (x < 1 || x > 122 || y > 55) {
      return true;
    } else {
      return false;
    }
  }
};

struct projectileManager {
  static const uint8_t maximum = 40;
  projectile list[maximum];

  void clearProjectile(uint8_t index) {
    bitClear(list[index].typeActiveLevel, BIT_PROJECTILE_ACTIVE);
  }

  void setProjectileActive(uint8_t index) {
    bitSet(list[index].typeActiveLevel, BIT_PROJECTILE_ACTIVE);
  }

  bool isProjectileActive(uint8_t index) {
    return getBit(list[index].typeActiveLevel, BIT_PROJECTILE_ACTIVE);
  }

  void init() {
    for (uint8_t i = 0; i < maximum; i++) {
      clearProjectile(i);
    }
  }

  void add(uint8_t x, uint8_t y, uint8_t type, uint8_t lvl, int8_t state, int8_t boost) {

    bool foundSlot = false;
    for (uint8_t i = 0; i < maximum; i++) {

      if (isProjectileActive(i))
        continue;

      setProjectileActive(i);
      foundSlot = true;

      //Serial.println("add p: " + String(i) + " of type: " + String(type));

      // this is only relevant for the canon tower
      bitClear(list[i].typeActiveLevel, BIT_PROJECTILE_IMPACT);

      // set values
      list[i].x = x;
      list[i].y = y;
      list[i].setType(type);
      list[i].setLevel(lvl);      
      list[i].state = state;
      list[i].boost = boost;     
      break;
    }

#ifdef DEBUG_ADD_FUNCTIONS
    if (!foundSlot)
      Serial.println("Warning, no slot for projectile!");
#endif
  }

  uint8_t isProjectileAt(uint8_t x, uint8_t y) {

    for (uint8_t i = 0; i < maximum; i++) {

      // check only active items
      if (isProjectileActive(i) == false)
        continue;

      // if active[0].isPosition(..) return index 1, 0 is no item
      if (list[i].touchesPosition(x, y)) {
        return i;
      }
    }

    return 0xff;
  }

  void update() {

    for (uint8_t i = 0; i < maximum; i++) {

      // only active items
      if (isProjectileActive(i) == false)
        continue;

      list[i].update();

      if (list[i].offScreen())
        clearProjectile(i);

      list[i].draw();
    }
  }
};

#endif

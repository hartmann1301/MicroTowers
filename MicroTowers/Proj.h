#ifndef Projectiles_h
#define Projectiles_h

struct projectile {
  int8_t x;
  int8_t y;

  uint8_t type;
  bool active;

  uint8_t state;

  void draw() {

    switch (type) {
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
        fillChessRect(x - 1, y - 1, 3, 3, BLACK);
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

    /*
      } else if (type == WEAPON_FLAME) {

      if (state < 4 * 4) {
        arduboy.fillRect(x, y, 4, 1, BLACK);

      } else if (state < 4 * 7) {
        fillChessRect(x, y - 1, 4, 3, BLACK);

    */
  }

  void update() {
    int8_t vX, vY;

    // moves projectile depinding on sektor
    x += getDirectionX(state);
    y += getDirectionY(state);
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

  void init() {
    for (uint8_t i = 0; i < maximum; i++) {
      list[i].active = false;
    }
  }

  void add(uint8_t x, uint8_t y, uint8_t type, int8_t state) {

    bool foundSlot = false;
    for (uint8_t i = 0; i < maximum; i++) {

      if (list[i].active == true)
        continue;

      list[i].active = true;
      foundSlot = true;

      //Serial.println("add p: " + String(i) + " of type: " + String(type));

      // set values
      list[i].x = x;
      list[i].y = y;
      list[i].type = type;
      list[i].state = state;
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
      if (list[i].active == false)
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
      if (list[i].active == false)
        continue;

      if (list[i].offScreen())
        list[i].active = false;

      list[i].draw();

      list[i].update();
    }
  }
};

#endif

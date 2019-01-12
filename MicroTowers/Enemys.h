#ifndef Enemys_h
#define Enemys_h

#define MAX_ENEMYS      12

#define STATE_DEAD     250
#define STATE_SPAWN    255

enum {
  ENEMY_ROCKET,
  ENEMY_STD,
  ENEMY_MG,
  ENEMY_SHIELD,
  ENEMY_FLAME
};

struct enemy {
  uint8_t x;
  uint8_t y;
  uint8_t type;
  uint8_t state;
  uint16_t health;
  bool active = false;

  void update() {

 
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
    Serial.println(float(tmpDmg)/100, 2);
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
    state = STATE_DEAD;

    if (type == ENEMY_FLAME) {
      aM.add(x + 4, y, ANIMATION_BOOM, 0);
    }
  }

  void drawHealthBar() {
    // draw healt bar
    uint8_t hLen = (health / 100) / 14 + 1;
    arduboy.fillRect((8 - hLen) + x, y, hLen, 1, BLACK);
  }

  void drawFeet(uint8_t s) {
    if (s < 2) {
      // standing still
      arduboy.drawPixel(x + 3, y + 7, BLACK);
      arduboy.drawPixel(x + 4, y + 7, BLACK);

    } else if (s == 2) {
      // step forward and shoot
      arduboy.drawPixel(x + 2, y + 7, BLACK);
      arduboy.drawPixel(x + 4, y + 7, BLACK);

    } else if (s == 3) {
      // step back
      arduboy.drawPixel(x + 3 , y + 7, BLACK);
      arduboy.drawPixel(x + 5, y + 7, BLACK);
    }
  }

  void draw() {
    // this item just waits to be removed
    if (state == STATE_SPAWN)
      return;

  }

  bool touchesPosition(uint8_t p_x, uint8_t p_y, uint8_t p_w, uint8_t p_h) {
    // player to far left
    if (p_x + p_w < x + 3)
      return false;

    // player to high up
    if (p_y + p_h < y + 3)
      return false;

    // player to far right
    if (x + 7 < p_x + 3)
      return false;

    // player to far down
    if (y + 7 < p_y + 3)
      return false;

    return true;
  }

};

struct enemyManager {
  static const uint8_t maximum = 32;  
  enemy list[maximum];

  void add(uint8_t x, uint8_t y, uint8_t type, uint8_t state) {

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
      list[i].state = state;
      list[i].health = 100 * 100;
      break;
    }

#ifdef DEBUG_ADD_FUNCTIONS
    if (!foundSlot)
      Serial.println("Warning, no slot for enemy!");
#endif      
  }

  uint8_t isEnemyAt(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {

    for (uint8_t i = 0; i < maximum; i++) {

      // check only active enemys
      if (list[i].active == false)
        continue;

      // if active[0].isPosition(..) return index 1, 0 is no item
      if (list[i].touchesPosition(x, y, w, h))
        return i;

    }

    return 0xff;
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

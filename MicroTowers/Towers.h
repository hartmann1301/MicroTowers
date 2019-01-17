#ifndef Towers_h
#define Towers_h

struct tower {
  uint8_t xR;
  uint8_t yR;
  uint8_t type;
  uint8_t range;
  int16_t degree;
  
  bool active;

  void draw() {

    getDegree


  }

  int8_t getTarget() {

    int8_t bestTarget = -1;
    
    for (uint8_t i = 0; i < eM.maximum; i++) {
      // check only active enemys
      if (eM.list[i].active == false)
        continue;

      // only if the enemy is in Range
      if (!eM.list[i].isInRange(x + 3, y + 3, range))
        continue;

      // set new target if enemy has more map progress
      if (eM.list[i].dijkstraPos > bestTarget)
        bestTarget = i;
    }

    return bestTarget;
  }

  void update() {

    // there is nothing to do if no enemy is near
    if (getTarget() == -1)
      return;

    // fire

  }
};

struct towerManager {
  static const uint8_t maximum = 20;
  tower list[maximum];

  void add(uint8_t xR, uint8_t yR, uint8_t type, int8_t state) {

    bool foundSlot = false;
    for (uint8_t i = 0; i < maximum; i++) {

      if (list[i].active == true)
        continue;

      list[i].active = true;
      foundSlot = true;

      // set values
      list[i].xR = xR;
      list[i].yR = yR;
      list[i].type = type;
      list[i].state = state;
      break;
    }

#ifdef DEBUG_ADD_FUNCTIONS
    if (!foundSlot)
      Serial.println("Warning, no slot for animation!");
#endif
  }

  void update() {

    for (uint8_t i = 0; i < maximum; i++) {

      // only active items
      if (list[i].active == false)
        continue;

      list[i].update();

      list[i].draw();
    }
  }
};


#endif

#ifndef Animations_h
#define Animations_h

#define BIT_ANIMATION_ACTIVE  3

struct animation {
  int8_t x;
  int8_t y;

  // highNibble: state, Bit3: active, Bit2: unused, Bits1&0: unused
  uint8_t stateActive;

  uint8_t getState() {
    return getHighNibble(stateActive);
  }

  void setState(uint8_t value) {
    setHighNibble(stateActive, value);
  }

  void draw() {
    // draw impact
    drawBitmapFast(x + 3, y + 2, animationImpact, 4, getState());
  }

  void update() {
    // slow down the animation speed
    if (gameFrames % 3 != 0)
      return;

    uint8_t state = getState();

    if (state < 3) {
      // set to next state
      setState(state + 1);

    } else {
      // clear this animation because it is finished
      bitClear(stateActive, BIT_ANIMATION_ACTIVE);
    }
  }
};

struct animationManager {
  static const uint8_t maximum = 20;
  animation list[maximum];

  void setAnimationActive(uint8_t index) {
    bitSet(list[index].stateActive, BIT_ANIMATION_ACTIVE);
  }

  bool isAnimationActive(uint8_t index) {
    return getBit(list[index].stateActive, BIT_ANIMATION_ACTIVE);
  }

  void init() {
    for (uint8_t i = 0; i < maximum; i++) {
      bitClear(list[i].stateActive, BIT_ANIMATION_ACTIVE);
    }
  }

  void add(uint8_t x, uint8_t y) {

    bool foundSlot = false;
    for (uint8_t i = 0; i < maximum; i++) {

      if (isAnimationActive(i))
        continue;

      setAnimationActive(i);
      foundSlot = true;

      // set values
      list[i].x = x;
      list[i].y = y;
      list[i].setState(0);
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
      if (isAnimationActive(i) == false)
        continue;

      list[i].draw();

      list[i].update();
    }
  }
};

#endif

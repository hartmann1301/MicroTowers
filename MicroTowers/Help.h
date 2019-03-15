#ifndef Helper_h
#define Helper_h

#ifdef DEBUG_PERFORMANCE
uint32_t measureTimer = 0;

void startMeasure(String name) {

  Serial.print("Start: " + name);

  measureTimer = micros();
}

void endMeasure() {

  uint32_t diff = micros() - measureTimer;

  Serial.println(String(" took: ") + String(diff));
}
#endif

bool getBit(uint8_t data, uint8_t pos) {
  return data & (0b00000001 << pos);
}

uint8_t getHighNibble(uint8_t nib) {
  return (nib & 0xf0) >> 4;
}

uint8_t getLowNibble(uint8_t nib) {
  return nib & 0x0f;
}

uint8_t getLow2Bits(uint8_t data) {
  return data & 0x03;
}

void setLow2Bits(uint8_t &data, uint8_t value) {
  data = (data & 0xfc) | (value & 0x03);
}

void setHighNibble(uint8_t &nib, uint8_t value) {
  nib = (nib & 0x0f) | value << 4;
}

void setLowNibble(uint8_t &nib, uint8_t value) {
  nib = (nib & 0xf0) | (value & 0xf);
}

uint8_t getUnlockedMaps() {
  return 5 + campainStars / 2;
}

bool isRotatingTower(uint8_t type) {
  return type <= TOWER_LASER;
}

uint8_t getProgMem(const uint8_t *pointer, uint8_t offset) {
  return pgm_read_byte(pointer + offset);
}

uint8_t getTowerRange(uint8_t type, uint8_t lvl) {

  // start with the minimal range of every tower
  uint8_t range = 10;

  // add tower specific range times 2
  range += getProgMem(towerBasicRanges, type) * 2;

  // add tower specific range plus pro level
  range += getProgMem(towerExtraRanges, type) * lvl;

  //Serial.println("Tower: " + String(type) + " has Range of:" + String(range));

  return range;
}

uint8_t getTowerPrice(uint8_t type, uint8_t lvl) {

  //
  uint8_t price = getProgMem(towerBasicPrices, type) * (lvl + 1);

  // add tower specific extra update cost
  price += getProgMem(towerExtraPrices, type) * lvl;

  return price;
}

uint16_t getPriceUpdate() {

  // write update price to global var and check if possible
  if (towerLevel < 3) {
    return getTowerPrice(towerType, towerLevel + 1);

  } else {
    return NO_PRICE;
  }
}

uint16_t getPriceSell() {

  // calculte the costs buying the tower
  uint16_t priceSell = 0;
  for (uint8_t i = 0; i <= towerLevel; i++) {
    priceSell += getTowerPrice(towerType, i);
  }

  // div by 2 because of lost value
  return priceSell / 2;
}

uint8_t getTowerReload(uint8_t type, uint8_t lvl) {

  uint8_t reloadFrames = getProgMem(towerReloadTimes, type);

  // reduce depending on current level
  return reloadFrames - lvl;
}

uint16_t getTowerDamage(uint8_t type, uint8_t lvl, uint8_t boost) {

  uint32_t dmg = getProgMem(towerDamages, type) * (lvl + 1);

  // add boost percentage
  if (boost != 0)
    dmg += uint32_t(dmg * boost) / 100;

  return dmg;
}

uint8_t getTowerCategory(uint8_t type) {

  return getProgMem(towerCategories, type);
}

// could be done with getProgMem() but is called 3 times so it is maybe worth a function
int8_t getDirectionX(uint8_t sektor) {
  return pgm_read_byte(sektorStartX + sektor);
}

int8_t getDirectionY(uint8_t sektor) {
  return pgm_read_byte(sektorStartY + sektor);
}

bool isPressed(bool pressed) {

  // set global only if the button is pressed
  if (pressed)
    cursorPressed = pressed;

  // return bool for specific button functions
  return pressed;
}

bool isTimeoutActive() {
  // set default value for global cursorPressed bool
  cursorPressed = false;

  if (millis() < nextButtonInput) {
    return true;
  } else {
    return false;
  }
}

void setCursorTimeout() {
  if (cursorPressed) {
    nextButtonInput = millis() + 150;
  }
}

bool isLongPressInfo(int8_t buttonState) {
  return buttonState > LONGPRESS_INFO;
}

bool isLongPressed(int8_t buttonState) {
  return buttonState > LONGPRESS_TIME;
}

bool inPlayingMode(uint8_t m) {
  return m >= MODE_PLAYING_INFO && m <= MODE_PLAYING_END;
}

bool inEditorMode(uint8_t m) {
  return m == MODE_EDITOR || m == MODE_EDITOR_MENU;
}

bool inMapsListMode(uint8_t m) {
  return m == MODE_MAPS_CAMPAIN || m == MODE_MAPS_EDITOR;
}

uint8_t getIndex(uint8_t xR, uint8_t yR) {
  return xR + yR * ROWS;
}

uint8_t getxR(uint8_t index) {
  return index % ROWS;
}

uint8_t getyR(uint8_t index) {
  return index / ROWS;
}

uint16_t getEnemyHp(uint8_t wave, uint8_t mapDifficulty) {

  // minimal value for hp is at least 2 coins reward, needs to be 32 bit!
  uint32_t hp = HP_FOR_A_COIN * 2;

  for (uint8_t i = 1; i < wave + 1; i++) {

    // is scaled to get values from wave:1 170hp to wave:30 32650hp
    hp += i * HP_INCREASE_FAKTOR;
  }

  // do difficulty in percent, first multiply than divide!
  hp = (hp * mapDifficulty) / 100;

  //Serial.println("getEnemyHp wave: " + String(wave) + " difficulty: " + String(mapDifficulty) + " is: " + String(hp));

  return hp;
}

void calculateScore() {
  //Serial.println("lifePoints: " + String(lifePoints) + " waveCounter: " + String(waveCounter) + " currentCoins: " + String(currentCoins));

  // max score would be: 
  // 30 waves * 20 =  600 
  // 100 life * 3 =   300
  // 999 coins / 10 =  99 
  // is               999

  // write to global variable
  currentScore = uint16_t(waveCounter * 20) + uint16_t(lifePoints * 3) + currentCoins / 10;

  //Serial.println("is score: " + String(currentScore));
}

uint16_t getEnemyReward(uint8_t wave) {

  // calculate the reward for killing one enemy, always at difficulty 100%
  return getEnemyHp(wave, 100) / HP_FOR_A_COIN;
}

#ifdef DEBUG_FRAMES

uint8_t getFramesPerSecond() {

  // get the current second
  secondsNow = millis() / 1000;

  //Serial.println("secondsNow:" + String(secondsNow) + " at mapIndex:" + String(mapIndex));

  if (secondsNow != secondsCurrent) {

    // to reset only with every new second
    secondsCurrent = secondsNow;

    // save the last frames per second
    framesCurrent = framesNow;

    // reset the counter
    framesNow = 0;

  } else {
    // count
    framesNow++;
  }

  return framesCurrent;
}


#endif

uint8_t integerSqrt(uint8_t n) {
  if (n < 2)
    return n;

  uint8_t smallCandidate = integerSqrt(n >> 2) << 1;
  uint8_t largeCandidate = smallCandidate + 1;

  if (largeCandidate * largeCandidate > n) {
    return smallCandidate;
  } else {
    return largeCandidate;
  }
}

// big thanks to the following page for the awesome code
// https://www.romanblack.com/integer_degree.htm
uint16_t fastAtan(int16_t x, int16_t y) {

  // Fast XY vector to integer degree algorithm - Jan 2011 www.RomanBlack.com
  // Converts any XY values including 0 to a degree value that should be
  // within +/- 1 degree of the accurate value without needing
  // large slow trig functions like ArcTan() or ArcCos().
  // NOTE! at least one of the X or Y values must be non-zero!
  // This is the full version, for all 4 quadrants and will generate
  // the angle in integer degrees from 0-360.
  // Any values of X and Y are usable including negative values provided
  // they are between -1456 and 1456 so the 16bit multiply does not overflow.

  unsigned char negflag;
  unsigned char tempdegree;
  unsigned char comp;
  unsigned int degree;     // this will hold the result
  //signed int x;            // these hold the XY vector at the start
  //signed int y;            // (and they will be destroyed)
  unsigned int ux;
  unsigned int uy;

  // Save the sign flags then remove signs and get XY as unsigned ints
  negflag = 0;
  if (x < 0)
  {
    negflag += 0x01;    // x flag bit
    x = (0 - x);        // is now +
  }
  ux = x;                // copy to unsigned var before multiply
  if (y < 0)
  {
    negflag += 0x02;    // y flag bit
    y = (0 - y);        // is now +
  }
  uy = y;                // copy to unsigned var before multiply

  // 1. Calc the scaled "degrees"
  if (ux > uy)
  {
    degree = (uy * 45) / ux;   // degree result will be 0-45 range
    negflag += 0x10;    // octant flag bit
  }
  else
  {
    degree = (ux * 45) / uy;   // degree result will be 0-45 range
  }

  // 2. Compensate for the 4 degree error curve
  comp = 0;
  tempdegree = degree;    // use an unsigned char for speed!
  if (tempdegree > 22)     // if top half of range
  {
    if (tempdegree <= 44) comp++;
    if (tempdegree <= 41) comp++;
    if (tempdegree <= 37) comp++;
    if (tempdegree <= 32) comp++; // max is 4 degrees compensated
  }
  else    // else is lower half of range
  {
    if (tempdegree >= 2) comp++;
    if (tempdegree >= 6) comp++;
    if (tempdegree >= 10) comp++;
    if (tempdegree >= 15) comp++; // max is 4 degrees compensated
  }
  degree += comp;   // degree is now accurate to +/- 1 degree!

  // Invert degree if it was X>Y octant, makes 0-45 into 90-45
  if (negflag & 0x10) degree = (90 - degree);

  // 3. Degree is now 0-90 range for this quadrant,
  // need to invert it for whichever quadrant it was in
  if (negflag & 0x02)  // if -Y
  {
    if (negflag & 0x01)  // if -Y -X
      degree = (180 + degree);
    else        // else is -Y +X
      degree = (180 - degree);
  }
  else    // else is +Y
  {
    if (negflag & 0x01)  // if +Y -X
      degree = (360 - degree);
  }

  return degree;
}


#endif

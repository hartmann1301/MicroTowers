#ifndef Helper_h
#define Helper_h

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

uint8_t getTowerReload(uint8_t type, uint8_t lvl) {

  uint8_t reloadFrames = getProgMem(towerReloadTimes, type);

  // reduce depending on current level
  return reloadFrames - lvl;
}

uint16_t getTowerDamage(uint8_t type, uint8_t lvl, uint8_t boost) {

  uint16_t dmg = getProgMem(towerDamages, type) * (lvl + 1);

  // add boost percentage
  if (boost != 0)
    dmg += (dmg / 100) * boost;

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

  // minimal value for hp
  uint16_t hp = 100;

  for (uint8_t i = 1; i < wave + 1; i++) {

    // is scaled to get values from wave:1 170hp to wave:30 32650hp
    hp += i * 70;
  }

  // difficulty can be from 10 to 20, where 10 is normal and 20 is double hp
  hp = (hp / 10) * mapDifficulty;

  //Serial.println("getEnemyHp wave: " + String(wave) + " difficulty: " + String(mapDifficulty) + " is: " + String(hp));

  return hp;
}

void calculateScore() {
  //Serial.println("currentLifePoints: " + String(currentLifePoints) + " waveCounter: " + String(waveCounter) + " currentCoins: " + String(currentCoins));

  // write to global variable
  currentScore = uint16_t(waveCounter * 10) + currentLifePoints + currentCoins / 2;

  //Serial.println("is score: " + String(currentScore));
}

uint16_t getEnemyReward(uint8_t wave) {

  // calculate the reward for killing one enemy
  return getEnemyHp(wave, 10) / 150;
}

void drawBitmapSlow(int16_t x, int16_t y, const uint8_t *image, uint8_t w, uint8_t h, uint8_t offset, uint8_t rotation, uint8_t color) {

  // return because there is nothing to draw
  if (w <= 0 || h <= 0)
    return;

  // the offset is always multiplied by the width to get easier values
  uint16_t wOffset = w * offset;

  // calc how many full bytes are horizontally
  uint8_t xBytes = (w + 7) / 8;

  // this must be added for every byte I am drawing vertically down from top
  uint8_t xByteOffset = xBytes * w;

  for (uint8_t i = 0; i < w; i++) {

    for (uint8_t j = 0; j < h; j++) {

      uint8_t yOffset = j / 8;
      uint8_t bitOffset = j % 8;

      // continue to next for value if the bit is not set
      if (!(pgm_read_byte(image + xByteOffset * yOffset + i + wOffset) & (B00000001 << bitOffset)))
        continue;

      // do the rotation
      int8_t k, l;
      switch (rotation) {
        case NOROT: //no rotation
          k = i;
          l = j;
          break;
        case ROTCCW: //90° counter-clockwise
          k = j;
          l = w - i - 1;
          break;
        case ROT180: //180°
          k = w - i - 1;
          l = h - j - 1;
          break;
        case ROTCW: //90° clockwise
          k = h - j - 1;
          l = i;
          break;
      }

      // draw the turned pixels added to x and y coordinates
      arduboy.drawPixel(x + k, y + l, color);
    }
  }
}

void drawBitmapSlow(int16_t x, int16_t y, const uint8_t *image, uint8_t w, uint8_t h, uint8_t offset, uint8_t rotation) {
  drawBitmapSlow(x, y, image, w, h, offset, rotation, BLACK);
}


void drawBitmapFast(int16_t x, int16_t y, const uint8_t *data, uint8_t w, uint8_t offset, bool hFlip, uint8_t color) {

  // no need to draw if out of screen
  if ((y + 8 < 0 || y > HEIGHT) || (x + w  < 0 || x > WIDTH))
    return;

  // this is a fast modulo 8
  int8_t yBitOffset = y & 7;

  // the horizontal bytes line where to draw
  int16_t yPos = (y / 8) * WIDTH;

  // multiply with width before the for loop, it is only to get easier offsets values
  uint16_t offsetWidth = offset * w;

  for (uint16_t i = 0; i < w; i++) {

    // flip the direction
    uint16_t pos;
    if (hFlip) {
      pos = w - i - 1;
    } else {
      pos = i;
    }

    // get the byte to draw
    byte currentByte = pgm_read_byte(data + offsetWidth + i);

    int16_t xPos = x + pos;
    int16_t bytePos = xPos + yPos;

    if (bytePos >=  0  && bytePos < BUFFER_MAX &&
        xPos    >=  0  && xPos    < WIDTH ) {

      if (y >= 0) {
        // shift pixels left/down and draw the top of the byte
        if (color == BLACK) {
          arduboy.sBuffer[bytePos] &= ~(currentByte << yBitOffset);
        } else {
          arduboy.sBuffer[bytePos] |= (currentByte << yBitOffset);
        }

        // shift the pixels right/up and draw cut bitmap
        if (yBitOffset != 0 && bytePos < (BUFFER_MAX - WIDTH)) {
          if (color == BLACK) {
            arduboy.sBuffer[bytePos + WIDTH] &= ~(currentByte >> (8 - yBitOffset));
          } else {
            arduboy.sBuffer[bytePos + WIDTH] |= (currentByte >> (8 - yBitOffset));
          }
        }

      }  else {
        // shift the pixels right/up and draw cut bitmap at the top of the screen
        if (color == BLACK) {
          arduboy.sBuffer[bytePos] &= ~(currentByte >> (8 - yBitOffset));
        } else {
          arduboy.sBuffer[bytePos] |= (currentByte >> (8 - yBitOffset));
        }

      }
    }
  }
}

// comfort functions
void drawBitmapFast(int16_t x, int16_t y, const uint8_t *image, uint8_t w, uint8_t offset, bool hFlip) {
  drawBitmapFast(x, y, image, w, offset, hFlip, BLACK);
}

void drawBitmapFast(int16_t x, int16_t y, const uint8_t *image, uint8_t w, uint8_t offset) {
  drawBitmapFast(x, y, image, w, offset, false, BLACK);
}

void fillChessRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
  if (w + x > 128)
    return;

  if (h + y > 64)
    return;

  for (uint8_t i = x; i < w + x; i++) {
    for (uint8_t j = y; j < h + y; j++) {

      if (i >= 128)
        continue;

      if (j >= 64)
        continue;

      // to draw every second pixel
      if ((i % 2 && j % 2) || (!(i % 2) && !(j % 2)))
        arduboy.drawPixel(i, j, color);
    }
  }
}

void drawCursor() {

  // the cursor is only drawn while playing or in editor mode
  if (gameMode < MODE_PLAYING || gameMode > MODE_EDITOR_MENU)
    return;

  // get the pixel positions
  uint8_t xPos = xCursor * RASTER + RASTER_OFFSET_X;
  uint8_t yPos = yCursor * RASTER + RASTER_OFFSET_Y;

  // draw black and white cursor
  for (uint8_t i = 0; i < 2; i++) {
    uint8_t s = statCursor;
    if (i) {
      s = (s + 2) % 4;
    }

    uint8_t offset = s % 2;
    uint8_t rotation = s < 2;

    if (gameMode <= MODE_PLAYING_TOWER) {
      // draw 2x2 nodes cursor
      drawBitmapSlow(xPos + 1, yPos + 1, cursorFrame, 11, 11, offset, rotation, i);
    } else {
      // draw 1 node cursor
      drawBitmapSlow(xPos, yPos, symbolSet, ICON_WIDTH, ICON_HEIGHT, offset, rotation, i);
    }
  }

  // the cursor will not be animated in the side menu modes
  if (gameMode != MODE_PLAYING && gameMode != MODE_EDITOR)
    return;

  // do the increments only every default frame time
  if (isFastSpeedFrame)
    return;

  // increment status
  statCursor++;
  if (statCursor == 4)
    statCursor = 0;
}

void drawTowerSocket(uint8_t x, uint8_t y, uint8_t lvl) {
  // calculate how round the corner should be
  uint8_t socketCorner = 3 - lvl + uint8_t(lvl < 2);

  arduboy.drawRoundRect(x, y, 11, 11, socketCorner, BLACK);
}

void drawTowerWeapon(uint8_t x, uint8_t y, uint8_t type, uint8_t sektor, uint8_t lvl) {

  if (isRotatingTower(type)) {

    // TODO: draw 1/4 of those towers fast

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
    drawBitmapFast(x + 2, y + 2, allTowers, 7, offset);
  }
}

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

#ifndef DrawHelper_h
#define DrawHelper_h

//for extended bitmap function :
#define NOROT 0
#define ROTCCW 1
#define ROT180 2
#define ROTCW 3

void drawOffsetBitmap(int16_t x, int16_t y, const uint8_t *image, int16_t iW,
                      int16_t w, int16_t h, int16_t oX, uint8_t flip, uint16_t color);

/*
uint8_t getRotation(int8_t x, int8_t y) {

  int16_t deg = getDegree(x, y)
  
}
*/

int16_t getDegree(int8_t x, int8_t y) {
  if (x == 0 && y == 0)
    return -1;

  if (x == 0 && y > 0)
    return 90;

  if (x == 0 && y < 0)
    return 270;

  if (y == 0 && x > 0)
    return 0;

  if (y == 0 && x < 0)
    return 180;

  //Serial.print("getDegree from x:" + String(x) + " y:" + String(y));
  //uint32_t start = micros();

  static const float radToDegree = 180.0 / PI;
  int16_t deg = atanf(float(y) / x) * radToDegree;

  //uint32_t dur = micros() - start;
  //Serial.println(" atan(:" + String(deg) + ") took:" + String(dur));

  if (x < 0) {
    return 180 + deg;

  } else {
    if (deg < 0)
      deg += 360;

    return deg;
  }
}

void drawRaster() {

  for (uint8_t xR = 0; xR < 126 / RASTER; xR++) {
    uint8_t xPos = RASTER_X_OFFSET + xR * RASTER;

    for (uint8_t yR = 0; yR < 60 / RASTER; yR++) {
      uint8_t yPos = RASTER_Y_OFFSET + yR * RASTER;

      arduboy.drawPixel(xPos + 1, yPos, BLACK);
      arduboy.drawPixel(xPos + 5, yPos, BLACK);
      arduboy.drawPixel(xPos, yPos + 1, BLACK);
      arduboy.drawPixel(xPos, yPos + 5, BLACK);
    }
  }
}

void drawCursor() {
  uint8_t xPos = xCursor * RASTER + RASTER_X_OFFSET;
  uint8_t yPos = yCursor * RASTER + RASTER_Y_OFFSET;

  drawOffsetBitmap(xPos + 1, yPos + 1, cursorFrame, 44, 11, 11, statCursor, 0, INVERT);

  // increment cursor for the next draw
  statCursor++;
  if (statCursor == 4)
    statCursor = 0;
}

void drawTower(int8_t xR, int8_t yR, uint8_t tower, int16_t degree, uint8_t lev) {

  // get actual pixel positions from raster
  int8_t x = xR * 6 + RASTER_X_OFFSET + 1;
  int8_t y = yR * 6 + RASTER_Y_OFFSET + 1;

  // draw socket depending on level
  uint8_t levelCorner = 3 - lev + uint8_t(lev < 2);
  arduboy.drawRoundRect(x, y, 11, 11, levelCorner, BLACK);

  // clear raster mark, if in build menu
  if (true)
    arduboy.fillRect(x + 4, y + 4, 3, 3, WHITE);

  const uint8_t *bitmap;

  if (tower == TOWER_MG) {
    bitmap = towerMG;

  } else if (tower == TOWER_CANON) {
    bitmap = towerCanon;

  } else if (tower == TOWER_LASER) {
    bitmap = towerLaser;

  } else if (tower == TOWER_FLAME) {
    bitmap = towerFlame;

  } else {
    // draw one of the fixed towers
    drawOffsetBitmap(x + 2, y + 2, towerFix, 112, 7, 7, tower * 4 + lev, false, BLACK);

    return;
  }

  // create rotation value from degrees
  uint16_t rotation = (degree + 180) % 360; // because the sprites point to left

  // make rotation bigger to avoid using floats
  rotation *= 100;

  // if float I would have added 11.25
  rotation += 1125;

  // if float I would have used 22.5, multiply by 100 is than neutral
  rotation /= 225;

  uint8_t offset = lev % 4 + (rotation % 4) * 4;

  // divide again by 4 because sprite can only rotate every 90 degrees
  rotation /= 4;
  rotation %= 4;

  int8_t i, j, //coordinates in the raw bitmap
         k, l, //coordinates in the rotated/flipped bitmap
         byteNum, bitNum, byteWidth = (112 + 7) >> 3;

  //Serial.println(" degree:" + String(degree) + " rotation:" + String(rotation) + " offset:" + String(offset));

  // multiply by 7 because the sprite is 7 pixels long
  offset *= 7;

  uint8_t oW = 7 + offset;
  for (i = offset; i < oW; i++ ) {
    byteNum = i / 8;
    bitNum = i % 8;

    uint8_t iTmp = i - offset;

    for (j = 0; j < 7; j++) {
      if (!(pgm_read_byte(bitmap + j * byteWidth + byteNum) & (B10000000 >> bitNum))) {
        switch (rotation) {
          case NOROT: //no rotation
            k = iTmp;
            l = j;
            break;
          case ROTCCW: //90° counter-clockwise
            k = j;
            l = 7 - iTmp - 1;
            break;
          case ROT180: //180°
            k = 7 - iTmp - 1;
            l = 7 - j - 1;
            break;
          case ROTCW: //90° clockwise
            k = 7 - j - 1;
            l = iTmp;
            break;
        }

        k += x; //place the bitmap on the screen
        l += y;

        // it is plus 2 because the tower is centered in the socket
        arduboy.drawPixel(k + 2, l + 2, BLACK);
      }
    }
  }
}



void drawOffsetBitmap(int16_t x, int16_t y, const uint8_t *image, int16_t iW,
                      int16_t w, int16_t h, int16_t oX, uint8_t flip, uint16_t color) {

  // this would cause the for loop to count forever
  if (w + oX < oX)
    return;

  // the offset is always multiplied by the lenght
  oX *= w;

  int16_t xi, yi, byteWidth = (iW + 7) / 8;
  for (yi = 0; yi < h; yi++) {
    for (xi = oX; xi < w + oX; xi++ ) {
      if (pgm_read_byte(image + yi * byteWidth + xi / 8) & (128 >> (xi & 7)))
        continue;

      // flip == 0 no Flip
      // flip == 1 horizontal Flip
      // flip == 2 vertical Flip
      // flip == 3 vertical and horizontal Flip

      int16_t xPos, yPos;

      // calc xPos
      if (flip == 1 || flip == 3) {
        xPos = x + w - xi + oX - 1;
      } else {
        xPos = x + xi - oX;
      }

      // calc yPos
      if (flip == 2 || flip == 3) {
        yPos = y + h - yi - 1;
      } else {
        yPos = y + yi;
      }

      // add invert function to drawPixel
      if (color == INVERT)
        color = !arduboy.getPixel(xPos, yPos);

      arduboy.drawPixel(xPos, yPos, color);
    }
  }
};

/*
   this function draws only every second pixel
*/
void fillRectChess(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
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

      if ((i % 2 && j % 2) || (!(i % 2) && !(j % 2)))
        arduboy.drawPixel(i, j, color);
    }
  }
}

/*
   this function is to print the 3 special stages
*/

void drawSpecialStatus(uint8_t xPos, uint8_t yPos, uint8_t s, uint8_t color) {

  uint8_t g1 = uint8_t(s > 1);

  arduboy.drawRoundRect(xPos + 4 - s, yPos + 2 - g1, 2 * s, 2 + g1 * 2, g1, color);
}

/*
   functions to Print numbers without the arduboy print functionality
*/

// 'menuNumbers', 40x6px
const unsigned char menuNumbers [] PROGMEM = {
  0x9d, 0x91, 0x70, 0x90, 0x99, 0x69, 0x6e, 0x57, 0x7e, 0x66, 0x6d, 0xd9, 0x51, 0x0d, 0x96, 0x6d,
  0xbe, 0x0e, 0x6b, 0x68, 0x6d, 0x7e, 0xd6, 0x6b, 0x6e, 0x98, 0x01, 0xd9, 0x9b, 0x99
};

void drawNumber6x4(int8_t x, int8_t y, uint8_t v) {
  drawOffsetBitmap(x, y, menuNumbers, 40, 4, 6, v * 4, false, WHITE);
}

uint8_t drawNumbers(int8_t x, int8_t y, int16_t v) {
  uint8_t stellen;
  if (v < 10) {
    stellen = 1;
  } else if (v < 100) {
    stellen = 2;
  } else if (v < 1000) {
    stellen = 3;
  } else if (v < 10000) {
    stellen = 4;
  } else {
    stellen = 5;
  }

  uint8_t xOffset = x + (stellen - 1) * 5;

  for (uint8_t i = 0; i < stellen; i++) {

    drawNumber6x4(xOffset, y, v % 10);

    v = v / 10;
    xOffset -= 5;
  }

  return stellen;
}

#endif

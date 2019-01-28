#ifndef DrawHelper_h
#define DrawHelper_h

//for extended bitmap function :
#define NOROT 0
#define ROTCCW 1
#define ROT180 2
#define ROTCW 3

void drawBitmapSlow(int16_t x, int16_t y, const uint8_t *image, uint8_t w, uint8_t h, uint16_t offset, uint8_t rotation) {

  // return because there is nothing to draw
  if (w <= 0 || h <= 0)
    return;

  // the offset is always multiplied by the width to get easier values
  uint8_t wOffset = w * offset; 

  for (uint8_t i = 0; i < w; i++) {

    for (uint8_t j = 0; j < h; j++ ) {

      uint8_t yOffset = j / 8;
      uint8_t bitOffset = j % 8;

      // continue to next for value if the bit is not set
      if (!(pgm_read_byte(image + yOffset * w + i + wOffset) & (B00000001 << bitOffset)))
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
          l = 7 - i - 1;
          break;
        case ROT180: //180°
          k = 7 - i - 1;
          l = 7 - j - 1;
          break;
        case ROTCW: //90° clockwise
          k = 7 - j - 1;
          l = i;
          break;
      }

      // draw the turned pixels added to x and y coordinates
      arduboy.drawPixel(x + k, y + l, BLACK);
    }
  }
}

void drawBitmapFast(int16_t x, int16_t y, const uint8_t *data, uint8_t w, uint8_t h, uint16_t offset, bool horizontalFlip) {

  // no need to draw if out of screen
  if ((y + 8 < 0 || y > HEIGHT) || (x + w  < 0 || x > WIDTH))
    return;

  // this is a fast modulo 8
  int8_t yBitOffset = y & 7;

  // calc how many full bytes are verticaly
  uint8_t yBytes = (h - 1) / 8;

  // the horizontal bytes line where to draw
  int16_t yPos = (y / 8) * WIDTH;

  // multiply with width before the for loop, it is only to get easier offsets values
  uint8_t offsetWidth = offset * w;

  for (uint16_t i = 0; i < w; i++) {

    // flip the direction
    uint16_t pos;
    if (horizontalFlip) {
      pos = w - i - 1;
    } else {
      pos = i;
    }

    int8_t yByteOffset = yBytes * w;

    // get the byte to draw
    byte currentByte = pgm_read_byte(data + offsetWidth + i + yByteOffset);

    int16_t xPos = x + pos;
    int16_t bytePos = xPos + yPos + yByteOffset;

    if (bytePos >=  0  && bytePos < BUFFER_MAX &&
        xPos    >=  0  && xPos    < WIDTH ) {

      if (y >= 0) {
        // shift pixels left/down and draw the top of the byte
        arduboy.sBuffer[bytePos] &= ~(currentByte << yBitOffset);

        // check height

        // shift the pixels right/up and draw cut bitmap
        if (yBitOffset != 0 && bytePos < (BUFFER_MAX - WIDTH)) {
          arduboy.sBuffer[bytePos + WIDTH] &= ~(currentByte >> (8 - yBitOffset));
        }

      }  else {
        // shift the pixels right/up and draw cut bitmap at the top of the screen
        arduboy.sBuffer[bytePos] &= ~(currentByte >> (8 - yBitOffset));
      }
    }
  }
}

/*
void drawTower(int16_t x, int16_t y, const uint8_t *image, int16_t offset, uint8_t rotation) {

  // use fixed values for height, width and add a drawing offset of 2
  drawBitmapSlow(x + 2, y + 2, image, 7, 7, offset, rotation);
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

int8_t getSektor(int8_t x, int8_t y) {

  // multiply by 100 to aviod float and add offset 11.25°
  uint16_t deg = getDegree(x, y) * 100 + 1125;

  // calc section of 22.5° circle
  deg /= 2250;

  //Serial.print(" x:" + String(x) + " y:" + String(y));
  //Serial.println(" degree:" + String(getDegree(x, y)) + " ret:" + String(deg % 16));

  // to return number from 0 to 15 use mod
  return deg % 16;
}

void drawCursor() {
  uint8_t xPos = xCursor * RASTER + RASTER_OFFSET_X;
  uint8_t yPos = yCursor * RASTER + RASTER_OFFSET_Y;

  //drawBitmapSlow(xPos + 1, yPos + 1, cursorFrame, 44, 11, 11, statCursor, 0, INVERT);

  // increment cursor for the next draw
  statCursor++;
  if (statCursor == 4)
    statCursor = 0;
}

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
  drawBitmapSlow(x, y, menuNumbers, /*40,*/ 4, 6, v, false, BLACK);
}

uint8_t drawNumbers(int8_t x, int8_t y, int16_t v) {
  Serial.println("drawNumbers at x: " + String(x) + " y:" + String(y) + " value:" + String(v));

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

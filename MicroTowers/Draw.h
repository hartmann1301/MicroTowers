#ifndef DrawHelper_h
#define DrawHelper_h

//for extended bitmap function :
#define NOROT 0
#define ROTCCW 1
#define ROT180 2
#define ROTCW 3

/*
#define NOFLIP 0
#define FLIPH 1
#define FLIPV 2
#define FLIPVH 3
*/

/*
   these function are needed to print the images, thanks to gamebuino for the drawTowerBitmap function
*/

void drawTower(int8_t x, int8_t y, const uint8_t *bitmap, int16_t degree, uint8_t lev) {
  
  drawTowerBitmap(x, y, bitmap, 0, 0);
}

void drawTowerBitmap(int8_t x, int8_t y, const uint8_t *bitmap, uint8_t rotation, /* uint8_t flip, */ uint8_t offset) {

  offset *= 7;

  int8_t i, j, //coordinates in the raw bitmap
         k, l, //coordinates in the rotated/flipped bitmap
         byteNum, bitNum, byteWidth = (112 + 7) >> 3;

  rotation %= 4;

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
        /*
        if (flip) {
          flip %= 4;
          if (flip & B00000001) { //horizontal flip
            k = 7 - k - 1;
          }
          if (flip & B00000010) { //vertical flip
            l = 7 - l;
          }
        }
        */
        k += x; //place the bitmap on the screen
        l += y;

        arduboy.drawPixel(k, l, BLACK);
      }
    }
  }
}

void drawOffsetBitmap(int16_t x, int16_t y, const uint8_t *image, int16_t iW,
                      int16_t w, int16_t h, int16_t oX, uint8_t flip, uint16_t color) {

  // this would cause the for loop to count forever
  if (w + oX < oX)
    return;

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

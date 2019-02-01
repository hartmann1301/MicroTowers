#ifndef DrawHelper_h
#define DrawHelper_h

//for extended bitmap function :
#define NOROT 0
#define ROTCCW 1
#define ROT180 2
#define ROTCW 3

void drawBitmapSlow(int16_t x, int16_t y, const uint8_t *image, uint8_t w, uint8_t h, uint8_t offset, uint8_t rotation, uint8_t color) {

  // return because there is nothing to draw
  if (w <= 0 || h <= 0)
    return;

  // the offset is always multiplied by the width to get easier values
  uint16_t wOffset = w * offset;

  // calc how many full bytes are verticaly
  uint8_t yBytes = (h + 7) / 8;
  int8_t yByteOffset = yBytes * w;

  for (uint8_t i = 0; i < w; i++) {

    for (uint8_t j = 0; j < h; j++ ) {

      uint8_t yOffset = j / 8;
      uint8_t bitOffset = j % 8;

      // continue to next for value if the bit is not set
      if (!(pgm_read_byte(image + yByteOffset * yOffset + i + wOffset) & (B00000001 << bitOffset)))
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

// maybe this is not needed
void drawBitmapFast(int16_t x, int16_t y, const uint8_t *image, uint8_t w, uint8_t offset, bool hFlip) {
  drawBitmapFast(x, y, image, w, offset, hFlip, BLACK);
}


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

      // to draw every second pixel
      if ((i % 2 && j % 2) || (!(i % 2) && !(j % 2)))
        arduboy.drawPixel(i, j, color);
    }
  }
}

void drawCursor() {
  uint8_t xPos = xCursor * RASTER + RASTER_OFFSET_X;
  uint8_t yPos = yCursor * RASTER + RASTER_OFFSET_Y;

  for (uint8_t i = 0; i < 2; i++) {
    uint8_t s = statCursor;
    if (i) {
      s = (s + 2) % 4;
    }

    // draw black and white cursor
    drawBitmapSlow(xPos + 1, yPos + 1, cursorFrame, 11, 11, s % 2, uint8_t(s < 2), i);
  }

  statCursor++;
  if (statCursor == 4)
    statCursor = 0;
}

uint8_t indexTowerShop = 0;

void drawTowerShop() {

  uint8_t offset = 0;

  for (uint8_t i = 0; i < 8; i++) {

    drawBitmapFast(2, 2, allTowers, 7, offset, false);
    
  }

  
}



#endif

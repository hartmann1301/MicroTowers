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

// maybe this is not needed
void drawBitmapFast(int16_t x, int16_t y, const uint8_t *image, uint8_t w, uint8_t offset, bool hFlip) {
  drawBitmapFast(x, y, image, w, offset, hFlip, BLACK);
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

void drawLongPress(int8_t pressTime) {

  if (!isLongPressed)
    return;

  // to keep it on the screen
  if (pressTime > 61)
    pressTime = 61;

  arduboy.fillRect(0, 56, pressTime * 2, 1, BLACK);
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

void drawMapBorders() {
  // some fixed values just for better code reading
  const uint8_t xPos = 122;
  const uint8_t yEnd = 55;
  
  // draw left and bottom lines
  arduboy.drawLine(0, 0, 0, 24, BLACK);
  arduboy.drawLine(0, 30, 0, 54, BLACK);
  arduboy.drawLine(0, yEnd, xPos, yEnd, BLACK);

  if (xPosRightMenu > xPos + 2) {
    // draw complete line right to battlefield
    arduboy.drawLine(xPos, 0, xPos, yEnd, BLACK);

  } else {

    // with 8 tower there is no space for a right border
    if (gameMode == MODE_PLAYING_BUILD)
      return;

    uint8_t lineEnd, lineStart;
    if (inEditorMode(gameMode)) {
      // for the editor menu with 4 items
      lineEnd = 10;
      lineStart = 45;

    } else {
      // for the tower menu with 3 items
      lineEnd = 13;
      lineStart = 42;
    }

    // some line over and under the menu
    arduboy.drawLine(xPos, 0, xPos, lineEnd, BLACK);
    arduboy.drawLine(xPos, lineStart, xPos, yEnd, BLACK);
  }
}

#endif

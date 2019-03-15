#ifndef Draw_h
#define Draw_h

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

  // the cursor will not be animated in the editor side menu
  if (gameMode == MODE_EDITOR_MENU)
    return;

  // do the animation increments only every default frame time
  if (isFastSpeedFrame)
    return;

  // increment status
  statCursor++;
  if (statCursor == 4)
    statCursor = 0;
}

void drawEnemy(int8_t x, int8_t y, uint8_t type, uint8_t race, uint8_t state, bool isLookingLeft) {

  // default width is 6, can be changed later
  uint8_t w = 6;

  const uint8_t *img;
  if (race == ENEMY_RACE_CYBORG) {
    img = enemyCyborgs;

  } else if (race == ENEMY_RACE_TWOPOD) {
    img = enemyTwoPods;

  } else {
    img = enemyMonsters;

    // monsters need a bit more pixels
    w = 9;
  }

  //Serial.println("drawEnemy race:" + String(race) + " type:" + String(type) + " state:" + String(state));

  // draw the spirte fast
  drawBitmapFast(x, y, img, w, type * 3 + state, isLookingLeft);
}

void drawTowerSocket(uint8_t x, uint8_t y, uint8_t lvl) {
  // calculate how round the corner should be
  uint8_t socketCorner = 3 - lvl + uint8_t(lvl < 2);

  arduboy.drawRoundRect(x, y, 11, 11, socketCorner, BLACK);
}

void drawTowerWeapon(uint8_t x, uint8_t y, uint8_t type, uint8_t sektor, uint8_t lvl) {

  // get drawing starts
  int8_t xPos = x + 2;
  int8_t yPos = y + 2;

  
  //uint8_t offset = lvl % 4

  if (isRotatingTower(type)) {

    // set offset for correct level and fine rotation
    uint8_t offset = lvl % 4 + (sektor % 4) * 4;

    // set tower offset
    offset += type * 16;

    // TODO: try to draw this tower fast
    if (sektor >= 8 && sektor < 12) {

      // can be drawn fast
      drawBitmapFast(xPos, yPos, allTowers, ICON_WIDTH, offset);

    } else {
      // divide again by 4 because sprite can only rotate every 90 degrees
      uint8_t rotation = sektor / 4;

      // rotate 180° because the sprite looks to the left side and sektor 2 starts right
      rotation = (rotation + 2) % 4;

      //  Serial.println("drawSlow:" + String(type) + " offset " + String(offset) + " rotation " + String(rotation));
      drawBitmapSlow(xPos, yPos, allTowers, 7, 7, offset, rotation);
    }

  } else {
    // offset is a huge value, because the five other towers are on top
    uint8_t offset = lvl % 4 + (type - TOWER_SHOCK) * 4 + 16 * 6;

    // can be drawn fast
    drawBitmapFast(xPos, yPos, allTowers, ICON_WIDTH, offset);
  }
}

#endif

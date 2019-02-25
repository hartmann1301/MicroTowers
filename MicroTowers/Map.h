#ifndef Map_h
#define Map_h

#define PRINT_EDITOR_MAP

#ifdef ESP8266
#define DEBUG_PATH_MAP
//#define DEBUG_PATH_PRINT
#endif

struct pathList {
  static const uint8_t maximum = 20;
  uint8_t len = 0;
  uint8_t data[maximum];

  void add(uint8_t n) {
    // check for doubles
    for (uint8_t i = 0; i < len; i++) {
      if (data[i] == n)
        return;
    }

    data[len] = n;
    len++;
  }
};

struct mapMangager {

  int8_t pixelsToRasterX(int8_t x) {
    return (x - RASTER_OFFSET_X) / RASTER;

  }

  int8_t pixelsToRasterY(int8_t y) {
    return (y - RASTER_OFFSET_Y) / RASTER;
  }

  uint8_t getShifts(uint8_t node) {
    // depending on position in array it must be shiftes 2, 4 or 6 times
    return (3 - (node % 4)) * 2;
  }

  uint8_t getShiftedData(uint8_t node, uint8_t data) {
    // put data to the correct position in byte
    return data << getShifts(node);
  }

  uint8_t getCurrentMapNode(uint8_t node) {

    // because there are 4 byte in every pgm space map byte
    uint8_t index = node / 4;

    // get data from currently loaded map
    uint8_t rawData = mapComposition[index];

    uint8_t shifts = getShifts(node);

    // shift data to get the correct two bits
    return (rawData & (0b00000011 << shifts)) >> shifts;
  }

  uint8_t getCurrentMapNode(uint8_t xR, uint8_t yR) {
    return getCurrentMapNode(getIndex(xR, yR));
  }

  uint8_t getStoredMapNode(uint8_t node, int8_t mapNumber) {

    // because there are 4 byte in every pgm space map byte
    uint8_t index = node / 4;

    // read data for 4 bytes, from eeprom or program space
    uint8_t rawData;
    if (gameMode == MODE_MAPS_EDITOR || gameMode == MODE_EDITOR) {

      if (mapNumber == 0) {
        // read from global map array because map 0 was copied here from eeprom
        rawData = mapComposition[index];

      } else {
        // read from global costs array because map 1-4 were copied here from eeprom
        rawData = mapCosts[index + (mapNumber - 1) * NODES_COMPRESSED];
      }

    } else {
      // read data from huge pgm space map array
      rawData = pgm_read_byte(allMaps + index + mapNumber * NODES_COMPRESSED);

    }

    uint8_t shifts = getShifts(node);

    // shift data to get the correct two bits
    return (rawData & (0b00000011 << shifts)) >> shifts;
  }

  void setNode(uint8_t node, uint8_t data) {

    // holds the index
    uint8_t index = node / 4;

    //Serial.println("setNode " + String(node) + " data: " + String(data));

    // clear the 2 bits of this byte
    mapComposition[index] &= ~(0b00000011 << getShifts(node));

    // write only the 2 new bits
    mapComposition[index] |= getShiftedData(node, data);
  }

  void setNode(uint8_t xR, uint8_t yR, uint8_t type) {

    setNode(getIndex(xR, yR), type);
  }

  void set2x2Nodes(uint8_t towerIndex, uint8_t type) {
    //Serial.println("set2x2Nodes");

    // one tower uses 4 nodes
    setNode(towerIndex,            type);
    setNode(towerIndex + 1,        type);
    setNode(towerIndex + ROWS,     type);
    setNode(towerIndex + ROWS + 1, type);

    // to trigger new pathfinding
    mapChanged = true;
  }

  void set2x2Nodes(uint8_t xR, uint8_t yR, uint8_t type) {
    set2x2Nodes(getIndex(xR, yR), type);
  }

#ifdef DEBUG_PATH_MAP
  void printMap() {
    Serial.println("Map:");

    Serial.print("      ");
    for (uint8_t j = 0; j < ROWS; j++) {
      Serial.printf(" %02d ", j);
    }

    Serial.println();
    Serial.println();

    for (uint8_t i = 0; i < COLUMNS; i++) {

      Serial.printf("%02d", i);
      if (i == 4) {
        Serial.print("    ");
      } else {
        Serial.print(" |  ");
      }

      for (uint8_t j = 0; j < ROWS; j++) {
        uint8_t index = getIndex(j, i);

        switch (getCurrentMapNode(index)) {

          case MAP_FREE:
            Serial.printf(" %02d ", mapCosts[index]);
            break;

          case MAP_TOWER:
            if (index == headquarterPosition) {
              Serial.print("[HQ]");
            } else {
              Serial.print("[To]");
            }
            break;

          case MAP_ROCK:
            Serial.print("[__]");
            break;

          case MAP_NOBUILD:
            Serial.printf(".%02d.", mapCosts[index]);
            break;
        }
      }
      Serial.println("|");
    }

    Serial.println("done");
  }
#endif

  uint8_t getCurrentCost(int8_t x, int8_t y) {
    return getRasterCost(pixelsToRasterX(x), pixelsToRasterY(y));
  }

  uint8_t getDirection(int8_t x, int8_t y) {

    // if they are not even in the map it is simple
    if (x < 2)
      return GO_RIGHT;

    int8_t xR = pixelsToRasterX(x);
    int8_t yR = pixelsToRasterY(y);

    uint8_t currentCost = getRasterCost(xR, yR);

    //Serial.print("STEP: xR:" + String(xR) + " yR:" + String(yR));
    //Serial.println(" x:" + String(x) + " y:" + String(y) + " currentCost:" + String(currentCost));

    // use the modulo to go vertical
    uint8_t zeroOrOne = rand() % 2;
    for (uint8_t i = zeroOrOne; i < 2 + zeroOrOne; i++) {
      if (i % 2) {

        // check up
        if (getRasterCost(xR, yR - 1) < currentCost)
          return GO_UP;

        // check down
        if (getRasterCost(xR, yR + 1) < currentCost)
          return GO_DOWN;

      } else {
        // check right
        if (getRasterCost(xR + 1, yR) < currentCost)
          return GO_RIGHT;

        // check left
        if (getRasterCost(xR - 1, yR) < currentCost)
          return GO_LEFT;
      }
    }

    return NO_DIRECTION;
  }

  uint8_t getRasterCost(uint8_t xR, uint8_t yR) {

    // these ifs avoid the enemys leaving the grip
    if (xR < 0 || xR >= ROWS)
      return 0xff;

    if (yR < 0 || yR >= COLUMNS)
      return 0xff;

    return mapCosts[getIndex(xR, yR)];
  }

  void drawMap() {
    for (uint8_t yR = 0; yR < COLUMNS; yR++) {
      uint8_t yPos = RASTER_OFFSET_Y + yR * RASTER;

      for (uint8_t xR = 0; xR < ROWS; xR++) {
        uint8_t xPos = RASTER_OFFSET_X + xR * RASTER;

        // get index from xR and yR coordinates
        uint8_t index = getIndex(xR, yR);

        // get on of the four map states
        uint8_t currentMap = getCurrentMapNode(index);

        switch (currentMap) {

          case MAP_FREE:
            break;

          case MAP_TOWER:
            if (index == headquarterPosition) {
              drawBitmapFast(xPos + 1, yPos - 1, editorSymbole, 5, EDITOR_HQ, false);
            }
            break;

          case MAP_ROCK:
            drawBitmapFast(xPos + 1, yPos - 2, mapBlockades, 5, (xR + yR) % 4, yR % 2);
            break;

          case MAP_NOBUILD:
            drawBitmapFast(xPos + 1, yPos - 2, mapBlockades, 5, (xR + yR + xR % 3) % 6 + 4, yR % 2);
            break;
        }

        // no big house checks in the top and left row/column
        if (xR == 0 || yR == 0)
          continue;

        // get four map nodes to the top left direction
        uint8_t mapLeft    = getCurrentMapNode(getIndex(xR - 1, yR));
        uint8_t mapTop     = getCurrentMapNode(getIndex(xR, yR - 1));
        uint8_t mapLeftTop = getCurrentMapNode(getIndex(xR - 1, yR - 1));

        // check if they are all rocks
        if (currentMap == MAP_ROCK && mapLeft == MAP_ROCK && mapTop == MAP_ROCK && mapLeftTop == MAP_ROCK) {

          // get starting coordingates
          uint8_t xHouse = xPos - 5;
          uint8_t yHouse = yPos - 5;

          // clear map at this position
          arduboy.fillRect(xHouse, yHouse, 11, 11, WHITE);

          // draw variously one of two houses depending on index
          drawBitmapSlow(xHouse, yHouse, mapHouses, 11, 11, index % 2, 0, BLACK);
        }
      }
    }
  }

  void checkNeighbour(pathList & lNext, bool * lClosed, int16_t i) {
    // only valid nodes
    if (i < 0 || i >= NODES)
      return;

    if (lClosed[i] == true)
      return;

    uint8_t tmpData = getCurrentMapNode(i);

    if (tmpData == MAP_ROCK || tmpData == MAP_TOWER)
      return;

#ifdef DEBUG_PATH_PRINT
    Serial.println(" found neighbour:" + String(i));
    yield();
#endif
    lNext.add(i);
  }

  void drawMapPreview(uint8_t xOffset, int16_t yOffset, uint8_t mapNumber) {

    // draw frame around minimap
    arduboy.drawRect(xOffset, yOffset, 2 * ROWS + 2, 2 * COLUMNS + 2, BLACK);

    // check is this map is still locked
    if (mapNumber >= unlockedMaps) {
      // black background
      arduboy.fillRect(xOffset + 2, yOffset + 2, 2 * ROWS - 2, 2 * COLUMNS + 2, BLACK);

      // a lock symbol
      drawBitmapSlow(xOffset + 17, yOffset + 6, mapLocked, 8, 11, 0, 0, WHITE);
      //arduboy.drawBitmap(xOffset + 17, yOffset + 6, mapLocked, 8, 11, WHITE);
      return;
    }

    // open the frame on the left side
    arduboy.drawLine(xOffset, yOffset + 9, xOffset, yOffset + 11, WHITE);

    for (uint8_t yR = 0; yR < COLUMNS; yR++) {
      int16_t yPos = 1 + yOffset + yR * 2;

      for (uint8_t xR = 0; xR < ROWS; xR++) {
        uint8_t xPos = 1 + xOffset + xR * 2;

        uint8_t index = getIndex(xR, yR);

        uint16_t data = getStoredMapNode(index, mapNumber);

        switch (data) {
          case MAP_FREE:
            break;

          case MAP_TOWER:
            // this headquarter is too big for the 4 pixels per node, so it can be covered a bit
            arduboy.drawRoundRect(xPos - 1, yPos - 1, 4, 4, 1, BLACK);
            break;

          case MAP_ROCK:
            arduboy.fillRect(xPos, yPos, 2, 2, BLACK);
            break;

          case MAP_NOBUILD:
            arduboy.drawPixel(xPos, yPos + 1, BLACK);
            arduboy.drawPixel(xPos + 1, yPos, BLACK);
            break;
        }
      }
    }
  }

  bool findPath() {

    // reset this bool to find new path only once
    mapChanged = false;

    // they do not need to be global!
    pathList openList;
    pathList nextList;

    // TODO: find a bette way to solve this!!!!!!1
    bool closedList[NODES];

#ifdef DEBUG_PATH_MAP
    uint32_t startMap = micros();
#endif

    // init the mapCosts with number values
    for (uint8_t i = 0; i < NODES; i++) {
      mapCosts[i] = i;
      closedList[i] = false;
    }

    // set end position
    openList.add(headquarterPosition);

    uint8_t currentCost = 0;
    bool foundEntry = false;

    for (uint8_t i = 0; i < NODES / 2; i++) {

      // go to openList and set costs and neightbours
      for (int16_t i = 0; i < openList.len; i++) {

        // get current node from list
        uint8_t n = openList.data[i];

        // check if this is the headquarter
        if (n == ENTRY_POSITION)
          foundEntry = true;

#ifdef DEBUG_PATH_PRINT
        Serial.println("node:" + String(n) + " set Cost to:" + String(currentCost));
#endif
        // set costs and mark this node as closed
        mapCosts[n] = currentCost;
        closedList[n] = true;

        // check top and bottom neighbours
        checkNeighbour(nextList, closedList, n - ROWS);
        checkNeighbour(nextList, closedList, n + ROWS);

        // check top and bottom neighbours
        if (n % ROWS != 0)
          checkNeighbour(nextList, closedList, n - 1);

        if (n % ROWS != ROWS - 1)
          checkNeighbour(nextList, closedList, n + 1);
      }

      // add new neightbours to openList
      memcpy(&openList, &nextList, sizeof(pathList));

      if (openList.len == 0) {

        // delete the unused node costs/nums
        for (uint8_t i = 0; i < NODES; i++) {
          if (closedList[i] == false)
            mapCosts[i] = 0xff;
        }

#ifdef DEBUG_PATH_MAP
        Serial.println("Path took: " + String(micros() - startMap) + " found Entry:" + foundEntry);

        printMap();
#endif
        return foundEntry;
      }

      // clear the nextList
      nextList.len = 0;

      currentCost++;
    }

#ifdef DEBUG_PATH_MAP
    Serial.println("Map Error");

    printMap();
#endif

    // can not get here
    return false;
  }
};

#endif

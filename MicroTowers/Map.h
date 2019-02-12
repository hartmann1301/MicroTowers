#ifndef Map_h
#define Map_h

#define USE_MAP_BUILDING

#ifdef ESP8266
#define DEBUG_PATH_MAP
//#define DEBUG_PATH_PRINT
#endif

enum {
  MAP_FREE = 0,  
  MAP_TOWER,
  MAP_ROCK,     
  MAP_NOBUILD, 
};

struct list {
  static const uint8_t maximum = 12;
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

  uint8_t getIndex(uint8_t xR, uint8_t yR) {
    return xR + yR * ROWS;
  }

  int8_t getXR(int8_t x) {
    if (x <= RASTER_OFFSET_X) {
      return -1;
    } else {
      return (x - RASTER_OFFSET_X) / RASTER;
    }
  }

  int8_t getYR(int8_t y) {
    if (y <= RASTER_OFFSET_Y) {
      return -1;
    } else {
      return (y - RASTER_OFFSET_Y) / RASTER;
    }
  }

  void setNode(uint8_t index, uint8_t type) {
    //Serial.println("setNode at xR: " + String(xR) + " yR: " + String(yR) + " type: " + String(type));
    
    mapComposition[index] = type;
  }
  
  void setNode(uint8_t xR, uint8_t yR, uint8_t type) {

    setNode(getIndex(xR, yR), type);
  }

  uint8_t getNode(uint8_t index) {
    return mapComposition[index];
  }

  uint8_t getNode(uint8_t xR, uint8_t yR) {
    return getNode(getIndex(xR, yR));
  }

  void placeTower(uint8_t xR, uint8_t yR) {
    //Serial.println("placeTower");

    // one tower uses 4 nodes
    setNode(xR,     yR,     MAP_TOWER);
    setNode(xR,     yR + 1, MAP_TOWER);
    setNode(xR + 1, yR,     MAP_TOWER);
    setNode(xR + 1, yR + 1, MAP_TOWER);

    // to trigger new pathfinding
    mapChanged = true;
  }

#ifdef DEBUG_PATH_MAP
  void printMap() {
    Serial.println("Map:");

    Serial.print("    ");
    for (uint8_t j = 0; j < ROWS; j++) {
      Serial.printf(" %02d ", j);
    }

    Serial.println();
    Serial.println();

    for (uint8_t i = 0; i < COLUMNS; i++) {
      Serial.printf("%02d |", i);

      for (uint8_t j = 0; j < ROWS; j++) {
        uint8_t index = getIndex(j, i);

        switch (mapComposition[index]) {

          case MAP_FREE:
            if (mapCosts[index] != 0xff) {
              Serial.printf(" %02d ", mapCosts[index]);
            } else {
              Serial.print("    ");
            }
            break;

          case MAP_TOWER:
            Serial.print("[To]");
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

  uint8_t getDirection(int8_t x, int8_t y) {

    // if they are not even in they map it is simple
    if (x < 2)
      return GO_RIGHT;

    int8_t xR = getXR(x);
    int8_t yR = getXR(y);

    uint8_t currentCost = getCost(xR, yR);

    //Serial.print("STEP: xR:" + String(xR) + " yR:" + String(yR));
    //Serial.println(" x:" + String(x) + " y:" + String(y) + " currentCost:" + String(currentCost));

    // use the modulo to go vertical
    uint8_t zeroOrOne = rand() % 2;
    for (uint8_t i = zeroOrOne; i < 2 + zeroOrOne; i++) {
      if (i % 2) {

        // check up
        if (getCost(xR, yR - 1) < currentCost)
          return GO_UP;

        // check down
        if (getCost(xR, yR + 1) < currentCost)
          return GO_DOWN;

      } else {
        // check right
        if (getCost(xR + 1, yR) < currentCost)
          return GO_RIGHT;

        // check left
        if (getCost(xR - 1, yR) < currentCost)
          return GO_LEFT;
      }
    }

    return NO_DIRECTION;
  }

  uint8_t getCost(uint8_t xR, uint8_t yR) {

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

        uint8_t index = getIndex(xR, yR);

        switch (mapComposition[index]) {

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
      }
    }
  }

  void checkNeighbour(list &lNext, bool *lClosed, int16_t i) {
    // only valid nodes
    if (i < 0 || i >= NODES)
      return;

    if (lClosed[i] == true)
      return;

    if (mapComposition[i] == MAP_ROCK || mapComposition[i] == MAP_TOWER)
      return;

#ifdef DEBUG_PATH_PRINT
    Serial.println(" found neighbour:" + String(i));
#endif
    lNext.add(i);
  }

  void createMap() {

    setNode(4, 0, MAP_ROCK);

    setNode(10, 1, MAP_ROCK);
    setNode(10, 2, MAP_ROCK);
    setNode(10, 3, MAP_ROCK);
    setNode(10, 4, MAP_ROCK);
    setNode(10, 5, MAP_ROCK);
    setNode(10, 6, MAP_ROCK);
    setNode(10, 7, MAP_ROCK);
    setNode(10, 8, MAP_ROCK);

    setNode(7, 6, MAP_NOBUILD);
    setNode(7, 7, MAP_NOBUILD);
    setNode(8, 7, MAP_NOBUILD);
    setNode(6, 6, MAP_NOBUILD);
    setNode(6, 7, MAP_NOBUILD);
    setNode(6, 8, MAP_NOBUILD);
    setNode(5, 6, MAP_NOBUILD);
    setNode(5, 7, MAP_NOBUILD);
    setNode(5, 8, MAP_NOBUILD);
    setNode(4, 6, MAP_NOBUILD);
    setNode(4, 7, MAP_NOBUILD);
    setNode(4, 8, MAP_NOBUILD);

    setNode(15, 2, MAP_ROCK);
    setNode(15, 3, MAP_ROCK);
    setNode(15, 4, MAP_ROCK);
    setNode(15, 5, MAP_ROCK);

    setNode(12, 7, MAP_ROCK);
    setNode(13, 7, MAP_ROCK);

    setNode(18, 1, MAP_TOWER);
  }

  void drawMapPreview(uint8_t xOffset, int16_t yOffset, uint8_t mapNumber) {

    // draw frame around minimap
    arduboy.drawRect(xOffset, yOffset, 2 * ROWS + 2, 2 * COLUMNS + 2, BLACK);

  /*
      drawBitmapSlow(5, 5, mapLocked, 8, 11, 0, 0, BLACK);
      
      arduboy.drawBitmap(15, 5, mapLocked, 8, 11, BLACK);
  */

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

        uint16_t data = getMapNode(getIndex(xR, yR), mapNumber);

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

  uint16_t getMapNode(uint8_t node, uint8_t mapNumber) {

    // because there are 4 byte in every pgm space map byte
    uint8_t index = node / 4;

    // data for 4 bytes
    uint8_t rawData = pgm_read_byte(allMaps + index + mapNumber * 45);

    // must be shiftes 2, 4 or 6 times
    uint8_t shifts = (3 - (node % 4)) * 2;

    // shift data to get the correct two bits
    return (rawData & (0b00000011 << shifts)) >> shifts;
  }

  void loadMap(uint8_t mapNumber) {

    for (uint8_t n = 0; n < NODES; n++) {

      uint8_t data = getMapNode(n, mapNumber);

      // save position of headquarter
      if (data == MAP_TOWER) {
        Serial.println("headquarter is:" + String(n));
        headquarterPosition = n;
      }
      // write to the current map array
      mapComposition[n] = data;
    }
  }

#ifdef USE_MAP_BUILDING
  void generateMapArray() {

    // i use only 2 bits per map node
    const uint8_t COMPRESSED_NODES = NODES / 4;

    // create temporary array
    uint8_t mapData[COMPRESSED_NODES];

    for (uint8_t n = 0; n < NODES; n++) {

      // holds the index
      uint8_t index = n / 4;

      // shift data 2 bits left if it is no new byte
      if ((n % 4) != 0) {
        // get the current data of this byte
        uint8_t tmpData = mapData[index];

        // write it left shifted back
        mapData[index] = tmpData << 2;

      } else {

        // clear the byte before writing stuff
        mapData[index] = 0;
      }

      // add current note to byte
      mapData[index] |= mapComposition[n];
    }

    Serial.println("Generated following map bytes:");

    for (uint8_t n = 0; n < COMPRESSED_NODES; n++) {

      Serial.print("0x");

      if (mapData[n] <= 0xf)
        Serial.print("0");

      Serial.print(mapData[n], HEX);
      Serial.print(", ");

      if ((n + 1) % 16 == 0)
        Serial.println();
    }

    Serial.println();
  }
#endif

  bool findPath() {

    // reset this bool to find new path only once
    mapChanged = false;

    // they do not need to be global!
    list openList;
    list nextList;

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

    for (uint8_t i = 0; i < NODES / 2; i++) {

      // go to openList and set costs and neightbours
      for (int16_t i = 0; i < openList.len; i++) {

        uint8_t n = openList.data[i];

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
      memcpy(&openList, &nextList, sizeof(list));

      if (openList.len == 0) {

        // delete the unused node costs/nums
        for (uint8_t i = 0; i < NODES; i++) {
          if (closedList[i] == false)
            mapCosts[i] = 0xff;
        }

#ifdef DEBUG_PATH_MAP
        Serial.println("Path took: " + String(micros() - startMap));

        printMap();
#endif
        return true;
      }

      // clear the nextList
      nextList.len = 0;

      currentCost++;
    }

    // did not find a path
    return false;
  }
};

#endif

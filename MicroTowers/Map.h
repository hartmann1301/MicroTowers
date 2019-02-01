#ifndef Map_h
#define Map_h

#define COLUMNS  9
#define ROWS  20
#define NODES (ROWS*COLUMNS)

#define PATH_START  40


#define USE_MAP_BUILDING

#ifdef ESP8266
#define DEBUG_PATH_MAP
//#define DEBUG_PATH_PRINT

#endif

#define MAP_STYLE_ONE

enum {
  IS_FREE = 0,  // 00
  IS_TOWER,     // 01
  IS_ROCK,      // 10
  IS_NOBUILD,   // 11
};

// in this array the look and the behaviour of the current map is stored
uint8_t mapComposition[NODES];

// in this array the are the calculated costs from pathfinding
uint8_t mapCosts[NODES];

uint8_t headquarterPosition = 0;

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

          case IS_FREE:
            if (mapCosts[index] != 0xff) {
              Serial.printf(" %02d ", mapCosts[index]);
            } else {
              Serial.print("    ");
            }
            break;

          case IS_TOWER:
            Serial.print("[To]");
            break;

          case IS_ROCK:
            Serial.print("[__]");
            break;

          case IS_NOBUILD:
            Serial.printf(".%02d.", mapCosts[index]);
            break;
        }
      }
      Serial.println("|");
    }

    Serial.println("done");
  }
#endif

  void setNode(uint8_t xR, uint8_t yR, uint8_t type) {
    //Serial.println("setNode at xR: " + String(xR) + " yR: " + String(yR) + " type: " + String(type));

    mapComposition[getIndex(xR, yR)] = type;
  }

  void placeTower(uint8_t xR, uint8_t yR) {
    //Serial.println("placeTower");

    // one tower uses 4 nodes
    setNode(xR,     yR,     IS_TOWER);
    setNode(xR,     yR + 1, IS_TOWER);
    setNode(xR + 1, yR,     IS_TOWER);
    setNode(xR + 1, yR + 1, IS_TOWER);

    // to trigger new pathfinding
    mapChanged = true;
  }

  uint8_t getDirection(uint8_t x, uint8_t y) {

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

          case IS_FREE:
            if (drawRaster) {
#ifdef MAP_STYLE_ONE
              arduboy.drawPixel(xPos + 1, yPos, BLACK);
              arduboy.drawPixel(xPos + 5, yPos, BLACK);
              arduboy.drawPixel(xPos, yPos + 1, BLACK);
              arduboy.drawPixel(xPos, yPos + 5, BLACK);
#else
              arduboy.drawPixel(xPos, yPos, BLACK);
              arduboy.drawPixel(xPos, yPos + 5, BLACK);
              arduboy.drawPixel(xPos + 5, yPos, BLACK);
              arduboy.drawPixel(xPos + 5, yPos + 5, BLACK);
#endif
            }
            break;

          case IS_TOWER:

            if (index == headquarterPosition) {
              drawBitmapFast(xPos + 1, yPos - 1, symbolSet, 7, 0, false);
            }
            
            break;

          case IS_ROCK:
            drawBitmapFast(xPos + 1, yPos - 2, mapBlockades, 5, (xR + yR) % 4, yR % 2);
            break;

          case IS_NOBUILD:

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

    if (mapComposition[i] == IS_ROCK || mapComposition[i] == IS_TOWER)
      return;

#ifdef DEBUG_PATH_PRINT
    Serial.println(" found neighbour:" + String(i));
#endif
    lNext.add(i);
  }

  void createMap() {

      setNode(4, 0, IS_ROCK);

      setNode(10, 1, IS_ROCK);
      setNode(10, 2, IS_ROCK);
      setNode(10, 3, IS_ROCK);
      setNode(10, 4, IS_ROCK);
      setNode(10, 5, IS_ROCK);
      setNode(10, 6, IS_ROCK);
      setNode(10, 7, IS_ROCK);
      setNode(10, 8, IS_ROCK);

      setNode(7, 6, IS_NOBUILD);
      setNode(7, 7, IS_NOBUILD);
      setNode(8, 7, IS_NOBUILD);
      setNode(6, 6, IS_NOBUILD);
      setNode(6, 7, IS_NOBUILD);
      setNode(6, 8, IS_NOBUILD);
      setNode(5, 6, IS_NOBUILD);
      setNode(5, 7, IS_NOBUILD);
      setNode(5, 8, IS_NOBUILD);
      setNode(4, 6, IS_NOBUILD);
      setNode(4, 7, IS_NOBUILD);
      setNode(4, 8, IS_NOBUILD);

      setNode(15, 2, IS_ROCK);
      setNode(15, 3, IS_ROCK);
      setNode(15, 4, IS_ROCK);
      setNode(15, 5, IS_ROCK);

      setNode(12, 7, IS_ROCK);
      setNode(13, 7, IS_ROCK);

      setNode(19, 1, IS_TOWER);
  }

  void loadMap(uint16_t mapNumber) {

    for (uint8_t n = 0; n < NODES; n++) {

      // because there are 4 byte in every pgm space map byte
      uint8_t index = n / 4;

      // data for 4 bytes
      uint8_t rawData = pgm_read_byte(allMaps + index + mapNumber * 45);

      // must be shiftes 2, 4 or 6 times
      uint8_t shifts = (3 - (n % 4)) * 2;

      // shift data to get the correct two bits
      uint8_t data = (rawData & (0b00000011 << shifts)) >> shifts;
      
      // save position of headquarter
      if (data == IS_TOWER) {
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

      if (/*n == PATH_START*/  openList.len == 0) {

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

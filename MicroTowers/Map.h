#ifndef Map_h
#define Map_h

#define NODES 210
#define COLUMNS  10
#define ROWS  (NODES/COLUMNS)

#define PATH_START  42
#define PATH_END  62

#define HALF_RASTER (RASTER / 2)

#define DEBUG_PATH_MAP
//#define DEBUG_PATH_PRINT

#define MAP_STYLE_ONE

enum {
  IS_FREE = 0,
  IS_TOWER,
  IS_ROCK,
  IS_BLOCKED
};

struct node {
  // those are for the pathfinding
  uint8_t cost = 0xff;
  bool isClosed = false;

  // those are to descripe the map
  bool isOccupied = false;
  bool isBlocked = false;

  /*            isOccupied  isBlocked
    free place      false       false
    tower           true        false
    massive rock    true        true
    no build        false       true
  */
};

struct list {
  static const uint8_t maximum = 16;
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

list openList;
list nextList;

node nodeList[NODES];

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

  void setNode(uint8_t xR, uint8_t yR, uint8_t type) {
    //Serial.println("setNode at xR: " + String(xR) + " yR: " + String(yR) + " type: " + String(type));

    uint8_t index = getIndex(xR, yR);

    if (type == IS_FREE) {
      nodeList[index].isOccupied = false;
      nodeList[index].isBlocked = false;

    } else if (type == IS_TOWER) {
      nodeList[index].isOccupied = true;
      nodeList[index].isBlocked = false;

    } else if (type == IS_ROCK) {
      nodeList[index].isOccupied = true;
      nodeList[index].isBlocked = true;

    } else {
      nodeList[index].isOccupied = false;
      nodeList[index].isBlocked = true;

    }
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

    Serial.println("error");
    return 0xff;
  }

  uint8_t getCost(uint8_t xR, uint8_t yR) {

    // these ifs avoid the enemys leaving the grip
    if (xR < 0 || xR >= ROWS)
      return 0xff;

    if (yR < 0 || yR >= COLUMNS)
      return 0xff;

    return nodeList[getIndex(xR, yR)].cost;
  }

  void drawMap() {
    for (uint8_t yR = 0; yR < COLUMNS; yR++) {
      uint8_t yPos = RASTER_OFFSET_Y + yR * RASTER;

      for (uint8_t xR = 0; xR < ROWS; xR++) {
        uint8_t xPos = RASTER_OFFSET_X + xR * RASTER;

        uint8_t index = getIndex(xR, yR);

        if (nodeList[index].isOccupied) {

          // draw a massive thing
          if (nodeList[index].isBlocked) {
            arduboy.drawRect(xPos + 1, yPos + 1, 5, 5, BLACK);
          }

        } else {

          // draw the raster for better orientation
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

          // draw a few pixels to show no building areo
          if (nodeList[index].isBlocked) {
            arduboy.drawPixel(xPos + 2, yPos + 4, BLACK);
            arduboy.drawPixel(xPos + 3, yPos + 3, BLACK);
            arduboy.drawPixel(xPos + 4, yPos + 4, BLACK);
          }
        }

      }
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
        uint8_t index = j + i * ROWS;

        if (!nodeList[index].isOccupied && !nodeList[index].isBlocked) {
          if (nodeList[index].cost != 0xff) {
            Serial.printf(" %02d ", nodeList[index].cost);
          } else {
            Serial.print("    ");
          }
        } else if (!nodeList[index].isOccupied && !nodeList[index].isBlocked) {
          Serial.print("[To]");
        } else if (nodeList[index].cost == 0xff) {
          Serial.print("[__]");
        } else {
          Serial.printf(".%02d.", nodeList[index].cost);
        }

      }
      Serial.println("|");
    }

    Serial.println("done");
  }
#endif

  void checkNeighbour(int16_t i) {
    // only valid nodes
    if (i < 0 || i >= NODES)
      return;

    if (nodeList[i].isClosed)
      return;

    if (nodeList[i].isOccupied)
      return;

#ifdef DEBUG_PATH_PRINT
    Serial.println(" found neighbour:" + String(i));
#endif
    nextList.add(i);
  }

  void setMapData() {

    setNode(4, 0, IS_ROCK);

    setNode(10, 1, IS_ROCK);
    setNode(10, 2, IS_ROCK);
    setNode(10, 3, IS_ROCK);
    setNode(10, 4, IS_ROCK);
    setNode(10, 5, IS_ROCK);
    setNode(10, 6, IS_ROCK);
    setNode(10, 7, IS_ROCK);
    setNode(10, 8, IS_ROCK);
    setNode(10, 9, IS_ROCK);

    setNode(7, 6, IS_BLOCKED);
    setNode(7, 7, IS_BLOCKED);
    setNode(8, 7, IS_BLOCKED);

    setNode(15, 2, IS_ROCK);
    setNode(15, 3, IS_ROCK);
    setNode(15, 4, IS_ROCK);
    setNode(15, 5, IS_ROCK);
    setNode(15, 6, IS_ROCK);

    setNode(12, 8, IS_ROCK);
    setNode(13, 8, IS_ROCK);
  }

  bool findPath() {

    setMapData();

#ifdef DEBUG_PATH_MAP
    uint32_t startMap = micros();
#endif

    // init the cost with number values
    for (uint8_t i = 0; i < NODES; i++) {
      nodeList[i].cost = i;
      nodeList[i].isClosed = false;
    }

    // set end position
    openList.add(PATH_END);

    uint8_t currentCost = 0;

    for (uint8_t i = 0; i < NODES / 2; i++) {

      // go to openList and set costs and neightbours
      for (int16_t i = 0; i < openList.len; i++) {

        uint8_t n = openList.data[i];

#ifdef DEBUG_PATH_PRINT
        Serial.println("node:" + String(n) + " set Cost to:" + String(currentCost));
#endif
        // set costs and mark this node as closed
        nodeList[n].cost = currentCost;
        nodeList[n].isClosed = true;

        if (n == PATH_START) {

          // delete the unused node costs/nums
          for (uint8_t i = 0; i < NODES; i++) {
            if (nodeList[i].isClosed == false)
              nodeList[i].cost = 0xff;
          }

#ifdef DEBUG_PATH_MAP
          Serial.println("Path took: " + String(micros() - startMap));
#endif
          return true;
        }

        // check top and bottom neighbours
        checkNeighbour(n - ROWS);
        checkNeighbour(n + ROWS);

        // check top and bottom neighbours
        if (n % ROWS != 0)
          checkNeighbour(n - 1);

        if (n % ROWS != ROWS - 1)
          checkNeighbour(n + 1);
      }

      // add new neightbours to openList
      memcpy(&openList, &nextList, sizeof(list));

      // clear the nextList
      nextList.len = 0;

      currentCost++;
    }

    // did not find a path
    return false;
  }
};

#endif

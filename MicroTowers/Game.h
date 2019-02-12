#ifndef Gameplay_h
#define Gameplay_h

void tryToBuildTower() {
  // put tower to map
  tM.add(xCursor, yCursor, indexBuildMenu);  
}

void buyTower(uint8_t xR, uint8_t yR, uint8_t type) {
  
  tM.add(xR, yR, type);
}

#endif

#ifndef GameMenus_h
#define GameMenus_h

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

void drawMapInfos(uint8_t num) {

  mF.setCursor(5, 24);

  if (gameMode == MODE_MAPS_CAMPAIN) {
    mF.print(F("LEVEL"));

  } else {
    mF.print(F("SLOT"));
  }

  mF.print(num);  

  mF.setCursor(5, 34);
  mF.print(F("SCR:"));
  mF.print(num * 71 + 13);
}

void drawCenterMapHighlighter() {

  const uint8_t xPosStart = 72;
  const uint8_t xPosEnd = xPosStart + 7;

  const uint8_t yEntryStart = 30;
  const uint8_t yEntryEnd = yEntryStart + 4;

  arduboy.drawLine(xPosStart, 0, xPosStart, yEntryStart, BLACK);
  arduboy.drawLine(xPosStart, yEntryEnd, xPosStart, HEIGHT, BLACK);

  arduboy.drawLine(xPosStart, yEntryStart, xPosEnd, yEntryStart, BLACK);
  arduboy.drawLine(xPosStart, yEntryEnd, xPosEnd, yEntryEnd, BLACK);
}


void drawMapsListEditor() {
  // create a bigger index value for scrolling
  const uint8_t mapHeight = 23;
  int16_t indexInPixels = indexMapsEditor * mapHeight;
  static int16_t indexMapsEditorDelayed = 0;

  // approach argument 1 to argument 2
  scrollIndex(indexMapsEditorDelayed, indexInPixels);

  // draw all the content on left side, score usw.
  drawMapInfos(indexMapsEditor);

  // draw little frame thing to highlight the center map
  drawCenterMapHighlighter();

  // try to draw all of the menu items if they are on the screen or not
  for (int8_t i = 0; i < EDITOR_MAP_SLOTS; i++) {

    // calculate vertical offset
    int16_t yOffset = i * mapHeight + mapHeight - 1 - indexMapsEditorDelayed;

    // no need for drawing
    if (yOffset < -mapHeight || yOffset > HEIGHT)
      continue;

    mM.drawMapPreview(80, yOffset, i);
  }
}


void drawMapsListCampain() {
  // create a bigger index value for scrolling
  const uint8_t mapHeight = 23;
  int16_t indexInPixels = indexMapsCampain * mapHeight;
  static int16_t indexMapsCampainDelayed = 0;

  // approach argument 1 to argument 2
  scrollIndex(indexMapsCampainDelayed, indexInPixels);

  // draw all the content on left side, score usw.
  drawMapInfos(indexMapsCampain);

  // draw little frame thing to highlight the center map
  drawCenterMapHighlighter();

  // try to draw all of the menu items if they are on the screen or not
  for (int8_t i = 0; i < MAPS_IN_CAMPAIN; i++) {

    // calculate vertical offset
    int16_t yOffset = i * mapHeight + mapHeight - 1 - indexMapsCampainDelayed;

    // no need for drawing
    if (yOffset < -mapHeight || yOffset > HEIGHT)
      continue;

    mM.drawMapPreview(80, yOffset, i);
  }
}

uint8_t getDrawingxPosRightMenu(uint8_t xPosRightMenu, bool isActiveIndex) {
  if (isActiveIndex) {
    return xPosRightMenu - 1;
  } else {
    return xPosRightMenu;
  }
}

uint8_t drawCircleIfActive(uint8_t xDraw, uint8_t yPos, bool isActiveIndex) {

  if (isActiveIndex) {
    // clear the space
    arduboy.fillRect(xDraw - 1, yPos + 3, 9, 7, WHITE);

    drawTowerSocket(xDraw - 2, yPos + 1, 0);

    // set yPos for icon
    yPos += 3;
  }
  return yPos;
}

bool shiftMenuOut() {

  // shift menu outside screen
  if (xPosRightMenu < 132)
    xPosRightMenu++;
}


bool shiftMenuIn() {

  // shift menu inside screen
  if (xPosRightMenu > 121)
    xPosRightMenu--;

  // return true to not draw, no need to it will be outside screen
  if (xPosRightMenu > 127) {
    return true;
  } else {
    return false;
  }
}

void setVeritcalOffset(uint8_t &yPos, bool isActive) {

  // add some because active socket need more space
  if (isActive) {
    yPos += 3;
  }

  // set y position for next icon
  yPos += ICON_HEIGHT;
}

void drawPlayingTowerMenu() {

  // this will do the shifting and return true if outside display
  if (shiftMenuIn())
    return;

  uint8_t yPos = 14;
  for (uint8_t i = 0; i < MENU_ITEMS_TOWER; i++) {

    bool isActiveIndex = (i == indexTowerMenu);

    // the active index if a bit more in the screen
    uint8_t xDraw = getDrawingxPosRightMenu(xPosRightMenu, isActiveIndex);

    yPos = drawCircleIfActive(xDraw, yPos, isActiveIndex);

    // draw icon of editor item, xDraw + 1 because are only 5 width
    drawBitmapFast(xDraw, yPos, symbolSet, ICON_WIDTH, i + SYMBOL_UPGRADE, false);

    // adds more if current index is active
    setVeritcalOffset(yPos, isActiveIndex);
  }

  // get list index of currently selected tower
  uint8_t towerIndex = tM.getTowerAt(xCursor, yCursor);

  // draw the range of the current Tower
  tM.list[towerIndex].drawRange();
}


void drawPlayingBuildMenu() {

  // this will do the shifting and return true if outside display
  if (shiftMenuIn())
    return;

  uint8_t yPos = 1;
  for (uint8_t i = 0; i < MENU_ITEMS_BUILD; i++) {

    bool isActiveIndex = (i == indexBuildMenu);

    // the active index if a bit more in the screen
    uint8_t xDraw = getDrawingxPosRightMenu(xPosRightMenu, isActiveIndex);

    // draw socket and add y offset for icon if active
    yPos = drawCircleIfActive(xDraw, yPos, isActiveIndex);

    // get offset for differnet towers level 1
    uint8_t offset = 0;
    if (i < TOWER_SHOCK) {
      offset = i * 16;
    } else {
      offset = TOWER_SHOCK * 16 + (i - TOWER_SHOCK) * 4;
    }

    // draw icon of tower
    drawBitmapFast(xDraw, yPos, allTowers, ICON_WIDTH, offset, false);

    // adds more if current index is active
    setVeritcalOffset(yPos, isActiveIndex);
  }
}


void drawEditorMenu() {

  // this will do the shifting and return true if outside display
  if (shiftMenuIn())
    return;

  uint8_t yPos = 11;
  for (uint8_t i = 0; i < MENU_ITEMS_EDITOR; i++) {

    bool isActiveIndex = (i == indexBuildMenu);

    // the active index if a bit more in the screen
    uint8_t xDraw = getDrawingxPosRightMenu(xPosRightMenu, isActiveIndex);

    yPos = drawCircleIfActive(xDraw, yPos, isActiveIndex);

    // draw icon of editor item, xDraw + 1 because are only 5 width
    drawBitmapFast(xDraw + 1, yPos, editorSymbole, 5, i, false);

    // adds more if current index is active
    setVeritcalOffset(yPos, isActiveIndex);
  }
}


void drawOptionsMenu() {

  mF.setCursor(30, 30);
  mF.print(F("OPTIONS"));
}

void drawCredits() {

  mF.setCursor(30, 30);
  mF.print(F("CREDITS"));
}

#endif

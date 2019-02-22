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

void drawMapInfos(uint8_t lvl) {

  mF.setCursor(5, 24);

  if (lvl == 0) {
    mF.print(F("EDITOR"));

  } else {
    mF.print(F("LEVEL"));
    mF.print(lvl);
  }

  mF.setCursor(5, 34);
  mF.print(F("SCR:"));
  mF.print(lvl * 71 + 13);
}

void drawMapsList() {
  // create a bigger index value for scrolling
  const uint8_t mapHeight = 23;
  int16_t indexInPixels = indexMapsList * mapHeight;

  // approach argument 1 to argument 2
  scrollIndex(indexMapsListDelayed, indexInPixels);

  // draw all the content on left side, score usw.
  drawMapInfos(indexMapsList);

  // try to draw all of the menu items if they are on the screen or not
  for (int8_t i = 0; i < MAPSLIST_ITEMS; i++) {

    // calculate vertical offset
    int16_t yOffset = i * mapHeight + mapHeight - 1 - indexMapsListDelayed;

    // no need for drawing
    if (yOffset < -mapHeight || yOffset > HEIGHT)
      continue;

    mM.drawMapPreview(72, yOffset, i);
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

void drawMenus() {

  if (gameMode != MODE_MAPS_LIST) {

    // the bottom line with all the hints
    drawInfoLine();

    // function will decide if it needs to be cut for menus
    drawMapBorders();
  }

  if (gameMode == MODE_MAINMENU) {
    drawMainMenu();

  } else if (gameMode == MODE_MAPS_LIST) {
    drawMapsList();

  } else if (inPlayingMode(gameMode)) {

    if (gameMode == MODE_PLAYING) {
      shiftMenuOut();

    } else if (gameMode == MODE_PLAYING_BUILD) {
      drawPlayingBuildMenu();

    } else if (gameMode == MODE_PLAYING_TOWER) {
      drawPlayingTowerMenu();

    } else if (gameMode == MODE_PLAYING_INFO) {
      //TODO:
    }

  } else if (inEditorMode(gameMode)) {

    if (gameMode == MODE_EDITOR) {
      shiftMenuOut();

    } else if (gameMode == MODE_EDITOR_MENU) {
      drawEditorMenu();
    }

  } else if (gameMode == MODE_OPTIONS) {
    drawOptionsMenu();

  } else if (gameMode == MODE_CREDITS) {
    drawCredits();
  }

  // must be drawed at least because of the white parts
  drawCursor();
}


#endif

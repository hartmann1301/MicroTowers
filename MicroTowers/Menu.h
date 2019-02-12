#ifndef GameMenus_h
#define GameMenus_h

void drawMapInfos(uint8_t lvl) {

  mF.setCursor(5, 24);

  if (lvl == 0) {
    mF.print(F("EDITOR"));

  } else {
    mF.print(F("LEVEL"));
    mF.print(String(lvl));
  }

  mF.setCursor(5, 34);
  mF.print(F("SCR:"));
  mF.print(String(lvl * 71 + 13));
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

uint8_t getDrawingxPosMenu(uint8_t xPosMenu, bool isActiveIndex) {
  if (isActiveIndex) {
    return xPosMenu - 1;
  } else {
    return xPosMenu;
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

bool shiftMenuInIf(uint8_t m) {
  if (gameMode == m) {
    // shift shop inside screen
    if (xPosMenu > 121)
      xPosMenu--;

  } else {
    // shift shop outside screen
    if (xPosMenu < 132)
      xPosMenu++;
  }

  // return true to not draw, no need to it will be outside screen
  if (xPosMenu > 127) {
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


}

void drawPlayingBuildMenu() {

  // this will do the shifting and return true if outside display
  if (shiftMenuInIf(MODE_PLAYING_BUILD))
    return;

  uint8_t yPos = 1;
  for (uint8_t i = 0; i < MENU_ITEMS_PLAYING; i++) {

    // get offset for differnet towers level 1
    uint8_t offset = 0;
    if (i < TOWER_SHOCK) {
      offset = i * 16;
    } else {
      offset = TOWER_SHOCK * 16 + (i - TOWER_SHOCK) * 4;
    }

    bool isActiveIndex = (i == indexBuildMenu);

    // the active index if a bit more in the screen
    uint8_t xDraw = getDrawingxPosMenu(xPosMenu, isActiveIndex);

    // draw socket and add y offset for icon if active
    yPos = drawCircleIfActive(xDraw, yPos, isActiveIndex);

    // draw icon of tower
    drawBitmapFast(xDraw, yPos, allTowers, ICON_WIDTH, offset, false);

    // adds more if current index is active
    setVeritcalOffset(yPos, isActiveIndex);
  }
}

void drawEditorMenu() {

  // this will do the shifting and return true if outside display
  if (shiftMenuInIf(MODE_EDITOR_MENU))
    return;

  uint8_t yPos = 11;
  for (uint8_t i = 0; i < MENU_ITEMS_EDITOR; i++) {

    bool isActiveIndex = (i == indexBuildMenu);

    // the active index if a bit more in the screen
    uint8_t xDraw = getDrawingxPosMenu(xPosMenu, isActiveIndex);

    yPos = drawCircleIfActive(xDraw, yPos, isActiveIndex);

    // draw icon of tower
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

void updateGame() {

  tM.update();
  eM.update();

  if (mapChanged) {
    mM.findPath();
  }
}

void drawMenus() {

  if (gameMode != MODE_MAPS_LIST) {

    updateGame();

    mM.drawMap();

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

    // they need to be called here to shift the menu back
    drawPlayingBuildMenu();
    drawPlayingTowerMenu();

    if (gameMode == MODE_PLAYING_INFO) {
      //TODO:
    }

  } else if (inEditorMode(gameMode)) {

    // need to be called here to shift the menu back
    drawEditorMenu();

  } else if (gameMode == MODE_OPTIONS) {
    drawOptionsMenu();

  } else if (gameMode == MODE_CREDITS) {
    drawCredits();
  }

  // must be drawed at least because of the white parts
  drawCursor();
}


#endif

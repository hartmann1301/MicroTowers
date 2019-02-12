#ifndef InfoLine_h
#define InfoLine_h

const uint8_t yIcon = 57;
const uint8_t yText = yIcon + 1;

const uint8_t xPosLeft    = 1;
const uint8_t xPosCenter  = 64;

void scrollIndex(int16_t &indexInPixels, int16_t pixelValue) {

  // calculate the diff of the current scroll and the index
  int16_t delayDiff = indexInPixels - pixelValue;

  // if there is a diff scroll so many pixels
  uint8_t scollPixels = max(abs(delayDiff) / 4, 1);

  // create scrolling effect with the delayed value
  if (delayDiff < 0) {
    indexInPixels += scollPixels;

  } else if (delayDiff > 0) {
    indexInPixels -= scollPixels;
  }
}

void drawMainMenuText(int16_t xWrite, uint8_t index) {

  if (xWrite < INT8_MIN || xWrite > INT8_MAX)
    return;

  mF.setCursor(xWrite, yText);

  switch (index) {
    case 0:
      mF.print(F("  PLAY"));
      break;
    case 1:
      mF.print(F("EDITOR"));
      break;
    case 2:
      mF.print(F("OPTIONS"));
      break;
    case 3:
      mF.print(F("CREDITS"));
      break;
  }
}

void drawMainMenu() {
  // create a bigger index value for scrolling
  int16_t indexInPixels = indexMainMenu * 16;

  // approach argument 1 to argument 2
  scrollIndex(indexMainMenuDelayed, indexInPixels);

  // draw only if scrolling is over
  if (indexMainMenuDelayed == indexInPixels)  {
    // draw hints
    if (indexMainMenu > 0) {
      mF.setCursor(22, yText);
      mF.print(F("<"));
    }

    if (indexMainMenu < MAINMENU_ITEMS - 1) {
      mF.setCursor(84, yText);
      mF.print(F(">"));
    }
  }

  for (int8_t i = 0; i < MAINMENU_ITEMS; i++) {
    // try to draw all of the menu items if they are on the screen or not
    drawMainMenuText(i * 64 + 32 - indexMainMenuDelayed * 4,  i);
  }
}

void drawInfosEditor() {

  mF.setCursor(xPosLeft, yText);

  if (!isLongPressInfo(stateButtonB)) {

    mF.print(F("EDITOR"));

    // explains what these editor symbols mean
    mF.setCursor(xPosCenter, yText);

    // draw icon because side menu disappeared
    if (gameMode == MODE_EDITOR) {

      switch (indexBuildMenu) {
        case EDITOR_HQ:
          mF.print(F("THE HQ"));
          break;
        case EDITOR_ROCK:
          mF.print(F("A ROCK"));
          break;
        case EDITOR_TREE:
          mF.print(F("FOREST"));
          break;
        case EDITOR_DELETE:
          mF.print(F("DELETE"));
          break;
      }

      drawBitmapFast(121, yIcon, editorSymbole, 5, indexBuildMenu, false);

    } else {
      mF.print(F("CHOSE TYP"));
    }

  } else {
    mF.print(F("QUIT EDITOR?"));
  }
}

void drawInfosOptionsCredits() {

  mF.setCursor(xPosLeft, yText);
  mF.print(F("PRESS B TO EXIT"));
}

void drawCoins(uint8_t xPos, uint8_t coins) {

  // draw coin icon
  drawBitmapFast(xPos, yIcon, symbolSet, ICON_HEIGHT, SYMBOL_COIN, false);

  // draw current coins
  mF.setCursor(xPos + 8, yText);
  mF.print(String(coins));

}

void drawInfosPlaying() {

  // draw heart icon
  drawBitmapFast(45, yIcon, symbolSet, ICON_HEIGHT, SYMBOL_HEART, false);

  // draw current life
  mF.setCursor(53, yText);
  mF.print(String(79));

  // TODO draw wave icon



  // show that you in fast mode right now
  if (!isNormalSpeed)
    drawBitmapFast(110, yIcon, symbolSet, ICON_HEIGHT, SYMBOL_FASTMODE, false);
}

void drawInfosPlayingMenu() {

  mF.setCursor(30, yText);

  if (gameMode == MODE_PLAYING_BUILD) {

    switch (indexBuildMenu) {
      case TOWER_GATLING:
        mF.print(F("MINIGUN"));
        break;
      case TOWER_CANNON:
        mF.print(F("CANNON"));
        break;
      case TOWER_LASER:
        mF.print(F("LASER"));
        break;
      case TOWER_FLAME:
        mF.print(F("FLAME"));
        break;
      case TOWER_RAILGUN:
        mF.print(F("RAILGUN"));
        break;
      case TOWER_FROST:
        mF.print(F("FROST"));
        break;
      case TOWER_SHOCK:
        mF.print(F("SHOCK"));
        break;
      case TOWER_SUPPORT:
        mF.print(F("SUPPORT"));
        break;
    }

    // draw price of the tower
    drawCoins(90, pgm_read_byte(towerPrices + indexBuildMenu));

  } else if (gameMode == MODE_PLAYING_TOWER) {

    uint8_t coins;

    switch (indexTowerMenu) {
      case 0:
        mF.print(F("UPGRADE"));
        // draw price of the upgrade
        coins = 20;
        break;
      case 1:
        mF.print(F("SELL"));
        // get reward for selling the tower with upgrades
        coins = 7;
        break;  
      case 2:
        mF.print(F("INFOS"));

        // yes this is a return, because there will be no price
        return;
    }

    // TODO: return if tower has max level

    // draw price for upgrade or reward for sell
    drawCoins(90, coins);
  }
}

void drawInfosPlayingAll() {

  // draw the coins the play has
  drawCoins(1, currentCoins);

  if (gameMode == MODE_PLAYING) {
    drawInfosPlaying();

  } else if (gameMode == MODE_PLAYING_BUILD || gameMode == MODE_PLAYING_TOWER) {
    drawInfosPlayingMenu();

  } else if (gameMode == MODE_PLAYING_INFO) {
    // this says just return with b
    drawInfosOptionsCredits();
  }
}

void drawInfoLine() {

  if (gameMode == MODE_MAINMENU) {
    drawMainMenu();

  } else if (inPlayingMode(gameMode)) {
    drawInfosPlayingAll();

  } else if (inEditorMode(gameMode)) {
    drawInfosEditor();

  } else if (gameMode == MODE_OPTIONS || gameMode == MODE_CREDITS) {
    drawInfosOptionsCredits();
  }
}

#endif

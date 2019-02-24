#ifndef InfoLine_h
#define InfoLine_h

#define yIcon 57
#define yText (yIcon + 1)

void setInfoMessage(uint8_t infoType) {

  // set default screen time in frames
  infoMsgTimeout = 40;

  // set message type
  infoMsgType = infoType;
}

bool drawInfoMessage() {

  // decrement the time the current message is shown
  if (infoMsgTimeout > 0)
    infoMsgTimeout--;

  // do nothing if no info message is set
  if (infoMsgTimeout == 0)
    return false;

  mF.setCursor(1, yText);

  // print message, 18 letters fit in easiely more if 1 and i appears often
  if (infoMsgType == INFO_FORBIDDEN_BUILD) {
    //.print(F("123456789 12345678"));
    mF.print(F("NOT ALLOWED HERE"));

  } else if (infoMsgType == INFO_BLOCKED_AREA) {
    mF.print(F("BLOCKED AREA"));
    
  } else if (infoMsgType == INFO_ENTRY_BLOCK) {
    mF.print(F("NO ENTRY BLOCKING"));
    
  } else if (infoMsgType == INFO_JUST_A_HOUSE) {
    mF.print(F("JUST A BUILDING"));
   
  } else if (infoMsgType == INFO_SEND_NEXT_WAVE) {
    mF.print(F("A: TO SEND WAVE"));
   
  }

  return true;
}

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


void drawMainMenuRank() {
  const uint8_t xPos = 123;
  const uint8_t xWidth = 5;

  // background
  arduboy.fillRect(xPos, 0, xWidth, 56, BLACK);

  // bottom thingi
  arduboy.drawRect(xPos, 51, xWidth, 3, WHITE);

  // draw rank symbol
  drawBitmapFast(xPos, 42, rankSymbols, xWidth, 0, false, WHITE);
  drawBitmapFast(xPos, 37, rankSymbols, xWidth, 0, false, WHITE);
  drawBitmapFast(xPos, 32, rankSymbols, xWidth, 0, false, WHITE);
}

void drawMainMenu() {

  // write the games name to the screen
  mF.setCursor(22, 3);
  mF.print(F("MICRO TOWERS"));

  // rights part that shows the progress
  drawMainMenuRank();

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

  mF.setCursor(1, yText);

  if (!isLongPressInfo(stateButtonB)) {

    mF.print(F("EDITOR"));

    // explains what these editor symbols mean
    mF.setCursor(64, yText);

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

  mF.setCursor(1, yText);
  mF.print(F("PRESS B TO EXIT"));
}

void drawCoins(uint8_t xPos, uint8_t coins) {

  // draw coin icon
  drawBitmapFast(xPos, yIcon, symbolSet, ICON_WIDTH, SYMBOL_COIN, false);

  // draw current coins
  mF.setCursor(xPos + 8, yText);
  mF.print(coins);
}

void drawInfosPlaying() {

  // draw heart icon
  drawBitmapFast(47, yIcon, symbolSet, ICON_WIDTH, SYMBOL_HEART, false);

  // draw current life
  mF.setCursor(55, yText);
  mF.print(currentLifePoints);

  // draw wave icon
  drawBitmapFast(82, yIcon, symbolSet, ICON_WIDTH, SYMBOL_WAVE, false);

  // draw current life
  mF.setCursor(90, yText);
  mF.print(waveCounter);
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
    drawCoins(88, pgm_read_byte(towerPrices + indexBuildMenu));

  } else if (gameMode == MODE_PLAYING_TOWER) {

    uint8_t coins;

    switch (indexTowerMenu) {
      case TOWER_MENU_UPGRADE:
        mF.print(F("UPGRADE"));

        // draw price of the upgrade
        coins = 20;
        break;
      case TOWER_MENU_INFO:
        mF.print(F("INFOS"));

        // get reward for selling the tower with upgrades
        coins = 7;
        break;
      case TOWER_MENU_SELL:
        mF.print(F("SELL"));

        // yes this is a return, because there will be no price
        return;
    }

    // TODO: return if tower has max level

    // draw price for upgrade or reward for sell
    drawCoins(88, coins);
  }
}

void drawInfosPlayingAll() {

  // draw the coins the play has
  drawCoins(2, currentCoins);

  // show that you in fast mode right now
  if (!isNormalSpeed)
    drawBitmapFast(112, yIcon, symbolSet, ICON_HEIGHT, SYMBOL_FASTMODE, false);

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

  // info messages have highest prio
  if (drawInfoMessage())
    return;

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

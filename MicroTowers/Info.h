#ifndef InfoLine_h
#define InfoLine_h

#define yIcon 57
#define yText (yIcon + 1)

void setInfoTextCursor(int8_t xPos) {

  mF.setCursor(xPos, yText);
}

void setInfoMessage(uint8_t infoType) {

  // set default screen time in frames
  infoMsgTimeout = 40;

  // show messages longer when fast mode is active
  if (!isNormalSpeed)
    infoMsgTimeout *= 2;

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

  setInfoTextCursor(1);

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

  } else if (infoMsgType == INFO_TO_LESS_COINS) {
    mF.print(F("NOT ENOUGH COINS"));

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

  setInfoTextCursor(xWrite);

  switch (index) {
    case 0:
      mF.print(F("CAMPAIN"));
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
    case 4:
      mF.print(F("RESET GAME"));
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

  // unlocked maps is also the rank
  uint8_t yPos = 42;
  for (uint8_t i = 0; i < unlockedMaps / 2 - 3; i++) {

    // draw a rank symbol
    drawBitmapFast(xPos, yPos, rankSymbols, xWidth, i / 3, false, WHITE);

    //Serial.println(String(i) + " draw at yPos:" + String(yPos));

    // formula to set yPos for next symbol : yPos -= (5 + (i + 1)/3 * 2);
    yPos -= 5;
    if (i > 1)
      yPos -= 2;

    if (i > 4)
      yPos -= 2;
  }
}

void drawLeftHint(uint8_t xPos) {
  setInfoTextCursor(xPos);
  mF.print(F("<"));
}

void drawRightHint(uint8_t xPos) {
  setInfoTextCursor(xPos);
  mF.print(F(">"));
}

void drawMainMenu() {
  static int16_t indexMainMenuDelayed = 0;

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
    if (indexMainMenu > 0)
      drawLeftHint(22);

    if (indexMainMenu < MAINMENU_ITEMS - 1)
      drawRightHint(84);
  }

  for (int8_t i = 0; i < MAINMENU_ITEMS; i++) {
    // try to draw all of the menu items if they are on the screen or not
    drawMainMenuText(i * 64 + 32 - indexMainMenuDelayed * 4,  i);
  }
}

void drawInfosEditor() {

  setInfoTextCursor(1);

  if (!isLongPressInfo(stateButtonB)) {

    mF.print(F("EDITOR"));

    // explains what these editor symbols mean
    setInfoTextCursor(64);

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

  setInfoTextCursor(1);
  mF.print(F("PRESS B TO EXIT"));
}

void drawCoins(uint8_t xPos, uint8_t coins) {

  // draw coin icon
  drawBitmapFast(xPos, yIcon, symbolSet, ICON_WIDTH, SYMBOL_COIN, false);

  // draw current coins
  setInfoTextCursor(xPos + 8);
  mF.print(coins);
}

void drawInfosPlaying() {

  // draw heart icon
  drawBitmapFast(42, yIcon, symbolSet, ICON_WIDTH, SYMBOL_HEART, false);

  // draw current life
  setInfoTextCursor(50);
  mF.print(currentLifePoints);

  // draw wave icon
  setInfoTextCursor(82);
  if (waveType == ENEMY_IS_DEFAULT) {
    // D for default
    mF.print(F("D"));

  } else if (waveType == ENEMY_MIX) {
    // M for mixed wave
    mF.print(F("M"));

  } else {
    // draw an icon for the next wave
    drawBitmapFast(82, yText, waveTypes, 6, waveType - ENEMY_IS_FAST, false);
  }

  // draw wave counter
  setInfoTextCursor(90);
  mF.print(waveCounter + 1);
}

void drawCurrentTowerName() {
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
}

void drawInfosPlayingInfo() {

  setInfoTextCursor(1);

  // write the name of index build menu what is "cursor in info screen"
  drawCurrentTowerName();

  setInfoTextCursor(58);
  mF.print(F("LVL"));

  // draw left hint
  if (indexMapsEditor > 0)
    drawLeftHint(86);

  // draw tower level index incremented to have levels 1-4
  setInfoTextCursor(94);
  mF.print(indexMapsEditor + 1);

  // draw right hint
  if (indexMapsEditor < 3)
    drawRightHint(102);

  // clear space for tower symbol
  arduboy.fillRect(127 - 13, 63 - 13, 13, 13, WHITE);

  const uint8_t xTower = 127 - 11;
  const uint8_t yTower = 63 - 11;

  // draw socket depending on level
  drawTowerSocket(xTower, yTower, indexMapsEditor);

  // should rotate slowly
  uint32_t rotation = (gameFrames / 8) % 16;

  // draw tower
  drawTowerWeapon(xTower, yTower, indexBuildMenu, rotation, indexMapsEditor);
}


void drawInfosPlayingMenu() {

  setInfoTextCursor(32);

  if (gameMode == MODE_PLAYING_BUILD) {

    // write the name of index build menu
    drawCurrentTowerName();

    // draw price of the tower
    drawCoins(88, getTowerPrice(indexBuildMenu, 0));

  } else if (gameMode == MODE_PLAYING_TOWER) {

    uint8_t coins;

    switch (indexTowerMenu) {
      case TOWER_MENU_UPGRADE:

        if (currentTowerLvl == 3) {
          mF.print(F("MAX LEVEL"));

        } else {
          mF.print(F("UPGRADE"));

          // draw price of the upgrade
          coins = priceUpdate;
        }
        break;

      case TOWER_MENU_INFO:
        mF.print(F("INFOS"));

        // give infos about boost only if boosted
        if (towerBoost != 0) {

          // draw the level 3 support tower icon
          drawBitmapFast(86, 57, allTowers, ICON_WIDTH, 6 * 16 + 7);

          setInfoTextCursor(98);
          mF.print(towerBoost);
        }

        // yes this is a return, because there will be no price
        return;

      case TOWER_MENU_SELL:
        mF.print(F("SELL"));

        // get reward for selling the tower with upgrades
        coins = priceSell;
        break;
    }

    // TODO: return if tower has max level

    // draw price for upgrade or reward for sell
    drawCoins(86, coins);
  }
}

void drawInfosPlayingAll() {

  // the gameover screen will be painted in main scheduler
  if (gameMode == MODE_PLAYING_END)
    return;

  // info screen need all the space
  if (gameMode != MODE_PLAYING_INFO) {

    // draw the coins the play has
    drawCoins(2, currentCoins);

    // show that you in fast mode right now
    if (!isNormalSpeed)
      drawBitmapFast(112, yIcon, symbolSet, ICON_HEIGHT, SYMBOL_FASTMODE, false);
  }

  if (gameMode == MODE_PLAYING) {
    drawInfosPlaying();

  } else if (gameMode == MODE_PLAYING_BUILD || gameMode == MODE_PLAYING_TOWER) {
    drawInfosPlayingMenu();

  } else if (gameMode == MODE_PLAYING_INFO) {
    drawInfosPlayingInfo();
  }
}

void drawGameOverInfo() {
  setInfoTextCursor(1);
  mF.print(F("GAMEOVER"));

  setInfoTextCursor(66);
  mF.print(F("SCORE"));

  setInfoTextCursor(108);
  mF.print(currentScore);
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

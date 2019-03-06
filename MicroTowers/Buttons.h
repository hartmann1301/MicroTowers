#ifndef Buttons_h
#define Buttons_h

void getButtons() {
#ifdef ESP8266
  // this function is called inside arduboy whenever buttonStates() is called
  arduboy.setExternalButtons(ps2x.getArduboyButtons());
#endif
}

void monitorButtonState(int8_t &button, bool isPressed) {
  if (isPressed) {
    if (button < 0)
      button = 1;

    if (button < INT8_MAX)
      button++;

  } else {
    if (button > 0)
      button = 0;

    if (button > INT8_MIN)
      button--;
  }
}

void monitorButtons() {
  // do the button checks only every default frame time
  if (isFastSpeedFrame)
    return;

  // write to stateButtonA&B
  monitorButtonState(stateButtonA, arduboy.pressed(A_BUTTON));
  monitorButtonState(stateButtonB, arduboy.pressed(B_BUTTON));
}

void editorSetNode() {
  // if same node is already on the map no need to recalculate because nothing changed
  if (indexBuildMenu == mM.getCurrentMapNode(xCursor, yCursor))
    return;

  // this triggers calculating a new path
  mapChanged = true;

  // change the map
  if (indexBuildMenu == EDITOR_HQ) {
    // the place where the old headquarter was will be free
    mM.setNode(headquarterPosition, MAP_FREE);

    // the headquarter will be overwritten
    headquarterPosition = getIndex(xCursor, yCursor);
  }

  // the editor enum is the almost the same as the map enum
  mM.setNode(xCursor, yCursor, indexBuildMenu);
}

bool isPressed(bool pressed) {

  // set global only if the button is pressed
  if (pressed)
    cursorPressed = pressed;

  // return bool for specific button functions
  return pressed;
}

bool isTimeoutActive() {
  // set default value for global cursorPressed bool
  cursorPressed = false;

  if (millis() < nextButtonInput) {
    return true;
  } else {
    return false;
  }
}

void setCursorTimeout() {
  if (cursorPressed) {
    nextButtonInput = millis() + 150;
  }
}

void checkUpDown(uint8_t &value, const uint8_t vMax) {

  if (isPressed(arduboy.pressed(UP_BUTTON)) && value > 0)
    value--;

  if (isPressed(arduboy.pressed(DOWN_BUTTON)) && value < vMax - 1)
    value++;
}

void checkLeftRight(uint8_t &value, const uint8_t vMax) {

  if (isPressed(arduboy.pressed(LEFT_BUTTON)) && value > 0)
    value--;

  if (isPressed(arduboy.pressed(RIGHT_BUTTON)) && value < vMax - 1)
    value++;
}

void moveCursor() {

  // because the playing cursor can move less
  uint8_t extraNode = uint8_t(gameMode == MODE_PLAYING);

  if (isTimeoutActive())
    return;

  checkUpDown(yCursor, COLUMNS - extraNode);

  checkLeftRight(xCursor, ROWS - extraNode);

  setCursorTimeout();
}

bool isCursorAreaType(const uint8_t type) {

  if (mM.getCurrentMapNode(xCursor, yCursor) != type)
    return false;

  if (mM.getCurrentMapNode(xCursor + 1, yCursor) != type)
    return false;

  if (mM.getCurrentMapNode(xCursor, yCursor + 1) != type)
    return false;

  if (mM.getCurrentMapNode(xCursor + 1, yCursor + 1) != type)
    return false;

  return true;
}

void goToMainMenu() {
  // set main game mode
  gameMode = MODE_MAINMENU;

  // fast forward is not needed in main menu
  isNormalSpeed = true;

  // set wave to finish so the auto sender can start a wave
  sendWaveStatus = WAVE_FINISHED;

  // delete all existing enemys on map
  eM.init();
  aM.init();

  // the loaded map is main menu background and playable map
  loadMap(9);

  // needs to be after loading the map
  tM.add(9, 5, TOWER_GATLING);
  tM.add(5, 3, TOWER_CANNON);
  tM.add(1, 2, TOWER_FROST);
  tM.add(7, 4, TOWER_SUPPORT);
  tM.add(13, 5, TOWER_FLAME);
  tM.add(2, 4, TOWER_SHOCK);
  tM.add(5, 5, TOWER_RAILGUN);
  tM.add(12, 3, TOWER_LASER);
}

void buttonsMainMenu() {

  // TODO: do something on B button

  if (isLongPressed(stateButtonA) && indexMainMenu == 4)
    resetGame();

  // go to next mode
  if (arduboy.justReleased(A_BUTTON)) {
    switch (indexMainMenu) {
      case 0:
        gameMode = MODE_MAPS_CAMPAIN;

        // is used to load scores from eeprom
        isInCampainMode = true;

        break;
      case 1:
        gameMode = MODE_MAPS_EDITOR;

        // is used to load scores from eeprom
        isInCampainMode = false;

        // load editor maps from eeprom to ram array for preview
        loadMapsFromEEPROM();

        break;
      case 2:
        gameMode = MODE_OPTIONS;
        break;
      case 3:
        gameMode = MODE_CREDITS;
        break;
    }
  }

  if (isTimeoutActive())
    return;

  checkLeftRight(indexMainMenu, MAINMENU_ITEMS);

  //checkUpDown(unlockedMaps, 11);

  setCursorTimeout();
}

void buttonsMapsCampain() {

  // go to next mode
  if (arduboy.justReleased(A_BUTTON)) {

    gameMode = MODE_PLAYING;

    // put the current map from pgm space to mapComposition array
    loadMap(indexMapsCampain);
  }

  if (isTimeoutActive())
    return;

  checkUpDown(indexMapsCampain, min(unlockedMaps, MAPS_IN_CAMPAIN));

  // get the score of the new map();
  if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON))
    updateCurrentScore();

  setCursorTimeout();
}

void buttonsMapsEditor() {

  // play
  if (arduboy.justReleased(A_BUTTON)) {

    // put the current map from pgm space to mapComposition array
    loadMap(indexMapsEditor);

    // this needs to be done after loading the map!
    gameMode = MODE_PLAYING;
  }

  // edit
  if (arduboy.justReleased(RIGHT_BUTTON)) {

    // because the editor menu is using the same index but is only 4 long
    indexBuildMenu = 0;

    // put the current map from pgm space to mapComposition array
    loadMap(indexMapsEditor);

    // should be done after loading the map
    gameMode = MODE_EDITOR;
  }

  if (isTimeoutActive())
    return;

  checkUpDown(indexMapsEditor, EDITOR_MAP_SLOTS);

  // get the score of the new map();
  if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON))
    updateCurrentScore();

  setCursorTimeout();
}

void checkPrepareTowerMenu() {
  // get tower where the cursor is
  uint8_t towerIndex = tM.getTowerAt(xCursor, yCursor);

  // there are towers but the cursor is wrong between them
  if (towerIndex == NO_INDEX) {

    // use blocked area again
    setInfoMessage(INFO_BLOCKED_AREA);

    return;
  }

  gameMode = MODE_PLAYING_TOWER;

  uint8_t type = tM.list[towerIndex].getType();
  uint8_t lvl = tM.list[towerIndex].getLevel();

  // save boost for the menu
  towerBoost = tM.list[towerIndex].boost;

  //Serial.println("towerBoost: " + String(towerBoost));

  // write update price to global var and check if possible
  if (lvl < 3) {
    priceUpdate = getTowerPrice(type, lvl + 1);

  } else {
    priceUpdate = 0xff;
  }

  //Serial.println("priceUpdate: " + String(priceUpdate));

  // calculte the costs buying the tower
  priceSell = 0;
  for (uint8_t i = 0; i <= lvl; i++) {
    priceSell += getTowerPrice(type, i);
  }

  // div by 2 because of selling
  priceSell /= 2;
}

void buttonsPlaying() {

  // trigger to send next wave
  if (arduboy.justReleased(A_BUTTON) && sendWaveStatus == WAVE_FINISHED)
    sendWaveStatus = WAVE_START;

  if (arduboy.justReleased(B_BUTTON)) {

    // check if cursor is right in front of the entry for enemys
    if (xCursor == 0 && (yCursor == 3 || yCursor == 4)) {
      setInfoMessage(INFO_ENTRY_BLOCK);

    } else if (isCursorAreaType(MAP_FREE)) {

      // add some test tower to the current cursor position
      tM.add(xCursor, yCursor, 0);

      // check if it is allowed to build here
      if (mM.findPath()) {
        gameMode = MODE_PLAYING_BUILD;

      } else {
        // put not allowed to build here message to the info line
        setInfoMessage(INFO_FORBIDDEN_BUILD);
      }

      // sell this test tower again
      tM.sell(tM.getTowerAt(xCursor, yCursor));

      // check if cursor area is a tower
    } else if (isCursorAreaType(MAP_TOWER)) {

      checkPrepareTowerMenu();

    } else if (isCursorAreaType(MAP_FREE)) {
      setInfoMessage(INFO_JUST_A_HOUSE);

    } else {
      // put something is in cursor area message
      setInfoMessage(INFO_BLOCKED_AREA);
    }
  }

  moveCursor();
}

void buttonsPlayingMenuBuild() {

  if (arduboy.justReleased(A_BUTTON))
    tryToBuildTower();

  // go back to play mode
  if (arduboy.justReleased(B_BUTTON))
    gameMode = MODE_PLAYING;

  if (isLongPressed(stateButtonB))
    gameMode = MODE_MAPS_CAMPAIN;

  if (isTimeoutActive())
    return;

  checkUpDown(indexBuildMenu, MENU_ITEMS_BUILD);

  setCursorTimeout();
}

void buttonsPlayingMenuTower() {

  if (arduboy.justReleased(A_BUTTON)) {

    if (indexTowerMenu == TOWER_MENU_UPGRADE) {

      tryToUpgradeTower();

    } else if (indexTowerMenu == TOWER_MENU_INFO) {

      // get tower where the cursor is
      uint8_t towerIndex = tM.getTowerAt(xCursor, yCursor);

      uint8_t type = tM.list[towerIndex].getType();
      uint8_t lvl = tM.list[towerIndex].getLevel();

      // set the buildmenu index to show infos of the current cursored tower
      indexBuildMenu = type;

      // use editor index as level marker
      indexMapsEditor = lvl;

      // now go to the info screen
      gameMode = MODE_PLAYING_INFO;

    } else if (indexTowerMenu == TOWER_MENU_SELL) {

      sellTower();
    }
  }

  // go back to play mode
  if (arduboy.justReleased(B_BUTTON))
    gameMode = MODE_PLAYING;

  if (isTimeoutActive())
    return;

  checkUpDown(indexTowerMenu, MENU_ITEMS_TOWER);

  setCursorTimeout();
}

void buttonsPlayingInfo() {

  if (arduboy.justReleased(A_BUTTON) || arduboy.justReleased(B_BUTTON))
    gameMode = MODE_PLAYING;

  if (isTimeoutActive())
    return;

  // change tower
  checkUpDown(indexBuildMenu, MENU_ITEMS_BUILD);

  // change level
  checkLeftRight(indexMapsEditor, 4);

  setCursorTimeout();
}

void buttonsEditor() {

  if (arduboy.justReleased(A_BUTTON))
    editorSetNode();

  if (arduboy.justReleased(B_BUTTON)) {
    gameMode = MODE_EDITOR_MENU;
  }

#ifdef DEBUG_CREATE_MAPS
  // this is to create the maps
  if (isLongPressed(stateButtonA))
    saveMapToEEPROM(indexMapsEditor);
#endif

  moveCursor();
}

void buttonsEditorMenu() {

  if (arduboy.justReleased(A_BUTTON) || arduboy.justReleased(B_BUTTON))
    gameMode = MODE_EDITOR;

  if (isLongPressed(stateButtonB))
    gameMode = MODE_MAINMENU;

  if (isTimeoutActive())
    return;

  checkUpDown(indexBuildMenu, MENU_ITEMS_EDITOR);

  setCursorTimeout();
}

void buttonsOptions() {

  //TODO: add the options
  //checkUpDown(indexOptions, MENU_ITEMS_OPTIONS);
}

void checkLongPress(int8_t &pressTime) {

  // to not be anonying and blink all the time
  if (pressTime < LONGPRESS_TIME)
    return;

  // set value to maximum to mark that there is a long press this frame
  pressTime = INT8_MAX;

  //Serial.println(String(millis()) + " found long press");
  foundLongPress = true;
}

void manipulateLongPress() {

  // check if one of the states is max, which means longpress is active
  if (stateButtonA != INT8_MAX && stateButtonB != INT8_MAX)
    return;

  // manipulate the button state so longpress will be reseted
  stateButtonA = 1;
  stateButtonB = 1;
}

bool skipLongPress() {

  // return because there was no long press
  if (!foundLongPress)
    return false;

  // return because there is still a button pressed
  if (foundLongPress && (stateButtonA > 0 || stateButtonB > 0))
    return false;

  // reset global var
  foundLongPress = false;

  //Serial.println("skip");
  return true;
}

void checkNormalTime() {
  // set the global bool, the minus 1 is just to be sure
  isFastSpeedFrame = (millis() < normalSpeedTime + MS_PRO_FRAME - 1);

  // return in every fast frame
  if (isFastSpeedFrame)
    return;

  normalSpeedTime = millis();
}

void leaveToMainMenu() {

  // return to main menu on both buttons
  if (arduboy.justReleased(A_BUTTON) || arduboy.justReleased(B_BUTTON))
    goToMainMenu();
}

void checkButtons() {
  // this is to know if this is a fastMode frame
  checkNormalTime();

  // keep track how long a button is held or released
  monitorButtons();

  // this allows multiple longpress without releasing the button
  manipulateLongPress();

  //Serial.println(String(millis()) + " checkButtons() states a:" + String(stateButtonA) + " b:" + String(stateButtonB));

  // ignore this frame because last one was a long press, to aviod justReleased functions
  if (skipLongPress())
    return;

  // check if next frame should be ignored
  checkLongPress(stateButtonA);
  checkLongPress(stateButtonB);

  if (gameMode == MODE_MAINMENU) {
    buttonsMainMenu();

  } else if (inMapsListMode(gameMode)) {

    // go back to main menu
    if (arduboy.justReleased(B_BUTTON))
      goToMainMenu();

    if (gameMode == MODE_MAPS_CAMPAIN)
      buttonsMapsCampain();

    if (gameMode == MODE_MAPS_EDITOR)
      buttonsMapsEditor();

  } else if (inPlayingMode(gameMode)) {

    // is global while playing to go back to the maps list
    if (isLongPressed(stateButtonB))
      goToMainMenu();

    // toggles the gamespeed for fast mode
    if (isLongPressed(stateButtonA))
      isNormalSpeed = !isNormalSpeed;

    // specifice mode funtions
    if (gameMode == MODE_PLAYING) {
      buttonsPlaying();

    } else if (gameMode == MODE_PLAYING_BUILD) {
      buttonsPlayingMenuBuild();

    } else if (gameMode == MODE_PLAYING_TOWER) {
      buttonsPlayingMenuTower();

    } else if (gameMode == MODE_PLAYING_INFO) {
      buttonsPlayingInfo();

    } else if (gameMode == MODE_PLAYING_END) {
      leaveToMainMenu();
    }

  } else if (inEditorMode(gameMode)) {

    // delete the whole map
    if (isLongPressed(stateButtonA) && indexBuildMenu == EDITOR_DELETE)
      memset(mapComposition, MAP_FREE, NODES_COMPRESSED);

    // is global in editor to go back to main menu
    if (isLongPressed(stateButtonB)) {
      // also generate and print map array via serial interface
      saveMapToEEPROM(indexMapsEditor);

      goToMainMenu();
    }

    // specifice mode funtions
    if (gameMode == MODE_EDITOR) {
      buttonsEditor();

    } else if (gameMode == MODE_EDITOR_MENU) {
      buttonsEditorMenu();
    }

  } else if (gameMode == MODE_OPTIONS || gameMode == MODE_CREDITS) {

    // leave on button a and b
    leaveToMainMenu();

    // controll the options list with cursor
    if (gameMode == MODE_OPTIONS)
      buttonsOptions();

    // credits has no own buttons function
  }
}

#endif

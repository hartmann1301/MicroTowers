#ifndef Buttons_h
#define Buttons_h


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
  if (indexBuildMenu == mM.getNode(xCursor, yCursor))
    return;

  // this triggers calculating a new path
  mapChanged = true;

  // change the map
  if (indexBuildMenu == EDITOR_HQ) {
    // the place where the old headquarter was will be free
    mM.setNode(headquarterPosition, MAP_FREE);

    // the headquarter will be overwritten
    headquarterPosition = mM.getIndex(xCursor, yCursor);
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

  if (mM.getNode(xCursor, yCursor) != type)
    return false;

  if (mM.getNode(xCursor + 1, yCursor) != type)
    return false;

  if (mM.getNode(xCursor, yCursor + 1) != type)
    return false;

  if (mM.getNode(xCursor + 1, yCursor + 1) != type)
    return false;

  return true;
}

void buttonsMainMenu() {

  // TODO: do something on B button

  // go to next mode
  if (arduboy.justReleased(A_BUTTON)) {
    switch (indexMainMenu) {
      case 0:
        gameMode = MODE_MAPS_LIST;
        break;
      case 1:
        gameMode = MODE_EDITOR;

        // because the editor menu is using the same index
        indexBuildMenu = 0;
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

  setCursorTimeout();
}

void buttonsMapsList() {
  // a and b Button

  // go to next mode
  if (arduboy.justReleased(A_BUTTON)) {

    gameMode = MODE_PLAYING;

    // put the current map from pgm space to mapComposition array
    mM.loadMap(indexMapsList);
  }

  // go back to main menu
  if (arduboy.justReleased(B_BUTTON)) {

    gameMode = MODE_MAINMENU;

    // TODO maybe load the main menu map
  }

  if (isTimeoutActive())
    return;

  checkUpDown(indexMapsList, min(unlockedMaps, MAPSLIST_ITEMS));

  setCursorTimeout();
}

void buttonsPlaying() {



  if (arduboy.justReleased(B_BUTTON)) {

    // check if cursor area is free to build
    if (isCursorAreaType(MAP_FREE))
      gameMode = MODE_PLAYING_BUILD;

    // check if cursor area is a tower
    if (isCursorAreaType(MAP_TOWER))
      gameMode = MODE_PLAYING_TOWER;
  }

  moveCursor();
}

void buttonsPlayingMenu() {

  if (arduboy.justReleased(A_BUTTON))
    tryToBuildTower();

  // go back to play mode
  if (arduboy.justReleased(B_BUTTON))
    gameMode = MODE_PLAYING;

  if (isLongPressed(stateButtonB))
    gameMode = MODE_MAPS_LIST;

  if (isTimeoutActive())
    return;

  checkUpDown(indexBuildMenu, MENU_ITEMS_PLAYING);

  setCursorTimeout();
}

void buttonsPlayingTower() {

  if (arduboy.justReleased(A_BUTTON) || arduboy.justReleased(B_BUTTON))
    gameMode = MODE_PLAYING;
}

void buttonsPlayingInfo() {

  if (arduboy.justReleased(A_BUTTON) || arduboy.justReleased(B_BUTTON))
    gameMode = MODE_PLAYING;
}

void buttonsEditor() {

  if (arduboy.justReleased(A_BUTTON))
    editorSetNode();

  if (arduboy.justReleased(B_BUTTON)) {
    Serial.println("go to menu");
    gameMode = MODE_EDITOR_MENU;
  }

  // this is to create the maps
  if (isLongPressed(stateButtonA))
    mM.generateMapArray();

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
}

bool skipLongPress() {
  // check if one of the states is max, which means longpress is active
  if (stateButtonA != INT8_MAX && stateButtonB != INT8_MAX)
    return false;

  // manipulate the button state so longpress will be reseted
  stateButtonA = 1;
  stateButtonB = 1;

  return true;
}

void checkNormalTime() {
  // set the global bool, the minus 1 is just to be sure
  isFastSpeedFrame = (millis() < normalSpeedTime + GAME_FRAME_MS - 1);

  // return in every fast frame
  if (isFastSpeedFrame)
    return;

  normalSpeedTime = millis();
}

void checkButtons() {
  // this is to know if this is a fastMode frame
  checkNormalTime();

  // keep track how long a button is held or released
  monitorButtons();

  // ignore this frame because last one was a long press
  if (skipLongPress())
    return;

  // check if next frame should be ignored
  checkLongPress(stateButtonA);
  checkLongPress(stateButtonB);

  if (gameMode == MODE_MAINMENU) {
    buttonsMainMenu();

  } else if (gameMode == MODE_MAPS_LIST) {
    buttonsMapsList();

  } else if (inPlayingMode(gameMode)) {

    // is global while playing to go back to the maps list
    if (isLongPressed(stateButtonB))
      gameMode = MODE_MAPS_LIST;

    // toggles the gamespeed for fast mode
    if (isLongPressed(stateButtonA))
      isNormalSpeed = !isNormalSpeed;

    // specifice mode funtions
    if (gameMode == MODE_PLAYING) {
      buttonsPlaying();

    } else if (gameMode == MODE_PLAYING_BUILD) {
      buttonsPlayingMenu();

    } else if (gameMode == MODE_PLAYING_TOWER) {
      buttonsPlayingTower();

    } else if (gameMode == MODE_PLAYING_INFO) {
      buttonsPlayingInfo();
    }

  } else if (inEditorMode(gameMode)) {

    // is global in editor to go back to main menu
    if (isLongPressed(stateButtonB))
      gameMode = MODE_MAINMENU;

    // generate and print map array via serial interface
    if (isLongPressed(stateButtonA))
      mM.generateMapArray();

    // specifice mode funtions
    if (gameMode == MODE_EDITOR) {
      buttonsEditor();

    } else if (gameMode == MODE_EDITOR_MENU) {
      buttonsEditorMenu();
    }

  } else if (gameMode == MODE_OPTIONS || gameMode == MODE_CREDITS) {

    // return to main menu
    if (arduboy.justReleased(A_BUTTON) || arduboy.justReleased(B_BUTTON))
      gameMode = MODE_MAINMENU;

    // controll the options list with cursor
    if (gameMode == MODE_OPTIONS)
      buttonsOptions();

    // credits has no own buttons function
  }
}

#endif

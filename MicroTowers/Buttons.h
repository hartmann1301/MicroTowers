#ifndef Buttons_h
#define Buttons_h

void menuButtons() {

  /*
    if (arduboy.pressed(UP_BUTTON))
    changeMenuCursor(pT.menuCursor - 1);

    if (arduboy.pressed(DOWN_BUTTON))
    changeMenuCursor(pT.menuCursor + 1);

    if (arduboy.pressed(LEFT_BUTTON)) {
    if (millis() > nextButtonInput) {
      arduboy.audio.toggle();

      audioMessageTimeout = GAME_FRAMES * 2;
      nextButtonInput = millis() + 400;
    }
    }

    if (arduboy.pressed(RIGHT_BUTTON)) {
    if (millis() > nextButtonInput) {
      if (pT.buyUpgrade())
        nextButtonInput = millis() + 400;
    }
    }

    if (arduboy.pressed(A_BUTTON))  {
    if (pT.menuCursor < PLAYER_WEAPONS)
      pT.weaponType = pT.menuCursor;
    }
  */
}

void playButtons() {

  /*
    float moveSpeed = 1.0 + 0.1 * float(pT.lvlSpeed);

    // increase the move speed for the rocket tank if the weapon is loaded
    if (pT.weaponType == WEAPON_ROCKET && pT.power > 200) {
    // the rocket tank is a bit faster
    moveSpeed += 0.2;

    // show extra speed with little animation
    if (arduboy.pressed(RIGHT_BUTTON) ) {
      bool o = (gameFrames % 2 == 0);

      // draw little fire animation
      drawSmallFire(pT.x + 1 - uint8_t(o), pT.y + 1, false);
    }
    }

    if (pT.weaponType == WEAPON_FLAME)
    // the flame tank is a bit slower
    moveSpeed -= 0.3;

    // check move
    if (arduboy.pressed(DOWN_BUTTON))
    pT.y += moveSpeed;

    if (arduboy.pressed(UP_BUTTON))
    pT.y -= moveSpeed;

    if (arduboy.pressed(RIGHT_BUTTON))
    pT.x += moveSpeed;

    if (arduboy.pressed(LEFT_BUTTON))
    pT.x -= moveSpeed;

  */
}

void changeGameMode() {
  if (millis() < nextButtonInput)
    return;

  isPlayMode = !isPlayMode;

  nextButtonInput = millis() + 200;
}

void checkButtons() {

  // button b not pressed helps to activate the special
  if (arduboy.pressed(B_BUTTON) && !arduboy.pressed(A_BUTTON))
    changeGameMode();

  if (isPlayMode) {
    playButtons();
  } else {
    menuButtons();
  }
}

#endif

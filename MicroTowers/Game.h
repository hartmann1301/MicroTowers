#ifndef Gameplay_h
#define Gameplay_h

void tryToBuildTower() {
  Serial.println("tryToBuildTower()");
  
  // get the needed money
  uint8_t price = getProgMem(towerPrices, indexBuildMenu);

  // return because player has to less money
  if (currentCoins < price)
    return;

  // buy the tower
  currentCoins -= price;

  // put tower to map
  tM.add(xCursor, yCursor, indexBuildMenu);

  // if tower was placed return to playing mode
  gameMode = MODE_PLAYING;
}

void tryToUpgradeTower() {
  // get the tower where the cursor is
  uint8_t index = tM.getTowerAt(xCursor, yCursor);
  uint8_t currentLevel = tM.list[index].getLevel();

  if (currentLevel == TOWER_LEVEL_MAX)
    return;

  uint8_t type = tM.list[index].getType();

  // get the needed money
  uint8_t price = getProgMem(towerPrices, type);

  // return because player has to less money
  if (currentCoins < price)
    return;

  // buy the upgrade
  currentCoins -= price;

  // write incremented level back
  tM.list[index].setLevel(currentLevel + 1);

  // recalculate the boosts
  if (type == TOWER_SUPPORT)
    tM.setBoosts();

  // if update was done return to game
  gameMode = MODE_PLAYING;
}

void sellTower() {
  // get the tower where the cursor is
  uint8_t index = tM.getTowerAt(xCursor, yCursor);

  // get worth of the tower
  uint8_t price = getProgMem(towerPrices, tM.list[index].getType());

  // give half of the buying und upgrading price back
  currentCoins += (price * (tM.list[index].getLevel() + 1)) / 2;

  // delete this tower
  tM.sell(index);

  // if tower was placed return to playing mode
  gameMode = MODE_PLAYING;
}

void loadMap(uint8_t mapNumber) {

  Serial.println("load Map: ");
  Serial.println(mapNumber, DEC);

  // trigger new path finding
  mapChanged = true;

  // set defaults of in level variables
  currentLifePoints = 100;
  waveCounter = 0;
  nextEnemyFrame = 0;

  // if playing show message that wave can be triggered
  if (inPlayingMode(gameMode))
    setInfoMessage(INFO_SEND_NEXT_WAVE);

  // TODO: change this
  currentMapDifficulty = getProgMem(mapDifficulties, mapNumber);

  // reset all towers and enemys
  tM.init();
  eM.init();

  // get 2 bits of map data and store each in 8 bits mapCompositon array where it can be modified
  for (uint8_t n = 0; n < NODES; n++) {

    // reads from program space or eeprom
    uint8_t data = mM.getStoredMapNode(n, mapNumber);

    // save position of headquarter
    if (data == MAP_TOWER) {
      headquarterPosition = n;

      //Serial.println("headquarterPosition is: " + String(headquarterPosition));
    }

    // write to mapCompositon
    mM.setNode(n, data);
  }
}

void checkHits() {

  for (uint8_t i = 0; i < eM.maximum; i++) {

    // check only active enemys
    if (eM.isEnemyActive(i) == false)
      continue;

    uint8_t projIndex = pM.isProjectileAt(eM.list[i].x, eM.list[i].y);

    // continue because no index was found
    if (projIndex == 0xff)
      continue;

    //Serial.println(String(projIndex) + " is projIndex");

    uint8_t enemyType = eM.list[i].type;
    uint8_t projectileType = pM.list[projIndex].getType();

    uint8_t projectileDmg = 2;
    /*
      if (projectileType == WEAPON_STDGUN) {
      projectileDmg = 30 + weaponLevel * 5;

      } else if (projectileType == WEAPON_ROCKET) {
      projectileDmg = 70 + weaponLevel * 8;
    */

    // do the damage and decide if it dies
    bool isEnemyDead = eM.list[i].damage(projectileDmg, projectileType);

    //aM.add(impactPosX, pM.list[projIndex].y, ANIMATION_IMPACT_L, 0);

    // railgun projectiles fly trough the whole map, flames destroy themself
    if (projectileType == TOWER_FLAME || projectileType == TOWER_RAILGUN)
      return;

    // set flag to destroy this projectile later
    // if (projectileType == TOWER_CANON)

    //Serial.println("hit of enemy: " + String(i) + " with projectile:" + String(projectileType));

    pM.clearProjectile(projIndex);
  }
}

void mainMenuWaveSender() {

  // start sending a new wave if the last is done
  if (sendWaveStatus == WAVE_FINISHED)
    sendWaveStatus = WAVE_START;
}

void mainScheduler() {

  if (!inMapsListMode(gameMode)) {

    // the bottom line with all the hints
    drawInfoLine();

    // function will decide if it needs to be cut for menus
    drawMapBorders();
  }

  if (gameMode == MODE_MAINMENU) {
    drawMainMenu();

    mainMenuWaveSender();

  } else if (gameMode == MODE_MAPS_CAMPAIN) {
    drawMapsListCampain();

  } else if (gameMode == MODE_MAPS_EDITOR) {
    drawMapsListEditor();

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


void updateGame() {

  mM.drawMap();

  if (mapChanged) {
    // write new cost list with pathfinding algorithm
    mM.findPath();

    // recalculate how much every tower is boosted
    tM.setBoosts();
  }

  tM.update();
  eM.update();
  pM.update();

  checkHits();
}

#endif

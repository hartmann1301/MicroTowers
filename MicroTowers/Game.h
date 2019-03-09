#ifndef Gameplay_h
#define Gameplay_h

void tryToBuildTower() {
  //Serial.println("tryToBuildTower()");

  // get the needed money
  uint8_t price = getTowerPrice(indexBuildMenu, 0);

  // return because player has to less money
  if (currentCoins < price) {

    // stay in menu but show this message
    setInfoMessage(INFO_TO_LESS_COINS);

    return;
  }

  // buy the tower
  currentCoins -= price;

  // change prototype tower to actual tower
  tM.list[towerIndex].setType(indexBuildMenu);

  sound.tones(soundSomethingBad);

  // recalculate how much every tower is boosted
  if (indexBuildMenu == TOWER_SUPPORT)
    tM.setBoosts();

  // if tower was placed return to playing mode
  gameMode = MODE_PLAYING;
}

void tryToUpgradeTower() {

  // do nothing, there is a max info printed
  if (towerLevel == TOWER_LEVEL_MAX)
    return;

  uint16_t priceUpdate = getPriceUpdate();

  // return because player has to less money, price is already calulated
  if (currentCoins < priceUpdate) {

    // stay in menu but show this message
    setInfoMessage(INFO_TO_LESS_COINS);

    return;
  }

  sound.tones(soundSomethingBad);

  // buy the upgrade
  currentCoins -= priceUpdate;

  // write incremented level back
  tM.list[towerIndex].setLevel(towerLevel + 1);

  // recalculate the boosts
  if (towerType == TOWER_SUPPORT)
    tM.setBoosts();

  // if update was done return to game
  gameMode = MODE_PLAYING;
}

void sellTower() {
  // give half of the buying und upgrading price back
  currentCoins += getPriceSell();

  // delete this tower
  tM.sell(towerIndex);

  sound.tones(soundSomethingGood);

  // if tower was placed return to playing mode
  gameMode = MODE_PLAYING;
}

void resetEEPROM() {
  // lock the maps
  unlockedMaps = 5;

  sound.tones(soundSomethingGood);

  // reset list index
  indexMapsCampain = 0;
  indexMapsEditor = 0;

  // clear the scores from editor list and campain list
  clearEepromScores();
}

void loadMap(uint8_t mapNumber) {

  //Serial.println("load Map: ");
  //Serial.println(mapNumber, DEC);

  // trigger new path finding
  mapChanged = true;

  // set lifepoints
  lifePoints = 100;

  // is incremented in info line to start with 1 not 0
  waveCounter = 0;

  // to always start with the cyborgs type 0
  enemysRace = 0;
  waveType = 0;

  // to be able to start immediatelly
  nextEnemyFrame = 0;
  sendWaveStatus = WAVE_FINISHED;

  // if playing show message that wave can be triggered
  if (inPlayingMode(gameMode))
    setInfoMessage(INFO_SEND_NEXT_WAVE);

  // every map has a different difficulty needed for hp calulation
  mapDifficulty = getProgMem(mapDifficulties, mapNumber);

  // load map specific coins
  currentCoins = getProgMem(mapStartCoins, mapNumber);

  // reset all towers and enemys and stuff to make screen clear
  tM.init();
  eM.init();
  aM.init();

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
    uint8_t projectileLvl = pM.list[projIndex].getLevel();
    uint8_t projectileBoost = pM.list[projIndex].boost;

    uint8_t projectileDmg = getTowerDamage(projectileType, projectileLvl, projectileBoost);

    //Serial.println("got damage:" + String(projectileDmg) + " type:" + String(projectileType) + " lvl:" + String(projectileLvl));

    // do the damage and decide if it dies
    bool isEnemyDead = eM.list[i].damage(projectileDmg, projectileType);

    // some projectiles will not be destroyed
    bool destroyProjectile = true;

    // railgun projectiles fly trough the whole map, flames destroy themself
    if (projectileType == TOWER_FLAME || projectileType == TOWER_RAILGUN)
      destroyProjectile = false;

    // set flag to destroy this projectile next time
    if (projectileType == TOWER_CANNON) {

      // check if this projectile has already been set to impact/destroy
      destroyProjectile = getBit(pM.list[projIndex].typeActiveLevel, BIT_PROJECTILE_IMPACT);

      // set to impact/destroy next time
      bitSet(pM.list[projIndex].typeActiveLevel, BIT_PROJECTILE_IMPACT);
    }

    //Serial.println("hit of enemy: " + String(i) + " with projectile:" + String(projectileType));

    if (destroyProjectile)
      pM.clearProjectile(projIndex);
  }
}

void mainMenuWaveSender() {

  // change wave types
  waveCounter %= TYPES_OF_WAVES;

  // set wave difficulty range
  waveCounter += 4;

  // start sending a new wave if the last is done
  if (sendWaveStatus == WAVE_FINISHED)
    sendWaveStatus = WAVE_START;
}

void mainScheduler() {

  if (!inMapsListMode(gameMode)) {

    // function will decide if it needs to be cut for menus
    drawMapBorders();

    // the bottom line with all the hints
    drawInfoLine();
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

      shiftMenuOut();

      drawPlayingTowerInfo();

    } else if (gameMode == MODE_PLAYING_END) {
      drawEndGameInfo();
    }

  } else if (inEditorMode(gameMode)) {

    if (gameMode == MODE_EDITOR) {
      shiftMenuOut();

    } else if (gameMode == MODE_EDITOR_MENU) {
      drawEditorMenu();
    }

  } else if (gameMode == MODE_CREDITS) {
    drawCredits();
  }

  // must be drawed at least because of the white parts
  if (gameMode == MODE_PLAYING || gameMode ==  MODE_EDITOR || gameMode ==  MODE_EDITOR_MENU)
    drawCursor();
}


void updateGame() {

  if (mapChanged) {
    // write new cost list with pathfinding algorithm
    mM.findPath();
  }

  mM.drawMap();


  tM.update();

  if (gameMode != MODE_PLAYING_END)
    eM.update();

  pM.update();
  aM.update();

  checkHits();
}

#endif

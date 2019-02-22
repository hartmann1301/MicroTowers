#ifndef Gameplay_h
#define Gameplay_h

void tryToBuildTower() {
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

  // get the needed money
  uint8_t price = getProgMem(towerPrices, tM.list[index].getType());

  // return because player has to less money
  if (currentCoins < price)
    return;

  // buy the upgrade
  currentCoins -= price;

  // write incremented level back
  tM.list[index].setLevel(currentLevel + 1);

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

  // reset all towers
  tM.init();

  for (uint8_t n = 0; n < NODES; n++) {

    // reads from program space or eeprom
    uint8_t data = mM.getStoredMapNode(n, mapNumber);

    // save position of headquarter
    if (data == MAP_TOWER) {
      headquarterPosition = n;

      //Serial.println("headquarterPosition is: " + String(headquarterPosition));
    }

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

void updateGame() {

  mM.drawMap();

  if (mapChanged) {
    mM.findPath();
  }

  tM.update();
  eM.update();
  pM.update();

  checkHits();
}

#endif

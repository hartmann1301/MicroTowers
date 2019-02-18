#ifndef Gameplay_h
#define Gameplay_h

void tryToBuildTower() {
  // put tower to map
  tM.add(xCursor, yCursor, indexBuildMenu);

  // money stuff

  // if tower was placed return to playing mode
  gameMode = MODE_PLAYING;
}

void upgradeTower() {
  // get the tower where the cursor is
  uint8_t index = tM.getTowerAt(xCursor, yCursor);

  //Serial.println("upgradeTower " + String(index));

  uint8_t currentLevel = tM.list[index].lev;

  if (currentLevel == TOWER_LEVEL_MAX)
    return;

  // get the needed money

  // set to next level
  tM.list[index].lev = currentLevel + 1;
}

void sellTower() {
  // get the tower where the cursor is
  uint8_t index = tM.getTowerAt(xCursor, yCursor);

  //Serial.println("sellTower " + String(index));

  // get the money from this tower

  // delete this tower
  tM.sell(index);
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
    if (eM.list[i].active == false)
      continue;

    uint8_t projIndex = pM.isProjectileAt(eM.list[i].x, eM.list[i].y);

    // continue because no index was found
    if (projIndex == 0xff)
      continue;

    uint8_t enemyType = eM.list[i].type;
    uint8_t projectileType = pM.list[projIndex].type;

    uint8_t projectileDmg = 2;
    /*
      if (projectileType == WEAPON_STDGUN) {
      projectileDmg = 30 + weaponLevel * 5;

      } else if (projectileType == WEAPON_ROCKET) {
      projectileDmg = 70 + weaponLevel * 8;
    */

    // do the damage and decide if it dies
    bool isEnemyDead = eM.list[i].damage(projectileDmg);

    //aM.add(impactPosX, pM.list[projIndex].y, ANIMATION_IMPACT_L, 0);

    pM.list[projIndex].active = false;
  }
}

#endif

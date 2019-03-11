#ifndef EEPROMUTILS_H
#define EEPROMUTILS_H

#define EEPROM_EDITOR_SLOTS     EEPROM_STORAGE_SPACE_START
#define EEPROM_UNLOCKED_MAPS    (EEPROM_EDITOR_SLOTS + NODES_COMPRESSED * EDITOR_MAP_SLOTS)
#define EEPROM_MAP_SCORES       (EEPROM_UNLOCKED_MAPS + 1)
#define EEPROM_GAME_KEY         (EEPROM_MAP_SCORES + (MAPS_IN_CAMPAIN + EDITOR_MAP_SLOTS)*2)
#define EEPROM_NEEDED_BYTES     (EEPROM_GAME_KEY + EEPROM_KEY_LEN)

void loadEEPROM () {

#ifdef DEBUG_FRAMES
  // unlock all the maps
  unlockedMaps = MAPS_IN_CAMPAIN;
#else

  // load last unlocked maps, but at least a few
  unlockedMaps = max(EDITOR_MAP_SLOTS, EEPROM.read(EEPROM_UNLOCKED_MAPS));
#endif
}

void updateEEPROMmaps() {

  // update the unlocked maps in the eeprom
  EEPROM.update(EEPROM_UNLOCKED_MAPS, unlockedMaps);
}

void initEEPROM () {

  bool keyInEEPROM = true;

  for (uint8_t i = 0; i < EEPROM_KEY_LEN; i++) {

    // it is a char, but uint8_t is also fine
    uint8_t c = EEPROM.read(EEPROM_GAME_KEY + i);

    if (c != getProgMem(eepromKey, i))
      keyInEEPROM = false;
  }

  // return because key was found in eeprom
  if (keyInEEPROM)
    return;

  // clear all
  for (uint16_t i = EEPROM_STORAGE_SPACE_START; i < EEPROM_GAME_KEY; i++) {

    EEPROM.update(i, 0x00);
  }

  // write key to eeprom
  for (uint8_t i = 0; i < EEPROM_KEY_LEN; i++) {

    EEPROM.update(EEPROM_GAME_KEY + i, getProgMem(eepromKey, i));
  }

#ifdef ESP8266
  EEPROM.commit();
#endif
}

void saveMapToEEPROM(uint8_t mapNumber) {

#ifdef DEBUG_CREATE_MAPS
  Serial.print("Generated following map bytes in slot:");
  Serial.println(mapNumber, DEC);

  for (uint8_t n = 0; n < NODES_COMPRESSED; n++) {

    // get data from global array
    uint8_t mapData = mapComposition[n];

    Serial.print("0x");
    if (mapData <= 0xf)
      Serial.print("0");

    Serial.print(mapData, HEX);
    Serial.print(", ");

    if ((n + 1) % 16 == 0)
      Serial.println();
  }

  Serial.println();
#endif

  for (uint8_t n = 0; n < NODES_COMPRESSED; n++) {

    // where to write in the eeprom
    uint8_t eepromOffset = EEPROM_EDITOR_SLOTS + mapNumber * NODES_COMPRESSED + n;;

    //Serial.println("write to EEPROM at:" + String(eepromOffset) + " data:" + String(mapComposition[n]));
    EEPROM.update(eepromOffset, mapComposition[n]);
  }

#ifdef ESP8266
  EEPROM.commit();
#endif
}

uint16_t get2BytesData(uint16_t address) {

  // it is important to cast to 16 bit variable before shifting!
  uint16_t twoBytes = (uint16_t(EEPROM.read(address)) << 8) + EEPROM.read(address + 1);

  //Serial.println("get2BytesData: " + String(twoBytes));
  return twoBytes;
}

void set2BytesData(uint16_t address, uint16_t data) {
  // write high byte
  EEPROM.update(address, data >> 8);

  // write low byte
  EEPROM.update(address + 1, data & 0xff);

  //Serial.println("write at: " + String(address) + " high byte :" + String(data >> 8));
  //Serial.println("write at: " + String(address + 1) + " low byte :" + String(data & 0xff));

#ifdef ESP8266
  EEPROM.commit();
#endif
}

void clearEepromScores() {

  for (uint8_t i = 0; i < (MAPS_IN_CAMPAIN + EDITOR_MAP_SLOTS) * 2; i++) {
    EEPROM.update(EEPROM_MAP_SCORES + i, 0);
  }

#ifdef ESP8266
  EEPROM.commit();
#endif
}

uint16_t getEepromAddress() {
  // calculate address in eeprom
  if (isInCampainMode) {
    //Serial.println("address: " + String(EEPROM_MAP_SCORES + indexMapsCampain * 2));
    return EEPROM_MAP_SCORES + indexMapsCampain * 2;

  } else {
    //Serial.println("address: " + String(EEPROM_MAP_SCORES + (indexMapsEditor + MAPS_IN_CAMPAIN) * 2));
    return EEPROM_MAP_SCORES + (indexMapsEditor + MAPS_IN_CAMPAIN) * 2;
  }
}

void updateCurrentScore() {
  // reads the current score of this map from eeprom
  currentScore = get2BytesData(getEepromAddress());

  //Serial.println("load saved score " + String(currentScore) + " at address:" + String(getEepromAddress()));
}

void updateEepromScore() {

  // address is depending on campain mode or editor
  uint16_t eepromAdress = getEepromAddress();

  // read currently saved score
  uint16_t savedScore = get2BytesData(eepromAdress);

  //Serial.println("currently saved score " + String(savedScore));
  if (currentScore > savedScore) {

    //Serial.println(" update to " + String(currentScore));
    set2BytesData(eepromAdress, currentScore);
  }
}

void loadMapsFromEEPROM() {

  // trigger new path finding
  mapChanged = true;

  // load map 0
  for (uint8_t n = 0; n < NODES_COMPRESSED; n++) {
    mapComposition[n] = EEPROM.read(EEPROM_EDITOR_SLOTS + n);

    //Serial.println("read EEPROM at:" + String(EEPROM_EDITOR_SLOTS + n) + " data:" + String(mapComposition[n]));
  }

  // load map 1-4
  for (uint8_t n = 0; n < NODES; n++) {
    mapCosts[n] = EEPROM.read(EEPROM_EDITOR_SLOTS + NODES_COMPRESSED + n);

    //Serial.println("read EEPROM at:" + String(EEPROM_EDITOR_SLOTS + NODES_COMPRESSED + n) + " data:" + String(mapCosts[n]));
  }
}


#endif

#ifndef EEPROMUTILS_H
#define EEPROMUTILS_H

#define EEPROM_EDITOR_SLOTS     EEPROM_STORAGE_SPACE_START
#define EEPROM_CURRENT_COINS    (EEPROM_EDITOR_SLOTS + NODES_COMPRESSED * EDITOR_MAP_SLOTS)
#define EEPROM_UNLOCKED_MAPS    (EEPROM_CURRENT_COINS + 1)

void saveMapToEEPROM(uint8_t mapNumber) {

#ifdef PRINT_EDITOR_MAP
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

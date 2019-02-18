#ifndef EEPROMUTILS_H
#define EEPROMUTILS_H

#define EEPROM_EDITOR_LEVEL    EEPROM_STORAGE_SPACE_START
#define EEPROM_CURRENT_COINS   EEPROM_EDITOR_LEVEL + NODES_COMPRESSED;
#define EEPROM_UNLOCKED_MAPS   EEPROM_CURRENT_COINS + 1;

void saveMapToEEPROM() {

#ifdef PRINT_EDITOR_MAP
    Serial.println("Generated following map bytes:");

    for (uint8_t n = 0; n < NODES_COMPRESSED; n++) {

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
    //Serial.println("write to EEPROM + index:" + String(n) + " data:" + String(mapComposition[n]));

    EEPROM.update(EEPROM_EDITOR_LEVEL + n, mapComposition[n]);
  }

#ifdef ESP8266
  EEPROM.commit();
#endif

}

void loadMapFromEEPROM() {

  // trigger new path finding
  mapChanged = true;

  // reset all towers
  tM.init();

  for (uint8_t n = 0; n < NODES_COMPRESSED; n++) {

    mapComposition[n] = EEPROM.read(EEPROM_EDITOR_LEVEL + n);
  }

  // check for the headquarter in the loaded map
  for (uint8_t n = 0; n < NODES; n++) {
    if (mM.getCurrentMapNode(n) == MAP_TOWER) {
      headquarterPosition = n;
    }
  }
}


#endif

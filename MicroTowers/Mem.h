#ifndef ProgramMemory_h
#define ProgramMemory_h

const uint16_t soundFinishedWave [] PROGMEM = {550, 100, 700, 100, 950, 100, 850, 100, TONES_END};
const uint16_t soundSomethingGood [] PROGMEM = {1100, 50, 400, 50, 150, 50, TONES_END};
const uint16_t soundSomethingBad [] PROGMEM = {250, 50, 350, 50, 450, 100, 500, 50, TONES_END};
const uint16_t soundWeaponNormal [] PROGMEM = {150, 50, TONES_END};
const uint16_t soundWeaponLight [] PROGMEM = {350, 50, TONES_END};
const uint16_t soundWeaponWave [] PROGMEM = {50, 50, TONES_END};

// this is also the eeprom key
const uint8_t eepromKey[] PROGMEM  = {"arduTD"};
#define EEPROM_KEY_LEN   6

// GATLING, CANNON, FROST, RAILGUN, FLAME, LASER, SHOCK, SUPPORT

const uint8_t towerDamages [] PROGMEM = {
    95,       60,     10,    150,    22,    50,    20,  BOOST_PRO_LVL
};        

const uint8_t towerCategories [] PROGMEM = {
  C_NORMAL, C_NORMAL,  C_ELSE, C_LIGHT,  C_WAVE,  C_LIGHT,  C_WAVE,  C_ELSE
};

const uint8_t towerBasicPrices [] PROGMEM = {
  10,  12,   18,   22,   30,   38,   40,   22
};

const uint8_t towerExtraPrices [] PROGMEM = {
   6,   8,    4,    6,   15,    2,   20,    4
};

// in frames but will be times 2 because is checked only every second frame
const uint8_t towerReloadTimes [] PROGMEM = {
  6,   11,   8,    12,   50,   50,    0,    0
};

// in pixels but will always be multiplyed by 2 and an offset added
const uint8_t towerBasicRanges [] PROGMEM = {
  4,    5,    3,    3,    2,    4,    1,    4
};

// these values will be added with every extra level
const uint8_t towerExtraRanges [] PROGMEM = {
  2,    3,    2,    4,    2,    4,    1,    0
};

// those values are the projetile starts points from tower center
const int8_t sektorStartX [] PROGMEM = {
  3,  3,  2,  1,  0,  -1,  -2,  -3,  -3,  -3,  -2,  -1,  0,  1,  2,  3
};

const int8_t sektorStartY [] PROGMEM = {
  0,  -1,  -2,  -3,  -3,  -3,  -2,  -1,  0,  1,  2,  3,  3,  3,  2,  1
};

// those help to find index next to tower
const int8_t xPostionsNear [] PROGMEM = {
  -1,  0,  1, -2,  2, -2,  2, -2,  2, -1,  0,  1
};

const int8_t yPostionsNear [] PROGMEM = {
  -2, -2, -2, -1, -1,  0,  0,  1,  1,  2,  2,  2
};

// difficulty is percent if hp a wave has, going over 200% could case uint16_t overflows!
// 1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20
const uint8_t mapDifficulties [] PROGMEM = {
   80,  90, 100, 110, 110, 100,  90, 110,  90, 100, 80, 110, 130,  90,  120, 100, 110, 100,  90, 110,
};

const uint8_t mapStartCoins [] PROGMEM = {
   50,  60,  70,  50,  90, 100, 150, 180,  70,  80,  70,  80, 180,  80,  70,  80,  70,  80, 120, 150
};

// in pixels but will always be multiplyed by 2 and an offset added
const uint8_t xMainMenuTower [] PROGMEM = {
  9,    5,    1,    5,   13,   12,    2,    7
};

// these values will be added with every extra level
const uint8_t yMainMenuTower [] PROGMEM = {
  5,    5,    2,    3,    5,    3,    4,    4
};

#endif

#ifndef GameSprites_h
#define GameSprites_h

/*
  Sounds
*/

/*
  const uint16_t soundStdWeapon [] PROGMEM = {100, 50, 75, 50, 50, 50, TONES_END};
  //const uint16_t soundRocketWeapon [] PROGMEM = {};
  const uint16_t soundLaserWeapon [] PROGMEM = {300, 50, 350, 50, 400, 50, TONES_END};
  const uint16_t soundMGWeapon [] PROGMEM = {150, 50, TONES_END};
  const uint16_t soundFlameWeapon [] PROGMEM = {40, 50, 60, 50, 60, 50, 50, 50, TONES_END};

  const uint16_t soundGotItem [] PROGMEM = {550, 100, 700, 100, 950, 100, 850, 100, TONES_END};
  const uint16_t soundPlayerHit [] PROGMEM = {1100, 50, 400, 50, 150, 50, TONES_END};
  const uint16_t soundShopBuy [] PROGMEM = {250, 50, 350, 50, 450, 100, 500, 50, TONES_END};
*/

// 'allTowers', 56x104px
const unsigned char allTowers [] PROGMEM = {
  0x00, 0x08, 0x08, 0x1c, 0x1c, 0x08, 0x00, 0x00, 0x14, 0x14, 0x1c, 0x1c, 0x08, 0x00, 0x00, 0x08,
  0x2a, 0x2a, 0x3e, 0x3e, 0x08, 0x08, 0x2a, 0x2a, 0x2a, 0x7f, 0x7f, 0x3e, 0x00, 0x10, 0x10, 0x18,
  0x0c, 0x0c, 0x00, 0x00, 0x08, 0x28, 0x3c, 0x3c, 0x0c, 0x00, 0x00, 0x14, 0x14, 0x52, 0x5e, 0x3e,
  0x04, 0x14, 0x14, 0x56, 0x4a, 0x7f, 0x3f, 0x3e, 0x00, 0x00, 0x10, 0x0c, 0x0e, 0x06, 0x00, 0x00,
  0x08, 0x04, 0x2c, 0x1e, 0x06, 0x00, 0x00, 0x48, 0x24, 0x12, 0x4e, 0x2c, 0x18, 0x48, 0x24, 0x13,
  0x4f, 0x2f, 0x1f, 0x1e, 0x00, 0x00, 0x38, 0x0e, 0x06, 0x00, 0x00, 0x00, 0x1c, 0x0c, 0x3e, 0x0e,
  0x00, 0x00, 0x0c, 0x02, 0x3e, 0x06, 0x37, 0x0e, 0x00, 0x1c, 0x07, 0x77, 0x0f, 0x77, 0x1f, 0x06,
  0x00, 0x1c, 0x1c, 0x1c, 0x14, 0x08, 0x00, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x14, 0x08, 0x1c, 0x1c,
  0x1c, 0x1c, 0x3e, 0x36, 0x1c, 0x1c, 0x1c, 0x1c, 0x3e, 0x7f, 0x77, 0x3e, 0x00, 0x30, 0x38, 0x3c,
  0x14, 0x08, 0x00, 0x38, 0x1c, 0x1c, 0x1e, 0x0a, 0x04, 0x00, 0x38, 0x3c, 0x1c, 0x1e, 0x0f, 0x1b,
  0x1e, 0x38, 0x3c, 0x3c, 0x1f, 0x1f, 0x3b, 0x1f, 0x00, 0x10, 0x38, 0x1c, 0x0a, 0x06, 0x00, 0x60,
  0x70, 0x38, 0x1c, 0x0a, 0x06, 0x00, 0x20, 0x70, 0x38, 0x1e, 0x0f, 0x0d, 0x07, 0x60, 0x70, 0x3e,
  0x1f, 0x1f, 0x1d, 0x0f, 0x00, 0x38, 0x3c, 0x1a, 0x0c, 0x00, 0x00, 0x00, 0x40, 0x78, 0x7c, 0x3a,
  0x0c, 0x00, 0x00, 0x60, 0x7b, 0x7f, 0x3d, 0x0f, 0x06, 0x00, 0x72, 0x7f, 0x7f, 0x3d, 0x0f, 0x0f,
  0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1c, 0x08, 0x08, 0x08, 0x08, 0x08, 0x14, 0x1c, 0x08, 0x08,
  0x08, 0x08, 0x14, 0x36, 0x1c, 0x08, 0x08, 0x08, 0x14, 0x36, 0x77, 0x3e, 0x10, 0x10, 0x10, 0x08,
  0x08, 0x0e, 0x18, 0x10, 0x10, 0x10, 0x08, 0x0c, 0x0a, 0x0e, 0x10, 0x10, 0x08, 0x0e, 0x1a, 0x1a,
  0x0e, 0x10, 0x10, 0x18, 0x17, 0x1b, 0x3b, 0x3e, 0x00, 0x20, 0x10, 0x08, 0x04, 0x03, 0x02, 0x00,
  0x20, 0x10, 0x08, 0x06, 0x05, 0x03, 0x00, 0x20, 0x10, 0x0e, 0x0b, 0x0d, 0x07, 0x40, 0x20, 0x1c,
  0x17, 0x1b, 0x0d, 0x0f, 0x00, 0x00, 0x71, 0x0f, 0x02, 0x02, 0x00, 0x00, 0x00, 0x70, 0x0f, 0x05,
  0x03, 0x00, 0x00, 0x00, 0x66, 0x1f, 0x09, 0x0f, 0x00, 0x00, 0x03, 0x7f, 0x17, 0x09, 0x0f, 0x0e,
  0x08, 0x08, 0x14, 0x14, 0x08, 0x00, 0x00, 0x08, 0x08, 0x1c, 0x22, 0x1c, 0x00, 0x00, 0x08, 0x08,
  0x1c, 0x2a, 0x2a, 0x14, 0x00, 0x08, 0x1c, 0x2a, 0x49, 0x6b, 0x36, 0x00, 0x10, 0x18, 0x14, 0x14,
  0x08, 0x00, 0x00, 0x10, 0x1c, 0x1a, 0x12, 0x0c, 0x00, 0x00, 0x10, 0x18, 0x3c, 0x2a, 0x2a, 0x1e,
  0x00, 0x10, 0x1c, 0x3a, 0x29, 0x25, 0x17, 0x1c, 0x00, 0x20, 0x18, 0x14, 0x08, 0x00, 0x00, 0x00,
  0x20, 0x1c, 0x1a, 0x12, 0x0c, 0x00, 0x00, 0x2c, 0x12, 0x2a, 0x26, 0x1c, 0x00, 0x00, 0x2e, 0x13,
  0x29, 0x25, 0x32, 0x1c, 0x00, 0x00, 0x78, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00, 0x78, 0x34, 0x24,
  0x18, 0x00, 0x00, 0x1c, 0x72, 0x3e, 0x12, 0x0e, 0x00, 0x00, 0x1c, 0x73, 0x39, 0x27, 0x12, 0x0e,
  0x00, 0x14, 0x14, 0x14, 0x14, 0x08, 0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x1c, 0x14, 0x14,
  0x14, 0x36, 0x36, 0x36, 0x1c, 0x14, 0x14, 0x36, 0x36, 0x36, 0x77, 0x3e, 0x00, 0x08, 0x28, 0x24,
  0x14, 0x18, 0x00, 0x08, 0x28, 0x24, 0x14, 0x12, 0x0a, 0x0c, 0x08, 0x28, 0x24, 0x16, 0x13, 0x1b,
  0x1e, 0x08, 0x2c, 0x26, 0x37, 0x33, 0x3b, 0x1e, 0x00, 0x08, 0x04, 0x22, 0x14, 0x08, 0x00, 0x10,
  0x08, 0x44, 0x22, 0x16, 0x0c, 0x00, 0x10, 0x08, 0x46, 0x23, 0x17, 0x1e, 0x0c, 0x10, 0x0c, 0x47,
  0x23, 0x37, 0x1f, 0x1e, 0x00, 0x18, 0x06, 0x32, 0x0c, 0x00, 0x00, 0x00, 0x30, 0x0c, 0x63, 0x19,
  0x06, 0x00, 0x00, 0x30, 0x0f, 0x63, 0x19, 0x0f, 0x06, 0x00, 0x3e, 0x0f, 0x63, 0x39, 0x1f, 0x0e,
  0x00, 0x08, 0x14, 0x3e, 0x22, 0x00, 0x00, 0x08, 0x14, 0x2a, 0x36, 0x00, 0x00, 0x00, 0x08, 0x14,
  0x22, 0x49, 0x7f, 0x00, 0x00, 0x08, 0x14, 0x22, 0x41, 0x5d, 0x63, 0x00, 0x00, 0x18, 0x16, 0x2c,
  0x30, 0x20, 0x00, 0x00, 0x18, 0x16, 0x22, 0x2c, 0x30, 0x00, 0x00, 0x18, 0x16, 0x23, 0x2c, 0x70,
  0x00, 0x00, 0x18, 0x16, 0x21, 0x23, 0x4c, 0x70, 0x00, 0x00, 0x1f, 0x12, 0x14, 0x18, 0x10, 0x00,
  0x00, 0x1f, 0x11, 0x16, 0x14, 0x18, 0x00, 0x3f, 0x22, 0x2c, 0x28, 0x30, 0x20, 0x00, 0x3f, 0x21,
  0x26, 0x2c, 0x28, 0x30, 0x00, 0x0e, 0x34, 0x28, 0x18, 0x10, 0x00, 0x00, 0x0e, 0x32, 0x24, 0x14,
  0x18, 0x00, 0x02, 0x0e, 0x32, 0x24, 0x14, 0x18, 0x08, 0x03, 0x0d, 0x31, 0x22, 0x12, 0x14, 0x0c,
  0x00, 0x00, 0x1c, 0x14, 0x1c, 0x00, 0x00, 0x00, 0x1c, 0x3e, 0x36, 0x3e, 0x1c, 0x00, 0x1c, 0x3e,
  0x6b, 0x77, 0x6b, 0x3e, 0x1c, 0x3e, 0x63, 0x5d, 0x55, 0x5d, 0x63, 0x3e, 0x00, 0x08, 0x08, 0x3e,
  0x08, 0x08, 0x00, 0x08, 0x08, 0x08, 0x7f, 0x08, 0x08, 0x08, 0x1c, 0x08, 0x49, 0x7f, 0x49, 0x08,
  0x1c, 0x3c, 0x19, 0x4b, 0x7f, 0x69, 0x4c, 0x1e
};

const unsigned char allMaps [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, // map 1
  0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
  0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x0F, 0x00, 0x20, // map 3
  0x00, 0x00, 0x03, 0xC0, 0x21, 0x00, 0x00, 0x00, 0xC0, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00,
  0xC0, 0x00, 0x00, 0x20, 0x03, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA0, // map 5
  0x40, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x02, 0x00, 0x02, 0xAA, 0xAA, 0xAB, 0xC0, 0x00, 0x00,
  0x00, 0x03, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x04, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, // map test
  0x00, 0x08, 0x02, 0x00, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0xFF,
  0x08, 0x00, 0x00, 0x00, 0xFF, 0xC8, 0xA0, 0x00, 0x00, 0xFC, 0x08, 0x00, 0x00,

  0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x20, 0x02, 0x02, 0x00, 0x00, 0x80, 0x00, // main menu screen
  0x02, 0x00, 0x02, 0x00, 0x00, 0x02, 0x00, 0x80, 0x10, 0x00, 0x00, 0x00, 0x80, 0x00, 0x08, 0x28,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x28, 0x00, 0x00, 0x80, 0x00, 0x80,


  0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x04, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, // map test
  0x00, 0x08, 0x02, 0x00, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0xFF,
  0x08, 0x00, 0x00, 0x00, 0xFF, 0xC8, 0xA0, 0x00, 0x00, 0xFC, 0x08, 0x00, 0x00,
};

// 'mapBlockades', 50x8px
const unsigned char mapBlockades [] PROGMEM = {
  0x60, 0xd0, 0xe8, 0x68, 0x70, 0x30, 0xf8, 0xe8, 0x90, 0x60, 0x70, 0xc8, 0xe8, 0xf0, 0x60, 0x30,
  0xe8, 0xe8, 0xf0, 0x60, 0x00, 0x20, 0x40, 0x00, 0x00, 0x40, 0x00, 0x00, 0x60, 0x00, 0x20, 0x00,
  0x00, 0x00, 0x00, 0x50, 0x64, 0xff, 0x58, 0x60, 0x00, 0x00, 0x40, 0x20, 0x40, 0x08, 0x1c, 0x6c,
  0x54, 0x08
};

// 'mapLocked', 8x16px
const unsigned char mapLocked [] PROGMEM = {
  0xe0, 0x3e, 0x3f, 0xa3, 0xa3, 0x3f, 0x3e, 0xe0, 0x07, 0x04, 0x04, 0x05, 0x05, 0x04, 0x04, 0x07
};

// 'rankSymbols', 15x8px
const unsigned char rankSymbols [] PROGMEM = {
  0x70, 0x38, 0x1c, 0x38, 0x70, 0x48, 0x38, 0x1e, 0x38, 0x48, 0x1c, 0x49, 0x7f, 0x49, 0x1c
};

// 'cursorFrame', 22x16px
const unsigned char cursorFrame [] PROGMEM = {
  0x33, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x66, 0x99, 0x01, 0x00, 0x00, 0x01,
  0x01, 0x00, 0x00, 0x01, 0x01, 0xcc, 0x03, 0x00, 0x04, 0x04, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00,
  0x06, 0x01, 0x04, 0x04, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x04, 0x04
};

// 'symbolSet', 63x8px
const unsigned char symbolSet [] PROGMEM = {
  0x19, 0x41, 0x40, 0x00, 0x01, 0x41, 0x4c, 0x4c, 0x41, 0x01, 0x00, 0x40, 0x41, 0x19, 0x1c, 0x22,
  0x45, 0x5d, 0x41, 0x22, 0x1c, 0x06, 0x0b, 0x17, 0x3e, 0x1f, 0x0f, 0x06, 0x1c, 0x1c, 0x1c, 0x7f,
  0x3e, 0x1c, 0x08, 0x08, 0x0c, 0x7a, 0x01, 0x7a, 0x0c, 0x08, 0x00, 0x00, 0x7b, 0x7b, 0x40, 0x00,
  0x00, 0x2c, 0x2e, 0x2a, 0x6b, 0x2a, 0x3a, 0x1a, 0x7f, 0x3e, 0x1c, 0x7f, 0x3e, 0x1c, 0x08
};


// 'editorSymbole', 20x8px
const unsigned char editorSymbole [] PROGMEM = {
  0x22, 0x14, 0x08, 0x14, 0x22, 0x70, 0x18, 0x5f, 0x1b, 0x73, 0x0c, 0x32, 0x26, 0x3e, 0x1c, 0x04,
  0x0e, 0x36, 0x2a, 0x04
};


/*
   Animations
*/

// 'animationStdGun', 24x4px
const unsigned char animationStdGun [] PROGMEM = {
  0xfe, 0xda, 0xe6, 0xf5, 0x25, 0x3f, 0x69, 0x2b, 0xff, 0xbb, 0x5d, 0xff
};

// 'animationImpact', 16x3px
const unsigned char animationImpact [] PROGMEM = {
  0xfd, 0x96, 0xea, 0x6f, 0xcd, 0xbb
};

// 'animationBoom', 32x10px
const unsigned char animationBoom [] PROGMEM = {
  0xff, 0xff, 0xff, 0xfa, 0xff, 0xff, 0xff, 0x85, 0xff, 0xff, 0xc8, 0x0b, 0xff, 0xfc, 0x80, 0x36,
  0xff, 0xf8, 0x33, 0x4f, 0xff, 0xcb, 0x44, 0xef, 0xfe, 0xac, 0xdd, 0xff, 0xed, 0xdd, 0xde, 0xff,
  0xcc, 0x45, 0x6e, 0xff, 0x91, 0x13, 0x77, 0xff
};

/*
   Enemys
*/

// 'enemyHumans', 18x40px
const unsigned char enemyHumans [] PROGMEM = {
  0x00, 0x20, 0xf8, 0x68, 0xa0, 0x00, 0x00, 0xa0, 0x78, 0xe8, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xe8,
  0x40, 0x00, 0x00, 0x00, 0xf8, 0x68, 0xa0, 0x20, 0x00, 0x80, 0x78, 0xe8, 0x20, 0x20, 0x00, 0x00,
  0xf8, 0xe8, 0x20, 0x20, 0x00, 0x00, 0xf8, 0x68, 0xa0, 0x78, 0x00, 0x80, 0x78, 0xe8, 0x20, 0x78,
  0x00, 0x00, 0xf8, 0xe8, 0x20, 0x78, 0x10, 0xf8, 0x68, 0x90, 0x10, 0x10, 0x90, 0x78, 0xe8, 0x10,
  0x10, 0x10, 0x10, 0xf8, 0xe8, 0x10, 0x10, 0x10, 0x70, 0x78, 0x20, 0xf8, 0x68, 0x80, 0x70, 0x78,
  0xa0, 0x78, 0xe8, 0x00, 0x70, 0x78, 0x20, 0xf8, 0xe8, 0x00
};

// 'enemyTwoPods', 18x40px
const unsigned char enemyTwoPods [] PROGMEM = {
  0x00, 0xee, 0x1f, 0x3b, 0xcd, 0x00, 0xc0, 0x2e, 0x1f, 0xfb, 0x0d, 0x00, 0x00, 0xee, 0x1f, 0xfb,
  0x0d, 0x00, 0x00, 0xf6, 0x0f, 0x3d, 0xc6, 0x00, 0xc0, 0x36, 0x0f, 0xfd, 0x06, 0x00, 0x00, 0xf6,
  0x0f, 0xfd, 0x06, 0x00, 0xe3, 0x97, 0x0f, 0x3d, 0xc6, 0x84, 0x03, 0xf7, 0x8f, 0x3d, 0xc6, 0x84,
  0x03, 0xf7, 0x8f, 0xfd, 0x86, 0x04, 0xe6, 0x97, 0x0f, 0x3d, 0xce, 0x8c, 0x06, 0xf7, 0x8f, 0x3d,
  0xce, 0x8c, 0x06, 0xf7, 0x8f, 0xfd, 0x8e, 0x0c, 0xe3, 0x9f, 0x0f, 0x3d, 0xce, 0x9e, 0x07, 0xf7,
  0x8f, 0x3d, 0xce, 0x9e, 0x07, 0xf7, 0x8f, 0xfd, 0x8e, 0x1e
};

// 'enemyAnimals', 27x40px
const unsigned char enemyAnimals [] PROGMEM = {
  0x80, 0x40, 0x40, 0xe0, 0x60, 0x60, 0xd0, 0x60, 0x40, 0x80, 0x40, 0x40, 0xe0, 0x60, 0xe0, 0x50,
  0x60, 0x40, 0x80, 0x40, 0x40, 0x60, 0xe0, 0x60, 0xd0, 0x60, 0x40, 0x80, 0x60, 0x30, 0xf0, 0x30,
  0x78, 0xa8, 0x30, 0x10, 0x80, 0x60, 0xb0, 0x70, 0x30, 0x38, 0xe8, 0x30, 0x10, 0x80, 0x60, 0x30,
  0xf0, 0x30, 0xf8, 0x28, 0x30, 0x10, 0x00, 0x20, 0x90, 0xf8, 0x78, 0xbc, 0x74, 0x0c, 0x08, 0x00,
  0x40, 0x20, 0xf8, 0x78, 0x3c, 0xf4, 0x0c, 0x08, 0x00, 0x20, 0x90, 0x78, 0xb8, 0xfc, 0x74, 0x0c,
  0x08, 0x10, 0x20, 0x70, 0x70, 0xf0, 0x70, 0xf8, 0x68, 0x10, 0x10, 0x20, 0x70, 0xf0, 0x70, 0x70,
  0xf8, 0x68, 0x10, 0x20, 0x20, 0x70, 0xf0, 0x70, 0xf0, 0x78, 0x68, 0x10, 0x40, 0x20, 0xf0, 0x70,
  0xf8, 0x78, 0x3c, 0x2c, 0x14, 0x40, 0x20, 0xf0, 0x70, 0x78, 0xf8, 0x3c, 0x2c, 0x14, 0x40, 0x20,
  0x70, 0xf0, 0x78, 0x78, 0xbc, 0x2c, 0x14
};

bool getBit(uint8_t data, uint8_t pos) {
  return data & (0b00000001 << pos);
}

uint8_t getHighNibble(uint8_t nib) {
  return (nib & 0xf0) >> 4;
};

uint8_t getLowNibble(uint8_t nib) {
  return nib & 0x0f;
};

void setHighNibble(uint8_t &nib, uint8_t value) {
  nib = (nib & 0x0f) | value << 4;
};

void setLowNibble(uint8_t &nib, uint8_t value) {
  nib = (nib & 0xf0) | value;
};

uint8_t integerSqrt(uint8_t n) {
  if (n < 2)
    return n;

  uint8_t smallCandidate = integerSqrt(n >> 2) << 1;
  uint8_t largeCandidate = smallCandidate + 1;

  if (largeCandidate * largeCandidate > n) {
    return smallCandidate;
  } else {
    return largeCandidate;
  }
}

// big thanks to the following page for the awesome code
// https://www.romanblack.com/integer_degree.htm
uint16_t fastAtan(int16_t x, int16_t y) {

  // Fast XY vector to integer degree algorithm - Jan 2011 www.RomanBlack.com
  // Converts any XY values including 0 to a degree value that should be
  // within +/- 1 degree of the accurate value without needing
  // large slow trig functions like ArcTan() or ArcCos().
  // NOTE! at least one of the X or Y values must be non-zero!
  // This is the full version, for all 4 quadrants and will generate
  // the angle in integer degrees from 0-360.
  // Any values of X and Y are usable including negative values provided
  // they are between -1456 and 1456 so the 16bit multiply does not overflow.

  unsigned char negflag;
  unsigned char tempdegree;
  unsigned char comp;
  unsigned int degree;     // this will hold the result
  //signed int x;            // these hold the XY vector at the start
  //signed int y;            // (and they will be destroyed)
  unsigned int ux;
  unsigned int uy;

  // Save the sign flags then remove signs and get XY as unsigned ints
  negflag = 0;
  if (x < 0)
  {
    negflag += 0x01;    // x flag bit
    x = (0 - x);        // is now +
  }
  ux = x;                // copy to unsigned var before multiply
  if (y < 0)
  {
    negflag += 0x02;    // y flag bit
    y = (0 - y);        // is now +
  }
  uy = y;                // copy to unsigned var before multiply

  // 1. Calc the scaled "degrees"
  if (ux > uy)
  {
    degree = (uy * 45) / ux;   // degree result will be 0-45 range
    negflag += 0x10;    // octant flag bit
  }
  else
  {
    degree = (ux * 45) / uy;   // degree result will be 0-45 range
  }

  // 2. Compensate for the 4 degree error curve
  comp = 0;
  tempdegree = degree;    // use an unsigned char for speed!
  if (tempdegree > 22)     // if top half of range
  {
    if (tempdegree <= 44) comp++;
    if (tempdegree <= 41) comp++;
    if (tempdegree <= 37) comp++;
    if (tempdegree <= 32) comp++; // max is 4 degrees compensated
  }
  else    // else is lower half of range
  {
    if (tempdegree >= 2) comp++;
    if (tempdegree >= 6) comp++;
    if (tempdegree >= 10) comp++;
    if (tempdegree >= 15) comp++; // max is 4 degrees compensated
  }
  degree += comp;   // degree is now accurate to +/- 1 degree!

  // Invert degree if it was X>Y octant, makes 0-45 into 90-45
  if (negflag & 0x10) degree = (90 - degree);

  // 3. Degree is now 0-90 range for this quadrant,
  // need to invert it for whichever quadrant it was in
  if (negflag & 0x02)  // if -Y
  {
    if (negflag & 0x01)  // if -Y -X
      degree = (180 + degree);
    else        // else is -Y +X
      degree = (180 - degree);
  }
  else    // else is +Y
  {
    if (negflag & 0x01)  // if +Y -X
      degree = (360 - degree);
  }

  return degree;
}



#endif

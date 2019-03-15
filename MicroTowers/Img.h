#ifndef Images_h
#define Images_h

// 'allMaps'
const unsigned char allMaps [] PROGMEM = {

  // map 0, litte town in the center
  0xFC, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0xAC, 0x00, 0x00, 0x00,
  0x03, 0xAC, 0x00, 0x00, 0x00, 0x03, 0xEC, 0x00, 0x04, 0x00, 0x03, 0xE8, 0x00, 0x00, 0x00, 0x00,
  0x28, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x0F,

  // map 1, stony but perfect for a labrinth
  0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xC0, 0x00, 0x37, 0x00, 0x02, 0x80, 0xA0, 0x3F, 0x28,
  0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x28, 0x00, 0x00, 0x00, 0xA0,
  0x00, 0xE8, 0x00, 0x00, 0x00, 0x00, 0xE8, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00,

  // map 2, really free with a few trees and hq in top center
  0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0xC3, 0xA4, 0x00, 0x00, 0x0C, 0x02, 0xA0, 0x30, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x30, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x30, 0x00, 0x0F,

  // map 3, stonewall right in front of the entry
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x38,
  0x00, 0x00, 0x00, 0x00, 0xF8, 0x40, 0x00, 0x00, 0x00, 0x38, 0x00, 0x03, 0x00, 0x00, 0x38, 0x00,
  0x0F, 0xC0, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  // map 4, long horizontal rockline  with hq at top left
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAB,
  0xD0, 0x00, 0x00, 0x28, 0x02, 0xC0, 0x00, 0x00, 0xE8, 0x00, 0xAA, 0xAA, 0xAA, 0xBC, 0x00, 0x00,
  0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  // map 5, trees are placed like a chessboard with 2 buildings
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x02, 0x84, 0x00, 0x0F, 0x0F, 0x02, 0x80, 0x00,
  0xC0, 0xA0, 0xF0, 0x00, 0x00, 0xF0, 0xA0, 0xF0, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x0F,
  0x0F, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00,

  // map 6, cracy creepy face
  0x00, 0x3F, 0xFF, 0xF0, 0x00, 0x00, 0x28, 0x00, 0xA0, 0x00, 0x00, 0x22, 0x02, 0x20, 0x00, 0x00,
  0x0A, 0x12, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0xAA, 0x80, 0x00, 0x00, 0x08,
  0x00, 0x80, 0x00, 0x00, 0x02, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  // map 7, single stones and trees over the whole map
  0x03, 0x00, 0x30, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xC8, 0x30, 0x20, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC8, 0x00, 0x00, 0x08, 0x00, 0x00,
  0x00, 0x80, 0x00, 0x20, 0x03, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x03, 0x03, 0x00,

  // map 8, hq in center with a ring of trees around it
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00,
  0x0C, 0x00, 0xC0, 0x00, 0x00, 0x0C, 0x10, 0xC0, 0x00, 0x00, 0x0C, 0x00, 0xC0, 0x00, 0x00, 0x03,
  0x03, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  // map 9, nice town to right, also start map
  0x2C, 0x00, 0x00, 0x00, 0x08, 0xC8, 0x00, 0x00, 0x00, 0x2F, 0xC3, 0xC0, 0x00, 0x3E, 0x83, 0xC3,
  0x00, 0x00, 0x0A, 0xB7, 0xC0, 0x00, 0x3C, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x03,
  0xC0, 0x00, 0xA8, 0x00, 0x8C, 0x08, 0x00, 0xAF, 0xF2, 0xBF, 0x3E, 0x00, 0xFE,

  // map 10, numbers 007 written with stones
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA0, 0x0A, 0x06, 0xA8, 0x02, 0x08, 0x20, 0x80, 0x08, 0x08,
  0x08, 0x80, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x08, 0x08, 0x80, 0x80, 0x80, 0x08, 0x20,
  0x82, 0x00, 0x80, 0x02, 0x80, 0x28, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,

  // map 11, small gap in between a stonewall
  0x0C, 0x20, 0xC2, 0x00, 0x00, 0x00, 0x20, 0xC2, 0x00, 0x00, 0x00, 0x00, 0xC2, 0x00, 0x00, 0x00,
  0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
  0xC2, 0x00, 0x00, 0x00, 0x20, 0xC2, 0x00, 0x00, 0x0C, 0x20, 0xC2, 0x00, 0x00,

  // map 12, all free and hq in left top corner
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  // map 13, horizontal line at front and end open
  0x00, 0x02, 0x80, 0x00, 0x00, 0x00, 0x02, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x2A, 0xAA, 0xAA, 0xAA, 0x84, 0x00, 0x00, 0x00, 0x30, 0x00, 0x0C, 0xC0,
  0x00, 0x03, 0x00, 0xC0, 0x00, 0x00, 0x0C, 0x30, 0xFC, 0x00, 0x00, 0xF0, 0x00,

  // map 14, stonewalls where from top and bottom, little town right
  0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x02, 0x00, 0x02, 0x3A, 0x00, 0x02, 0x00, 0x02, 0xCA, 0x02,
  0x02, 0x02, 0x02, 0xDC, 0x02, 0x02, 0x02, 0x02, 0x83, 0xAA, 0x02, 0x02, 0x02, 0xB0, 0xF0, 0x00,
  0x02, 0x00, 0x00, 0xF0, 0x00, 0x02, 0x00, 0x00, 0xFC, 0x00, 0x02, 0x00, 0x00,

  // map 15, light spot in the forest
  0xC0, 0x00, 0x30, 0x00, 0x0F, 0x00, 0xCC, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x33, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x10, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x30,
  0x00, 0x00, 0x00, 0x0C, 0x0C, 0xC0, 0x00, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x0F,

  // map 16, a forest on the left side, free on the right side
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x04, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0x02,
  0x03, 0xC0, 0x00, 0x00, 0x03, 0xCB, 0x00, 0x00, 0x00, 0x03, 0x33, 0xC0, 0x00, 0x00, 0x00, 0xCB,
  0x00, 0x00, 0x00, 0x00, 0xBC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  // map 17, a bit stonecircle around the whole map
  0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0xAA, 0xAA, 0xAA, 0xA8, 0x20, 0x00, 0x00, 0x00, 0x08, 0x20,
  0x00, 0x01, 0x00, 0x08, 0x20, 0x00, 0x00, 0x00, 0x08, 0xA0, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
  0x00, 0x00, 0x08, 0x2A, 0xAA, 0xAA, 0xAA, 0xA8, 0x00, 0x00, 0x00, 0x00, 0x00,

  // map 18, a wooden path to the hq
  0x00, 0xF0, 0x00, 0x00, 0x00, 0x03, 0xFF, 0x00, 0x03, 0xF4, 0x03, 0x0F, 0x00, 0x0F, 0xF0, 0x3F,
  0x03, 0x00, 0x3C, 0x00, 0xF0, 0x03, 0xC0, 0xF0, 0x00, 0xC0, 0x03, 0x00, 0x30, 0x00, 0x00, 0x00,
  0xCC, 0xC0, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  // map 19, this is the end
  0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xA8, 0x81, 0x2A, 0x00, 0x02, 0x00, 0xA2, 0x20, 0x80, 0x02,
  0x80, 0x8A, 0x20, 0x80, 0x02, 0x00, 0x82, 0x20, 0x80, 0x02, 0xA8, 0x82, 0x2A, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// 'mapBlockades', 50x8px
const unsigned char mapBlockades [] PROGMEM = {
  0x60, 0xd0, 0xe8, 0x68, 0x70, 0x30, 0xf8, 0xe8, 0x90, 0x60, 0x70, 0xc8, 0xe8, 0xf0, 0x60, 0x30,
  0xe8, 0xe8, 0xf0, 0x60, 0x00, 0x20, 0x40, 0x00, 0x00, 0x40, 0x00, 0x00, 0x60, 0x00, 0x20, 0x00,
  0x00, 0x00, 0x00, 0x50, 0x64, 0xff, 0x58, 0x60, 0x00, 0x00, 0x40, 0x20, 0x40, 0x08, 0x1c, 0x6c,
  0x54, 0x08
};

// 'scoreStars', 15x16px
const unsigned char scoreStars [] PROGMEM = {
  0x20, 0x60, 0x60, 0xa0, 0xa0, 0x30, 0x0c, 0x03, 0x0c, 0x30, 0xa0, 0xa0, 0x60, 0x60, 0x20, 0x00,
  0x00, 0x60, 0x3c, 0x13, 0x08, 0x04, 0x06, 0x04, 0x08, 0x13, 0x3c, 0x60, 0x00, 0x00
};

// 'waveTypes', 28x8px
const unsigned char waveTypes [] PROGMEM = {
  0x0e, 0x0a, 0x0a, 0x1b, 0x11, 0x0a, 0x04, 0x1f, 0x13, 0x1f, 0x13, 0x13, 0x17, 0x0e, 0x04, 0x04, 
  0x04, 0x04, 0x15, 0x0a, 0x1b, 0x0a, 0x04, 0x11, 0x0e, 0x00, 0x11, 0x0e
};

// 'mapHouse1', 11x16px
const unsigned char mapHouse1 [] PROGMEM = {
  0xfc, 0x06, 0x56, 0x07, 0x54, 0x04, 0x7c, 0x40, 0xf8, 0x50, 0x00, 0x03, 0x00, 0x03, 0x00, 0x01, 
  0x00, 0x01, 0x00, 0x03, 0x00, 0x00
};
// 'mapHouse2', 11x16px
const unsigned char mapHouse2 [] PROGMEM = {
  0x40, 0xe0, 0x60, 0x7c, 0x06, 0x57, 0x06, 0x7c, 0x60, 0xe0, 0x40, 0x00, 0x03, 0x00, 0x01, 0x00, 
  0x03, 0x00, 0x01, 0x00, 0x03, 0x00
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

// 'symbolSet', 56x8px
const unsigned char symbolSet [] PROGMEM = {
  0x19, 0x41, 0x40, 0x00, 0x01, 0x41, 0x4c, 0x4c, 0x41, 0x01, 0x00, 0x40, 0x41, 0x19, 0x1c, 0x22,
  0x45, 0x5d, 0x41, 0x22, 0x1c, 0x06, 0x0b, 0x17, 0x3e, 0x1f, 0x0f, 0x06, 0x08, 0x0c, 0x3a, 0x01,
  0x3a, 0x0c, 0x08, 0x00, 0x00, 0x3b, 0x3b, 0x20, 0x00, 0x00, 0x2c, 0x2e, 0x2a, 0x6b, 0x2a, 0x3a,
  0x1a, 0x7f, 0x3e, 0x1c, 0x7f, 0x3e, 0x1c, 0x08
};

// 'editorSymbole', 20x8px
const uint8_t editorSymbole [] PROGMEM = {
  0x22, 0x14, 0x08, 0x14, 0x22, 0x70, 0x18, 0x5f, 0x1b, 0x73, 0x0c, 0x32, 0x26, 0x3e, 0x1c, 0x04,
  0x0e, 0x36, 0x2a, 0x04
};

// 'animationImpact', 16x8px
const uint8_t animationImpact [] PROGMEM = {
  0x1c, 0x08, 0x00, 0x00, 0x08, 0x14, 0x1a, 0x08, 0x00, 0x08, 0x14, 0x20, 0x00, 0x00, 0x04, 0x12
};

// 'enemyCyborgs', 18x40px
const unsigned char enemyCyborgs [] PROGMEM = {
  0x00, 0x00, 0xf8, 0x68, 0xa0, 0x20, 0x00, 0x80, 0x78, 0xe8, 0x20, 0x20, 0x00, 0x00, 0xf8, 0xe8, 
  0x20, 0x20, 0x00, 0x20, 0xf8, 0x68, 0xa0, 0x00, 0x00, 0xa0, 0x78, 0xe8, 0x00, 0x00, 0x00, 0x00, 
  0xf8, 0xe8, 0x40, 0x00, 0x00, 0x00, 0xf8, 0x68, 0xa0, 0x78, 0x00, 0x80, 0x78, 0xe8, 0x20, 0x78, 
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

// 'enemyMonsters', 27x40px
const unsigned char enemyMonsters [] PROGMEM = {
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

// 'controllerDesign', 51x32px
const unsigned char controllerDesign [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x38, 0x26, 0x16, 0x16, 0x16, 0x16, 0x16, 0x26, 0x38, 0x40,
  0xc0, 0x40, 0x40, 0x40, 0x40, 0xff, 0x01, 0x3d, 0x3d, 0x2d, 0x35, 0x3d, 0x01, 0xff, 0x40, 0x40,
  0x40, 0x40, 0xc0, 0x40, 0x38, 0x26, 0x16, 0x16, 0x96, 0x16, 0x16, 0x26, 0x38, 0x60, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x80, 0x7c, 0x83, 0x00, 0x00, 0x38, 0x28, 0x10, 0x83, 0x45, 0x83, 0x10,
  0x28, 0x38, 0x00, 0x80, 0x83, 0xfc, 0x80, 0x60, 0x21, 0x62, 0x04, 0xf8, 0x00, 0xf8, 0x04, 0x62,
  0x61, 0x20, 0x80, 0xfc, 0x83, 0x80, 0x38, 0x28, 0x38, 0x00, 0x83, 0x02, 0x83, 0x00, 0x10, 0x28,
  0x10, 0x00, 0x83, 0x7c, 0x80, 0x00, 0xe0, 0x1f, 0x00, 0x01, 0x02, 0x04, 0x08, 0x08, 0x10, 0x11,
  0x11, 0x11, 0x10, 0xbc, 0x42, 0x99, 0x3c, 0x66, 0x66, 0x3c, 0x99, 0x42, 0xbc, 0xc8, 0x7b, 0x0a,
  0x7b, 0xc8, 0xbc, 0x42, 0x99, 0x3c, 0x66, 0x66, 0x3c, 0x99, 0x42, 0xbc, 0x10, 0x12, 0x11, 0x12,
  0x10, 0x08, 0x08, 0x04, 0x02, 0x01, 0x00, 0x1f, 0xe0, 0x3f, 0x40, 0x40, 0x80, 0x80, 0x80, 0x80,
  0x40, 0x40, 0x20, 0x18, 0x04, 0x02, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x03,
  0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x02,
  0x04, 0x18, 0x20, 0x40, 0x40, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x3f
};

// 'allTowers', 56x104px
const unsigned char allTowers [] PROGMEM = {
  0x00, 0x08, 0x08, 0x1c, 0x1c, 0x08, 0x00, 0x00, 0x14, 0x14, 0x1c, 0x1c, 0x08, 0x00, 0x00, 0x08, 
  0x2a, 0x2a, 0x3e, 0x3e, 0x08, 0x08, 0x2a, 0x2a, 0x2a, 0x7f, 0x7f, 0x3e, 0x00, 0x10, 0x10, 0x18, 
  0x0c, 0x0c, 0x00, 0x00, 0x08, 0x28, 0x3c, 0x1c, 0x0c, 0x00, 0x00, 0x14, 0x14, 0x52, 0x5e, 0x3e, 
  0x04, 0x14, 0x14, 0x56, 0x4a, 0x7f, 0x3f, 0x3e, 0x00, 0x00, 0x10, 0x0c, 0x0e, 0x06, 0x00, 0x00, 
  0x08, 0x04, 0x2c, 0x1e, 0x06, 0x00, 0x00, 0x24, 0x12, 0x09, 0x27, 0x16, 0x0c, 0x48, 0x24, 0x13, 
  0x4f, 0x2f, 0x1f, 0x1e, 0x00, 0x00, 0x38, 0x0e, 0x06, 0x00, 0x00, 0x00, 0x18, 0x0c, 0x3e, 0x0e, 
  0x00, 0x00, 0x0c, 0x02, 0x3e, 0x06, 0x37, 0x0e, 0x00, 0x1c, 0x07, 0x77, 0x0f, 0x77, 0x1f, 0x06, 
  0x00, 0x1c, 0x1c, 0x1c, 0x14, 0x08, 0x00, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x14, 0x08, 0x1c, 0x1c, 
  0x1c, 0x1c, 0x3e, 0x36, 0x1c, 0x1c, 0x1c, 0x1c, 0x3e, 0x7f, 0x77, 0x3e, 0x00, 0x30, 0x38, 0x3c, 
  0x14, 0x08, 0x00, 0x18, 0x3c, 0x1c, 0x1e, 0x0a, 0x04, 0x00, 0x38, 0x3c, 0x1c, 0x1e, 0x0f, 0x1b, 
  0x1e, 0x38, 0x3c, 0x3c, 0x1f, 0x1f, 0x3b, 0x1f, 0x00, 0x10, 0x38, 0x1c, 0x0a, 0x04, 0x00, 0x60, 
  0x70, 0x38, 0x1c, 0x0a, 0x04, 0x00, 0x20, 0x70, 0x38, 0x1e, 0x0f, 0x0d, 0x07, 0x60, 0x70, 0x3e, 
  0x1f, 0x1f, 0x1d, 0x0f, 0x00, 0x38, 0x3c, 0x1a, 0x0c, 0x00, 0x00, 0x00, 0x20, 0x78, 0x7c, 0x3a, 
  0x0c, 0x00, 0x00, 0x60, 0x7b, 0x7f, 0x3d, 0x0f, 0x06, 0x00, 0x72, 0x7f, 0x7f, 0x3d, 0x0f, 0x0f, 
  0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1c, 0x08, 0x08, 0x08, 0x08, 0x08, 0x14, 0x1c, 0x08, 0x08, 
  0x08, 0x08, 0x14, 0x36, 0x1c, 0x08, 0x08, 0x08, 0x14, 0x36, 0x77, 0x3e, 0x10, 0x10, 0x10, 0x08, 
  0x08, 0x0e, 0x18, 0x10, 0x10, 0x10, 0x08, 0x0c, 0x0a, 0x0e, 0x10, 0x10, 0x08, 0x0e, 0x1a, 0x1a, 
  0x0e, 0x10, 0x10, 0x18, 0x17, 0x1b, 0x3b, 0x3e, 0x00, 0x20, 0x10, 0x08, 0x04, 0x03, 0x02, 0x00, 
  0x20, 0x10, 0x08, 0x06, 0x05, 0x03, 0x00, 0x20, 0x10, 0x0e, 0x0b, 0x0d, 0x07, 0x40, 0x20, 0x1c, 
  0x17, 0x1b, 0x0d, 0x0f, 0x00, 0x00, 0x71, 0x0f, 0x02, 0x02, 0x00, 0x00, 0x00, 0x70, 0x0f, 0x05, 
  0x03, 0x00, 0x00, 0x00, 0x66, 0x1f, 0x09, 0x0f, 0x00, 0x00, 0x03, 0x7f, 0x17, 0x09, 0x0f, 0x0e, 
  0x00, 0x14, 0x14, 0x14, 0x14, 0x08, 0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x1c, 0x14, 0x14, 
  0x14, 0x36, 0x36, 0x36, 0x1c, 0x14, 0x14, 0x36, 0x36, 0x36, 0x77, 0x3e, 0x00, 0x08, 0x28, 0x24, 
  0x14, 0x18, 0x00, 0x08, 0x28, 0x24, 0x14, 0x12, 0x0a, 0x0c, 0x08, 0x28, 0x24, 0x16, 0x13, 0x1b, 
  0x1e, 0x08, 0x2c, 0x26, 0x37, 0x33, 0x3b, 0x1e, 0x00, 0x08, 0x04, 0x22, 0x14, 0x08, 0x00, 0x10, 
  0x08, 0x44, 0x22, 0x16, 0x0c, 0x00, 0x10, 0x08, 0x46, 0x23, 0x17, 0x1e, 0x0c, 0x10, 0x0c, 0x47, 
  0x23, 0x37, 0x1f, 0x1e, 0x00, 0x18, 0x06, 0x32, 0x0c, 0x00, 0x00, 0x00, 0x30, 0x0c, 0x63, 0x19, 
  0x06, 0x00, 0x00, 0x30, 0x0f, 0x63, 0x19, 0x0f, 0x06, 0x00, 0x3e, 0x0f, 0x63, 0x39, 0x1f, 0x0e, 
  0x08, 0x08, 0x14, 0x14, 0x08, 0x00, 0x00, 0x08, 0x08, 0x1c, 0x22, 0x14, 0x08, 0x00, 0x08, 0x08, 
  0x1c, 0x2a, 0x2a, 0x14, 0x00, 0x08, 0x1c, 0x2a, 0x49, 0x6b, 0x36, 0x00, 0x10, 0x18, 0x14, 0x14, 
  0x08, 0x00, 0x00, 0x10, 0x1c, 0x1a, 0x12, 0x0c, 0x00, 0x00, 0x10, 0x18, 0x3c, 0x2a, 0x2a, 0x1e, 
  0x00, 0x10, 0x1c, 0x3a, 0x29, 0x25, 0x17, 0x1c, 0x00, 0x20, 0x18, 0x14, 0x08, 0x00, 0x00, 0x40, 
  0x38, 0x34, 0x24, 0x18, 0x00, 0x00, 0x00, 0x2c, 0x12, 0x2a, 0x26, 0x1c, 0x00, 0x00, 0x2e, 0x13, 
  0x29, 0x25, 0x32, 0x1c, 0x00, 0x00, 0x3c, 0x12, 0x0c, 0x00, 0x00, 0x00, 0x3c, 0x1a, 0x12, 0x0c, 
  0x00, 0x00, 0x00, 0x1c, 0x72, 0x3e, 0x12, 0x0e, 0x00, 0x00, 0x1c, 0x73, 0x39, 0x27, 0x12, 0x0e, 
  0x00, 0x08, 0x14, 0x3e, 0x22, 0x00, 0x00, 0x00, 0x08, 0x14, 0x2a, 0x36, 0x00, 0x00, 0x08, 0x14, 
  0x22, 0x49, 0x7f, 0x00, 0x00, 0x08, 0x14, 0x22, 0x49, 0x5d, 0x63, 0x00, 0x00, 0x18, 0x16, 0x2c, 
  0x30, 0x20, 0x00, 0x00, 0x18, 0x16, 0x22, 0x2c, 0x30, 0x00, 0x00, 0x18, 0x16, 0x23, 0x2c, 0x70, 
  0x00, 0x00, 0x18, 0x16, 0x21, 0x2b, 0x4c, 0x70, 0x00, 0x3e, 0x24, 0x28, 0x30, 0x20, 0x00, 0x00, 
  0x3e, 0x22, 0x2c, 0x28, 0x30, 0x00, 0x00, 0x3f, 0x22, 0x2c, 0x28, 0x30, 0x20, 0x00, 0x3f, 0x21, 
  0x26, 0x2c, 0x28, 0x30, 0x00, 0x0e, 0x34, 0x28, 0x18, 0x10, 0x00, 0x00, 0x0e, 0x32, 0x24, 0x14, 
  0x18, 0x00, 0x02, 0x0e, 0x32, 0x24, 0x14, 0x18, 0x08, 0x03, 0x0d, 0x31, 0x26, 0x12, 0x14, 0x0c, 
  0x00, 0x00, 0x1c, 0x14, 0x1c, 0x00, 0x00, 0x00, 0x1c, 0x3e, 0x36, 0x3e, 0x1c, 0x00, 0x1c, 0x3e, 
  0x6b, 0x77, 0x6b, 0x3e, 0x1c, 0x3e, 0x63, 0x5d, 0x55, 0x5d, 0x63, 0x3e, 0x00, 0x08, 0x08, 0x3e, 
  0x08, 0x08, 0x00, 0x08, 0x08, 0x08, 0x7f, 0x08, 0x08, 0x08, 0x1c, 0x08, 0x49, 0x7f, 0x49, 0x08, 
  0x1c, 0x3c, 0x19, 0x4b, 0x7f, 0x69, 0x4c, 0x1e
};


#endif

#ifndef Globals_h
#define Globals_h

#include <Arduino.h>
#include <EEPROM.h>
#include <Arduboy2.h>
#include <ArduboyTones.h>

// mSelectedMenuItem == 0 ? 12 : mSelectedMenuItem - 1;

#define USE_SERIAL
//#define DEBUG_ADD_FUNCTIONS
#define DEBUG_FRAME_TIME

#ifdef ESP8266
#define PS2_DAT       D6 // brown/green
#define PS2_CMD       D0 // orange 
#define PS2_SEL       D5 // yellow
#define PS2_CLK       D8 // blue
SSD1306Brzo oled(OLED_I2C_ADRESS, D2, D1);

#include <PS2X_lib.h>
PS2X ps2x;
#endif

Arduboy2Base arduboy;
ArduboyTones sound(arduboy.audio.enabled);

#define GAME_FRAMES     30
#define GAME_FRAME_MS  (1000 / GAME_FRAMES)

void getButtons() {
#ifdef ESP8266
  // this function is called inside arduboy whenever buttonStates() is called
  arduboy.setExternalButtons(ps2x.getArduboyButtons());
#endif
}

#define COLUMNS             9
#define ROWS                20
#define NODES               (ROWS*COLUMNS)
#define NODES_COMPRESSED    (NODES / 4)

#define BUFFER_MAX          (WIDTH*HEIGHT/8)

#define RASTER              6
#define HALF_RASTER         (RASTER / 2)

#define RASTER_OFFSET_X     1
#define RASTER_OFFSET_Y     0

#define LONGPRESS_INFO      15
#define LONGPRESS_TIME      30

#define MAINMENU_ITEMS      4
#define MAPSLIST_ITEMS      6

#define MENU_ITEMS_BUILD    8
#define MENU_ITEMS_EDITOR   4
#define MENU_ITEMS_TOWER    3

#define TOWER_LEVEL_MAX     3

#define ICON_HEIGHT         7  
#define ICON_WIDTH          ICON_HEIGHT

#define INT8_MIN            -128
#define INT8_MAX            127

// this buffer is for the drawing function
uint8_t* buffer = arduboy.getBuffer();

enum {
  MODE_MAINMENU = 0,
  MODE_MAPS_LIST,
  MODE_PLAYING_INFO,  
  MODE_PLAYING,
  MODE_PLAYING_BUILD,
  MODE_PLAYING_TOWER,
  MODE_EDITOR,
  MODE_EDITOR_MENU,
  MODE_OPTIONS,  
  MODE_CREDITS
};
uint8_t gameMode;

// in this array the look and the behaviour of the current map is stored
uint8_t mapComposition[NODES / 4];

// in this array are the calculated costs from pathfinding
uint8_t mapCosts[NODES];

uint8_t headquarterPosition = 0;

uint8_t xPosRightMenu = 128;
bool cursorPressed = false;

bool isNormalSpeed = true;
bool mapChanged = true;

uint32_t nextButtonInput = 0;
uint32_t gameFrames = 0;
bool isFramesMod2 = true;

uint32_t normalSpeedTime = 0;
bool isFastSpeedFrame = false;

int8_t stateButtonA = 0; 
int8_t stateButtonB = 0;

uint8_t xCursor = 1;
uint8_t yCursor = 4;
uint8_t statCursor = 0;

uint8_t indexMainMenu = 0;
uint8_t indexBuildMenu = 0;
uint8_t indexTowerMenu = 0;
uint8_t indexMapsList = 0;
uint8_t indexOptions = 0;

int16_t indexMainMenuDelayed = 0;
int16_t indexMapsListDelayed = 0;

//eeprom data
uint8_t unlockedMaps = 5;
uint8_t currentCoins = 189;
//

bool isLongPressInfo(int8_t buttonState) {
  return buttonState > LONGPRESS_INFO;
}

bool isLongPressed(int8_t buttonState) {
  return buttonState > LONGPRESS_TIME;
}

bool inPlayingMode(uint8_t m) {
  return m >= MODE_PLAYING_INFO && m <= MODE_PLAYING_TOWER;
}

bool inEditorMode(uint8_t m) {
  return m == MODE_EDITOR || m == MODE_EDITOR_MENU;
}

enum {
  GO_RIGHT = 0,
  GO_UP,
  GO_LEFT,  
  GO_DOWN,
  NO_DIRECTION
};

enum {
  SYMBOL_CURSOR1 = 0,
  SYMBOL_CURSOR2,  
  SYMBOL_COIN,
  SYMBOL_HEART,
  SYMBOL_WAVE,
  SYMBOL_UPGRADE,  
  SYMBOL_INFO,
  SYMBOL_SELL,
  SYMBOL_FASTMODE
};

enum {
  EDITOR_DELETE = 0,  
  EDITOR_HQ,
  EDITOR_ROCK,
  EDITOR_TREE
};

enum {
  TOWER_MENU_UPGRADE = 0,  
  TOWER_MENU_INFO,
  TOWER_MENU_SELL
};

enum {
  TOWER_GATLING = 0,
  TOWER_CANNON,
  TOWER_FROST,
  TOWER_RAILGUN,    
  TOWER_FLAME,
  TOWER_LASER,   
  TOWER_SHOCK,  
  TOWER_SUPPORT
};

bool isRotatingTower(uint8_t type) {
  return type <= TOWER_LASER;
}

const uint8_t towerPrices [] PROGMEM = {
  10,  15,  10,  25,  35,  15,  20,  20  
};

// in frames but will be times 2 because is checked only every second frame
const uint8_t towerReloadTimes [] PROGMEM = {
  5,  12,  12,  8
}; // deleted laser, shock and support tower in this list

// in pixels but will always be multiplyed by 2 and an offset added
const uint8_t towerBasicRanges [] PROGMEM = {
  4,  5,  8,  2,  0,  10,  2,  0  
}; 

// these values will be added with every extra level
const uint8_t towerExtraRanges [] PROGMEM = {
  2,  2,  1,  3,  0,  3,  1,  0  
}; 

const int8_t sektorStartX [] PROGMEM = {
  3,  3,  2,  1,  0,  -1,  -2,  -3,  -3,  -3,  -2,  -1,  0,  1,  2,  3    
};

const int8_t sektorStartY [] PROGMEM = {
  0,  -1,  -2,  -3,  -3,  -3,  -2,  -1,  0,  1,  2,  3,  3,  3,  2,  1 
};

uint8_t getProgMem(const uint8_t *pointer, uint8_t offset) {
  return pgm_read_byte(pointer + offset);
}

int8_t getDirectionX(uint8_t sektor) {
  return pgm_read_byte(sektorStartX + sektor);
}

int8_t getDirectionY(uint8_t sektor) {
  return pgm_read_byte(sektorStartY + sektor);
}

#endif

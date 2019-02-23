#ifndef Globals_h
#define Globals_h

#include <Arduino.h>
#include <EEPROM.h>
#include <Arduboy2.h>
#include <ArduboyTones.h>

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

#define FRAMES_PRO_SEC     30
#define MS_PRO_FRAME      (1000 / FRAMES_PRO_SEC)

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
#define ENTRY_POSITION      ((COLUMNS / 2) * ROWS)

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

#define ENEMYS_IN_WAVE      5
#define TYPES_OF_WAVES      6
#define MAXIMAL_WAVE        30

// in gameFrames to work in fast mode
#define NEXT_ENEMY_TIMEOUT  FRAMES_PRO_SEC
#define NEXT_WAVE_TIMEOUT   FRAMES_PRO_SEC * 8

#define ICON_HEIGHT         7
#define ICON_WIDTH          ICON_HEIGHT

#define INT8_MIN            -128
#define INT8_MAX            127

// rotations for the draw bitmap slow function
#define NOROT 0
#define ROTCCW 1
#define ROT180 2
#define ROTCW 3

// this buffer is for the drawing function
uint8_t* buffer = arduboy.getBuffer();

uint8_t gameMode;

uint8_t infoMsgTimeout = 0;
uint8_t infoMsgType = 0;

// in this array the look and the behaviour of the current map is stored
uint8_t mapComposition[NODES / 4];

// in this array are the calculated costs from pathfinding
uint8_t mapCosts[NODES];

uint8_t headquarterPosition = 0;

uint8_t xPosRightMenu = 128;
bool cursorPressed = false;

bool isNormalSpeed = true;
bool mapChanged = true;

bool isFramesMod2;

uint32_t nextButtonInput = 0;
uint32_t gameFrames = 0;

uint8_t currentMapDifficulty;
uint16_t currentWaveHp;

uint8_t currentLifePoints;
uint8_t currentWaveCounter = 1;

uint8_t currentEnemysOfWave = 0;
uint8_t currentEnemysRace = 0;
uint32_t nextEnemyTime = 0;

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

enum {
  ENEMY_TYPE_CYBORG = 0,
  ENEMY_TYPE_TWOPOD,
  ENEMY_TYPE_MONSTER
};

enum {
  ENEMY_IS_DEFAULT = 0,
  ENEMY_IS_FAST,
  ENEMY_RESITS_BULLETS,
  ENEMY_RESITS_LASERS,
  ENEMY_RESITS_AOES,
  ENEMY_MIX
};

enum {
  MAP_FREE = 0,
  MAP_TOWER,
  MAP_ROCK,
  MAP_NOBUILD,
};

enum {
  INFO_FORBIDDEN_BUILD = 0,
  INFO_BLOCKED_AREA,
  INFO_ENTRY_BLOCK,
  INFO_JUST_A_HOUSE
};

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

#endif

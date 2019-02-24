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

// in gameFrames to work also in fast mode
#define NEXT_ENEMY_TIMEOUT  FRAMES_PRO_SEC

#define ICON_HEIGHT         7
#define ICON_WIDTH          ICON_HEIGHT

// min and max value of a int8_t
#define INT8_MIN            -128
#define INT8_MAX            127

// rotations for the draw bitmap slow function
#define NOROT 0
#define ROTCCW 1
#define ROT180 2
#define ROTCW 3

// this pointer to the display buffer is for my drawing functions
uint8_t* buffer = arduboy.getBuffer();

// holds the main status of this game
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

// will be indecremted everytime it is shown
uint8_t infoMsgTimeout = 0;

// stores what kind of message is shown in the info bar
enum {
  INFO_FORBIDDEN_BUILD = 0,
  INFO_BLOCKED_AREA,
  INFO_ENTRY_BLOCK,
  INFO_JUST_A_HOUSE,
  INFO_SEND_NEXT_WAVE
};
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

// every frame is counted, is used for "every n frames" functions and for fast forward
uint32_t gameFrames = 0;

// is calculated at he start of a wave and stored to draw the hp lines over enemys easier
uint16_t currentWaveHp;

// when a map is loaded the difficultiy is stored here and used for enemy hp calculations
uint8_t currentMapDifficulty;

// while playing in a level the life points where decremented when an enemy reaches your hq
uint8_t currentLifePoints;

// counts the waves in a level from 1 to 30 
uint8_t waveCounter = 0;

// your coins to buy new towers or upgrade existing towers, could overflow!! (uint16_t)
uint8_t currentCoins = 189;

// on wave start ENEMYS_IN_WAVE is loaded to the variable and than decremented
int8_t enemysOfWave = 0;

// every wave needs to be started by the player, see enum
enum {
  WAVE_START = 0,
  WAVE_ACTIVE,
  WAVE_FINISHED
};
uint8_t sendWaveStatus = WAVE_FINISHED;

// stores how the current enemys are looking
enum {
  ENEMY_TYPE_CYBORG = 0,
  ENEMY_TYPE_TWOPOD,
  ENEMY_TYPE_MONSTER
};
uint8_t currentEnemysType = ENEMY_TYPE_CYBORG;

// while a wave is active this variable stores the gameFrames until next enemy is stored
uint32_t nextEnemyFrame = 0;

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

//



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

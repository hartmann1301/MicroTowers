#ifndef Globals_h
#define Globals_h

#include <Arduino.h>
#include <EEPROM.h>
#include <Arduboy2.h>
#include <ArduboyTones.h>

#ifdef ESP8266

#define USE_SERIAL

//#define DEBUG_FRAMES
//#define DEBUG_ADD_FUNCTIONS
//#define DEBUG_PERFORMANCE
//#define DEBUG_CAMPAIN_STARS
#define DEBUG_FAKE_SCORES

#define PS2_DAT       D6 // brown/green
#define PS2_CMD       D0 // orange 
#define PS2_SEL       D5 // yellow
#define PS2_CLK       D8 // blue
SSD1306Brzo oled(OLED_I2C_ADRESS, D2, D1);

#include <PS2X_lib.h>
PS2X ps2x;
#endif

#ifdef DEBUG_FRAMES
uint8_t framesNow = 0;
uint8_t framesCurrent = 0;
uint8_t secondsNow = 0;
uint8_t secondsCurrent = 255;
#endif

Arduboy2Base arduboy;
ArduboyTones sound(arduboy.audio.enabled);

#define FRAMES_PRO_SEC      30
#define MS_PRO_FRAME        (1000 / FRAMES_PRO_SEC)

#define COLUMNS             9
#define ROWS                20
#define NODES               (ROWS*COLUMNS)
#define NODES_COMPRESSED    (NODES / 4)
#define ENTRY_POSITION      ((COLUMNS / 2) * ROWS)

#define IS_CLOSED           true
#define NOT_CLOSED          false

#define NO_PRICE            255
#define NO_INDEX            255
#define BUFFER_MAX          (WIDTH*HEIGHT/8)

#define RASTER              6
#define HALF_RASTER         (RASTER / 2)
#define SEKTORS             16

// is used for calculation the reward for killing enemies
#define HP_FOR_A_COIN       500
#define HP_INCREASE_FAKTOR  68

// set how the boost tower boost every other tower around it
#define BOOST_PRO_LVL       15

#define MAPS_IN_CAMPAIN     20
#define POINTS_PRO_STAR     250

#define RASTER_OFFSET_X     1
#define RASTER_OFFSET_Y     0

#define LONGPRESS_INFO      15
#define LONGPRESS_TIME      30

#define MAINMENU_ITEMS      5
#define MAPS_IN_CAMPAIN     20
#define EDITOR_MAP_SLOTS    5

#define MENU_ITEMS_BUILD    8
#define MENU_ITEMS_EDITOR   4
#define MENU_ITEMS_TOWER    3

#define TOWER_LEVEL_MAX     3

#define TYPES_OF_ENEMIES    5
#define ENEMYS_IN_WAVE      5
#define TYPES_OF_WAVES      6
#define MAXIMAL_WAVE        30

#define MENU_RIGHT_MAX      132
#define MENU_RIGHT_MIN      121

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

// holds the main status of this game
enum {
  MODE_MAINMENU = 0,
  MODE_MAPS_CAMPAIN,
  MODE_MAPS_EDITOR,
  MODE_PLAYING_INFO,
  MODE_PLAYING,
  MODE_PLAYING_BUILD,
  MODE_PLAYING_TOWER,
  MODE_PLAYING_END,
  MODE_EDITOR,
  MODE_EDITOR_MENU,
  MODE_ENEMIES,
  MODE_CREDITS
};
uint8_t gameMode;

// the items in the main menu
enum {
  MAIN_CAMPAIN = 0,
  MAIN_EDITOR,
  MAIN_ENEMIES,
  MAIN_CREDITS,
  MAIN_SOUND
};

// will be indecremted everytime it is shown
uint8_t infoMsgTimeout = 0;

// stores what kind of message is shown in the info bar
enum {
  INFO_FORBIDDEN_BUILD = 0,
  INFO_BLOCKED_AREA,
  INFO_ENTRY_BLOCK,
  INFO_JUST_A_HOUSE,
  INFO_SEND_NEXT_WAVE,
  INFO_TO_LESS_COINS,
};
uint8_t infoMsgType = 0;

// in this array the look and the behaviour of the current map is stored
uint8_t mapComposition[NODES / 4];

// in this array are the calculated costs from pathfinding
uint8_t mapCosts[NODES];

uint8_t headquarterPosition = 0;

// the shared x coordinate for all right menus
uint8_t xPosRightMenu = MENU_RIGHT_MAX;

bool cursorPressed = false;

bool isNormalSpeed = true;
bool mapChanged = true;
bool controlRailgunTower = false;
bool isInCampainMode;

// this is uint8_t because of the check button function it is used like a bool 
uint8_t isInEditMode = 1;

bool isFramesMod2;

// houlds the time when next button can be pressed
uint32_t nextButtonInput = 0;

// every frame is counted, is used for "every n frames" functions and for fast forward
uint32_t gameFrames = 0;

// is calculated at he start of a wave and stored to draw the hp lines over enemys easier
uint16_t currentWaveHp;

// when a map is loaded the difficultiy is stored here and used for enemy hp calculations
uint8_t mapDifficulty;

// while playing in a level the life points where decremented when an enemy reaches your hq
uint8_t lifePoints;

// counts the waves in a level from 1 to 30
uint8_t waveCounter = 0;

// holds what kind of enemy wave is currently spawning
uint8_t waveType = 0;

// your coins to buy new towers or upgrade existing towers, could overflow!! (uint16_t)
uint16_t currentCoins;

// this is for the maps list score
uint16_t currentScore;

// this holds the lvl of the tower where the cursor is in menu
uint8_t currentTowerLvl;

// before entering build or tower menu the index from tM.list is saved here
uint8_t towerIndex;

// before entering tower menu those are saved
uint8_t towerLevel;
uint8_t towerType;

// on wave start ENEMYS_IN_WAVE is loaded to the variable and than decremented
int8_t enemysOfWave;

// every wave needs to be started by the player, see enum
enum {
  WAVE_START = 0,
  WAVE_ACTIVE,
  WAVE_FINISHED
};
uint8_t sendWaveStatus = WAVE_FINISHED;

// stores how the current enemys are looking
enum {
  ENEMY_RACE_CYBORG = 0,
  ENEMY_RACE_TWOPOD,
  ENEMY_RACE_MONSTER
};
uint8_t enemysRace = ENEMY_RACE_CYBORG;

// the more stars the more unlocked maps you get
uint8_t campainStars = 0;

// while a wave is active this variable stores the gameFrames until next enemy is stored
uint32_t nextEnemyFrame = 0;

uint32_t normalSpeedTime = 0;
bool isFastSpeedFrame = false;

int8_t stateButtonA = 0;
int8_t stateButtonB = 0;
bool foundLongPress = false;

uint8_t xCursor = 1;
uint8_t yCursor = 4;
uint8_t statCursor = 0;

uint8_t indexMainMenu = 0;
uint8_t indexBuildMenu = 0;
uint8_t indexTowerMenu = 0;
uint8_t indexMapsCampain = 0;
uint8_t indexMapsEditor = 0;
uint8_t indexOptions = 0;

enum {
  TOWER_GATLING = 0,
  TOWER_CANNON,
  TOWER_FROST,
  TOWER_RAILGUN,
  TOWER_FLAME,
  TOWER_LASER,
  TOWER_SHOCK,
  TOWER_SUPPORT,
  TOWER_PROTOTYPE
};

enum {
  C_NORMAL = 0,
  C_LIGHT,
  C_WAVE,
  C_ELSE
};

enum {
  ENEMY_IS_DEFAULT = 0,
  ENEMY_IS_FAST,
  ENEMY_RESITS_NORMAL,
  ENEMY_RESITS_LIGHT,
  ENEMY_RESITS_WAVES,
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
  SYMBOL_UPGRADE,
  SYMBOL_INFO,
  SYMBOL_SELL,
  SYMBOL_FASTMODE
};

enum {
  ANIMATION_IMPACT = 0,
  ANIMATION_RESIST
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

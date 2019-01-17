#ifndef Globals_h
#define Globals_h

#include <Arduino.h>
#include <math.h>

#include <Arduboy2.h>
#include <ArduboyTones.h>

// just an or || mSelectedMenuItem == 0 ? 12 : mSelectedMenuItem - 1;

#define USE_SERIAL
//#define DEBUG_ADD_FUNCTIONS
//#define DEGUG_DMG_PLAYER
//#define DEGUG_DMG_ENEMYS
//#define DEBUG_FRAME_TIME

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

#define GAME_FRAMES 30

void getButtons() {
#ifdef ESP8266
  // this function is called inside arduboy whenever buttonStates() is called
  arduboy.setExternalButtons(ps2x.getArduboyButtons());
#endif
}

// this is better not even
#define MOVE_MAP_INTERVAL     3

#define RASTER 6
#define RASTER_X_OFFSET 1
#define RASTER_Y_OFFSET 2

bool isPlayMode = false;
bool isGameOver = false;
bool isStartAnimation = true;

uint32_t nextButtonInput = 0;
uint32_t gameFrames = 0;

int8_t xCursor = 0;
int8_t yCursor = 0;
uint8_t statCursor = 0;
  
enum {
  TOWER_SHOCK = 0,  
  TOWER_FLAK,
  TOWER_SILO,  
  TOWER_HELP,
  TOWER_MG,
  TOWER_CANON,
  TOWER_LASER,
  TOWER_FLAME
};

#endif

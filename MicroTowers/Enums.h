#ifndef Enumerations_h
#define Enumerations_h

// all buildable tower types
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

// holds the main status of this game
enum class GameMode {
  MAINMENU = 0,
  MAPS_CAMPAIN,
  MAPS_EDITOR,
  PLAYING_INFO,
  PLAYING,
  PLAYING_BUILD,
  PLAYING_TOWER,
  PLAYING_END,
  EDITOR,
  MENU_EDITOR_MENU,
  ENEMIES,
  CREDITS
};

// the items in the main menu
enum {
  MAIN_CAMPAIN = 0,
  MAIN_EDITOR,
  MAIN_ENEMIES,
  MAIN_CREDITS,
  MAIN_SOUND
};

// side menu icon order in editor
enum {
  MENU_EDITOR_DELETE = 0,
  MENU_EDITOR_HQ,
  MENU_EDITOR_ROCK,
  MENU_EDITOR_TREE
};

// side menu icon order on a tower
enum {
  MENU_TOWER_UPGRADE = 0,
  MENU_TOWER_INFO,
  MENU_TOWER_SELL
};

// stores what kind of message is shown in the info bar
enum class infoPopup {
  FORBIDDEN_BUILD = 0,
  BLOCKED_AREA,
  ENTRY_BLOCK,
  JUST_A_HOUSE,
  SEND_NEXT_WAVE,
  TO_LESS_COINS,
};

// every wave needs to be started by the player, see enum
enum class WaveStatus{
  START = 0,
  ACTIVE,
  FINISHED
};

// stores how the current enemys are looking
enum class EnemyRace{
  CYBORG = 0,
  TWOPOD,
  MONSTER
};

// categories of the damage the tower will do
enum {
  C_NORMAL = 0,
  C_LIGHT,
  C_WAVE,
  C_ELSE
};

// the different enemy groups that can spawn
enum {
  ENEMY_IS_DEFAULT = 0,
  ENEMY_IS_FAST,
  ENEMY_RESITS_NORMAL,
  ENEMY_RESITS_LIGHT,
  ENEMY_RESITS_WAVES,
  ENEMY_MIX
};

// states of a map tile
enum class MapTile {
  FREE = 0,
  TOWER,
  ROCK,
  NOBUILD,
};

// a direction where a enemy will go next
enum {
  GO_RIGHT = 0,
  GO_UP,
  GO_LEFT,
  GO_DOWN,
  NO_DIRECTION
};

// the order these symbols are placed in progmem symbolSet array
enum SymbolSet{
  SYMBOL_CURSOR1 = 0,
  SYMBOL_CURSOR2,
  SYMBOL_COIN,
  SYMBOL_HEART,
  SYMBOL_UPGRADE,
  SYMBOL_INFO,
  SYMBOL_SELL,
  SYMBOL_FASTMODE
};

#endif

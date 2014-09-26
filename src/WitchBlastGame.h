/**  This file is part of Witch Blast.
  *
  *  Witch Blast is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  Witch Blast is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with Witch Blast.  If not, see <http://www.gnu.org/licenses/>.
  */

#ifndef WITCH_BLAST_GAME_H
#define WITCH_BLAST_GAME_H

#include "sfml_game/Game.h"
#include "sfml_game/TileMapEntity.h"
#include "PlayerEntity.h"
#include "EnemyEntity.h"
#include "DoorEntity.h"
#include "GameFloor.h"
#include "Config.h"

#include <queue>

const int ALIGN_LEFT    = 0;  /*!< Text alignment left */
const int ALIGN_RIGHT   = 1;  /*!< Text alignment right */
const int ALIGN_CENTER  = 2;  /*!< Text alignment centered */

const int X_GAME_FADE_IN  = 0;
const int X_GAME_FADE_OUT = 1;

unsigned const int NumberKeys = 12;
const std::string inputKeyString[NumberKeys] =
{
  "key_move_up",
  "key_move_down",
  "key_move_left",
  "key_move_right",
  "key_fire_up",
  "key_fire_down",
  "key_fire_left",
  "key_fire_right",
  "key_fire_select",
  "key_spell",
  "key_time",
  "key_fire"
};

/*! \class WitchBlastGame
* \brief Main class of the game
*
*  WitchBlastGame load the game ressources and do a large part of the game logic (game loop...),
*  watches the input, manages the game states, ...
*/
class WitchBlastGame : public Game
{
public:
  /*!
   *  \brief Constructor
   *
   *  Constructor of the WitchBlastGame class.
   */
  WitchBlastGame();

  /*!
   *  \brief Destructor
   *
   *  Destructor of the WitchBlastGame class.
   */
  virtual ~WitchBlastGame();

  /*!
  *  \brief Accessor on the current dungeon map
  *
  *  Accessor on the current dungeon map (room).
  *
  *  \return a pointer to the current dungeon map
  */
  DungeonMap* getCurrentMap();

  /*!
   *  \brief Accessor on the player
   *
   *  Accessor on the player.
   *
   *  \return a pointer to the player
   */
  PlayerEntity* getPlayer();

  /*!
   *  \brief Accessor on the player's position
   *
   *  Accessor on the player's position.
   *
   *  \return a Vector2D of the player's position
   */
  Vector2D getPlayerPosition();

  /*!
   *  \brief accessor on the level
   *
   *  Accessor on the level.
   *
   *  \return : the level
   */
  int getLevel();

  /*!
   *  \brief accessor on showLogical flag
   *  \return : the value of the flag
   */
  bool getShowLogical();

  /*!
   *  \brief Start the game and the game loop
   *
   *  This method starts the game and the game loop.
   *  The game loop watches the events, the inputs, update and draw the game elements.
   */
  virtual void startGame();

  /*!
  *  \brief Move the player to another map (room)
  *
  *  Moves the player to another room of the dungeon.
  *  It's called when a player walk through an opened door.
  *
  *  \param direction : direction of the new map. 4 = left, 8 = north, 6 = right, 2 = south
  */
  void moveToOtherMap(int direction);

  /*!
   *  \brief Closes the doors of the room
   *
   *  Closes the doors of the room.
   *  It's called when the player enter in an area with monsters.
   */
  void closeDoors();

  /*!
   *  \brief Opens the doors of the room
   *
   *  Opens the doors of the room.
   *  It's called when the player destroy the last monster of a room.
   */
  void openDoors();

  /*!
  *  \brief Count the enemies in the room
  *
  *  Count the enemies in the room.
  *  It's called when the room is closed : 0 enemy = room is cleared and doors shall open.
  *
  *  \return amount of enemies
  */
  int getEnnemyCount();

  /*!
  *  \brief Return the position of the nearest enemy
  *
  *  \param x : x position of the source
  *  \param y : y position of the source
  *  \return amount of enemies
  */
  Vector2D getNearestEnnemy(float x, float y);


  /*!
  *  \brief Generates blood
  *
  *  Generates blood, it's called when someone is hurt or dies.
  *
  *  \param x : x position of the blood
  *  \param y : y position of the blood
  *  \param bloodColor : color of the blood (red; green, ...)
  */
  void generateBlood(float x, float y, BaseCreatureEntity::enumBloodColor bloodColor);

  /*!
   *  \brief Show a "popup" with artefact's description
   *
   *  Show a "popup" with artefact's description.
   *  Artefact's description consists of zoomed picture of the item, name and description.
   *  It's called when a player get an equipment item.
   *
   *  \param itemType : item identifier
   */
  void showArtefactDescription(enumItemType itemType);

  /*!
   *  \brief Make a "shake" effet
   *  \param duration : duration of the effect
   */
  void makeShake(float duration);

  void write(std::string str, int size, float x, float y, int align, sf::Color color, sf::RenderTarget* app, int xShadow, int yShadow);

  /*!
   *  \brief Save the game
   *
   *  Save the game to file : complete floor and maps, items and blood position, player current equipment and stats....
   */
  void saveGame();

  /*!
   *  \brief Load the game
   *
   *  Load the game from file. After restoring the game, the file is destroy.
   *
   *  \return true if succeeded
   */
  bool loadGame();

  struct saveHeaderStruct
  {
    bool ok;            /**< Save game OK ? */
    int level;          /**< Level the save game */
    std::string date;   /**< Date of the save game */
    std::string time;   /**< Time of the save game */
  };
  /*!
   *  \brief Load the savegame data
   *  \return true if succeeded
   */
  saveHeaderStruct loadGameHeader();

  /*!
   *  \brief Returns a random equip object
   *
   * Returns a random equip object (not an object the player already possesses) .
   *  \param toSale : true if it's an item for sale
   *
   *  \return the equipment item ID
   */
  item_equip_enum getRandomEquipItem(bool toSale, bool noFairy);

  /*!
   *  \brief Returns a random spell object
   */
  enumItemType getItemSpell();

  /*!
   *  \brief Generate a random challenge loot
   *
   *  \param x : x position of the bonus
   *  \param y : y position of the bonus
   */
  void generateChallengeBonus(float x, float y);

  /*!
   *  \brief Adds monsters
   *
   * Adds monsters to the room in suitable places.
   *  \param monsterType : monster type
   *  \param amount : amount of monsters
   */
  void findPlaceMonsters(enemyTypeEnum monsterType, int amount);

  void addKilledEnemy(enemyTypeEnum enemyType);
  void displayKilledEnemies();

  void proceedEvent(EnumWorldEvents event);

  void addMessageToQueue(EnumMessages type);

protected:
  /*!
   *  \brief Rendering method
   *
   *  Render all the game objects to the screen.
   *  Called in the game loop.
   */
  virtual void onRender();

  /*!
   *  \brief Update method
   *
   *  Update all the game objects to the screen.
   *  Called in the game loop.
   */
  virtual void onUpdate();

  /*!
   *  \brief render the HUD for shot types
   *
   *  Render the HUD for shot types.
   *  Display the available shot types and highlight the current one.
   *
   *  \param app : Rendering target
   */
  void renderHudShots(sf::RenderTarget* app);

private:
  Config config;
  float deltaTime;
  // game logic / data
  GameMap* miniMap;           /*!< Pointer to the logical minimap */
  DungeonMap* currentMap;     /*!< Pointer to the logical current map */
  GameFloor* currentFloor;    /*!< Pointer to the logical floor (level) */
  bool showLogical;           /*!< True if showing bounding boxes, z and center */

  // game play
  int level;                  /*!< Level (floor) */
  int floorX;                 /*!< X position of the room in the level */
  int floorY;                 /*!< Y position of the room in the level */
  bool roomClosed;            /*!< True if the room is closed */
  bool bossRoomOpened;        /*!< True if the boss gate has been opened in this level */
  bool isPausing;             /*!< True if the game is currently pausing */
  int firingDirection;        /*!< Save the firing direction - for the "one button" gameplay */
  bool isPlayerAlive;         /*!< Dying sets this bool to false (trigger the ending music) */
  bool monsterArray[MAP_WIDTH][MAP_HEIGHT]; /*!<  use to remember if a case has a monster in monster spawn */
  int killedEnemies[NB_ENEMY];

  // game objects
  PlayerEntity* player;           /*!< Pointer to the player entity */
  TileMapEntity* currentTileMap;  /*!< TileMap of the room (main game board) */
  GameMap* menuMap;               /*!< TileMap of the menu background (logical) */
  TileMapEntity* menuTileMap;     /*!< TileMap of the menu background (graphical) */

  // displaying objects
  DoorEntity* doorEntity[4];  /*!< Pointers to the door graphical entity */
  sf::Sprite keySprite;       /*!< A simple sprite with the boss key (displayed on the HUD) */
  sf::Sprite shotsSprite;     /*!< A simple sprite for the available shot types (displayed on the HUD) */
  sf::Font font;              /*!< The font used for displaying text */
  sf::Text myText;            /*!< The text to be displayed */

  sf::Music music;            /*!< Current game music */

  /** Music enum
   *  Identify the various music tracks of the game.
   */
  enum musicEnum
  {
    MusicDungeon,   /**< Main game music - played when playing the game */
    MusicEnding,    /**< Ending music - played when the player has died */
    MusicBoss,      /**< Boss music - for epic fights ! */
    MusicIntro      /**< Main menu music */
  };

  /** Game states enum
   *  Used for the different game states
   */
  enum gameStateEnum
  {
    gameStateInit,      /**< Game initialization */
    gameStateMenu,      /**< Menu */
    gameStatePlaying,   /**< Playing */
    gameStateKeyConfig  /**< Key config */
  };
  gameStateEnum gameState;  /*!< Store the game state */

  /** Special game states enum
   *  Used for effects such as fade in...
   */
  enum xGameTypeEnum
  {
    xGameTypeFade,        /**< Fade effect */
    xGameTypeShake,       /**< Shake effect */
    xGameTypeFadeColor,   /**< Color fade effect */
    NB_X_GAME
  };

  struct xGameStruct
  {
    bool active;
    int param;
    float timer;
  } xGame[NB_X_GAME];

  /** Input Keys enum
   *  Used for the input binding
   */
  enum inputKeyEnum
  {
    KeyUp,
    KeyDown,
    KeyLeft,
    KeyRight,
    KeyFireUp,
    KeyFireDown,
    KeyFireLeft,
    KeyFireRight,
    KeyFireSelect,
    KeySpell,
    KeyTimeControl,
    KeyFire
  };

  sf::Keyboard::Key input[NumberKeys];     /*!< Input key array */

  /*!
   *  \brief Starts the game
   *
   *  Start a new game or load it from file.
   *
   *  \param fromSaveFile : true if we want to try to load the game from a file
   */
  void startNewGame(bool fromSaveFile);

  /*!
   *  \brief Starts a new level
   *
   *  Start a new level.
   *  Called for each level of the game.
   */
  void startNewLevel();

  /*!
   *  \brief Starts the level
   *
   *  Starts the level.
   *  Called after loading the game or creating a new level.
   */
  void playLevel();

  /*!
   *  \brief Creates a level
   *
   *  Creates a random level (a level is a floor that consists of rooms).
   */
  void createFloor();

  /*!
   *  \brief Create or refresh the room
   *
   * Create a room (if this is a new one) or refresh it.
   * Called when the player changes room.
   * Checks the visibility of the doors and close it if there are monsters.
   * Loads the map items and sprites.
   */
  void refreshMap();

  /*!
   *  \brief Check if a door is present, and its style
   *
   *  \param doorId : index of the door
   *  \param roomCurrent : type of the current door
   *  \param roomNeighbour : type of the neighbour door
   */
   void checkDoor(int doorId, roomTypeEnum roomCurrent, roomTypeEnum roomNeighbour);

  /*!
   *  \brief Refreshes the minimap
   *
   * Refresh the minimap.
   * Called when the player changes room.
   */
  void refreshMinimap();

  /*!
   *  \brief Generates a room
   *
   * Generates a room.
   * Called when the player moves to a room for the first time.
   */
  void generateMap();

  /*!
   *  \brief Generates a standard room
   *
   * Generates a standard room with monsters.
   * Called during the generation when the map has no particular type.
   */
  void generateStandardMap();

  /*!
   *  \brief Checks if the room will be closed
   *
   * Checks if the room will be closed.
   * Called when entering a room. If the room is not clear, closes the doors.
   */
  void checkEntering();

  /*!
   *  \brief Saves the map items
   *
   * Saves the map objects such as items, corpses, blood, chest.
   * Called when leaving the door.
   */
  void saveMapItems();

  /*!
   *  \brief Initializes the monster array
   *
   * Initializes the monster array (to empty).
   */
  void initMonsterArray();

  /*!
   *  \brief Adds a monster
   *
   * Adds a monster to the room.
   *  \param monsterType : monster type
   *  \param xm : x position of the monster
   *  \param ym : y position of the monster
   */
  void addMonster(enemyTypeEnum monsterType, float xm, float ym);

  /*!
   *  \brief Checks if player opens a door
   *
   * Checks if player opens a door (collide with the door and gets the key).
   * If positive, opens the door.
   */
  void verifyDoorUnlocking();

  /*!
   *  \brief Plays a music
   *
   * Plays a music.
   *
   *  \param musicChoice : music track ID
   */
  void playMusic(musicEnum musicChoice);

  /*!
   *  \brief Add a key to the player input map from a string key (from file)
   *  \param logicInput : input function (move left, fire up, etc...)
   *  \param key : Key as string
   */
  void addKey(int logicInput, std::string key);

  /*!
   *  \brief Save configuration to "config.dat"
   */
  void saveConfigurationToFile();

  /*!
   *  \brief Configure with data from "config.dat"
   */
  void configureFromFile();

  /*!
   *  \brief Update the game
   */
  void updateRunningGame();

  /*!
   *  \brief Render the game
   */
  void renderRunningGame();

  /*!
   *  \brief Update the menu
   */
  void updateMenu();

  /*!
   *  \brief Render the menu
   */
  void renderMenu();

/*!
   *  \brief Render the death screen (when the player dies)
   */
  void renderDeathScreen();

  /** Menu keys enum
   *  Identify the various keys of the menu.
   */
  enum menuItemEnum
  {
    MenuStartNew,     /**< When starting the game */
    MenuStartOld,     /**< When restoring the game */
    MenuConfig,       /**< When configuring the game */
    MenuKeys,         /**< When configuring keys */
    MenuConfigBack,   /**< Back to the main menu */
    MenuLanguage,     /**< When configuring the language */
    MenuExit          /**< When exiting the game */
  };

  /** Menu states enum
   *  Identify the various states of the menu.
   */
  enum menuStateEnum
  {
    MenuStateMain,
    MenuStateConfig,
    MenuStateKeys,
    MenuStateFirst    /**< First time, we choose language and keyboard */
  };
  menuStateEnum menuState;

  /*!
   *  \brief Menu item structure
   */
  struct menuItemStuct
  {
    menuItemEnum id;              /**< Id of the action */
    std::string label;            /**< Label of the menu item */
    std::string description;      /**< Description of the menu item */
  };

  /*!
  *  \brief Menu structure
  */
  struct menuStuct
  {
    std::vector<menuItemStuct> items; /**< Menu items */
    unsigned int index;               /**< Position int the menu */
    float age;                        /**< Age of the menu */
  };

  menuStuct menuMain;
  menuStuct menuFirst;
  menuStuct menuConfig;
  unsigned int menuKeyIndex;

  /*!
   *  \brief Build the menu items
   */
  void buildMenu(bool rebuild);

  /*!
   *  \brief Switch to the menu
   */
  void switchToMenu();

  /*!
   *  \brief Save language config and default keys configuration
   */
  void registerLanguage();

  struct parameterStruct
  {
    int language;
  } parameters;


  /*!
   *  \brief Check for falling (in holes) blood or bodies
   */
  void checkFallingEntities();

  void resetKilledEnemies();

  std::queue <messageStruct> messagesQueue;

  bool worldEvent[NB_EVENTS];
  void initEvents();
};

/*!
 *  \brief Returns the game reference
 *
 * Returns the game reference.
 *
 *  \return a reference to the game
 */
WitchBlastGame& game();

#endif // WITCH_BLAST_GAME_H

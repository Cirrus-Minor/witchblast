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
#include "DungeonMapEntity.h"
#include "MiniMapEntity.h"
#include "EnemyEntity.h"
#include "DoorEntity.h"
#include "GameFloor.h"
#include "Config.h"
#include "Achievements.h"

#include <queue>
#include <thread>

// for tests
//#define TEST_MODE

// for crashing systems - true enable to continue a game after a crash and send score to DB
const bool autosave = false;

const int ALIGN_LEFT    = 0;  /*!< Text alignment left */
const int ALIGN_RIGHT   = 1;  /*!< Text alignment right */
const int ALIGN_CENTER  = 2;  /*!< Text alignment centered */

const int X_GAME_FADE_IN  = 0;  /*!< Fade In effect ID */
const int X_GAME_FADE_OUT = 1;  /*!< Fade out effect ID */

const int X_GAME_COLOR_RED    = 0;  /*!< Red light color effect  ID */
const int X_GAME_COLOR_GREEN  = 1;  /*!< Green light color effect  ID */
const int X_GAME_COLOR_BLUE   = 2;  /*!< Blue light color effect  ID */
const int X_GAME_COLOR_VIOLET = 3;  /*!< Violet light color effect  ID */
const int X_GAME_COLOR_BROWN  = 4;  /*!< Brown light color effect  ID */
const int X_GAME_COLOR_WHITE  = 5;  /*!< White light color effect  ID */

unsigned const int NumberKeys = 13; /*!< Number of input keys on the game */

/** Input key string
 *  Keys in the config file.
 */
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
  "key_interact",
  "key_time",
  "key_fire"
};

// TODO: more players, multiple joysticks
const int NB_PLAYERS_MAX = 2;



/** Credits: coder */
const std::string creditsCode[]  =
{
  "Seby",
  "END"
};
/** Credits: 2D artists */
const std::string credits2D[]  =
{
  "Pierre \"dejam0rt\" Baron",
  "END"
};
/** Credits: Sound */
const std::string creditsSound[]  =
{
  "www.freesound.org/",
  "www.freesfx.co.uk/",
  "www.universal-soundbank.com/",
  "END"
};
/** Credits: Music */
const std::string creditsMusic[]  =
{
  "Le Guitariste Du Nord",
  "END"
};
/** Credits: Translation */
const std::string creditsTranslate[]  =
{
  "achpile (russian)",
  "AFS (spanish)",
  "Geheim (german)",
  "END"
};

/** Struct for game parameters */
struct parameterStruct
{
  int language;               /*!< Language ID (english = 0) */
  int musicVolume;            /*!< music volume (0 to 100) */
  int soundVolume;            /*!< sound volume (0 to 100) */
  bool zoom;                  /*!< zoom effect (false = disabled) */
  bool vsync;                 /*!< monitor vsync (false = disabled) */
  bool bloodSpread;           /*!< blood spread (false = disabled) */
  bool fullscreen;            /*!< full screen (false = disabled) */
  bool pauseOnFocusLost;      /*!< pause on focus lost (false = disabled) */
  bool particlesBatching;
  bool lowParticles;
  bool displayBossPortrait;
  std::string playerName;     /*!< player name */
};

struct structPotionMap
{
  enumItemType effect;
  bool known;
};

struct structStats
{
  int hpIni;
  int hpMax;
  int hpEnd;
  int hpMaxEnd;
  int hurtCounter;
  int healCounter;
  int hpLost;
  int hpHeal;
  int dam;
  int goldIni;
};

enum EnumScoreBonus
{
  BonusSecret,
  BonusChallenge,
  BonusPerfect,
  BonusTime,
  BonusPossession,
};

enum EnumButtonType
{
  ButtonConsumable,
  ButtonShotType,
};

struct ButtonStruct
{
  sf::IntRect zone;
  EnumButtonType type;
  int index;
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
// DATA TYPES
  /** Savegame header data */
  struct saveHeaderStruct
  {
    bool ok;            /**< Save game OK ? */
    int level;          /**< Level the save game */
    float gameTime;     /**< How long it has been played */
    int shotType;       /**< Current special shot type */
    std::string date;   /**< Date of the save game */
    std::string time;   /**< Time of the save game */
  };

// PUBLIC METHODS
  /*!
   *  \brief Constructor
   */
  WitchBlastGame();

  /*!
   *  \brief Destructor
   */
  virtual ~WitchBlastGame();

  /*!
  *  \brief Accessor on the current dungeon map
  *  \return a pointer to the current dungeon map
  */
  DungeonMap* getCurrentMap();
  GameFloor* getCurrentFloor();
  int getFloorX();
  int getFloorY();
  /*!
  *  \brief Accessor on the current dungeon map entity
  *  \return a pointer to the current dungeon map entity
  */
  DungeonMapEntity* getCurrentMapEntity();

  /*!
   *  \brief Accessor on the player
   *  \return a pointer to the player
   */
  PlayerEntity* getPlayer();

  /*!
   *  \brief Accessor on the player's position
   *  \return a Vector2D of the player's position
   */
  Vector2D getPlayerPosition();

  /*!
   *  \brief accessor on the level
   *  \return : the level
   */
  int getLevel();

  /*!
   *  \brief accessor on the challenge level
   *  \return : the challenge level
   */
  int getChallengeLevel();

  /*!
   *  \brief accessor on the number of found secrets
   *  \return : the number of found secrets
   */
  int getSecretsFound();

  /*!
   *  \brief accessor on showLogical flag
   *  \return : the value of the flag
   */
  bool getShowLogical();

  float getDeltaTime();

  /*!
   *  \brief accessor on the parameters
   *  \return : the parameters
   */
  parameterStruct getParameters();

  /*!
   *  \brief Start the game and the game loop
   *  This method starts the game and the game loop.
   *  The game loop watches the events, the inputs, update and draw the game elements.
   */
  virtual void startGame();

  /*!
  *  \brief Move the player to another map (room)
  *  Moves the player to another room of the dungeon.
  *  It's called when a player walk through an opened door.
  *  \param direction : direction of the new map. 4 = left, 8 = north, 6 = right, 2 = south
  */
  void moveToOtherMap(int direction);

  /*!
   *  \brief Closes the doors of the room
   *  Closes the doors of the room.
   *  It's called when the player enter in an area with monsters.
   */
  void closeDoors();

  /*!
   *  \brief Opens the doors of the room
   *  Opens the doors of the room.
   *  It's called when the player destroy the last monster of a room.
   */
  void openDoors();

 /*!
  *  \brief Count the enemies in the room
  *  Count the enemies in the room.
  *  It's called when the room is closed : 0 enemy = room is cleared and doors shall open.
  *  \return amount of enemies
  */

  /*!
   *  \brief Refreshes the minimap
   *
   * Refresh the minimap.
   * Called when the player changes room.
   */
  void refreshMinimap();

  int getEnemyCount();

  int getUndeadCount();

  bool existsEffectZone();

  void animateEffects();

  EnemyEntity* getBoss();

  void destroyUndead(int damage);

  int getItemsCount();

 /*!
  *  \brief Return the position of the nearest enemy
  *  \param x : x position of the source
  *  \param y : y position of the source
  *  \return position of the nearest enemy (negative position when no enemy found)
  */
  Vector2D getNearestEnemy(float x, float y);

 /*!
  *  \brief Generates blood
  *  \param x : x position of the blood
  *  \param y : y position of the blood
  *  \param bloodColor : color of the blood (red; green, ...)
  */
  void generateBlood(float x, float y, BaseCreatureEntity::enumBloodColor bloodColor);

/*!
  *  \brief Add a corpse on the map
  *  \param x : x position of the corpse
  *  \param y : y position of the corpse
  *  \param frame : frame of the corpse in the spritesheet
  */
  void addCorpse(float x, float y, int frame);

  /*!
   *  \brief Show a "popup" with artefact's description
   *  Show a "popup" with artefact's description.
   *  Artefact's description consists of zoomed picture of the item, name and description.
   *  It's called when a player get an equipment item.
   *  \param itemType : item identifier
   */
  void showArtefactDescription(enumItemType itemType);

  /*!
   *  \brief Make a "shake" effet
   *  \param duration : duration of the effect
   */
  void makeShake(float duration);

  /*!
   *  \brief Make a "color fade" effet
   *  \param color : the color of the effect
   *  \param duration : duration of the effect
   */
  void makeColorEffect(int color, float duration);

  /*!
   *  \brief Write a string on screen
   *  \param str : the string
   *  \param size : the character size
   *  \param x : x position
   *  \param y : y position
   *  \param align : alignment (ALIGN_LEFT, ALIGN_CENTER or ALIGN_RIGHT)
   *  \param color : color of the string
   *  \param app : the rendering target
   *  \param xShadow : offset of the shadow (x)
   *  \param yShadow : offset of the shadow (y)
   *  \param maxWidth : max width of the text image (0 = no limit)
   */
  void write(std::string str, int size, float x, float y, int align, sf::Color color, sf::RenderTarget* app, int xShadow = 0, int yShadow = 0, int maxWidth = 0);

  void writeGraphic(std::string str, int size, float x, float y, int align, sf::Color color, sf::RenderTarget* app, int xShadow = 0, int yShadow = 0, int maxWidth = 0);

  /*!
   *  \brief Save the game
   *  Save the game to file : complete floor and maps, items and blood position, player current equipment and stats....
   */
  void saveGame(bool autosave);

  /*!
   *  \brief Load the game
   *  Load the game from file. After restoring the game, the file is destroy.
   *  \return true if succeeded
   */
  bool loadGame();

  /*!
   *  \brief Save the game data (general)
   */
  void saveGameData();

  /*!
   *  \brief Load the game data (general)
   */
  void loadGameData();

  /*!
   *  \brief Load the savegame data
   *  \return the savegame data
   */
  saveHeaderStruct loadGameHeader();

  /*!
   *  \brief Returns a random equip object
   *
   *  Returns a random equip object (not an object the player already possesses) .
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
   *  Adds monsters to the room in suitable places.
   *  \param monsterType : monster type
   *  \param amount : amount of monsters
   */
  void findPlaceMonsters(enemyTypeEnum monsterType, int amount);

  /*!
   *  \brief Add a monster to the "killed monsters" table
   *  \param enemyType : ID of the monster
   */
  void addKilledEnemy(enemyTypeEnum enemyType, enumShotType damageType);

  /*!
   *  \brief Proceed an event
   *  \param event : ID of the event
   */
  void proceedEvent(EnumWorldEvents event);

  /*!
   *  \brief Proceed a message
   *  \param event : ID of the message
   */
  void testAndAddMessageToQueue(EnumMessages type);

  /*!
   *  \brief Proceed a divinity level up message
   *  \param label : the message to proceed
   */
  void addDivLevelMessageToQueue(std::string label);

  std::string enemyToString(enemyTypeEnum enemyType);
  std::string sourceToString(sourceTypeEnum sourceType, enemyTypeEnum enemyType);

  void saveScreen();
  void saveDeathScreen();

  struct StructScore
  {
    std::string name;
    int score;
    int level;
    int shotType;
    bool equip[NUMBER_EQUIP_ITEMS];
    int divinity;
    int killedBy;
    int time;
  };
  void calculateScore();

  void addLifeBarToDisplay(std::string label, int hp, int hpMax);

  void revealFloor();

  void activateKeyRoomEffect(bool withColorEffect);

  void generateStar(sf::Color starColor, float xStar, float yStar);

  void registerAchievement(enumAchievementType achievement);

  void renderDoors();
  void setDoorVisible(int n);

  bool isPresentItem(int n);
  void addPresentItem(int n);

  /*!
   *  \brief Checks if player opens a door
   *
   * Checks if player opens a door (collide with the door and gets the key).
   * If positive, opens the door.
   */
  void verifyDoorUnlocking();
  float getGameTime();
  std::string equipToString(bool equip[NUMBER_EQUIP_ITEMS]);

  enumItemType getPotion(enumItemType source);
  bool potionEffectKnown(enumItemType source);
  void setPotionToKnown(enumItemType source);
  void addPotionToMap(enumItemType source, enumItemType effect, bool known);
  void acquireAlchemyBook();
  void forget();
  void forgetPotions();

  /*!
   *  \brief Returns true if the game is in "advanced" mode
   */
  bool isAdvanced();

  /*!
   *  \brief Returns true if the difficulty for the map is "advanced"
   */
  bool isAdvancedLevel();

  void pauseMusic();
  void resumeMusic();

  void saveStats();
  void addHurtingStat(int hpLost);
  void addHealingStat(int hpHeal);

  void gainMultiplayerPower();

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

  void killArtefactDescription();

private:
  Config config;
  float deltaTime;
  // game logic / data
  GameMap* miniMap;           /*!< Pointer to the logical minimap */
  DungeonMap* currentMap;     /*!< Pointer to the logical current map */
  GameFloor* currentFloor;    /*!< Pointer to the logical floor (level) */
  bool showLogical;           /*!< True if showing bounding boxes, z and center */
  bool showGameTime;          /*!< True if showing the game time */

  // game play
  int level;                  /*!< Level (floor) */
  int score;                  /*!< score (calculated at the end of the game) */
  int scoreDisplayed;
  std::string scoreBonus;
  float scoreBonusTimer;
  int bodyCount;              /*!< killed monsters (calculated at the end of the game) */
  int challengeLevel;         /*!< Level (challenge) */
  int secretsFound;
  float gameTime;             /*!< "age" of the current game */
  int floorX;                 /*!< X position of the room in the level */
  int floorY;                 /*!< Y position of the room in the level */
  bool roomClosed;            /*!< True if the room is closed */
  bool bossRoomOpened;        /*!< True if the boss gate has been opened in this level */
  int firingDirection;        /*!< Save the firing direction - for the "one button" gameplay */
  bool isPlayerAlive;         /*!< Dying sets this bool to false (trigger the ending music) */
  bool monsterArray[MAP_WIDTH][MAP_HEIGHT]; /*!<  use to remember if a case has a monster in monster spawn */
  int killedEnemies[NB_ENEMY];
  int loopCounter;
  float endingTimer;        /*!< Counter before end of won game */
  bool isBonusTimeAdded;
  int nbPlayers;

  // game objects
  PlayerEntity* player;             /*!< Pointer to the player entity */
  DungeonMapEntity* dungeonEntity;  /*!< TileMap of the room (main game board) + blood, items, etc...*/
  MiniMapEntity* miniMapEntity;
  // displaying objects
  DoorEntity* doorEntity[4];  /*!< Pointers to the door graphical entity */
  sf::Font font;              /*!< The font used for displaying text */
  sf::Font graphicsFont;      /*!< The font used for displaying "medieval" text */
  sf::Text myText;            /*!< The text to be displayed */
  sf::Sprite introScreenSprite;
  sf::Sprite titleSprite;

  struct uiSpritesStruct
  {
    sf::Sprite gui;
    sf::Sprite keySprite;       /*!< A simple sprite with the boss key (displayed on the HUD) */
    sf::Sprite shotsSprite;     /*!< A simple sprite for the available shot types (displayed on the HUD) */
    sf::Sprite topLayer;
    sf::Sprite msgBoxSprite;
    sf::Sprite mapBgSprite;
    sf::Sprite bagSprite;
    sf::Sprite pauseSprite;
    sf::Sprite numberSprite;
  } uiSprites;

  struct lifeBarStruct
  {
    bool toDisplay;
    std::string label;
    int hp;
    int hpMax;
  } lifeBar;

  float xOffset, yOffset;     /*!< Main game client position in the GUI */

  sf::Music music;            /*!< Current game music */

  /** Music enum
   *  Identify the various music tracks of the game.
   */
  enum musicEnum
  {
    MusicDungeon,   /**< Main game music - played when playing the game */
    MusicEnding,    /**< Ending music - played when the player has died */
    MusicBoss,      /**< Boss music - for epic fights ! */
    MusicIntro,     /**< Main menu music */
    MusicChallenge  /**< Challenge music - for epic fights ! */
  };

  /** Game states enum
   *  Used for the different game states
   */
  enum gameStateEnum
  {
    gameStateInit,      /**< Game initialization */
    gameStateIntro,     /** < Intro animation */
    gameStateMenu,      /**< Menu */
    gameStateKeyConfig,  /**< Key config */
    gameStateJoystickConfig,  /**< Joystick config */
    gameStatePlaying,   /**< Playing */
    gameStatePlayingPause,   /**< Playing / Pause */
    gameStatePlayingDisplayBoss,   /**< Playing / DisplayBoss */
  };
  gameStateEnum gameState;  /*!< Store the game state */
  float bossDisplayTimer;
  int bossDisplayState;

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
    float duration;
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
    KeyInteract,
    KeyTimeControl,
    KeyFire
  };

  const std::string inputKeyStr[NumberKeys] =
  {
    "_move_up",
    "_move_down",
    "_move_left",
    "_move_right",
    "_fire_up",
    "_fire_down",
    "_fire_left",
    "_fire_right",
    "_spell",
    "_interact",
    "_fire",
    "_time_control",
    "_fire_select"
  };

  sf::Keyboard::Key input[NumberKeys];     /*!< Input key array */

  struct JoystickInputStruct
  {
    bool isButton;
    sf::Joystick::Axis axis;
    int value;
  };

  JoystickInputStruct joystickInput[NumberKeys];

  std::string scoreSaveFile;

  structStats statsData;
  std::string statsStr;

  /*!
   *  \brief Starts the game
   *
   *  Start a new game or load it from file.
   *
   *  \param fromSaveFile : true if we want to try to load the game from a file
   */
  void startNewGame(bool fromSaveFile, int startingLevel);

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
   *  \param isFight : true if loading in fight
   */
  void playLevel(bool isFight);

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
   void checkDoor(int doorId, roomTypeEnum roomCurrent, roomTypeEnum roomNeighbour, bool isNeighbourKnown);

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
   *  \brief Checks if player can interact with something
   */
  void checkInteraction();

  /*!
   *  \brief Plays a music
   *
   * Plays a music.
   *
   *  \param musicChoice : music track ID
   */
  void playMusic(musicEnum musicChoice);

  /*!
   *  \brief Update the music volume (with parameters.volumeMusic)
   */
  void updateMusicVolume();

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

  void renderGame();
  void renderHud();
  void renderLifeBar();
  void renderScore();
  void renderMessages();
  void renderBossPortrait();

  /*!
   *  \brief Update the menu
   */
  void updateMenu();

  /*!
   *  \brief Render the menu
   */
  void renderMenu();

  /*!
   *  \brief Render the menu
   */
  void renderInGameMenu();

  /*!
   *  \brief initialize the intro
   */
  void prepareIntro();

  /*!
   *  \brief Update the intro
   */
  void updateIntro();

  /*!
   *  \brief Render the intro
   */
  void renderIntro();

  /*!
   *  \brief Render the credits screen
   */
  void renderCredits();

  /*!
   *  \brief Render the achievements screen
   */
  void renderAchievements();

  /*!
   *  \brief Render the scores screen
   */
  void renderScores(std::vector <StructScore> scoresToRender, std::string title, bool blinkingName);

  /** Menu keys enum
   *  Identify the various keys of the menu.
   */
  enum menuItemEnum
  {
    MenuStartNew,     /**< When starting the game */
    MenuStartOld,     /**< When restoring the game */
    MenuConfig,       /**< When configuring the game */
    MenuKeys,         /**< When configuring keys */
    MenuJoystick,     /**< When configuring joystick */
    MenuConfigBack,   /**< Back to the main menu */
    MenuTutoReset,    /**< Reset the tutorials */
    MenuLanguage,     /**< When configuring the language */
    MenuExit,         /**< When exiting the game */
    MenuAchievements, /**< Display the Achievements */
    MenuCredits,      /**< Display the credits screen */
    MenuHiScores,     /**< Display the hi-scores screen */
    MenuPlayerName,   /**< To enter/change the player name */
    MenuVolumeSound,
    MenuVolumeMusic,

    MenuContinue,     /**< Continue the game */
    MenuSaveAndQuit,  /**< Save and return to main */
  };

  /** Menu states enum
   *  Identify the various states of the menu.
   */
  enum menuStateEnum
  {
    MenuStateMain,
    MenuStateConfig,
    MenuStateKeys,
    MenuStateJoystick,
    MenuStateHiScores,
    MenuStateChangeName,
    MenuStateCredits,
    MenuStateAchievements,
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

  int menuScoreIndex;

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
  menuStuct menuJoystick;
  menuStuct menuInGame;
  unsigned int menuKeyIndex;
  unsigned int menuAchIndex;

  /*!
   *  \brief Build the menu items
   */
  void buildMenu(bool rebuild);

  /*!
   *  \brief Build the menu items (in game)
   */
  void buildInGameMenu();

  /*!
   *  \brief Switch to the menu
   */
  void switchToMenu();

  /*!
   *  \brief Save language config and default keys configuration
   */
  void registerLanguage();

  parameterStruct parameters;

  void resetKilledEnemies();

  std::queue <messageStruct> messagesQueue;
  bool worldEvent[NB_EVENTS];
  void initEvents();
  bool gameMessagesToSkip[NB_MESSAGES];

  struct achievementStruct
  {
    enumAchievementType type;
    std::string message;
    float timer;
    bool hasStarted;
    int counter;
  };
  std::queue <achievementStruct> achievementsQueue;

  void renderPlayer(float x, float y, bool equip[NUMBER_EQUIP_ITEMS], int shotType,
                  int frame, int spriteDy);
  void renderDeathScreen(float x, float y);

  bool equipToDisplay[NUMBER_EQUIP_ITEMS];
  bool equipNudeToDisplay[NUMBER_EQUIP_ITEMS];
  saveHeaderStruct saveHeader;

  struct StructMonster
  {
    enemyTypeEnum id;
    float x;
    float y;
  };

  /*!
   *  \brief Data for "in fight" game saving
   */
  struct StructSaveInFight
  {
    bool isFight;                         /**< True if loading during a fight */
    float x;                              /**< Player x position */
    float y;                              /**< Player y position */
    int direction;                        /**< Player direction */
    std::vector<StructMonster> monsters;  /**< Monsters */
  };
  StructSaveInFight saveInFight;  /**< Data for "in fight" game saving */

  SpriteEntity* introSprites[8];
  int introState;
  int introSoundState;
  int fairySpriteOffsetY;

  int currentStandardMusic;

  std::vector <StructScore> scores;
  std::vector <StructScore> scoresOnline;
  std::vector <StructScore> scoresOnlineDay;
  StructScore lastScore;
  void loadHiScores();
  void loadHiScoresOnline(bool fromDayOnly);
  void saveHiScores();
  enum enumNetworkScoreState
  {
    ScoreOK,
    ScoreLoading,
    ScoreLoadingDay
  } scoreState;

  struct StructInteraction
  {
    bool active;
    EnumInteractionType type;
    int id;
    std::string label;
  } interaction;

  void enableAA(bool enable);

  enum achievementStatus { AchievementDone, AchievementUndone, AchievementPending};
  achievementStatus achievementState[NB_ACHIEVEMENTS];

  bool isItemLocked(enumItemType item);
  bool isFunctionalityLocked(enumFunctionalityType func);

  int getAchievementsPercents();

  struct globalDataStruct
  {
    int killedMonster[NB_ENEMY];
  } globalData;

  void generateUiParticle(float x, float y);

  bool presentItems[NUMBER_EQUIP_ITEMS];
  void resetPresentItems();

  bool isPressing(int p, inputKeyEnum k, bool oneShot);
  bool getPressingState(int p, inputKeyEnum k);
  void updateActionKeys();

  struct ActionKeyStruct
  {
    bool isPressed;
    bool isTriggered;
  };
  ActionKeyStruct actionKey[NB_PLAYERS_MAX][NumberKeys];

  bool isInputPressed[NB_PLAYERS_MAX][NumberKeys];

  bool gameFromSaveFile;

  // scoring server
  std::thread sendScoreThread;
  void sendScoreToServer();
  void sendScoreToServerThread();

  std::thread receiveScoreThread;
  void receiveScoreFromServer();
  void receiveScoreFromServerThread();

  void checkDestroyableObjects();

  std::map<enumItemType, structPotionMap> potionMap;
  void randomizePotionMap();

  void addBonusScore(EnumScoreBonus bonusType, int points);
  sf::Vector2i levelStrPosition;

  std::vector<ButtonStruct> buttons;
  void tryToClick(int xMouse, int yMouse, int mouseButton);
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

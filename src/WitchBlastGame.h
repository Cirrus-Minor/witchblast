#ifndef WITCH_BLAST_GAME_H
#define WITCH_BLAST_GAME_H

#include "sfml_game/Game.h"
#include "sfml_game/TileMapEntity.h"
#include "PlayerEntity.h"
#include "EnnemyEntity.h"
#include "DoorEntity.h"
#include "GameFloor.h"

const int ALIGN_LEFT    = 0;  /*!< Text alignment left */
const int ALIGN_RIGHT   = 1;  /*!< Text alignment right */
const int ALIGN_CENTER  = 2;  /*!< Text alignment centered */

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

    void Write(std::string str, int size, float x, float y, int align, sf::Color color);

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
    void renderHudShots(sf::RenderWindow* app);

  private:
    // game logic / data
    GameMap* miniMap;           /*!< Pointer to the logical minimap */
    DungeonMap* currentMap;     /*!< Pointer to the logical current map */
    GameFloor* currentFloor;    /*!< Pointer to the logical floor (level) */

    // game play
    int floorX;                 /*!< X position of the room in the level */
    int floorY;                 /*!< Y position of the room in the level */
    bool roomClosed;            /*!< True if the room is closed */
    bool bossRoomOpened;        /*!< True if the boss gate has been opened in this level */
    bool isPausing;             /*!< True if the game is currently pausing */
    int firingDirection;        /*!< Save the firing direction - for the "one button" gameplay */
    bool isPlayerAlive;         /*!< Dying sets this bool to false (trigger the ending music) */
    bool monsterArray[MAP_WIDTH][MAP_HEIGHT]; /*!<  use to remember if a case has a monster in monster spawn */

    // game objects
    PlayerEntity* player;           /*!< Pointer to the player entity */
    TileMapEntity* currentTileMap;  /*!< TileMap of the room (main game board) */

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
      MusicEnding     /**< Ending music - played when the player has died */
    };

    /** Game states enum
     *  Used for the different game states
     */
    enum gameStateEnum
    {
      gameStateInit,      /**< Game initialization */
      gameStatePlaying    /**< Playing */
    };
    gameStateEnum gameState;  /*!< Store the game state */

    /** Special game states enum
     *  Used for effects such as fade in...
     */
    enum xGameStateEnum
    {
      xGameStateNone,     /**< No effect */
      xGameStateFadeIn,   /**< Fade in effect - usually when starting a level */
      xGameStateFadeOut   /**< Fade out effect - usually when leaving a level */
    };
    xGameStateEnum xGameState; /*!< Store the effect game state */
    float xGameTimer;          /*!< Effect game timer */

    /*!
     *  \brief Starts the game
     *
     *  Start a new game or load it from file.
     *
     *  \param fromSaveFile : true if we want to try to load the game from a file
     */
    void startNewGame(bool fromSaveFile);

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
     * Loads the mas items and sprites.
     */
    void refreshMap();

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
    void addMonster(monster_type_enum monsterType, float xm, float ym);

    /*!
     *  \brief Adds monsters
     *
     * Adds monsters to the room in suitable places.
     *  \param monsterType : monster type
     *  \param amount : amount of monsters
     */
    void findPlaceMonsters(monster_type_enum monsterType, int amount);

    /*!
     *  \brief Returns a random equip object
     *
     * Returns a random equip object (not an object the player already possesses) .
     *  \param toSale : true if it's an item for sale
     *
     *  \return the equipment item ID
     */
    item_equip_enum getRandomEquipItem(bool toSale);

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

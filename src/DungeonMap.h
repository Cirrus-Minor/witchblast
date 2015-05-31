#ifndef MAGICMAP_H
#define MAGICMAP_H

#include "sfml_game/GameMap.h"
#include "sfml_game/MyTools.h"
#include "Constants.h"
#include "DoorEntity.h"
#include <list>

const int MAPOBJ_NONE          =   0;
const int MAPOBJ_DOOR_OPEN     =   1;
const int MAPOBJ_DOOR_CLOSED   =   2;
const int MAPOBJ_OBSTACLE      =   10;
const int MAPOBJ_BIG_OBSTACLE  =   40;
const int MAPOBJ_TOMB          =   49;
const int MAPOBJ_WALL_SPECIAL  =   14;
const int MAPOBJ_GRID          =   19;
const int MAPOBJ_BANK_TOP      =   18;
const int MAPOBJ_BANK          =   28;
const int MAPOBJ_BANK_BOTTOM   =   38;
const int MAPOBJ_CHURCH_FURN_R =   29;
const int MAPOBJ_CHURCH_FURN_L =   39;
const int MAPOBJ_LONG_LEFT     =   50;
const int MAPOBJ_LONG          =   51;
const int MAPOBJ_LONG_RIGHT    =   52;
const int MAPOBJ_PNW           =   60;
const int MAPOBJ_HOLE          =   3;
const int MAPOBJ_HOLE_TOP      =   MAPOBJ_HOLE;
const int MAPOBJ_HOLE_BOTTOM   =   MAPOBJ_HOLE + 1;
const int MAPOBJ_HOLE_LEFT     =   MAPOBJ_HOLE + 2;
const int MAPOBJ_HOLE_RIGHT    =   MAPOBJ_HOLE + 3;
const int MAPOBJ_TEMPLE_WALL   =   20;
const int MAPOBJ_SHOP_BEGIN    =   11;
const int MAPOBJ_SHOP_LEFT     =   0 + MAPOBJ_SHOP_BEGIN;
const int MAPOBJ_SHOP          =   1 + MAPOBJ_SHOP_BEGIN;
const int MAPOBJ_SHOP_RIGHT    =   2 + MAPOBJ_SHOP_BEGIN;

const int MAPOBJ_BARREL         =   1000;
const int MAPOBJ_BARREL_NO_DROP =   1010;
const int MAPOBJ_BARREL_EXPL    =   1020;
const int MAPOBJ_SKULL          =   1030;

const int MAP_NORMAL_FLOOR    =   8;
const int MAP_NB_FLOORS       =   10;
const int MAP_TEMPLE          =   MAP_NB_FLOORS * 24;
const int MAP_CARPET          =   MAP_TEMPLE + 8;

const int MAP_WALL_BEGIN    =   (MAP_NB_FLOORS + 3) * 24;

// walls
const int MAP_WALL_X        =   MAP_WALL_BEGIN - 1;

const int MAP_WALL_8        =   0 + MAP_WALL_BEGIN;
const int MAP_WALL_7        =   2 + MAP_WALL_BEGIN;
const int MAP_WALL_77       =   4 + MAP_WALL_BEGIN;
const int MAP_WALL_87       =   5 + MAP_WALL_BEGIN;

const int DOOR_WALL_SPRITE_X     = 1344;
const int DOOR_WALL_SPRITE_Y     = (MAP_NB_FLOORS + 3) * 64;

const int DOOR_GRID_SPRITE_X   = 1280;
const int DOOR_GRID_SPRITE_Y   = (MAP_NB_FLOORS + 2) * 64;

const int DOOR_STAIRS_SPRITE_X   = 1088;
const int DOOR_STAIRS_SPRITE_Y   = DOOR_GRID_SPRITE_Y;

const int DOOR_SHADOW_SPRITE_X   = 896;
const int DOOR_SHADOW_SPRITE_Y   = DOOR_GRID_SPRITE_Y;

const int ROOM_TYPE_CHECKER = 5;
const int ROOM_TYPE_ALL     = 6;

enum logicalMapStateEnum
{
  LogicalFloor,
  LogicalWall,
  LogicalObstacle,
  LogicalHole,
  LogicalDestroyable,
};

class GameFloor;

enum roomTypeEnum
{
  roomTypeNULL,
  roomTypeStandard,
  roomTypeBoss,
  roomTypeMerchant,
  roomTypeKey,
  roomTypeBonus,
  roomTypeExit,
  roomTypeStarting,
  roomTypeChallenge,
  roomTypeTemple,
  roomTypeSecret
};

enum positionDirection
{
  North, West, South, East,
};

struct randomDungeonTileStruct
{
  int width;
  int height;
  int xOffset;
  int yOffset;
  bool canRotate;
  bool canBeOnWall;
  bool randomPlace;
  int rarity;
};

const int NB_RANDOM_TILES = 21;
const int NB_RANDOM_TILE_TRY = 40;
const int NB_RANDOM_TILES_IN_ROOM = 4;

/** Array with all the items and data */
const randomDungeonTileStruct randomDungeonTiles[NB_RANDOM_TILES] =
{
  { 192, 192, 0, 0, true, true, true, 1},
  { 192, 192, 192, 0, true, true, true, 1},
  { 192, 192, 384, 0, true, true, true, 1},
  { 192, 192, 576, 0, true, true, true, 1},
  { 256, 256, 0, 192, true, true, true, 1},
  { 512, 256, 256, 192, true, true, true, 1},
  { 128, 128, 0, 448, true, false, true, 6},
  { 128, 128, 128, 448, true, false, true, 3},
  { 128, 128, 256, 448, true, false, true, 3},
  { 128, 128, 384, 448, false, false, false, 1},
  { 128, 128, 512, 448, false, false, false, 6},
  { 128, 128, 640, 448, false, false, false, 6},
  { 192, 192, 0, 576, true, true, true, 1},
  { 192, 192, 192, 576, true, true, true, 1},
  { 192, 192, 384, 576, true, true, true , 1},
  { 192, 192, 576, 576, true, true, true, 1},

  { 256, 256, 0, 768, true, true, true, 1},
  { 256, 256, 256, 768, true, true, true, 1},
  { 256, 192, 768, 0, true, true, true, 1},
  { 256, 192, 768, 192, true, true, true, 1},
  { 256, 192, 768, 384, true, true, true, 1},
};

class DungeonMap : public GameMap
{
  public:
    DungeonMap(int width, int height);
    DungeonMap(GameFloor* gameFloor, int x, int y);
    virtual ~DungeonMap();
    void displayToConsole();
    bool isVisited();
    void setVisited(bool b);
    bool isKnown();
    void setKnown(bool b);
    bool isRevealed();
    void setRevealed(bool b);
    bool isCleared();
    void setCleared(bool b);
    bool isWalkable(int x, int y);
    bool isFlyable(int x, int y);
    bool isShootable(int x, int y);

    int getObjectTile(int x, int y);
    logicalMapStateEnum getLogicalTile(int x, int y);
    void setObjectTile(int x, int y, int n);
    void setLogicalTile(int x, int y, logicalMapStateEnum state);

    int getDivinity(int x, int y);

    bool containsHealth();

    roomTypeEnum getNeighbourLeft();
    roomTypeEnum getNeighbourRight();
    roomTypeEnum getNeighbourUp();
    roomTypeEnum getNeighbourDown();

    // 0 == no, 1 == yes, 2 == boss
    int hasNeighbourLeft();
    int hasNeighbourRight();
    int hasNeighbourUp();
    int hasNeighbourDown();

    bool hasKnownNeighbour(int direction, bool mustBeRevealed);

    virtual bool isDownBlocking(int x, int y);
    virtual bool isUpBlocking(int x, int y);
    virtual bool isLeftBlocking(int x, int y);
    virtual bool isRightBlocking(int x, int y);

    doorEnum getDoorType(int direction);
    void setDoorType(int direction, doorEnum type);

    virtual void randomize(int n);
    void initRoom(int floorN = -1, int wallN = -1);
    enum patternEnum { PatternSmallChecker, PatternBigChecker, PatternBorder, PatternBigCircle, PatternSmallCircle, PatternSmallStar, PatternSmallDisc};
    void initPattern(patternEnum n);
    void generateCarpet(int x0, int y0, int w, int h, int n);
    void generateTable(int x0, int y0, int w, int h, int n);
    void generateLongObject(int x0, int y0, int w, int n);
    void generateRoomRandom(int type);
    void generateRoomWithoutHoles(int type);
    void generateRoomWithHoles(int type);
    void generateExitRoom();
    Vector2D generateBonusRoom();
    Vector2D generateChestRoom();
    Vector2D generateMerchantRoom();
    Vector2D generateKeyRoom();
    void generateTemple(int x, int y, enumDivinityType type);
    void generateTempleRoom();
    void generateSecretRoom();
    void generateInselRoom();
    void addRandomGrids(int n);
    void addDestroyableObject(int x, int y, int obj);

    void addItem(int itemType, float x, float y, bool merch);
    void addSprite(int spriteType, int frame,  float x, float y, float scale);
    void addChest(int chestType, bool state,  float x, float y);

    void restoreMapObjects();
    void cleanMapObjects();

    roomTypeEnum getRoomType();
    void setRoomType(roomTypeEnum roomType);

    struct itemListElement { int type; float x; float y; bool merch; };
    typedef std::list<itemListElement> ItemList;

    struct spriteListElement { int type; int frame; float x; float y; float scale;};
    typedef std::list<spriteListElement> SpriteList;

    struct chestListElement { int type; bool state; float x; float y;};
    typedef std::list<chestListElement> ChestList;

    std::list<itemListElement> getItemList();
    std::list<chestListElement> getChestList();
    std::list<spriteListElement> getSpriteList();

    struct RandomTileElement { int type; float x; float y; float rotation;};
    RandomTileElement getRandomTileElement(int n);
    void setRandomTileElement (int n, RandomTileElement rt);

    void openDoor(int x, int y);
    void closeDoor(int x, int y);
    bool isDoor(int x, int y);

    int getFloorOffset();
    int getWallType();
    void setFloorOffset(int n);
    void setWallType(int n);

    bool callRevelation();

  protected:
  private:
    GameFloor* gameFloor;
    int x, y;
    bool visited;
    bool known;
    bool revealed;
    bool cleared;

    int objectsMap[MAP_WIDTH][MAP_HEIGHT];
    logicalMapStateEnum logicalMap[MAP_WIDTH][MAP_HEIGHT];

    roomTypeEnum roomType;
    ItemList itemList;
    SpriteList spriteList;
    ChestList chestList;

    RandomTileElement randomTileElements[NB_RANDOM_TILES_IN_ROOM];

    void generateCorridors();
    void generateRandomTile(int index);
    void generateRandomTiles();

    void restoreItems();
    void restoreSprites();
    void restoreChests();

    void makePatternTile(int x, int y);
    void addHole(int x, int y);

    int floorOffset;
    int wallType;

    doorEnum doorType[4];
};

#endif // MAGICMAP_H

#ifndef MAGICMAP_H
#define MAGICMAP_H

#include "sfml_game/GameMap.h"
#include "sfml_game/MyTools.h"
#include "Constants.h"
#include <list>

const int MAPOBJ_NONE          =   0;
const int MAPOBJ_DOOR_OPEN     =   1;
const int MAPOBJ_DOOR_CLOSED   =   2;
const int MAPOBJ_OBSTACLE      =   10;
const int MAPOBJ_BIG_OBSTACLE  =   40;
const int MAPOBJ_TOMB          =   49;
const int MAPOBJ_WALL_SPECIAL  =   14;
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

const int MAP_WALL_BEGIN    =   264;

const int MAP_NORMAL_FLOOR    =   8;
const int MAP_TEMPLE          =   192;
const int MAP_CARPET          =   200;
const int MAP_TILE_KEY        =   209;
const int MAP_GRID            =   240;
const int MAP_STAIRS_UP   =   2 + MAP_WALL_BEGIN;

// walls
const int MAP_WALL_X        =   0 + MAP_WALL_BEGIN;
const int MAP_WALL_EXIT_L   =   1 + MAP_WALL_BEGIN;
const int MAP_WALL_EXIT_R   =   3 + MAP_WALL_BEGIN;
const int MAP_WALL_START_L  =   4 + MAP_WALL_BEGIN;
const int MAP_WALL_START_M  =   5 + MAP_WALL_BEGIN;
const int MAP_WALL_START_R  =   6 + MAP_WALL_BEGIN;
const int MAP_WALL_EXIT_L_SPRITE =   7 + MAP_WALL_BEGIN;

const int MAP_WALL_8        =   8 + MAP_WALL_BEGIN;
const int MAP_WALL_7        =   9 + MAP_WALL_BEGIN;
const int MAP_WALL_87       =   10 + MAP_WALL_BEGIN;
const int MAP_WALL_77       =   11 + MAP_WALL_BEGIN;
const int MAP_WALL_ALTERN   =   12 + MAP_WALL_BEGIN;
const int MAP_WALL_DOOR_7   =   22 + MAP_WALL_BEGIN;
const int MAP_WALL_DOOR_8   =   23 + MAP_WALL_BEGIN;

const int MAP_OUTERDOOR_X   = 192;
const int MAP_OUTERDOOR_Y   = 768;

const int MAP_DOOR_BEGIN_X    = 0;
const int MAP_DOOR_BEGIN_Y    = 1152;

const int MAP_DOOR_STANDARD_W_X    = 0 + MAP_DOOR_BEGIN_X;
const int MAP_DOOR_STANDARD_W_Y    = 0 + MAP_DOOR_BEGIN_Y;
const int MAP_DOOR_CHALLENGE_W_X   = 0 + MAP_DOOR_BEGIN_X;
const int MAP_DOOR_CHALLENGE_W_Y   = 0 + MAP_DOOR_BEGIN_Y;
const int MAP_DOOR_BOSS_W_X        = 0 + MAP_DOOR_BEGIN_X;
const int MAP_DOOR_BOSS_W_Y        = 64 + MAP_DOOR_BEGIN_Y;

const int MAP_DOOR_STANDARD_H_X    = 192 + MAP_DOOR_BEGIN_X;
const int MAP_DOOR_STANDARD_H_Y    = 0 + MAP_DOOR_BEGIN_Y;
const int MAP_DOOR_CHALLENGE_H_X   = 192 + MAP_DOOR_BEGIN_X;
const int MAP_DOOR_CHALLENGE_H_Y   = 0 + MAP_DOOR_BEGIN_Y;
const int MAP_DOOR_BOSS_H_X        = 256 + MAP_DOOR_BEGIN_X;
const int MAP_DOOR_BOSS_H_Y        = 0 + MAP_DOOR_BEGIN_Y;

const int MAP_DECO_DOOR_BOSS_X   = 320 + MAP_DOOR_BEGIN_X;
const int MAP_DECO_DOOR_BOSS_Y   = 0 + MAP_DOOR_BEGIN_Y;
const int MAP_DECO_DOOR_CHAL_X   = 512 + MAP_DOOR_BEGIN_X;;
const int MAP_DECO_DOOR_CHAL_Y   = 0 + MAP_DOOR_BEGIN_Y;

const int ROOM_TYPE_CHECKER = 5;
const int ROOM_TYPE_ALL     = 6;

enum logicalMapStateEnum
{
  LogicalFloor,
  LogicalWall,
  LogicalObstacle,
  LogicalHole,
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
  roomTypeTemple
};

struct randomDungeonTileStruct
{
  int width;
  int height;
  int xOffset;
  int yOffset;
  bool totalRandom;
};

const int NB_RANDOM_TILES = 21;
const int NB_RANDOM_TILE_TRY = 40;

/** Array with all the items and data */
const randomDungeonTileStruct randomDungeonTiles[NB_RANDOM_TILES] =
{
  { 192, 192, 0, 0, true },
  { 192, 192, 192, 0, true },
  { 192, 192, 384, 0, true },
  { 192, 192, 576, 0, true },
  { 256, 256, 0, 192, true },
  { 512, 256, 256, 192, true },
  { 128, 128, 0, 448, true },
  { 128, 128, 128, 448, true },
  { 128, 128, 256, 448, true },
  { 128, 128, 384, 448, true },
  { 128, 128, 512, 448, false },
  { 128, 128, 640, 448, false },
  { 192, 192, 0, 576, true },
  { 192, 192, 192, 576, true },
  { 192, 192, 384, 576, true },
  { 192, 192, 576, 576, true },

  { 256, 256, 0, 768, true },
  { 256, 256, 256, 768, true },
  { 256, 192, 768, 0, true },
  { 256, 192, 768, 192, true },
  { 256, 192, 768, 384, true },
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

    virtual bool isDownBlocking(int x, int y);
    virtual bool isUpBlocking(int x, int y);
    virtual bool isLeftBlocking(int x, int y);
    virtual bool isRightBlocking(int x, int y);

    virtual void randomize(int n);
    void initRoom();
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
    Vector2D generateMerchantRoom();
    Vector2D generateKeyRoom();
    void generateTemple(int x, int y, enumDivinityType type);
    void generateTempleRoom();
    void generateInselRoom();
    void addRandomGrids(int n);

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
    RandomTileElement getRandomTileElement();
    void setRandomTileElement (RandomTileElement rt);

    void openDoor(int x, int y);
    void closeDoor(int x, int y);
    bool isDoor(int x, int y);

    int getFloorOffset();
    int getWallOffset();
    void setFloorOffset(int n);
    void setWallOffset(int n);

  protected:
  private:
    GameFloor* gameFloor;
    int x, y;
    bool visited;
    bool known;
    bool cleared;

    int objectsMap[MAP_WIDTH][MAP_HEIGHT];
    logicalMapStateEnum logicalMap[MAP_WIDTH][MAP_HEIGHT];

    roomTypeEnum roomType;
    ItemList itemList;
    SpriteList spriteList;
    ChestList chestList;

    RandomTileElement randomTileElement;

    void generateCorridors();
    void generateRandomTile();

    void restoreItems();
    void restoreSprites();
    void restoreChests();

    void makePatternTile(int x, int y);
    void addHole(int x, int y);

    int floorOffset;
    int wallOffset;
};

#endif // MAGICMAP_H

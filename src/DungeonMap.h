#ifndef MAGICMAP_H
#define MAGICMAP_H

#include "sfml_game/GameMap.h"
#include "sfml_game/MyTools.h"
#include "Constants.h"
#include <list>

const int MAP_NORMAL_FLOOR  =   7;
const int MAP_STAIRS_UP     =   39;
const int MAP_DOOR_OPEN     =   49;
const int MAP_WALL          =   70;
const int MAP_DOOR          =   71;
const int MAP_BIG_OBSTACLE  =   100;
const int MAP_TOMB          =   109;
const int MAP_WALL_SPECIAL  =   74;
const int MAP_BANK_TOP      =   78;
const int MAP_BANK          =   88;
const int MAP_BANK_BOTTOM   =   98;
const int MAP_LONG_LEFT     =   79;
const int MAP_LONG          =   89;
const int MAP_LONG_RIGHT    =   99;

// temple
const int MAP_TEMPLE          =   50;
const int MAP_TEMPLE_WALL     =   80;

// shop
const int MAP_SHOP_BEGIN      =   71;
const int MAP_SHOP_LEFT       = 0 + MAP_SHOP_BEGIN;
const int MAP_SHOP            = 1 + MAP_SHOP_BEGIN;
const int MAP_SHOP_RIGHT      = 2 + MAP_SHOP_BEGIN;

// walls
const int MAP_WALL_BEGIN    =   110;
const int MAP_WALL_7        =   6 + MAP_WALL_BEGIN;
const int MAP_WALL_77       =   8 + MAP_WALL_BEGIN;
const int MAP_WALL_8        =   0 + MAP_WALL_BEGIN;
const int MAP_WALL_9        =   7 + MAP_WALL_BEGIN;
const int MAP_WALL_99       =   9 + MAP_WALL_BEGIN;
const int MAP_WALL_4        =   2 + MAP_WALL_BEGIN;
const int MAP_WALL_6        =   3 + MAP_WALL_BEGIN;
const int MAP_WALL_1        =   16 + MAP_WALL_BEGIN;
const int MAP_WALL_11       =   18 + MAP_WALL_BEGIN;
const int MAP_WALL_2        =   1 + MAP_WALL_BEGIN;
const int MAP_WALL_3        =   17 + MAP_WALL_BEGIN;
const int MAP_WALL_33       =   19 + MAP_WALL_BEGIN;
const int MAP_WALL_87       =   10 + MAP_WALL_BEGIN;
const int MAP_WALL_89       =   11 + MAP_WALL_BEGIN;
const int MAP_WALL_21       =   12 + MAP_WALL_BEGIN;
const int MAP_WALL_23       =   13 + MAP_WALL_BEGIN;
const int MAP_WALL_47       =   4 + MAP_WALL_BEGIN;
const int MAP_WALL_41       =   14 + MAP_WALL_BEGIN;
const int MAP_WALL_69       =   5 + MAP_WALL_BEGIN;
const int MAP_WALL_63       =   15 + MAP_WALL_BEGIN;
const int MAP_WALL_EXIT_L   =   20 + MAP_WALL_BEGIN;
const int MAP_WALL_EXIT_R   =   21 + MAP_WALL_BEGIN;
const int MAP_WALL_START_L  =   22 + MAP_WALL_BEGIN;
const int MAP_WALL_START_M  =   23 + MAP_WALL_BEGIN;
const int MAP_WALL_START_R  =   24 + MAP_WALL_BEGIN;
const int MAP_WALL_X        =   29 + MAP_WALL_BEGIN;
const int MAP_WALL_ALTERN   =   30 + MAP_WALL_BEGIN;
const int MAP_WALL_SHOP     =   40 + MAP_WALL_BEGIN;

const int MAP_GRID          =   29;
const int MAP_HOLE          =   160;
const int MAP_HOLE_TOP      =   MAP_HOLE;
const int MAP_HOLE_BOTTOM   =   MAP_HOLE + 1;

const int ROOM_TYPE_CHECKER = 5;
const int ROOM_TYPE_ALL     = 6;

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
  bool canRotate;
};

const int NB_RANDOM_TILES = 16;
const int NB_RANDOM_TILE_TRY = 40;

/** Array with all the items and data */
const randomDungeonTileStruct randomDungeonTiles[NB_RANDOM_TILES] =
{
  { 192, 192, 0, 0, true },
  { 192, 192, 192, 0, true },
  { 192, 192, 384, 0, true },
  { 192, 192, 576, 0, true },
  { 256, 256, 0, 192, true },
  { 512, 256, 256, 192, false },
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
    void generateRoomRandom(int type);
    void generateRoomWithoutHoles(int type);
    void generateRoomWithHoles(int type);
    void generateExitRoom();
    Vector2D generateBonusRoom();
    Vector2D generateMerchantRoom();
    Vector2D generateKeyRoom();
    void generateTemple(int x, int y, enumDivinityType type);
    void generateTempleRoom();
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

  protected:
  private:
    GameFloor* gameFloor;
    int x, y;
    bool visited;
    bool known;
    bool cleared;

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
};

#endif // MAGICMAP_H

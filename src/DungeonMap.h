#ifndef MAGICMAP_H
#define MAGICMAP_H

#include "sfml_game/GameMap.h"
#include "sfml_game/MyTools.h"
#include "Constants.h"
#include <list>

const int MAP_NORMAL_FLOOR  =   7;
const int MAP_STAIRS_UP     =   39;
const int MAP_DOOR_OPEN     =   49;
const int MAP_WALL          =   50;
const int MAP_DOOR          =   51;

// temple
const int MAP_TEMPLE          =   42;
const int MAP_TEMPLE_WALL     =   52;

// walls
const int MAP_WALL_7        =   76;
const int MAP_WALL_77       =   78;
const int MAP_WALL_8        =   70;
const int MAP_WALL_9        =   77;
const int MAP_WALL_99       =   79;
const int MAP_WALL_4        =   72;
const int MAP_WALL_6        =   73;
const int MAP_WALL_1        =   86;
const int MAP_WALL_11       =   88;
const int MAP_WALL_2        =   71;
const int MAP_WALL_3        =   87;
const int MAP_WALL_33       =   89;
const int MAP_WALL_87       =   80;
const int MAP_WALL_89       =   81;
const int MAP_WALL_21       =   82;
const int MAP_WALL_23       =   83;
const int MAP_WALL_47       =   74;
const int MAP_WALL_41       =   84;
const int MAP_WALL_69       =   75;
const int MAP_WALL_63       =   85;
const int MAP_WALL_EXIT_L   =   90;
const int MAP_WALL_EXIT_R   =   91;
const int MAP_WALL_START_L  =   92;
const int MAP_WALL_START_M  =   93;
const int MAP_WALL_START_R  =   94;
const int MAP_WALL_X        =   99;

const int MAP_GRID          =   29;
const int MAP_HOLE          =   110;
const int MAP_HOLE_TOP      =   MAP_HOLE;
const int MAP_HOLE_BOTTOM   =   MAP_HOLE + 1;

const int ROOM_TYPE_CHECKER = 4;
const int ROOM_TYPE_ALL     = 5;

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

    void generateCorridors();

    void restoreItems();
    void restoreSprites();
    void restoreChests();
};

#endif // MAGICMAP_H

#ifndef MAGICMAP_H
#define MAGICMAP_H

#include "sfml_game/GameMap.h"
#include "sfml_game/MyTools.h"
#include <list>

const int MAP_NORMAL_FLOOR  =   7;
const int MAP_STAIRS_UP     =   39;
const int MAP_DOOR_OPEN     =   49;
const int MAP_WALL          =   50;
const int MAP_DOOR          =   51;
const int MAP_WALL_7        =   52;
const int MAP_WALL_8        =   53;
const int MAP_WALL_9        =   54;
const int MAP_WALL_4        =   55;
const int MAP_WALL_6        =   56;
const int MAP_WALL_1        =   57;
const int MAP_WALL_2        =   58;
const int MAP_WALL_3        =   59;
const int MAP_GRID          =   29;
const int MAP_HOLE          =   80;
const int MAP_HOLE_7        =   80;
const int MAP_HOLE_8        =   81;
const int MAP_HOLE_9        =   82;
const int MAP_HOLE_4        =   90;
const int MAP_HOLE_5        =   91;
const int MAP_HOLE_6        =   92;
const int MAP_HOLE_1        =   100;
const int MAP_HOLE_2        =   101;
const int MAP_HOLE_3        =   102;
const int MAP_HOLE_SINGLE   =   94;

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
  roomTypeStarting
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
    void generateCarpet(int x0, int y0, int w, int h, int n);
    void generateRoomRandom(int type);
    void generateRoomWithoutHoles(int type);
    void generateRoomWithHoles(int type);
    void generateExitRoom();
    Vector2D generateBonusRoom();
    Vector2D generateMerchantRoom();
    Vector2D generateKeyRoom();
    void addRandomGrids(int n);

    void addItem(int itemType, float x, float y, bool merch);
    void addSprite(int spriteType, int frame,  float x, float y, float scale);
    void addChest(int chestType, bool state,  float x, float y);
    void restoreItems();
    void restoreSprites();
    void restoreChests();
    void restoreMapObjects();

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
};

#endif // MAGICMAP_H

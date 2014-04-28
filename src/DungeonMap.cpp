#include "DungeonMap.h"
#include "GameFloor.h"
#include "ItemEntity.h"
#include "ChestEntity.h"
#include "sfml_game/ImageManager.h"
#include <cstdlib>
#include <stdio.h>
#include <iostream>

DungeonMap::DungeonMap(int width, int height) : GameMap(width, height)
{

}

DungeonMap::DungeonMap(GameFloor* gameFloor, int x, int y) : GameMap(MAP_WIDTH, MAP_HEIGHT)
{
  this->gameFloor = gameFloor;
  this->x = x;
  this->y = y;
  cleared = false;
  visited = false;
  known = false;
}

DungeonMap::~DungeonMap()
{
  //dtor
}

bool DungeonMap::isVisited()
{
  return visited;
}

void DungeonMap::setVisited(bool b)
{
  visited = b;
}

bool DungeonMap::isKnown()
{
  return known;
}

void DungeonMap::setKnown(bool b)
{
  known = b;
}

bool DungeonMap::isCleared()
{
  return cleared;
}

void DungeonMap::setCleared(bool b)
{
  cleared = b;
}

roomTypeEnum DungeonMap::getRoomType()
{
  return roomType;
}

void DungeonMap::setRoomType(roomTypeEnum roomType)
{
  this->roomType = roomType;
}

std::list<DungeonMap::itemListElement> DungeonMap::getItemList()
{
  return (itemList);
}

std::list<DungeonMap::chestListElement> DungeonMap::getChestList()
{
  return (chestList);
}

std::list<DungeonMap::spriteListElement> DungeonMap::getSpriteList()
{
  return (spriteList);
}


void DungeonMap::displayToConsole()
{
  for (int j=0; j < MAP_HEIGHT; j++)
  {
    for (int i=0; i < MAP_WIDTH; i++)
    {
      printf("%d", map[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

bool DungeonMap::isDownBlocking(int x, int y)
{
    if (!inMap(x, y)) return false;
    if (map[x][y] >= MAP_WALL) return true;
    return false;
}

bool DungeonMap::isUpBlocking(int x, int y)
{
    if (!inMap(x, y)) return false;
    if (map[x][y] >= MAP_WALL) return true;
    return false;
}

bool DungeonMap::isLeftBlocking(int x, int y)
{
    if (!inMap(x, y)) return false;
    if (map[x][y] >= MAP_WALL) return true;
    return false;
}

bool DungeonMap::isRightBlocking(int x, int y)
{
    if (!inMap(x, y)) return false;
    if (map[x][y] >= MAP_WALL) return true;
    return false;
}

bool DungeonMap::isWalkable(int x, int y)
{
  return (map[x][y] < MAP_WALL);
}

void DungeonMap::randomize(int n)
{
  int i, j;
  int x0 = MAP_WIDTH / 2;
  int y0 = MAP_HEIGHT / 2;

  initRoom();

  // bonus
  if (n == 5)
  {
    roomType = roomTypeBonus;
  }
  // others
  else if (n > 0)
  {
    int r = rand() % 4;

    if (r == 0) // corner blocks
    {
      map[1][1] = 4;
      map[1][MAP_HEIGHT -2] = 4;
      map[MAP_WIDTH - 2][1] = 4;
      map[MAP_WIDTH - 2][MAP_HEIGHT -2] = 4;
    }

    else if (r == 1) // bloc in the middle
    {
      for (i = x0-1; i <= x0+1; i++)
        for (j = y0-1; j <= y0+1; j++)
          map[i][j] = 4;
    }

    else if (r == 2) // checker
    {
      for (i = 2; i < MAP_WIDTH - 2; i = i + 2)
        for (j = 2; j < MAP_HEIGHT - 2; j = j + 2)
          map[i][j] = 4;
    }

    cleared = false;
    roomType = (roomTypeEnum)(rand() % 3);
  }
  else
  {
    cleared = true;
  }
}

int DungeonMap::hasNeighbourLeft()
{
  if (x > 0 && gameFloor->getRoom(x-1, y) > 0)
  {
    if (gameFloor->getRoom(x-1, y) == roomTypeBoss) return 2;
    else return 1;
  }
  return 0;
}
int DungeonMap::hasNeighbourRight()
{
  if (x < MAP_WIDTH -1 && gameFloor->getRoom(x+1, y) > 0)
  {
    if (gameFloor->getRoom(x+1, y) == roomTypeBoss) return 2;
    else return 1;
  }
  return 0;
}
int DungeonMap::hasNeighbourUp()
{
  if (y > 0 && gameFloor->getRoom(x, y-1) > 0)
  {
    if (gameFloor->getRoom(x, y-1) == roomTypeBoss) return 2;
    else return 1;
  }
  return 0;
}
int DungeonMap::hasNeighbourDown()
{
  if (y < MAP_HEIGHT -1 && gameFloor->getRoom(x, y+1) > 0)
  {
    if (gameFloor->getRoom(x, y+1) == roomTypeBoss) return 2;
    else return 1;
  }
  return 0;
}

void DungeonMap::initRoom()
{
  int x0 = MAP_WIDTH / 2;
  int y0 = MAP_HEIGHT / 2;
  int i, j;

  map[0][0] = MAP_WALL_7;
  for ( i = 1 ; i < width -1 ; i++)
  {
    map[i][0] = MAP_WALL_8;
    map[i][height - 1] = MAP_WALL_2;
  }
  map[width - 1][0] = MAP_WALL_9;
  for ( int i = 1 ; i < height -1 ; i++)
  {
    map[0][i] = MAP_WALL_4;
    map[width - 1][i] = MAP_WALL_6;
  }
  map[0][height - 1] = MAP_WALL_1;
  map[width - 1][height - 1] = MAP_WALL_3;



  for ( i = 1 ; i < width - 1 ; i++)
    for ( j = 1 ; j < height - 1 ; j++)
    {
      map[i][j] = 0;
      if (rand()%8 == 0) map[i][j] = rand()%(MAP_NORMAL_FLOOR + 1);
    }

  if (gameFloor != NULL)
  {

    if (x > 0 && gameFloor->getRoom(x-1, y) > 0)
    {
      //map[0][y0-1] = 0;
      map[0][y0] = 0;
      //map[0][y0+1] = 0;
    }
    if (x < MAP_WIDTH -1 && gameFloor->getRoom(x+1, y) > 0)
    {
      //map[MAP_WIDTH -1][y0-1] = 0;
      map[MAP_WIDTH -1][y0] = 0;
      //map[MAP_WIDTH -1][y0+1] = 0;
    }
    if (y > 0 && gameFloor->getRoom(x, y-1) > 0)
    {
      //map[x0-1][0] = 0;
      map[x0][0] = 0;
      //map[x0+1][0] = 0;
    }
    if (y < MAP_HEIGHT -1 && gameFloor->getRoom(x, y+1) > 0)
    {
      //map[x0-1][MAP_HEIGHT -1] = 0;
      map[x0][MAP_HEIGHT -1] = 0;
      //map[x0+1][MAP_HEIGHT -1] = 0;
    }
  }
}

Vector2D DungeonMap::generateBonusRoom()
{
  initRoom();
  int x0 = MAP_WIDTH / 2;
  int y0 = MAP_HEIGHT / 2;

  map[x0 - 1][y0 - 1] = MAP_WALL;
  map[x0 - 1][y0 + 1] = MAP_WALL;
  map[x0 + 1][y0 - 1] = MAP_WALL;
  map[x0 + 1][y0 + 1] = MAP_WALL;

  return (Vector2D(OFFSET_X + x0 * TILE_WIDTH + TILE_WIDTH / 2, OFFSET_Y + y0 * TILE_HEIGHT + TILE_HEIGHT / 2));
}

void DungeonMap::generateCarpet(int x0, int y0, int w, int h, int n)
{
  int xf = x0 + w - 1;
  int yf = y0 + h - 1;

  map[x0][y0] = n;
  map[x0][yf] = n + 6;
  map[xf][y0] = n + 2;
  map[xf][yf] = n + 8;

  int i, j;
  for (i = x0 + 1; i <= xf - 1; i++)
  {
    map[i][y0] = n + 1;
    map[i][yf] = n + 7;
    for (j = y0 + 1; j <= yf - 1; j++)
      map[i][j] = n + 4;
  }

  for (j = y0 + 1; j <= yf - 1; j++)
  {
    map[x0][j] = n + 3;
    map[xf][j] = n + 5;
  }
}

Vector2D DungeonMap::generateMerchantRoom()
{
  initRoom();
  int x0 = 3;
  int y0 = 3;

  generateCarpet(3, 3, 9, 3, 20);


  return (Vector2D(OFFSET_X + x0 * TILE_WIDTH + TILE_WIDTH / 2, OFFSET_Y + y0 * TILE_HEIGHT + TILE_HEIGHT / 2));
}

Vector2D DungeonMap::generateKeyRoom()
{
  initRoom();
  int x0 = MAP_WIDTH / 2;
  int y0 = MAP_HEIGHT / 2;

  for (int i = x0 - 1; i <= x0 + 1; i++)
    for (int j = y0 - 1; j <= y0 + 1; j++)
      map[i][j] = MAP_WALL;

  map[x0][y0] = 0;
  map[x0][y0+1] = MAP_DOOR;

  return (Vector2D(OFFSET_X + x0 * TILE_WIDTH + TILE_WIDTH / 2, OFFSET_Y + y0 * TILE_HEIGHT + TILE_HEIGHT / 2));
}

void DungeonMap::generateRoom(int type)
{
  initRoom();
  int x0 = MAP_WIDTH / 2;
  int y0 = MAP_HEIGHT / 2;
  int i, j, r;

  if (type == 0)
  {
    if (roomType == roomTypeStarting)
      generateCarpet(5, 3, 5, 3, 30);
  }
  if (type == 1)
  {
    // corner block
    map[1][1] = MAP_WALL;
    map[1][MAP_HEIGHT -2] = MAP_WALL;
    map[MAP_WIDTH - 2][1] = MAP_WALL;
    map[MAP_WIDTH - 2][MAP_HEIGHT -2] = MAP_WALL;
  }
  if (type == 2)
  {
    r = 1 + rand() % 3;
    for (i = x0 - r; i <= x0 + r; i++)
      for (j = y0 - 1; j <= y0 + 1; j++)
        map[i][j] = MAP_WALL;
  }
}

void DungeonMap::addItem(int itemType, float x, float y, bool merch)
{
  itemListElement ilm;
  ilm.type = itemType;
  ilm.x = x;
  ilm.y = y;
  ilm.merch = merch;
  itemList.push_back(ilm);
}

void DungeonMap::addSprite(int spriteType, int frame,  float x, float y, float scale)
{
  spriteListElement slm;
  slm.type = spriteType;
  slm.frame = frame;
  slm.x = x;
  slm.y = y;
  slm.scale = scale;
  spriteList.push_back(slm);
}

void DungeonMap::addChest(int chestType, bool state,  float x, float y)
{
  chestListElement clm;
  clm.type = chestType;
  clm.state = state;
  clm.x = x;
  clm.y = y;
  chestList.push_back(clm);
}

void DungeonMap::restoreItems(WitchBlastGame* parent)
{
  ItemList::iterator it;
  for (it = itemList.begin (); it != itemList.end ();)
  {
    itemListElement ilm = *it;
    it++;

    ItemEntity* itemEntity = new ItemEntity((enumItemType)(ilm.type), ilm.x, ilm.y, parent);
    itemEntity->setMerchandise(ilm.merch);
	}
	itemList.clear();
}

void DungeonMap::restoreSprites()
{
	SpriteList::iterator it;

  for (it = spriteList.begin (); it != spriteList.end ();)
  {
    spriteListElement ilm = *it;
    it++;

    if (ilm.type == ENTITY_BLOOD)
    {
      SpriteEntity* blood = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_BLOOD), ilm.x, ilm.y, 16, 16, 6);
      blood->setZ(OFFSET_Y - 1);
      blood->setFrame(ilm.frame);
      blood->setType(ENTITY_BLOOD);
      blood->setScale(ilm.scale, ilm.scale);
    }
    else if (ilm.type == ENTITY_CORPSE)
    {
      SpriteEntity* corpse;

      if (ilm.frame >= FRAME_CORPSE_KING_RAT)
      {
        corpse = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES_BIG), ilm.x, ilm.y, 128, 128);
        corpse->setFrame(ilm.frame - FRAME_CORPSE_KING_RAT);
      }
      else
      {
        corpse = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES), ilm.x, ilm.y, 64, 64);
        corpse->setFrame(ilm.frame);
      }

      corpse->setZ(OFFSET_Y);
      corpse->setType(ENTITY_CORPSE);
    }
  }
	spriteList.clear();
}

void DungeonMap::restoreChests(WitchBlastGame* parent)
{
  ChestList::iterator it;

  for (it = chestList.begin (); it != chestList.end ();)
  {
    chestListElement clm = *it;
    it++;

    ChestEntity* chestEntity = new ChestEntity(clm.x, clm.y, clm.type, clm.state, parent);
	}
	chestList.clear();
}

void DungeonMap::restoreMapObjects(WitchBlastGame* parent)
{
  restoreItems(parent);
  restoreSprites();
  restoreChests(parent);
}

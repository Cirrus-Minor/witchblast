#include "DungeonMap.h"
#include "GameFloor.h"
#include "ItemEntity.h"
#include "ChestEntity.h"
#include "sfml_game/ImageManager.h"
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include "WitchBlastGame.h"

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
  if (!inMap(x, y)) return true;
  return (map[x][y] < MAP_WALL);
}

bool DungeonMap::isFlyable(int x, int y)
{
  if (x <= 0) return false;
  if (x >= MAP_WIDTH - 1) return false;
  if (y <= 0) return false;
  if (y >= MAP_HEIGHT - 1) return false;
  if (map[x][y] >= MAP_WALL_8 && map[x][y] <= MAP_WALL_X) return false;
  return true;
}

bool DungeonMap::isShootable(int x, int y)
{
  if (!inMap(x, y)) return true;
  if (map[x][y] >=  MAP_HOLE) return true;
  return (map[x][y] < MAP_WALL);
}

bool DungeonMap::containsHealth()
{
  ItemList::iterator it;
  for (it = itemList.begin (); it != itemList.end ();)
  {
    itemListElement ilm = *it;
    it++;

    if (ilm.type >= ItemHealthVerySmall && ilm.type <= ItemHealthVerySmallPoison)
      return true;
	}

  return false;
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

roomTypeEnum DungeonMap::getNeighbourLeft()
{
  if (x > 0) return gameFloor->getRoom(x - 1, y);
  else return roomTypeNULL;
}
roomTypeEnum DungeonMap::getNeighbourRight()
{
  if (x < MAP_WIDTH - 1) return gameFloor->getRoom(x + 1, y);
  else return roomTypeNULL;
}
roomTypeEnum DungeonMap::getNeighbourUp()
{
  if (y > 0) return gameFloor->getRoom(x, y - 1);
  else return roomTypeNULL;
}
roomTypeEnum DungeonMap::getNeighbourDown()
{
  if (y < MAP_HEIGHT - 1) return gameFloor->getRoom(x, y + 1);
  else return roomTypeNULL;
}

int DungeonMap::getDivinity(int x, int y)
{
  if (x <= 0 || (x >= MAP_WIDTH - 1) || y <= 0 || (y >= MAP_HEIGHT - 1)) return -1;

  if (map[x][y] >= MAP_TEMPLE && map[x][y] < MAP_TEMPLE + NB_DIVINITY)
    return (map[x][y] - MAP_TEMPLE);
  else
    return -1;
}

void DungeonMap::initRoom()
{
  int x0 = MAP_WIDTH / 2;
  int y0 = MAP_HEIGHT / 2;
  int i, j;

  map[0][0] = MAP_WALL_7;
  for ( i = 1 ; i < width -1 ; i++)
  {
    if (i == width / 2)
    {
      map[i][0] = MAP_WALL_8;
      map[i][height - 1] = MAP_WALL_2;
    }
    else if (i < width / 2)
    {
      map[i][0] = MAP_WALL_87;
      map[i][height - 1] = MAP_WALL_21;
    }
    else
    {
      map[i][0] = MAP_WALL_89;
      map[i][height - 1] = MAP_WALL_23;
    }
  }
  map[width - 1][0] = MAP_WALL_9;
  for ( int i = 1 ; i < height -1 ; i++)
  {
    if (i == height / 2)
    {
      map[0][i] = MAP_WALL_4;
      map[width - 1][i] = MAP_WALL_6;
    }
    else if (i < height / 2)
    {
      map[0][i] = MAP_WALL_47;
      map[width - 1][i] = MAP_WALL_69;
    }
    else
    {
      map[0][i] = MAP_WALL_41;
      map[width - 1][i] = MAP_WALL_63;
    }
  }
  map[0][height - 1] = MAP_WALL_1;
  map[width - 1][height - 1] = MAP_WALL_3;



  for ( j = 1 ; j < height - 1 ; j++)
    for ( i = 1 ; i < width - 1 ; i++)
    {
      //map[i][j] = 0;
      //if (rand()%7 == 0) map[i][j] = rand()%(MAP_NORMAL_FLOOR + 1);

      map[i][j] = rand()%(MAP_NORMAL_FLOOR + 1);
      while (map[i][j] == map[i - 1][j] || map[i][j] == map[i][j - 1] || map[i][j] == map[i - 1][j - 1] || map[i][j] == map[i + 1][j - 1])
        map[i][j] = rand()%(MAP_NORMAL_FLOOR + 1);
    }

  if (gameFloor != NULL)
  {

    if (x > 0 && gameFloor->getRoom(x-1, y) > 0)
      map[0][y0] = 0;
    if (x < MAP_WIDTH -1 && gameFloor->getRoom(x+1, y) > 0)
      map[MAP_WIDTH -1][y0] = 0;
    if (y > 0 && gameFloor->getRoom(x, y-1) > 0)
      map[x0][0] = 0;
    if (y < MAP_HEIGHT -1 && gameFloor->getRoom(x, y+1) > 0)
      map[x0][MAP_HEIGHT -1] = 0;
  }
}

void DungeonMap::initPattern(patternEnum n)
{
  int i, j;

  if (n == PatternSmallChecker)
  {
    for ( i = 2 ; i < width - 2 ; i++)
      for ( j = 2 ; j < height - 2 ; j++)
      {
        if ((i + j) % 2 == 1)
          map[i][j] += 10;
      }
  }
  if (n == PatternBigChecker)
  {
    for ( i = 1 ; i < width - 1 ; i++)
      for ( j = 1 ; j < height - 1 ; j++)
      {
        if ((i + j) % 2 == 1)
          map[i][j] += 10;
      }
  }
  if (n == PatternBorder)
  {
    for ( i = 1 ; i < width - 1 ; i++)
      for ( j = 1 ; j < height - 1 ; j++)
      {
        if (i == 1 || j == 1 || i == width - 2 || j == height - 2)
          map[i][j] += 10;
      }
  }
  if (n == PatternBigCircle)
  {
    for ( i = 2 ; i < width - 2 ; i++)
      for ( j = 2 ; j < height - 2 ; j++)
      {
        if (i == 2 || j == 2 || i == width - 3 || j == height - 3)
          map[i][j] += 10;
      }
  }
  if (n == PatternSmallCircle || n == PatternSmallStar)
  {
    for ( i = 5 ; i < 10 ; i++)
      for ( j = 2 ; j < height - 2 ; j++)
      {
        if (i == 5 || i == 9 || j == 2 || j == height - 3)
          map[i][j] += 10;
      }
  }
  if (n == PatternSmallStar)
  {
    map[7][1] += 10;
    map[7][height - 2] += 10;
    map[4][4] += 10;
    map[10][4] += 10;
  }
  if (n == PatternSmallDisc)
  {
    for ( i = 5 ; i < 10 ; i++)
      for ( j = 2 ; j < height - 2 ; j++)
      {
        map[i][j] += 10;
      }
  }
}

Vector2D DungeonMap::generateBonusRoom()
{
  initRoom();
  int x0 = MAP_WIDTH / 2;
  int y0 = MAP_HEIGHT / 2;

  if (rand() % 3 == 0)
  {
    if (rand() % 2 == 0) initPattern(PatternSmallDisc);
    else initPattern(PatternSmallStar);
  }

  map[x0 - 1][y0 - 1] = MAP_WALL;
  map[x0 - 1][y0 + 1] = MAP_WALL;
  map[x0 + 1][y0 - 1] = MAP_WALL;
  map[x0 + 1][y0 + 1] = MAP_WALL;

  return (Vector2D(x0 * TILE_WIDTH + TILE_WIDTH / 2, y0 * TILE_HEIGHT + TILE_HEIGHT / 2));
}

void DungeonMap::generateTemple(int x, int y, enumDivinityType type)
{
  map[x - 1][y - 1] = MAP_WALL;
  map[x][y - 2] = MAP_TEMPLE_WALL + (int)type;
  map[x][y - 1] = MAP_TEMPLE_WALL + 10 + (int)type;
  map[x + 1][y - 1] = MAP_WALL;

  map[x - 1][y] = MAP_WALL;
  map[x][y] = MAP_TEMPLE + (int)type;
  map[x + 1][y] = MAP_WALL;
}

void DungeonMap::generateTempleRoom()
{
  initRoom();
  int x0 = MAP_WIDTH / 2;
  int y0 = MAP_HEIGHT / 2;

  if (rand() % 3 == 0)
  {
    if (rand() % 2 == 0) initPattern(PatternSmallDisc);
    else initPattern(PatternSmallStar);
  }

  if (rand() % 2 == 0)
  {
    // one temple
    generateTemple(x0, y0, (enumDivinityType)(rand() % NB_DIVINITY));
  }
  else
  {
    // two temples
    generateTemple(x0 - 2, y0, DivinityHeal);
    generateTemple(x0 + 2, y0, DivinityPower);
  }

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


  return (Vector2D(x0 * TILE_WIDTH + TILE_WIDTH / 2, y0 * TILE_HEIGHT + TILE_HEIGHT / 2));
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

  if (rand() % 3 == 0)
  {
    if (rand() % 2 == 0) initPattern(PatternSmallCircle);
    else initPattern(PatternSmallStar);
  }

  return (Vector2D(x0 * TILE_WIDTH + TILE_WIDTH / 2, y0 * TILE_HEIGHT + TILE_HEIGHT / 2));
}

void DungeonMap::generateExitRoom()
{
  initRoom();
  int x0 = MAP_WIDTH / 2;
  map[x0][0] = MAP_STAIRS_UP;
  map[x0 - 1][0] = MAP_WALL_EXIT_L;
  map[x0 + 1][0] = MAP_WALL_EXIT_R;

  if (rand() % 3 == 0) initPattern(PatternBorder);
}

void DungeonMap::generateRoomRandom(int type)
{
  if (rand() % 3 == 0) generateRoomWithHoles(type);
  else generateRoomWithoutHoles(type);
}

void DungeonMap::generateRoomWithoutHoles(int type)
{
  initRoom();
  int x0 = MAP_WIDTH / 2;
  int y0 = MAP_HEIGHT / 2;
  int i, j, r;

  if (type == 0)
  {
    if (roomType == roomTypeStarting)
    {
      generateCarpet(5, 3, 5, 3, 30);

      if (game().getLevel() > 1)
      {
        map[x0 - 1][MAP_HEIGHT - 1] = MAP_WALL_START_L;
        map[x0][MAP_HEIGHT - 1]     = MAP_WALL_START_M;
        map[x0 + 1][MAP_HEIGHT - 1] = MAP_WALL_START_R;
      }
    }
    else if (roomType == roomTypeBoss && (game().getLevel() == 2 || game().getLevel() > 5) ) // giant slime
    {
      map[1][1] = MAP_GRID;
      map[1][MAP_HEIGHT -2] = MAP_GRID;
      map[MAP_WIDTH - 2][1] = MAP_GRID;
      map[MAP_WIDTH - 2][MAP_HEIGHT -2] = MAP_GRID;
    }
    if (roomType == roomTypeStandard)
    {
      if (rand() % 3 > 0 && gameFloor->neighboorCount(x, y) > 1)
      {
        generateCorridors();
      }
      else
      {
        if (rand() % 3 == 0) initPattern((patternEnum)(rand() % 4));
      }
    }
  }
  if (type == 1)
  {
    // corner block
    if (rand() % 3 == 0) initPattern(PatternSmallChecker);

    map[0][0] = MAP_WALL_X;
    map[0][1] = MAP_WALL_7;
    map[1][1] = MAP_WALL_33;
    map[1][0] = MAP_WALL_7;

    map[0][MAP_HEIGHT -1] = MAP_WALL_X;
    map[0][MAP_HEIGHT -2] = MAP_WALL_1;
    map[1][MAP_HEIGHT -2] = MAP_WALL_99;
    map[1][MAP_HEIGHT -1] = MAP_WALL_1;

    map[MAP_WIDTH - 1][0] = MAP_WALL_X;
    map[MAP_WIDTH - 1][1] = MAP_WALL_9;
    map[MAP_WIDTH - 2][1] = MAP_WALL_11;
    map[MAP_WIDTH - 2][0] = MAP_WALL_9;

    map[MAP_WIDTH - 1][MAP_HEIGHT -1] = MAP_WALL_X;
    map[MAP_WIDTH - 1][MAP_HEIGHT -2] = MAP_WALL_3;
    map[MAP_WIDTH - 2][MAP_HEIGHT -2] = MAP_WALL_77;
    map[MAP_WIDTH - 2][MAP_HEIGHT -1] = MAP_WALL_3;
  }
  if (type == 2)
  {
    // blocks in the middle
    if (rand() % 3 == 0) initPattern(PatternBorder);

    r = 1 + rand() % 3;
    for (i = x0 - r; i <= x0 + r; i++)
      for (j = y0 - 1; j <= y0 + 1; j++)
        map[i][j] = MAP_WALL;
  }
  if (type == 3)
  {
    // blocks in the corners
    for (i = 2; i <= 3; i++)
      for (j = 2; j <= 3; j++)
      {
        map[i][MAP_HEIGHT - 1 - j] = MAP_WALL;
        map[i][j] = MAP_WALL;
        map[MAP_WIDTH - 1 - i][MAP_HEIGHT - 1 - j] = MAP_WALL;
        map[MAP_WIDTH - 1 - i][j] = MAP_WALL;
      }
  }
  if (type == ROOM_TYPE_CHECKER)
  {
    // "checker"
    if (rand() % 3 == 0) initPattern(PatternSmallChecker);

    for (i = 2; i < MAP_WIDTH - 2; i = i + 2)
      for (j = 2; j < MAP_HEIGHT - 2; j = j + 2)
        map[i][j] = MAP_WALL;
  }
}

void DungeonMap::generateRoomWithHoles(int type)
{
  initRoom();
  int x0 = MAP_WIDTH / 2;
  int y0 = MAP_HEIGHT / 2;
  int i, j, r;

  if (type == 0)
  {
    if (roomType == roomTypeStarting)
    {
      generateCarpet(5, 3, 5, 3, 30);

      if (game().getLevel() > 1)
      {
        map[x0 - 1][MAP_HEIGHT - 1] = 62;
        map[x0][MAP_HEIGHT - 1]     = 63;
        map[x0 + 1][MAP_HEIGHT - 1] = 64;
      }
    }
    if (roomType == roomTypeBoss && game().getLevel() == 2) // giant slime
    {
      map[1][1] = MAP_GRID;
      map[1][MAP_HEIGHT -2] = MAP_GRID;
      map[MAP_WIDTH - 2][1] = MAP_GRID;
      map[MAP_WIDTH - 2][MAP_HEIGHT -2] = MAP_GRID;
    }
  }
  if (type == 1)
  {
    // corner hole
    if (rand() % 3 == 0) initPattern(PatternSmallChecker);

    map[1][1] = MAP_HOLE_TOP;
    map[1][MAP_HEIGHT -2] = MAP_HOLE_TOP;
    map[MAP_WIDTH - 2][1] = MAP_HOLE_TOP;
    map[MAP_WIDTH - 2][MAP_HEIGHT -2] = MAP_HOLE_TOP;
  }
  if (type == 2)
  {
    // blocks in the middle
    if (rand() % 3 == 0) initPattern(PatternBorder);

    r = 1 + rand() % 2;
    for (i = x0 - r; i <= x0 + r; i++)
      for (j = y0 - 1; j <= y0 + 1; j++)
      {
        if (i == x0 - r && j == y0 - 1) map[i][j] = MAP_HOLE_TOP;
        else if (i == x0 - r && j == y0 + 1) map[i][j] = MAP_HOLE_BOTTOM;
        else if (i == x0 - r) map[i][j] = MAP_HOLE_BOTTOM;
        else if (i == x0 + r && j == y0 - 1) map[i][j] = MAP_HOLE_TOP;
        else if (i == x0 + r && j == y0 + 1) map[i][j] = MAP_HOLE_BOTTOM;
        else if (i == x0 + r) map[i][j] = MAP_HOLE_BOTTOM;
        else if (j == y0 - 1) map[i][j] = MAP_HOLE_TOP;
        else if (j == y0 + 1) map[i][j] = MAP_HOLE_BOTTOM;
        else map[i][j] = MAP_HOLE_BOTTOM;
      }
  }
  if (type == 3)
  {
    // 4 holes
    map[2][2] = MAP_HOLE_TOP;
    map[2][3] = MAP_HOLE_BOTTOM;
    map[3][2] = MAP_HOLE_TOP;
    map[3][3] = MAP_HOLE_BOTTOM;

    map[MAP_WIDTH - 4][2] = MAP_HOLE_TOP;
    map[MAP_WIDTH - 4][3] = MAP_HOLE_BOTTOM;
    map[MAP_WIDTH - 3][2] = MAP_HOLE_TOP;
    map[MAP_WIDTH - 3][3] = MAP_HOLE_BOTTOM;

    map[2][MAP_HEIGHT - 4] = MAP_HOLE_TOP;
    map[2][MAP_HEIGHT - 3] = MAP_HOLE_BOTTOM;
    map[3][MAP_HEIGHT - 4] = MAP_HOLE_TOP;
    map[3][MAP_HEIGHT - 3] = MAP_HOLE_BOTTOM;

    map[MAP_WIDTH - 4][MAP_HEIGHT - 4] = MAP_HOLE_TOP;
    map[MAP_WIDTH - 4][MAP_HEIGHT - 3] = MAP_HOLE_BOTTOM;
    map[MAP_WIDTH - 3][MAP_HEIGHT - 4] = MAP_HOLE_TOP;
    map[MAP_WIDTH - 3][MAP_HEIGHT - 3] = MAP_HOLE_BOTTOM;
  }
  if (type == 4)
  {
    // "checker"
    for (i = 2; i < MAP_WIDTH - 2; i = i + 2)
      for (j = 2; j < MAP_HEIGHT - 2; j = j + 2)
        map[i][j] = MAP_HOLE_TOP;
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

void DungeonMap::restoreItems()
{
  ItemList::iterator it;
  for (it = itemList.begin (); it != itemList.end ();)
  {
    itemListElement ilm = *it;
    it++;

    ItemEntity* itemEntity = new ItemEntity((enumItemType)(ilm.type), ilm.x, ilm.y);
    itemEntity->setMerchandise(ilm.merch);
	}
}

void DungeonMap::restoreSprites()
{
	SpriteList::iterator it;

  for (it = spriteList.begin (); it != spriteList.end ();)
  {
    spriteListElement ilm = *it;
    it++;

    if (ilm.type == ENTITY_BLOOD)
      game().getCurrentMapEntity()->addBlood(ilm.x, ilm.y, ilm.frame, ilm.scale);

    else if (ilm.type == ENTITY_CORPSE)
      game().getCurrentMapEntity()->addCorpse(ilm.x, ilm.y, ilm.frame);
  }
}

void DungeonMap::restoreChests()
{
  ChestList::iterator it;

  for (it = chestList.begin (); it != chestList.end ();)
  {
    chestListElement clm = *it;
    it++;

    new ChestEntity(clm.x, clm.y, clm.type, clm.state);
	}
}

void DungeonMap::restoreMapObjects()
{
  restoreItems();
  restoreSprites();
  restoreChests();
  cleanMapObjects();
}

void DungeonMap::cleanMapObjects()
{
	itemList.clear();
	spriteList.clear();
  chestList.clear();
}

void DungeonMap::addRandomGrids(int n)
{
  int counter = n;

  while (counter > 0)
  {
    int rx = 1 + rand() % (MAP_WIDTH - 2);
    int ry = 1 + rand() % (MAP_HEIGHT - 2);
    if (map[rx][ry] < 5)
    {
      map[rx][ry] = MAP_GRID;
      counter--;
    }
  }
}

void DungeonMap::generateCorridors()
{
  int xCor = 1 + rand()% 4;
  int yCor = 1 + rand()% 1;

  if (!hasNeighbourLeft())
  {
    for (int i = 0; i < xCor; i++)
      for (int j = 0; j < MAP_HEIGHT; j++)
        map[i][j] = MAP_WALL_X;
  }
  if (!hasNeighbourRight())
  {
    for (int i = MAP_WIDTH - 1; i > MAP_WIDTH - 1 - xCor; i--)
      for (int j = 0; j < MAP_HEIGHT; j++)
        map[i][j] = MAP_WALL_X;
  }
  if (!hasNeighbourUp())
  {
    for (int i = 0; i < MAP_WIDTH; i++)
      for (int j = 0; j < yCor; j++)
        map[i][j] = MAP_WALL_X;
  }
  if (!hasNeighbourDown())
  {
    for (int i = 0; i < MAP_WIDTH; i++)
      for (int j = MAP_HEIGHT - 1; j > MAP_HEIGHT - 1 - yCor; j--)
        map[i][j] = MAP_WALL_X;
  }

  //
  for (int i = 0; i < MAP_WIDTH; i++)
  {
    for (int j = 0; j < MAP_HEIGHT; j++)
    {
      if (map[i][j] != MAP_WALL_X)
      {
        if (getTile(i - 1, j) == MAP_WALL_X)
        {
          if (j == 0 || getTile(i, j - 1) == MAP_WALL_X) map[i][j] = MAP_WALL_7;
          else if (j == MAP_HEIGHT - 1 || getTile(i, j + 1) == MAP_WALL_X) map[i][j] = MAP_WALL_1;
          else
          {
            if (j < MAP_HEIGHT / 2) map[i][j] = MAP_WALL_47;
            else if (j > MAP_HEIGHT / 2) map[i][j] = MAP_WALL_41;
            else map[i][j] = MAP_WALL_4;
          }

        }
        else if (getTile(i + 1, j) == MAP_WALL_X)
        {
          if (j == 0 || getTile(i, j - 1) == MAP_WALL_X) map[i][j] = MAP_WALL_9;
          else if (j == MAP_HEIGHT - 1 || getTile(i, j + 1) == MAP_WALL_X) map[i][j] = MAP_WALL_3;
          else
          {
            if (j < MAP_HEIGHT / 2) map[i][j] = MAP_WALL_69;
            else if (j > MAP_HEIGHT / 2) map[i][j] = MAP_WALL_63;
            else map[i][j] = MAP_WALL_6;
          }
        }
        else if (getTile(i, j - 1) == MAP_WALL_X)
        {
          if (i == 0) map[i][j] = MAP_WALL_7;
          else if (i == MAP_WIDTH - 1) map[i][j] = MAP_WALL_9;
          else
          {
            if (i < MAP_WIDTH / 2) map[i][j] = MAP_WALL_87;
            else if (i > MAP_WIDTH / 2) map[i][j] = MAP_WALL_89;
            else map[i][j] = MAP_WALL_8;
          }
        }
        else if (getTile(i, j + 1) == MAP_WALL_X)
        {
          if (i == 0) map[i][j] = MAP_WALL_1;
          else if (i == MAP_WIDTH - 1) map[i][j] = MAP_WALL_3;
          else
          {
            if (i < MAP_WIDTH / 2) map[i][j] = MAP_WALL_21;
            else if (i > MAP_WIDTH / 2) map[i][j] = MAP_WALL_23;
            else map[i][j] = MAP_WALL_2;
          }
        }
      }
    }
  }
}

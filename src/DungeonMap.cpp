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
  randomTileElement.type = -1;
  randomTileElement.x = 0;
  randomTileElement.y = 0;
  randomTileElement.rotation = 0;
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

int DungeonMap::getObjectTile(int x, int y)
{
  return objectsMap[x][y];
}

int DungeonMap::getShadowTile(int x, int y)
{
  return shadowMap[x][y];
}

logicalMapStateEnum DungeonMap::getLogicalTile(int x, int y)
{
  return logicalMap[x][y];
}

void DungeonMap::setObjectTile(int x, int y, int n)
{
  objectsMap[x][y] = n;
}

void DungeonMap::setShadowTile(int x, int y, int n)
{
  shadowMap[x][y] = n;
}

void DungeonMap::setLogicalTile(int x, int y, logicalMapStateEnum state)
{
  logicalMap[x][y] = state;
}

int DungeonMap::getFloorOffset()
{
  return floorOffset;
}
int DungeonMap::getWallOffset()
{
  return wallOffset;
}
void DungeonMap::setFloorOffset(int n)
{
  floorOffset = n;
}
void DungeonMap::setWallOffset(int n)
{
  wallOffset = n;
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

DungeonMap::RandomTileElement DungeonMap::getRandomTileElement()
{
  return randomTileElement;
}

void DungeonMap::setRandomTileElement (RandomTileElement rt)
{
  randomTileElement = rt;
  hasChanged = true;
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
  return (logicalMap[x][y] != LogicalFloor);
}

bool DungeonMap::isUpBlocking(int x, int y)
{
  if (!inMap(x, y)) return false;
  return (logicalMap[x][y] != LogicalFloor);
}

bool DungeonMap::isLeftBlocking(int x, int y)
{
  if (!inMap(x, y)) return false;
  return (logicalMap[x][y] != LogicalFloor);
}

bool DungeonMap::isRightBlocking(int x, int y)
{
  if (!inMap(x, y)) return false;
  return (logicalMap[x][y] != LogicalFloor);
}

bool DungeonMap::isWalkable(int x, int y)
{
  if (!inMap(x, y)) return true;
  if (roomType == roomTypeKey && !cleared)
  {
    int x0 = MAP_WIDTH / 2;
    int y0 = MAP_HEIGHT / 2;
    if (x >= x0 - 1 && x <= x0 +1 && y >= y0 - 1 && y <= y0 + 1)
      return false;
  }
  return (logicalMap[x][y] == LogicalFloor);
}

bool DungeonMap::isFlyable(int x, int y)
{
  if (x < 0) return true;
  if (x > MAP_WIDTH - 1) return true;
  if (y < 0) return true;
  if (y > MAP_HEIGHT - 1) return true;

  return (logicalMap[x][y] != LogicalWall);
}

bool DungeonMap::isShootable(int x, int y)
{
  if (!inMap(x, y)) return true;
  return (logicalMap[x][y] != LogicalWall && logicalMap[x][y] != LogicalObstacle);
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

  // style
  floorOffset = ((game().getLevel() - 1) % 8) * 24 ;
  wallOffset = 0; //((game().getLevel() - 1) % 3) * 48;

  // outer walls
  map[0][0] = wallOffset + MAP_WALL_7;
  for ( i = 1 ; i < width -1 ; i++)
  {
    if (i == width / 2)
    {
      map[i][0] = wallOffset + MAP_WALL_8;
      map[i][height - 1] = wallOffset + MAP_WALL_8;
    }
    else if (i < width / 2)
    {
      map[i][0] = wallOffset + MAP_WALL_87;
      map[i][height - 1] = wallOffset + MAP_WALL_87;
    }
    else
    {
      map[i][0] = wallOffset + MAP_WALL_87;
      map[i][height - 1] = wallOffset + MAP_WALL_87;
    }
  }
  map[width - 1][0] = wallOffset + MAP_WALL_7;
  for ( int i = 1 ; i < height -1 ; i++)
  {
    if (i == height / 2)
    {
      map[0][i] = wallOffset + MAP_WALL_8;
      map[width - 1][i] = wallOffset + MAP_WALL_8;
    }
    else if (i < height / 2)
    {
      map[0][i] = wallOffset + MAP_WALL_87;
      map[width - 1][i] = wallOffset + MAP_WALL_87;
    }
    else
    {
      map[0][i] = wallOffset + MAP_WALL_87;
      map[width - 1][i] = wallOffset + MAP_WALL_87;
    }
  }
  map[0][height - 1] = wallOffset + MAP_WALL_7;
  map[width - 1][height - 1] = wallOffset + MAP_WALL_7;

  // floor
  for ( j = 1 ; j < height - 1 ; j++)
  {
    for ( i = 1 ; i < width - 1 ; i++)
    {
      map[i][j] = floorOffset + rand()%(MAP_NORMAL_FLOOR);
      while (map[i][j] == map[i - 1][j] || map[i][j] == map[i][j - 1] || map[i][j] == map[i - 1][j - 1] || map[i][j] == map[i + 1][j - 1])
        map[i][j] = floorOffset + rand()%(MAP_NORMAL_FLOOR);
    }
  }

  // objects
  for ( j = 0 ; j < height; j++)
  {
    for ( i = 0 ; i < width; i++)
    {
      objectsMap[i][j] = 0;
      shadowMap[i][j] = MAPSHAD_EMPTY;
      if (i == 0 || j == 0 || i == width - 1 || j == height - 1)
        logicalMap[i][j] = LogicalWall;
      else
        logicalMap[i][j] = LogicalFloor;
    }
  }

  // alternative floor
  for (i = 0; i < 8; i++)
  {
    if (rand() % 2 > 0)
    {
      map[1 + rand() % (MAP_WIDTH - 2)][1 + rand() % (MAP_HEIGHT - 2)] = floorOffset + 16 + i;
    }
  }

  // alternative walls
  for (i = 0; i < 9; i++)
  {
    if (rand() % 2 > 0)
    {
      int xTile = 0;
      int yTile = 0;
      int horizontal = rand() % 2;
      if (horizontal == 0)
      {
        yTile = rand() % 2 == 0 ? 0 : MAP_HEIGHT - 1;
        xTile = 1 + rand() % 12;
        if (xTile > 6) xTile++;
      }
      else
      {
        xTile = rand() % 2 == 0 ? 0 : MAP_WIDTH - 1;
        yTile = 1 + rand() % 6;
        if (yTile > 3) yTile++;
      }
      map[xTile][yTile] = i + wallOffset + MAP_WALL_ALTERN;
    }
  }
  if (hasNeighbourUp() && rand() % 3 == 0)
  {
    map[x0 - 2][0] = 9 + wallOffset + MAP_WALL_ALTERN;
    map[x0 + 2][0] = 9 + wallOffset + MAP_WALL_ALTERN;
  }
  if (hasNeighbourDown() && rand() % 3 == 0)
  {
    map[x0 - 2][MAP_HEIGHT - 1] = 9 + wallOffset + MAP_WALL_ALTERN;
    map[x0 + 2][MAP_HEIGHT - 1] = 9 + wallOffset + MAP_WALL_ALTERN;
  }
  if (hasNeighbourLeft() && rand() % 3 == 0)
  {
    map[0][y0 - 2] = 9 + wallOffset + MAP_WALL_ALTERN;
    map[0][y0 + 2] = 9 + wallOffset + MAP_WALL_ALTERN;
  }
  if (hasNeighbourRight() && rand() % 3 == 0)
  {
    map[MAP_WIDTH - 1][y0 - 2] = 9 + wallOffset + MAP_WALL_ALTERN;
    map[MAP_WIDTH - 1][y0 + 2] = 9 + wallOffset + MAP_WALL_ALTERN;
  }

  // doors ?
  if (gameFloor != NULL)
  {
    if (x > 0 && gameFloor->getRoom(x - 1, y) > 0)
    {
      map[0][MAP_HEIGHT / 2] = wallOffset + MAP_WALL_DOOR_8;
      map[0][MAP_HEIGHT / 2 - 1] = wallOffset + MAP_WALL_DOOR_7;
      map[0][MAP_HEIGHT / 2 + 1] = wallOffset + MAP_WALL_DOOR_7;
      openDoor(0, y0);
    }

    if (x < MAP_WIDTH - 1 && gameFloor->getRoom(x + 1, y) > 0)
    {
      map[MAP_WIDTH - 1][MAP_HEIGHT / 2] = wallOffset + MAP_WALL_DOOR_8;
      map[MAP_WIDTH - 1][MAP_HEIGHT / 2 - 1] = wallOffset + MAP_WALL_DOOR_7;
      map[MAP_WIDTH - 1][MAP_HEIGHT / 2 + 1] = wallOffset + MAP_WALL_DOOR_7;
      openDoor(MAP_WIDTH - 1, y0);
    }

    if (y > 0 && gameFloor->getRoom(x, y-1) > 0)
    {
      map[MAP_WIDTH / 2][0] = wallOffset + MAP_WALL_DOOR_8;
      map[MAP_WIDTH / 2 - 1][0] = wallOffset + MAP_WALL_DOOR_7;
      map[MAP_WIDTH / 2 + 1][0] = wallOffset + MAP_WALL_DOOR_7;
      openDoor(x0, 0);
    }

    if (y < MAP_HEIGHT -1 && gameFloor->getRoom(x, y+1) > 0)
    {
      map[MAP_WIDTH / 2][MAP_HEIGHT - 1] = wallOffset + MAP_WALL_DOOR_8;
      map[MAP_WIDTH / 2 - 1][MAP_HEIGHT - 1] = wallOffset + MAP_WALL_DOOR_7;
      map[MAP_WIDTH / 2 + 1][MAP_HEIGHT - 1] = wallOffset + MAP_WALL_DOOR_7;
      openDoor(x0, MAP_HEIGHT -1);
    }
  }

  // shadows
  for (int i = 0; i < MAP_WIDTH; i++)
  {
    if (i == 0)
    {
      shadowMap[i][0] = MAPSHAD_7;
      shadowMap[i][MAP_HEIGHT - 1] = MAPSHAD_1;
    }
    else if (i == 1)
    {
      shadowMap[i][0] = MAPSHAD_87;
      shadowMap[i][MAP_HEIGHT - 1] = MAPSHAD_21;
      shadowMap[i][1] = MAPSHAD_GROUND_7;
      shadowMap[i][MAP_HEIGHT - 2] = MAPSHAD_GROUND_1;
    }
    else if (i == MAP_WIDTH - 1)
    {
      shadowMap[i][0] = MAPSHAD_9;
      shadowMap[i][MAP_HEIGHT - 1] = MAPSHAD_3;
    }
    else if (i == MAP_WIDTH - 2)
    {
      shadowMap[i][0] = MAPSHAD_89;
      shadowMap[i][MAP_HEIGHT - 1] = MAPSHAD_23;
      shadowMap[i][1] = MAPSHAD_GROUND_9;
      shadowMap[i][MAP_HEIGHT - 2] = MAPSHAD_GROUND_3;
    }
    else
    {
      shadowMap[i][0] = MAPSHAD_8;
      shadowMap[i][MAP_HEIGHT - 1] = MAPSHAD_2;
      shadowMap[i][1] = MAPSHAD_GROUND_8;
      shadowMap[i][MAP_HEIGHT - 2] = MAPSHAD_GROUND_2;
    }
  }

  for (int i = 1; i < MAP_HEIGHT - 1; i++)
  {
    if (i == 1)
    {
      shadowMap[0][i] = MAPSHAD_47;
      shadowMap[MAP_WIDTH - 1][i] = MAPSHAD_69;
    }
    else if (i == MAP_HEIGHT - 2)
    {
      shadowMap[0][i] = MAPSHAD_41;
      shadowMap[MAP_WIDTH - 1][i] = MAPSHAD_63;
    }
    else
    {
      shadowMap[0][i] = MAPSHAD_4;
      shadowMap[MAP_WIDTH - 1][i] = MAPSHAD_6;
      shadowMap[1][i] = MAPSHAD_GROUND_4;
      shadowMap[MAP_WIDTH - 2][i] = MAPSHAD_GROUND_6;
    }
  }

}

void DungeonMap::openDoor(int x, int y)
{
  objectsMap[x][y] = MAPOBJ_DOOR_OPEN;
  logicalMap[x][y] = LogicalFloor;
}

void DungeonMap::closeDoor(int x, int y)
{
  objectsMap[x][y] = MAPOBJ_DOOR_CLOSED;
  logicalMap[x][y] = LogicalWall;
}

bool DungeonMap::isDoor(int x, int y)
{
  return objectsMap[x][y] == MAPOBJ_DOOR_OPEN || objectsMap[x][y] == MAPOBJ_DOOR_CLOSED;
}

void DungeonMap::makePatternTile(int x, int y)
{
  if (map[x][y] < 24 * 8 && (map[x][y] % 24) < 8) map[x][y] += 8;
  else map[x][y] = (game().getLevel() - 1) * 24 + 8;
}

void DungeonMap::initPattern(patternEnum n)
{
  int i, j;

  if (n == PatternSmallChecker)
  {
    for ( i = 2 ; i < width - 2 ; i++)
      for ( j = 2 ; j < height - 2 ; j++)
      {
        if ((i + j) % 2 == 1) makePatternTile(i, j);
      }
  }
  if (n == PatternBigChecker)
  {
    for ( i = 1 ; i < width - 1 ; i++)
      for ( j = 1 ; j < height - 1 ; j++)
      {
        if ((i + j) % 2 == 1) makePatternTile(i, j);
      }
  }
  if (n == PatternBorder)
  {
    for ( i = 1 ; i < width - 1 ; i++)
      for ( j = 1 ; j < height - 1 ; j++)
      {
        if (i == 1 || j == 1 || i == width - 2 || j == height - 2)
          makePatternTile(i, j);
      }
  }
  if (n == PatternBigCircle)
  {
    for ( i = 2 ; i < width - 2 ; i++)
      for ( j = 2 ; j < height - 2 ; j++)
      {
        if (i == 2 || j == 2 || i == width - 3 || j == height - 3)
          makePatternTile(i, j);
      }
  }
  if (n == PatternSmallCircle || n == PatternSmallStar)
  {
    for ( i = 5 ; i < 10 ; i++)
      for ( j = 2 ; j < height - 2 ; j++)
      {
        if (i == 5 || i == 9 || j == 2 || j == height - 3)
          makePatternTile(i, j);
      }
  }
  if (n == PatternSmallStar)
  {
    makePatternTile(7, 1);
    makePatternTile(7, height - 2);
    makePatternTile(4, 4);
    makePatternTile(10, 4);
  }
  if (n == PatternSmallDisc)
  {
    for ( i = 5 ; i < 10 ; i++)
      for ( j = 2 ; j < height - 2 ; j++)
      {
        makePatternTile(i, j);
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

  objectsMap[x0 - 1][y0 - 1] = MAPOBJ_WALL_SPECIAL;
  objectsMap[x0 - 1][y0 + 1] = MAPOBJ_WALL_SPECIAL + 2;
  objectsMap[x0 + 1][y0 - 1] = MAPOBJ_WALL_SPECIAL + 1;
  objectsMap[x0 + 1][y0 + 1] = MAPOBJ_WALL_SPECIAL + 3;

  logicalMap[x0 - 1][y0 - 1] = LogicalObstacle;
  logicalMap[x0 - 1][y0 + 1] = LogicalObstacle;
  logicalMap[x0 + 1][y0 - 1] = LogicalObstacle;
  logicalMap[x0 + 1][y0 + 1] = LogicalObstacle;

  generateRandomTile();

  return (Vector2D(x0 * TILE_WIDTH + TILE_WIDTH / 2, y0 * TILE_HEIGHT + TILE_HEIGHT / 2));
}

void DungeonMap::generateTemple(int x, int y, enumDivinityType type)
{
  map[x][y] = MAP_TEMPLE + (int)type;
  addHole(x - 1, y - 2);
  addHole(x - 1, y - 1);
  addHole(x - 1, y);
  addHole(x + 1, y - 2);
  addHole(x + 1, y - 1);
  addHole(x + 1, y );

  objectsMap[x][y - 2] = MAPOBJ_TEMPLE_WALL + (int)type;
  objectsMap[x][y - 1] = MAPOBJ_TEMPLE_WALL + 10 + (int)type;
  logicalMap[x][y - 2] = LogicalObstacle;
  logicalMap[x][y - 1] = LogicalObstacle;
}

void DungeonMap::generateTempleRoom()
{
  initRoom();
  int x0 = MAP_WIDTH / 2;
  int y0 = 1 + MAP_HEIGHT / 2;

  if (rand() % 3 == 0)
  {
    if (rand() % 2 == 0) initPattern(PatternSmallDisc);
    else initPattern(PatternSmallStar);
  }

  int d0, d1, d2;
  d0 = rand() % NB_DIVINITY;
  d1 = d0;
  while (d0 == d1) d1 = rand() % NB_DIVINITY;

  if (rand() % 2 == 0)
  {
    // three temple
    d2 = d0;
    while (d0 == d2 || d1 == d2) d2 = rand() % NB_DIVINITY;
    generateTemple(x0 - 2, y0, (enumDivinityType)d0);
    generateTemple(x0 + 2, y0, (enumDivinityType)d1);
    generateTemple(x0, y0, (enumDivinityType)d2);
  }
  else
  {
    // two temples
    generateTemple(x0 - 1, y0, (enumDivinityType)d0);
    generateTemple(x0 + 1, y0, (enumDivinityType)d1);
  }

  generateRandomTile();
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

void DungeonMap::generateTable(int x0, int y0, int w, int h, int n)
{
  int xf = x0 + w - 1;
  int yf = y0 + h - 1;

  objectsMap[x0][y0] = n;
  objectsMap[x0][yf] = n + 6;
  objectsMap[xf][y0] = n + 2;
  objectsMap[xf][yf] = n + 8;

  int i, j;
  for (i = x0 + 1; i <= xf - 1; i++)
  {
    objectsMap[i][y0] = n + 1;
    objectsMap[i][yf] = n + 7;
    for (j = y0 + 1; j <= yf - 1; j++)
      objectsMap[i][j] = n + 4;
  }

  for (j = y0 + 1; j <= yf - 1; j++)
  {
    objectsMap[x0][j] = n + 3;
    objectsMap[xf][j] = n + 5;
  }

  for (i = x0; i <= xf; i++)
    for (j = y0; j <= yf; j++)
      logicalMap[i][j] = LogicalObstacle;
}

Vector2D DungeonMap::generateMerchantRoom()
{
  initRoom();

  int x0 = MAP_WIDTH / 2;
  int y0 = MAP_HEIGHT / 2;

  for (int i = x0 - 3; i <= x0 + 3; i++)
  {
    if (i == x0 - 3) objectsMap[i][y0] = MAPOBJ_SHOP_LEFT;
    else if (i == x0 + 3) objectsMap[i][y0] = MAPOBJ_SHOP_RIGHT;
    else objectsMap[i][y0] = MAPOBJ_SHOP;

    logicalMap[i][y0] = LogicalObstacle;
  }

  if (!hasNeighbourUp())
  {
    objectsMap[x0 - 1][0] = MAPOBJ_PNW;
    objectsMap[x0][0] = MAPOBJ_PNW + 1;
    objectsMap[x0 + 1][0] = MAPOBJ_PNW + 2;
  }
  else
  {
    objectsMap[x0 - 1][MAP_HEIGHT - 1] = MAPOBJ_PNW +3;
    objectsMap[x0][MAP_HEIGHT - 1] = MAPOBJ_PNW + 4;
    objectsMap[x0 + 1][MAP_HEIGHT - 1] = MAPOBJ_PNW + 5;
  }

  generateRandomTile();

  return (Vector2D(x0 * TILE_WIDTH + TILE_WIDTH / 2, y0 * TILE_HEIGHT + TILE_HEIGHT / 2));
}

Vector2D DungeonMap::generateKeyRoom()
{
  initRoom();
  int x0 = MAP_WIDTH / 2;
  int y0 = MAP_HEIGHT / 2;

  objectsMap[x0 - 1][y0 - 1] = MAPOBJ_WALL_SPECIAL;
  objectsMap[x0 - 1][y0 + 1] = MAPOBJ_WALL_SPECIAL + 2;
  objectsMap[x0 + 1][y0 - 1] = MAPOBJ_WALL_SPECIAL + 1;
  objectsMap[x0 + 1][y0 + 1] = MAPOBJ_WALL_SPECIAL + 3;

  logicalMap[x0 - 1][y0 - 1] = LogicalObstacle;
  logicalMap[x0 - 1][y0 + 1] = LogicalObstacle;
  logicalMap[x0 + 1][y0 - 1] = LogicalObstacle;
  logicalMap[x0 + 1][y0 + 1] = LogicalObstacle;

  if (rand() % 3 == 0)
  {
    if (rand() % 2 == 0) initPattern(PatternSmallCircle);
    else initPattern(PatternSmallStar);
  }
  generateRandomTile();

  return (Vector2D(x0 * TILE_WIDTH + TILE_WIDTH / 2, y0 * TILE_HEIGHT + TILE_HEIGHT / 2));
}

void DungeonMap::generateExitRoom()
{
  initRoom();
  int x0 = MAP_WIDTH / 2;
  map[x0][0] = wallOffset + MAP_STAIRS_UP;
  map[x0 - 1][0] = wallOffset + MAP_WALL_EXIT_L;
  map[x0 + 1][0] = wallOffset + MAP_WALL_EXIT_R;

  logicalMap[x0][0] = LogicalFloor;

  if (rand() % 3 == 0) initPattern(PatternBorder);

  generateRandomTile();
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

  if (type <= 0) // empty room
  {
    if (roomType == roomTypeStarting)
    {
      generateCarpet(5, 3, 5, 3, MAP_CARPET);

      if (game().getLevel() > 1)
      {
        map[x0 - 1][MAP_HEIGHT - 1] = wallOffset + MAP_WALL_START_L;
        map[x0][MAP_HEIGHT - 1]     = wallOffset + MAP_WALL_START_M;
        map[x0 + 1][MAP_HEIGHT - 1] = wallOffset + MAP_WALL_START_R;
      }
    }
    else if (roomType == roomTypeBoss && (game().getLevel() == 2) ) // giant slime
    {
      // good ?
      map[1][1] = MAP_GRID + 1;
      map[1][MAP_HEIGHT -2] = MAP_GRID + 1;
      map[MAP_WIDTH - 2][1] = MAP_GRID + 1;
      map[MAP_WIDTH - 2][MAP_HEIGHT -2] = MAP_GRID + 1;
    }
    if (roomType == roomTypeStandard)
    {
      if (rand() % 3 > 0 && gameFloor->neighboorCount(x, y) > 1)
      {
        if (type == 0 && game().getLevel() < 6) generateCorridors();
      }
      else
      {
        if (rand() % 3 == 0) initPattern((patternEnum)(rand() % 4));
      }
    }
  }
  if (type == 1) // corner block
  {
    if (rand() % 3 == 0) initPattern(PatternSmallChecker);

    map[0][0] = wallOffset + MAP_WALL_X;
    map[0][1] = wallOffset + MAP_WALL_7;
    map[1][1] = wallOffset + MAP_WALL_77;
    map[1][0] = wallOffset + MAP_WALL_7;
    shadowMap[0][0] = MAPSHAD_EMPTY;
    shadowMap[0][1] = MAPSHAD_7;
    shadowMap[0][2] = MAPSHAD_47;
    shadowMap[1][0] = MAPSHAD_7;
    shadowMap[2][0] = MAPSHAD_87;
    shadowMap[1][1] = MAPSHAD_77;
    shadowMap[1][2] = MAPSHAD_GROUND_47;
    shadowMap[2][1] = MAPSHAD_GROUND_78;
    logicalMap[1][1] = LogicalWall;

    map[0][MAP_HEIGHT - 1] = wallOffset + MAP_WALL_X;
    map[0][MAP_HEIGHT - 2] = wallOffset + MAP_WALL_7;
    map[1][MAP_HEIGHT - 2] = wallOffset + MAP_WALL_77;
    map[1][MAP_HEIGHT - 1] = wallOffset + MAP_WALL_7;
    shadowMap[0][MAP_HEIGHT - 1] = MAPSHAD_EMPTY;
    shadowMap[0][MAP_HEIGHT - 2] = MAPSHAD_1;
    shadowMap[0][MAP_HEIGHT - 3] = MAPSHAD_41;
    shadowMap[1][MAP_HEIGHT - 1] = MAPSHAD_1;
    shadowMap[2][MAP_HEIGHT - 1] = MAPSHAD_21;
    shadowMap[1][MAP_HEIGHT - 2] = MAPSHAD_11;
    shadowMap[1][MAP_HEIGHT - 3] = MAPSHAD_GROUND_41;
    shadowMap[2][MAP_HEIGHT - 2] = MAPSHAD_GROUND_12;
    logicalMap[1][MAP_HEIGHT -2] = LogicalWall;

    map[MAP_WIDTH - 1][0] = wallOffset + MAP_WALL_X;
    map[MAP_WIDTH - 1][1] = wallOffset + MAP_WALL_7;
    map[MAP_WIDTH - 2][1] = wallOffset + MAP_WALL_77;
    map[MAP_WIDTH - 2][0] = wallOffset + MAP_WALL_7;
    shadowMap[MAP_WIDTH - 1][0] = MAPSHAD_EMPTY;
    shadowMap[MAP_WIDTH - 1][1] = MAPSHAD_9;
    shadowMap[MAP_WIDTH - 1][2] = MAPSHAD_69;
    shadowMap[MAP_WIDTH - 2][0] = MAPSHAD_9;
    shadowMap[MAP_WIDTH - 3][0] = MAPSHAD_89;
    shadowMap[MAP_WIDTH - 2][1] = MAPSHAD_99;
    shadowMap[MAP_WIDTH - 2][2] = MAPSHAD_GROUND_69;
    shadowMap[MAP_WIDTH - 3][1] = MAPSHAD_GROUND_98;
    logicalMap[MAP_WIDTH - 2][1] = LogicalWall;

    map[MAP_WIDTH - 1][MAP_HEIGHT -1] = wallOffset + MAP_WALL_X;
    map[MAP_WIDTH - 1][MAP_HEIGHT -2] = wallOffset + MAP_WALL_7;
    map[MAP_WIDTH - 2][MAP_HEIGHT -2] = wallOffset + MAP_WALL_77;
    map[MAP_WIDTH - 2][MAP_HEIGHT -1] = wallOffset + MAP_WALL_7;
    shadowMap[MAP_WIDTH - 1][MAP_HEIGHT - 1] = MAPSHAD_EMPTY;
    shadowMap[MAP_WIDTH - 1][MAP_HEIGHT - 2] = MAPSHAD_3;
    shadowMap[MAP_WIDTH - 1][MAP_HEIGHT - 3] = MAPSHAD_63;
    shadowMap[MAP_WIDTH - 2][MAP_HEIGHT - 1] = MAPSHAD_3;
    shadowMap[MAP_WIDTH - 3][MAP_HEIGHT - 1] = MAPSHAD_23;
    shadowMap[MAP_WIDTH - 2][MAP_HEIGHT - 2] = MAPSHAD_33;
    shadowMap[MAP_WIDTH - 2][MAP_HEIGHT - 3] = MAPSHAD_GROUND_63;
    shadowMap[MAP_WIDTH - 3][MAP_HEIGHT - 2] = MAPSHAD_GROUND_32;
    logicalMap[MAP_WIDTH - 2][MAP_HEIGHT -2] = LogicalWall;
  }
  if (type == 2) // blocks in the middle
  {
    if (rand() % 3 == 0) initPattern(PatternBorder);

    r = 1 + rand() % 3;

    generateTable(x0 - r, y0 - 1, 1 + 2 * r, 3, MAPOBJ_BIG_OBSTACLE);
  }
  if (type == 3)
  {
    // big blocks in the corners
    generateTable(2, 2, 2, 2, MAPOBJ_BIG_OBSTACLE);
    generateTable(2, MAP_HEIGHT - 4, 2, 2, MAPOBJ_BIG_OBSTACLE);
    generateTable(MAP_WIDTH - 4, MAP_HEIGHT - 4, 2, 2, MAPOBJ_BIG_OBSTACLE);
    generateTable(MAP_WIDTH - 4, 2, 2, 2, MAPOBJ_BIG_OBSTACLE);
  }
  if (type == 4)
  {
    // objects
    //if (rand() % 2 == 0)
    {
      // type 1
      if (rand() % 3 == 0) initPattern(PatternSmallChecker);
      bool leftOriented = rand() % 2 == 0;

      int bankType = rand() % 3;

      int x0 = leftOriented ? 5 : 3;

      if (leftOriented)
      {
        objectsMap[2][4] = MAPOBJ_OBSTACLE;
        logicalMap[2][4] = LogicalObstacle;
      }
      else
      {
        objectsMap[12][4] = MAPOBJ_OBSTACLE;
        logicalMap[12][4] = LogicalObstacle;
      }

      for (int i = 0; i < 3; i++)
      {
        int xPos = x0 + i * 3;
        switch (bankType)
        {
          case 0:
            objectsMap[xPos][2] = MAPOBJ_BANK_TOP;
            objectsMap[xPos][3] = MAPOBJ_BANK_BOTTOM;
            objectsMap[xPos][5] = MAPOBJ_BANK_TOP;
            objectsMap[xPos][6] = MAPOBJ_BANK_BOTTOM;
            for (int j = 2; j <= 6; j++) if (j != 4) logicalMap[xPos][j] = LogicalObstacle;
            break;

          case 1:
            objectsMap[xPos][2] = MAPOBJ_BANK_TOP;
            objectsMap[xPos][3] = MAPOBJ_BANK;
            objectsMap[xPos][4] = MAPOBJ_BANK;
            objectsMap[xPos][5] = MAPOBJ_BANK;
            objectsMap[xPos][6] = MAPOBJ_BANK_BOTTOM;
            for (int j = 2; j <= 6; j++) logicalMap[xPos][j] = LogicalObstacle;
            break;

          case 2:
            objectsMap[xPos][1] = MAPOBJ_BANK_TOP;
            objectsMap[xPos][2] = MAPOBJ_BANK;
            objectsMap[xPos][3] = MAPOBJ_BANK_BOTTOM;
            objectsMap[xPos][5] = MAPOBJ_BANK_TOP;
            objectsMap[xPos][6] = MAPOBJ_BANK;
            objectsMap[xPos][7] = MAPOBJ_BANK_BOTTOM;
            for (int j = 1; j <= 7; j++) if (j != 4) logicalMap[xPos][j] = LogicalObstacle;
            break;
        }
      }
    }
    /*else
    {
      map[2][2] = MAP_LONG_LEFT;
      map[3][2] = MAP_LONG;
      map[4][2] = MAP_LONG_RIGHT;

      map[2][6] = MAP_LONG_LEFT;
      map[3][6] = MAP_LONG;
      map[4][6] = MAP_LONG_RIGHT;

      map[10][2] = MAP_LONG_LEFT;
      map[11][2] = MAP_LONG;
      map[12][2] = MAP_LONG_RIGHT;

      map[10][6] = MAP_LONG_LEFT;
      map[11][6] = MAP_LONG;
      map[12][6] = MAP_LONG_RIGHT;
    }*/
  }
  if (type == ROOM_TYPE_CHECKER)
  {
    // "checker"
    if (rand() % 3 == 0) initPattern(PatternSmallChecker);

    for (i = 2; i < MAP_WIDTH - 2; i = i + 2)
      for (j = 2; j < MAP_HEIGHT - 2; j = j + 2)
      {
        objectsMap[i][j] = game().getLevel() >= 6 ? MAPOBJ_TOMB : MAPOBJ_OBSTACLE;
        logicalMap[i][j] = LogicalObstacle;
      }
  }

  generateRandomTile();
}

void DungeonMap::addHole(int x, int y)
{
  int n = MAPOBJ_HOLE;
  if (y > 0 && (objectsMap[x][y - 1] == MAPOBJ_HOLE || objectsMap[x][y - 1] == MAPOBJ_HOLE_BOTTOM))
    n = MAPOBJ_HOLE_BOTTOM;

  objectsMap[x][y] = n;
  logicalMap[x][y] = LogicalHole;
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
      generateCarpet(5, 3, 5, 3, MAP_CARPET);

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
  else if (type == 1)
  {
    // corner hole
    if (rand() % 3 == 0) initPattern(PatternSmallChecker);
    addHole(1, 1);
    addHole(MAP_WIDTH - 2, MAP_HEIGHT -2);
    addHole(1, MAP_HEIGHT -2);
    addHole(MAP_WIDTH - 2, 1);
  }
  else if (type == 2)
  {
    // blocks in the middle
    if (rand() % 3 == 0) initPattern(PatternBorder);

    r = 1 + rand() % 2;
    for (i = x0 - r; i <= x0 + r; i++)
      for (j = y0 - 1; j <= y0 + 1; j++)
      {
        addHole(i, j);
      }
  }
  else if (type == 3)
  {
    // 4 holes
    addHole(2, 2);
    addHole(2, 3);
    addHole(3, 2);
    addHole(3, 3);

    addHole(MAP_WIDTH - 4, 2);
    addHole(MAP_WIDTH - 4, 3);
    addHole(MAP_WIDTH - 3, 2);
    addHole(MAP_WIDTH - 3, 3);

    addHole(2, MAP_HEIGHT - 4);
    addHole(2, MAP_HEIGHT - 3);
    addHole(3, MAP_HEIGHT - 4);
    addHole(3, MAP_HEIGHT - 3);

    addHole(MAP_WIDTH - 4, MAP_HEIGHT - 4);
    addHole(MAP_WIDTH - 4, MAP_HEIGHT - 3);
    addHole(MAP_WIDTH - 3, MAP_HEIGHT - 4);
    addHole(MAP_WIDTH - 3, MAP_HEIGHT - 3);
  }
  else if (type == 4)
  {
    if (rand() % 3 == 0) initPattern(PatternBigCircle);
    int r = 6 + rand()% 5;
    int obstacleType = rand() % 2;
    for (int i = 0; i < r; i++)
    {
      int rx = 1 + rand() % (MAP_WIDTH - 3);
      int ry = 1 + rand() % (MAP_HEIGHT - 3);

      bool ok = true;
      bool isObstacle = (obstacleType == 1) && rand() % 2 == 0;

      if ( (rx == 1 && ry == MAP_HEIGHT / 2)
          || (rx == MAP_WIDTH - 2 && ry == MAP_HEIGHT / 2)
          || (rx == MAP_WIDTH /2 && ry == MAP_HEIGHT - 2)
          || (rx == MAP_WIDTH /2 && ry == 1) )
      {
        ok = false;
      }

      else
      {
        for (int ix = -1; ix <= 1; ix++)
          for (int iy = -1; iy <= 1; iy++)
        {
          ok = ok && logicalMap[rx + ix][ry + iy] != LogicalHole;
          ok = ok && logicalMap[rx + ix][ry + iy] != LogicalObstacle;
        }
      }

      if (ok)
      {
        if (!isObstacle)
        {
          addHole(rx, ry);
        }

        else
        {
          objectsMap[rx][ry] = MAPOBJ_OBSTACLE;
          logicalMap[rx][ry] = LogicalObstacle;
        }
      }
      else
      {
        i--;
      }
    }
  }
  else  //if (type == 5)
  {
    // "checker"
    for (i = 2; i < MAP_WIDTH - 2; i = i + 2)
      for (j = 2; j < MAP_HEIGHT - 2; j = j + 2)
      {
        addHole(i, j);
      }
  }

  generateRandomTile();
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
    if (logicalMap[rx][ry] == LogicalFloor)
    {
      map[rx][ry] = MAP_GRID + game().getLevel() - 1;
      counter--;
    }
  }
}

void DungeonMap::castShadows(int x0, int y0, int xf, int yf)
{
  // init shadows
  int i, j;

  for ( j = 0 ; j < height; j++)
    for ( i = 0 ; i < width; i++)
      shadowMap[i][j] = MAPSHAD_EMPTY;

    // shadows
  for (int i = x0; i <= xf; i++)
  {
    if (i == x0)
    {
      shadowMap[i][y0] = MAPSHAD_7;
      shadowMap[i][yf] = MAPSHAD_1;
    }
    else if (i == x0 + 1)
    {
      shadowMap[i][y0] = MAPSHAD_87;
      shadowMap[i][yf] = MAPSHAD_21;
      shadowMap[i][y0 + 1] = MAPSHAD_GROUND_7;
      shadowMap[i][yf - 1] = MAPSHAD_GROUND_1;
    }
    else if (i == xf)
    {
      shadowMap[i][y0] = MAPSHAD_9;
      shadowMap[i][yf] = MAPSHAD_3;
    }
    else if (i == xf - 1)
    {
      shadowMap[i][y0] = MAPSHAD_89;
      shadowMap[i][yf] = MAPSHAD_23;
      shadowMap[i][y0 + 1] = MAPSHAD_GROUND_9;
      shadowMap[i][yf - 1] = MAPSHAD_GROUND_3;
    }
    else
    {
      shadowMap[i][y0] = MAPSHAD_8;
      shadowMap[i][yf] = MAPSHAD_2;
      shadowMap[i][y0 + 1] = MAPSHAD_GROUND_8;
      shadowMap[i][yf - 1] = MAPSHAD_GROUND_2;
    }
  }

  for (int i = y0 + 1; i < yf; i++)
  {
    if (i == y0 + 1)
    {
      shadowMap[x0][i] = MAPSHAD_47;
      shadowMap[xf][i] = MAPSHAD_69;
    }
    else if (i == yf - 1)
    {
      shadowMap[x0][i] = MAPSHAD_41;
      shadowMap[xf][i] = MAPSHAD_63;
    }
    else
    {
      shadowMap[x0][i] = MAPSHAD_4;
      shadowMap[xf][i] = MAPSHAD_6;
      shadowMap[x0 + 1][i] = MAPSHAD_GROUND_4;
      shadowMap[xf - 1][i] = MAPSHAD_GROUND_6;
    }
  }
}

void DungeonMap::generateCorridors()
{
  int xCor = 1 + rand()% 4;
  int yCor = 1 + rand()% 1;

  int x0 = 0, xf = MAP_WIDTH - 1, y0 = 0, yf = MAP_HEIGHT - 1;

  if (!hasNeighbourLeft())
  {
    for (int i = 0; i < xCor; i++)
      for (int j = 0; j < MAP_HEIGHT; j++)
      {
        map[i][j] = wallOffset + MAP_WALL_X;
        logicalMap[i][j] = LogicalWall;
        x0 = xCor;
      }

  }
  if (!hasNeighbourRight())
  {
    for (int i = MAP_WIDTH - 1; i > MAP_WIDTH - 1 - xCor; i--)
      for (int j = 0; j < MAP_HEIGHT; j++)
      {
        map[i][j] = wallOffset + MAP_WALL_X;
        logicalMap[i][j] = LogicalWall;
        xf = MAP_WIDTH - 1 - xCor;
      }
  }
  if (!hasNeighbourUp())
  {
    for (int i = 0; i < MAP_WIDTH; i++)
      for (int j = 0; j < yCor; j++)
      {
        map[i][j] = wallOffset + MAP_WALL_X;
        logicalMap[i][j] = LogicalWall;
        y0 = yCor;
      }
  }
  if (!hasNeighbourDown())
  {
    for (int i = 0; i < MAP_WIDTH; i++)
      for (int j = MAP_HEIGHT - 1; j > MAP_HEIGHT - 1 - yCor; j--)
      {
        map[i][j] = wallOffset + MAP_WALL_X;
        logicalMap[i][j] = LogicalWall;
        yf = MAP_HEIGHT - 1 - yCor;
      }
  }

  //
  for (int i = 0; i < MAP_WIDTH; i++)
  {
    for (int j = 0; j < MAP_HEIGHT; j++)
    {
      if (map[i][j] != wallOffset + MAP_WALL_X)
      {
        if (getTile(i - 1, j) == wallOffset + MAP_WALL_X)
        {
          if (j == 0 || getTile(i, j - 1) == wallOffset + MAP_WALL_X)
          {
            map[i][j] = wallOffset + MAP_WALL_7;
          }
          else if (j == MAP_HEIGHT - 1 || getTile(i, j + 1) == wallOffset + MAP_WALL_X)
          {
            map[i][j] = wallOffset + MAP_WALL_7;
          }
          else
          {
            if (j < MAP_HEIGHT / 2) map[i][j] = wallOffset + MAP_WALL_87;
            else if (j > MAP_HEIGHT / 2) map[i][j] = wallOffset + MAP_WALL_87;
            else map[i][j] = wallOffset + MAP_WALL_8;
            logicalMap[i][j] = LogicalWall;
          }

        }
        else if (getTile(i + 1, j) == wallOffset + MAP_WALL_X)
        {
          if (j == 0 || getTile(i, j - 1) == wallOffset + MAP_WALL_X)
          {
            map[i][j] = wallOffset + MAP_WALL_7;
            map[i][j] = wallOffset + MAP_WALL_7;
          }
          else if (j == MAP_HEIGHT - 1 || getTile(i, j + 1) == wallOffset + MAP_WALL_X) map[i][j] = wallOffset + MAP_WALL_7;
          else
          {
            if (j < MAP_HEIGHT / 2) map[i][j] = wallOffset + MAP_WALL_87;
            else if (j > MAP_HEIGHT / 2) map[i][j] = wallOffset + MAP_WALL_87;
            else map[i][j] = wallOffset + MAP_WALL_8;
            logicalMap[i][j] = LogicalWall;
          }
        }
        else if (getTile(i, j - 1) == wallOffset + MAP_WALL_X)
        {
          if (i == 0) map[i][j] = wallOffset + MAP_WALL_7;
          else if (i == MAP_WIDTH - 1) map[i][j] = wallOffset + MAP_WALL_7;
          else
          {
            if (i < MAP_WIDTH / 2) map[i][j] = wallOffset + MAP_WALL_87;
            else if (i > MAP_WIDTH / 2) map[i][j] = wallOffset + MAP_WALL_87;
            else map[i][j] = wallOffset + MAP_WALL_8;
            logicalMap[i][j] = LogicalWall;
          }
        }
        else if (getTile(i, j + 1) == wallOffset + MAP_WALL_X)
        {
          if (i == 0) map[i][j] = wallOffset + MAP_WALL_7;
          else if (i == MAP_WIDTH - 1) map[i][j] = wallOffset + MAP_WALL_7;
          else
          {
            if (i < MAP_WIDTH / 2) map[i][j] = wallOffset + MAP_WALL_87;
            else if (i > MAP_WIDTH / 2) map[i][j] = wallOffset + MAP_WALL_87;
            else map[i][j] = wallOffset + MAP_WALL_8;
            logicalMap[i][j] = LogicalWall;
          }
        }
      }
    }
  }

  castShadows(x0, y0, xf, yf);
}

void DungeonMap::generateRandomTile()
{
  bool ok = false;
  for (int i = 0; !ok && i < NB_RANDOM_TILE_TRY; i++)
  {
    int n = rand() % NB_RANDOM_TILES;

    int xTile, yTile;

    if (randomDungeonTiles[n].totalRandom)
    {
      xTile = rand() % (GAME_WIDTH - randomDungeonTiles[n].width);
      yTile = rand() % (GAME_HEIGHT - randomDungeonTiles[n].height);
    }
    else
    {
      xTile = TILE_WIDTH + rand() % (GAME_WIDTH - 2 * TILE_WIDTH - randomDungeonTiles[n].width);
      yTile = TILE_HEIGHT + rand() % (GAME_HEIGHT - 2 * TILE_HEIGHT - randomDungeonTiles[n].height);
    }

    int x0 = xTile / TILE_WIDTH;
    int xf = (xTile + randomDungeonTiles[n].width) / TILE_WIDTH;
    int y0 = yTile / TILE_HEIGHT;
    int yf = (yTile + randomDungeonTiles[n].height) / TILE_HEIGHT;

    ok = true;

    if (!randomDungeonTiles[n].totalRandom)
    {
      for (int ix = x0; ix <= xf; ix++)
        for (int iy = y0; iy <= yf; iy++)
        {
          ok = ok && isWalkable(ix, iy);
        }
    }

    if (ok)
    {
      randomTileElement.type = n;
      randomTileElement.x = xTile;
      randomTileElement.y = yTile;
      if (randomDungeonTiles[n].totalRandom)
        randomTileElement.rotation = rand()% 360;
      else
        randomTileElement.rotation = 0;
    }
  }
}

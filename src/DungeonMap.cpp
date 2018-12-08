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
  revealed = true;
  visited = false;
  known = false;
  for (int i = 0; i < NB_RANDOM_TILES_IN_ROOM; i++)
  {
    randomTileElements[i].type = -1;
    randomTileElements[i].x = 0;
    randomTileElements[i].y = 0;
    randomTileElements[i].rotation = 0;
  }
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

bool DungeonMap::isRevealed()
{
  return revealed;
}

void DungeonMap::setRevealed(bool b)
{
  revealed = b;
  if (revealed) known = true;
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
  if (roomType == roomTypeSecret)
    revealed = false;
}

int DungeonMap::getObjectTile(int x, int y)
{
  return objectsMap[x][y];
}

logicalMapStateEnum DungeonMap::getLogicalTile(int x, int y)
{
  return logicalMap[x][y];
}

void DungeonMap::setObjectTile(int x, int y, int n)
{
  objectsMap[x][y] = n;
  hasChanged = true;
}

void DungeonMap::setLogicalTile(int x, int y, logicalMapStateEnum state)
{
  logicalMap[x][y] = state;
  hasChanged = true;
}

int DungeonMap::getFloorOffset()
{
  return floorOffset;
}
int DungeonMap::getWallType()
{
  return wallType;
}
void DungeonMap::setFloorOffset(int n)
{
  floorOffset = n;
}
void DungeonMap::setWallType(int n)
{
  wallType = n;
}

doorEnum DungeonMap::getDoorType(int direction)
{
  return doorType[direction];
}

void DungeonMap::setDoorType(int direction, doorEnum type)
{
  doorType[direction] = type;
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

DungeonMap::RandomTileElement DungeonMap::getRandomTileElement(int n)
{
  if (n < 0 || n >= NB_RANDOM_TILES_IN_ROOM) return RandomTileElement { -1, 0, 0, 0};
  return randomTileElements[n];
}

void DungeonMap::setRandomTileElement (int n, RandomTileElement rt)
{
  if (n < 0 || n >= NB_RANDOM_TILES_IN_ROOM) return;
  randomTileElements[n] = rt;
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

bool DungeonMap::hasKnownNeighbour(int direction, bool mustBeRevealed)
{
  switch (direction)
  {
  case North:
    if (y > 0 && gameFloor->getRoom(x, y - 1) > 0)
    {
      if (mustBeRevealed)
        return (gameFloor->getRoom(x, y - 1) != roomTypeSecret || gameFloor->getMap(x, y - 1)->isRevealed());
      else
        return (gameFloor->getRoom(x, y - 1) != roomTypeSecret || gameFloor->getMap(x, y - 1)->isKnown());
    }
    break;

  case South:
    if (y < MAP_HEIGHT -1 && gameFloor->getRoom(x, y + 1) > 0)
    {
      if (mustBeRevealed)
        return (gameFloor->getRoom(x, y + 1) != roomTypeSecret || gameFloor->getMap(x, y + 1)->isRevealed());
      else
        return (gameFloor->getRoom(x, y + 1) != roomTypeSecret || gameFloor->getMap(x, y + 1)->isKnown());
    }
    break;

  case West:
    if (x > 0 && gameFloor->getRoom(x - 1, y) > 0)
    {
      if (mustBeRevealed)
        return (gameFloor->getRoom(x - 1, y) != roomTypeSecret || gameFloor->getMap(x - 1, y)->isRevealed());
      else
        return (gameFloor->getRoom(x - 1, y) != roomTypeSecret || gameFloor->getMap(x - 1, y)->isKnown());
    }
    break;

  case East:
    if (x < MAP_WIDTH -1 && gameFloor->getRoom(x+1, y) > 0)
    {
      if (mustBeRevealed)
        return (gameFloor->getRoom(x + 1, y) != roomTypeSecret || gameFloor->getMap(x + 1, y)->isRevealed());
      else
        return (gameFloor->getRoom(x + 1, y) != roomTypeSecret || gameFloor->getMap(x + 1, y)->isKnown());
    }
    break;
  }
  return false;
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

doorEnum getRandomDoor()
{
  return (doorEnum)(DoorStandard_0 + ((game().getLevel() - 1) % 8));

  /////
  int n = rand() % 12;

  switch (game().getLevel())
  {
  case 1:
    if (n < 6) return DoorStandard_0;
    else return DoorStandard_1;

  case 2:
    if (n < 5) return DoorStandard_0;
    else if (n < 10) return DoorStandard_1;
    else return DoorStandard_2;

  case 3:
    if (n < 6) return DoorStandard_1;
    else return DoorStandard_2;

  case 4:
    if (n < 5) return DoorStandard_1;
    else if (n < 10) return DoorStandard_2;
    else return DoorStandard_3;

  case 5:
    if (n < 6) return DoorStandard_2;
    else return DoorStandard_3;

  case 6:
    if (n < 5) return DoorStandard_2;
    else if (n < 10) return DoorStandard_3;
    else return DoorStandard_4;

  default:
    if (n < 6) return DoorStandard_3;
    else return DoorStandard_4;
  }

  return (doorEnum)(rand() % 5);
}

void DungeonMap::initRoom(int floorN, int wallN)
{
  int x0 = MAP_WIDTH / 2;
  int y0 = MAP_HEIGHT / 2;
  int i, j;

  // style
  if (roomType == roomTypeSecret)
    floorOffset = 8 * 24;
  else if (floorN < 0)
    floorOffset = ((game().getLevel() - 1) % 8) * 24 ;
  else
    floorOffset = floorN;

  if (wallN < 0) wallType = ((game().getLevel() - 1) % 8) ;
  else wallType = wallN;

  int wallOffset = wallType * 24;

  // doors
  for (i = 0; i < 4; i++) doorType[i] = DoorNone;

  // outer walls
  map[0][0] = wallOffset + MAP_WALL_7 + rand() % 2;
  for ( i = 1 ; i < width -1 ; i++)
  {
    if (i == width / 2)
    {
      map[i][0] = wallOffset + MAP_WALL_8 + rand() % 2;
      map[i][height - 1] = wallOffset + MAP_WALL_8 + rand() % 2;
    }
    else if (i < width / 2)
    {
      map[i][0] = wallOffset + MAP_WALL_87 + rand() % 8;
      map[i][height - 1] = wallOffset + MAP_WALL_87 + rand() % 8;
    }
    else
    {
      map[i][0] = wallOffset + MAP_WALL_87 + rand() % 8;
      map[i][height - 1] = wallOffset + MAP_WALL_87 + rand() % 8;
    }
  }
  map[width - 1][0] = wallOffset + MAP_WALL_7 + rand() % 2;
  for ( int i = 1 ; i < height -1 ; i++)
  {
    if (i == height / 2)
    {
      map[0][i] = wallOffset + MAP_WALL_8 + rand() % 2;
      map[width - 1][i] = wallOffset + MAP_WALL_8 + rand() % 2;
    }
    else if (i < height / 2)
    {
      map[0][i] = wallOffset + MAP_WALL_87 + rand() % 8;
      map[width - 1][i] = wallOffset + MAP_WALL_87 + rand() % 8;
    }
    else
    {
      map[0][i] = wallOffset + MAP_WALL_87 + rand() % 8;
      map[width - 1][i] = wallOffset + MAP_WALL_87 + rand() % 8;
    }
  }
  map[0][height - 1] = wallOffset + MAP_WALL_7 + rand() % 2;
  map[width - 1][height - 1] = wallOffset + MAP_WALL_7 + rand() % 2;

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
  for (i = 0; i < 8; i++)
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
      map[xTile][yTile] = i + wallOffset + MAP_WALL_87 + 8;
    }
  }

  // doors ?
  if (gameFloor != NULL)
  {
    if (x > 0 && gameFloor->getRoom(x - 1, y) > 0)
    {
      if (gameFloor->getRoom(x - 1, y) != roomTypeSecret)
      {
        map[0][MAP_HEIGHT / 2] = floorOffset;
        map[0][MAP_HEIGHT / 2 - 1] = floorOffset;
        map[0][MAP_HEIGHT / 2 + 1] = floorOffset;
        openDoor(0, y0);

        if (roomType == roomTypeBoss || gameFloor->getRoom(x - 1, y) == roomTypeBoss)
        {
          doorType[West] = DoorBoss;
        }
        else if (roomType == roomTypeChallenge || gameFloor->getRoom(x - 1, y) == roomTypeChallenge)
        {
          doorType[West] = DoorChallenge;
        }
        else if (gameFloor->getMap(x - 1, y)->isVisited())
        {
          doorType[West] = gameFloor->getMap(x - 1, y)->getDoorType(East);
        }
        else
        {
          // secret door
          doorType[West] = getRandomDoor();
        }
      }
      else
      {
        doorType[West] = getRandomDoor();
      }
    }

    if (x < MAP_WIDTH - 1 && gameFloor->getRoom(x + 1, y) > 0)
    {
      if (gameFloor->getRoom(x + 1, y) != roomTypeSecret)
      {
        map[MAP_WIDTH - 1][MAP_HEIGHT / 2] = floorOffset;
        map[MAP_WIDTH - 1][MAP_HEIGHT / 2 - 1] = floorOffset;
        map[MAP_WIDTH - 1][MAP_HEIGHT / 2 + 1] = floorOffset;
        openDoor(MAP_WIDTH - 1, y0);

        if (roomType == roomTypeBoss || gameFloor->getRoom(x + 1, y) == roomTypeBoss)
        {
          doorType[East] = DoorBoss;
        }
        else if (roomType == roomTypeChallenge || gameFloor->getRoom(x + 1, y) == roomTypeChallenge)
        {
          doorType[East] = DoorChallenge;
        }
        else if (gameFloor->getMap(x + 1, y)->isVisited())
        {
          doorType[East] = gameFloor->getMap(x + 1, y)->getDoorType(West);
        }
        else
        {
          doorType[East] = getRandomDoor();
        }
      }
      else
      {
        // secret door
        doorType[East] = getRandomDoor();
      }
    }

    if (y > 0 && gameFloor->getRoom(x, y - 1) > 0)
    {
      if (gameFloor->getRoom(x, y - 1) != roomTypeSecret)
      {
        map[MAP_WIDTH / 2][0] = floorOffset;
        map[MAP_WIDTH / 2 - 1][0] = floorOffset;
        map[MAP_WIDTH / 2 + 1][0] = floorOffset;
        openDoor(x0, 0);

        if (roomType == roomTypeBoss || gameFloor->getRoom(x, y - 1) == roomTypeBoss)
        {
          doorType[North] = DoorBoss;
        }
        else if (roomType == roomTypeChallenge || gameFloor->getRoom(x, y - 1) == roomTypeChallenge)
        {
          doorType[North] = DoorChallenge;
        }
        else if (gameFloor->getMap(x, y - 1)->isVisited())
        {
          doorType[North] = gameFloor->getMap(x, y - 1)->getDoorType(South);
        }
        else
        {
          doorType[North] = getRandomDoor();
        }
      }
      else
      {
        // secret door
        doorType[North] = getRandomDoor();
      }
    }

    if (y < MAP_HEIGHT -1 && gameFloor->getRoom(x, y + 1) > 0)
    {
      if (gameFloor->getRoom(x, y + 1) != roomTypeSecret)
      {
        map[MAP_WIDTH / 2][MAP_HEIGHT - 1] = floorOffset;
        map[MAP_WIDTH / 2 - 1][MAP_HEIGHT - 1] = floorOffset;
        map[MAP_WIDTH / 2 + 1][MAP_HEIGHT - 1] = floorOffset;
        openDoor(x0, MAP_HEIGHT -1);

        if (roomType == roomTypeBoss || gameFloor->getRoom(x, y + 1) == roomTypeBoss)
        {
          doorType[South] = DoorBoss;
        }
        else if (roomType == roomTypeChallenge || gameFloor->getRoom(x, y + 1) == roomTypeChallenge)
        {
          doorType[South] = DoorChallenge;
        }
        else if (gameFloor->getMap(x, y + 1)->isVisited())
        {
          doorType[South] = gameFloor->getMap(x, y + 1)->getDoorType(North);
        }
        else
        {
          doorType[South] = getRandomDoor();
        }
      }
      else
      {
        // secret door
        doorType[South] = getRandomDoor();
      }
    }

    if (game().getLevel() > 1 && roomType == roomTypeStarting)
    {
      doorType[South] = getRandomDoor();
    }
    else if (roomType == roomTypeExit && game().getLevel() < LAST_LEVEL)
    {
      doorType[North] = DoorExit;
    }
  }
}

void DungeonMap::addDestroyableObject(int x, int y, int obj)
{
  // out of map
  if (x < 0 || x >= MAP_WIDTH) return;
  if (y < 0 || y >= MAP_HEIGHT) return;

  // doors
  if (x == MAP_WIDTH / 2 && (y == 1 || y == MAP_HEIGHT - 2)) return;
  if (y == MAP_HEIGHT / 2 && (x == 1 || x == MAP_WIDTH - 2)) return;

  // wall
  if (logicalMap[x][y] != LogicalFloor) return;

  objectsMap[x][y] = obj;
  logicalMap[x][y] = LogicalDestroyable;
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
  if (map[x][y] < 24 * MAP_NB_FLOORS && (map[x][y] % 24) < 8) map[x][y] += 8;
  else map[x][y] = floorOffset + 8 + rand() % 8;
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

///// SUB-ROOM GENERATION
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

void DungeonMap::generateLongObject(int x0, int y0, int w, int n)
{
  int xf = x0 + w - 1;
  objectsMap[x0][y0] = n;
  logicalMap[x0][y0] = LogicalObstacle;
  for (int i = x0 + 1; i <= xf - 1; i++)
  {
    objectsMap[i][y0] = n + 1;
    logicalMap[i][y0] = LogicalObstacle;
  }
  objectsMap[xf][y0] = n + 2;
  logicalMap[xf][y0] = LogicalObstacle;
}

void DungeonMap::generateInselRoom()
{
  for (int i = 1; i < MAP_WIDTH - 1; i++)
    for (int j = 1; j < MAP_HEIGHT - 1; j++)
    {
      if (i != MAP_WIDTH / 2 && j != MAP_HEIGHT / 2
            && !(i == MAP_WIDTH / 2 - 1 && j == MAP_HEIGHT / 2 - 1)
            && !(i == MAP_WIDTH / 2 - 1 && j == MAP_HEIGHT / 2 + 1)
            && !(i == MAP_WIDTH / 2 + 1 && j == MAP_HEIGHT / 2 - 1)
            && !(i == MAP_WIDTH / 2 + 1 && j == MAP_HEIGHT / 2 + 1)
            )
        addHole(i, j);
    }
  objectsMap[MAP_WIDTH / 2 - 2][MAP_HEIGHT / 2 - 1] = MAPOBJ_WALL_SPECIAL;
  objectsMap[MAP_WIDTH / 2 - 1][MAP_HEIGHT / 2 - 2] = MAPOBJ_WALL_SPECIAL;
  logicalMap[MAP_WIDTH / 2 - 2][MAP_HEIGHT / 2 - 1] = LogicalObstacle;
  logicalMap[MAP_WIDTH / 2 - 1][MAP_HEIGHT / 2 - 2] = LogicalObstacle;

  objectsMap[MAP_WIDTH / 2 + 2][MAP_HEIGHT / 2 - 1] = MAPOBJ_WALL_SPECIAL + 1;
  objectsMap[MAP_WIDTH / 2 + 1][MAP_HEIGHT / 2 - 2] = MAPOBJ_WALL_SPECIAL + 1;
  logicalMap[MAP_WIDTH / 2 + 2][MAP_HEIGHT / 2 - 1] = LogicalObstacle;
  logicalMap[MAP_WIDTH / 2 + 1][MAP_HEIGHT / 2 - 2] = LogicalObstacle;

  objectsMap[MAP_WIDTH / 2 - 2][MAP_HEIGHT / 2 + 1] = MAPOBJ_WALL_SPECIAL + 2;
  objectsMap[MAP_WIDTH / 2 - 1][MAP_HEIGHT / 2 + 2] = MAPOBJ_WALL_SPECIAL + 2;
  logicalMap[MAP_WIDTH / 2 - 2][MAP_HEIGHT / 2 + 1] = LogicalObstacle;
  logicalMap[MAP_WIDTH / 2 - 1][MAP_HEIGHT / 2 + 2] = LogicalObstacle;

  objectsMap[MAP_WIDTH / 2 + 2][MAP_HEIGHT / 2 + 1] = MAPOBJ_WALL_SPECIAL + 3;
  objectsMap[MAP_WIDTH / 2 + 1][MAP_HEIGHT / 2 + 2] = MAPOBJ_WALL_SPECIAL + 3;
  logicalMap[MAP_WIDTH / 2 + 2][MAP_HEIGHT / 2 + 1] = LogicalObstacle;
  logicalMap[MAP_WIDTH / 2 + 1][MAP_HEIGHT / 2 + 2] = LogicalObstacle;

  if (!hasNeighbourUp())
    addHole(MAP_WIDTH / 2, 1);
  if (!hasNeighbourDown())
    addHole(MAP_WIDTH / 2, MAP_HEIGHT - 2);
  if (!hasNeighbourLeft())
    for (int i = 0; i < 4; i++)
    {
      addHole(1 + i, MAP_HEIGHT / 2);
      addHole(1 + i, MAP_HEIGHT / 2 + 1);
    }
  if (!hasNeighbourRight())
    for (int i = 0; i < 4; i++)
    {
      addHole(MAP_WIDTH - 2 - i, MAP_HEIGHT / 2);
      addHole(MAP_WIDTH - 2 - i, MAP_HEIGHT / 2 + 1);
    }

  addHole(MAP_WIDTH / 2 - 1 , MAP_HEIGHT - 2);
  addHole(MAP_WIDTH / 2 + 1 , MAP_HEIGHT - 2);
  addHole(MAP_WIDTH / 2 - 2 , MAP_HEIGHT - 3);
  addHole(MAP_WIDTH / 2 + 2 , MAP_HEIGHT - 3);

  if (rand() % 2 == 0) makePatternTile(MAP_WIDTH / 2, MAP_HEIGHT / 2);
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

///// ROOMS GENERATION
Vector2D DungeonMap::generateBonusRoom()
{
  int x0 = MAP_WIDTH / 2;
  int y0 = MAP_HEIGHT / 2;

  initRoom();

  if (game().getLevel() == 1 || rand() % 3 > 0)
  {
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

    if (rand() % 2 == 0)
    {
      if (rand() % 3 > 0) addDestroyableObject(1, MAP_HEIGHT - 2, MAPOBJ_BARREL);
      if (rand() % 3 > 0) addDestroyableObject(1, 1, MAPOBJ_BARREL);
      if (rand() % 3 > 0) addDestroyableObject(MAP_WIDTH - 2, MAP_HEIGHT - 2, MAPOBJ_BARREL);
      if (rand() % 3 > 0) addDestroyableObject(MAP_WIDTH - 2, 1, MAPOBJ_BARREL);
    }
  }
  else
  {
    generateInselRoom();
  }

  generateRandomTiles();

  return (Vector2D(x0 * TILE_WIDTH + TILE_WIDTH / 2, y0 * TILE_HEIGHT + TILE_HEIGHT / 2));
}

Vector2D DungeonMap::generateChestRoom()
{
  return generateBonusRoom();
}

void DungeonMap::generateTempleRoom()
{
  initRoom();
  int x0 = MAP_WIDTH / 2;
  int y0 = 1 + MAP_HEIGHT / 2;

  int playerDivinity = (game().getPlayer()->getDivinity().divinity > -1);
  bool threeTemple = rand() % 2 == 0;
  bool forcePlayerDivinity = playerDivinity >= 0 && rand() % 2 == 0;

  if (rand() % 3 == 0)
  {
    if (rand() % 2 == 0) initPattern(PatternSmallDisc);
    else initPattern(PatternSmallStar);
  }

  int d0, d1, d2;

  if (threeTemple)
  {
    bool ok = false;
    while (!ok)
    {
      d0 = rand() % NB_DIVINITY;
      d1 = d0;
      while (d0 == d1) d1 = rand() % NB_DIVINITY;
      d2 = d0;
      while (d0 == d2 || d1 == d2) d2 = rand() % NB_DIVINITY;

      ok = true;
      if (forcePlayerDivinity)
        if (d0 != playerDivinity && d1 != playerDivinity && d2 != playerDivinity)
          ok = false;
    }
    generateTemple(x0 - 2, y0, (enumDivinityType)d0);
    generateTemple(x0 + 2, y0, (enumDivinityType)d1);
    generateTemple(x0, y0, (enumDivinityType)d2);
  }
  else
  {
    bool ok = false;
    while (!ok)
    {
      d0 = rand() % NB_DIVINITY;
      d1 = d0;
      while (d0 == d1) d1 = rand() % NB_DIVINITY;

      ok = true;
      if (forcePlayerDivinity)
        if (d0 != playerDivinity && d1 != playerDivinity)
          ok = false;
    }
    generateTemple(x0 - 1, y0, (enumDivinityType)d0);
    generateTemple(x0 + 1, y0, (enumDivinityType)d1);
  }

  if (rand() % 2 == 0)
  {
    addDestroyableObject(1, MAP_HEIGHT - 2, MAPOBJ_BARREL);
    addDestroyableObject(2, MAP_HEIGHT - 2, MAPOBJ_BARREL);
  }
  else
  {
    addDestroyableObject(MAP_WIDTH - 3, MAP_HEIGHT - 2, MAPOBJ_BARREL);
    addDestroyableObject(MAP_WIDTH - 2, MAP_HEIGHT - 2, MAPOBJ_BARREL);
  }
}

void DungeonMap::generateSecretRoom()
{
  initRoom();

  if (rand() % 2 == 0)
  {
    initPattern(PatternSmallChecker);
  }

  addDestroyableObject(MAP_WIDTH / 2 - 2, MAP_HEIGHT / 2 - 1, MAPOBJ_SKULL);
  addDestroyableObject(MAP_WIDTH / 2 + 2, MAP_HEIGHT / 2 - 1, MAPOBJ_SKULL);
  addDestroyableObject(MAP_WIDTH / 2 - 2, MAP_HEIGHT / 2 + 1, MAPOBJ_SKULL);
  addDestroyableObject(MAP_WIDTH / 2 + 2, MAP_HEIGHT / 2 + 1, MAPOBJ_SKULL);

  generateRandomTiles();
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

  generateRandomTiles();

  if (rand() % 2 == 0)
  {
    if (rand() % 3 == 0) addDestroyableObject(1, MAP_HEIGHT - 2, MAPOBJ_BARREL);
    if (rand() % 3 == 0) addDestroyableObject(1, 1, MAPOBJ_BARREL);
    if (rand() % 3 == 0) addDestroyableObject(MAP_WIDTH - 2, MAP_HEIGHT - 2, MAPOBJ_BARREL);
    if (rand() % 3 == 0) addDestroyableObject(MAP_WIDTH - 2, 1, MAPOBJ_BARREL);
  }

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
  generateCarpet(x0 - 1, y0 - 1, 3, 3, MAP_CARPET);
  generateRandomTiles();

  return (Vector2D(x0 * TILE_WIDTH + TILE_WIDTH / 2, y0 * TILE_HEIGHT + TILE_HEIGHT / 2));
}

void DungeonMap::generateExitRoom()
{
  initRoom();
  int x0 = MAP_WIDTH / 2;
  if (game().getLevel() < LAST_LEVEL)
  {
    map[x0][0] = floorOffset;
    map[x0 - 1][0] = floorOffset;
    map[x0 + 1][0] = floorOffset;
    logicalMap[x0][0] = LogicalFloor;
  }

  if (rand() % 3 == 0) initPattern(PatternBorder);

  generateRandomTiles();

  if (rand() % 2 == 0)
  {
    if (rand() % 3 == 0) addDestroyableObject(1, MAP_HEIGHT - 2, MAPOBJ_BARREL);
    if (rand() % 3 == 0) addDestroyableObject(1, 1, MAPOBJ_BARREL);
    if (rand() % 3 == 0) addDestroyableObject(MAP_WIDTH - 2, MAP_HEIGHT - 2, MAPOBJ_BARREL);
    if (rand() % 3 == 0) addDestroyableObject(MAP_WIDTH - 2, 1, MAPOBJ_BARREL);
  }
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
        map[x0][MAP_HEIGHT - 1]     = floorOffset;
      }
    }
    else if (roomType == roomTypeBoss)
    {
      if (game().getLevel() == 1) // butcher
      {
        addDestroyableObject(1, 1, MAPOBJ_BARREL);
        addDestroyableObject(1, 2, MAPOBJ_BARREL);
        addDestroyableObject(2, 1, MAPOBJ_BARREL);
        addDestroyableObject(2, 2, MAPOBJ_BARREL);
      }
      else if (game().getLevel() == 2) // giant slime
      {
        objectsMap[1][1] = MAPOBJ_GRID;
        objectsMap[1][MAP_HEIGHT -2] = MAPOBJ_GRID;
        objectsMap[MAP_WIDTH - 2][1] = MAPOBJ_GRID;
        objectsMap[MAP_WIDTH - 2][MAP_HEIGHT -2] = MAPOBJ_GRID;
      }
    }
    if (roomType == roomTypeStandard)
    {
      bool corr = false;
      if (rand() % 3 > 0 && gameFloor->neighboorCount(x, y) > 1)
      {
        if (type == 0 && game().getLevel() < 5)
        {
          generateCorridors();
          corr = true;
          if (rand() % 2 == 0)
          {
            for (int i = 0; i < 5; i++)
            {
              int objX = 2 + rand() % (MAP_WIDTH - 4);
              int objY = 2 + rand() % (MAP_HEIGHT - 4);

              int type = (rand() % 4) % 3 + MAPOBJ_BARREL_NO_DROP;
              if (rand() % 5 == 0) type = MAPOBJ_BARREL;

              addDestroyableObject(objX, objY, type);
            }
          }
        }
      }
      else
      {
        if (rand() % 3 == 0) initPattern((patternEnum)(rand() % 4));
      }

      if (!corr)  // some barrels
      {
        if (rand() % 2 == 0)
        {
          for (int i = 0; i < 10; i++)
          {
            int objX = 2 + rand() % (MAP_WIDTH - 4);
            int objY = 2 + rand() % (MAP_HEIGHT - 4);

            int type = MAPOBJ_BARREL_NO_DROP;
            if (rand() % 2 == 0) type += (rand() % 2 == 0) ? 1 : 2;
            if (rand() % 5 == 0) type = MAPOBJ_BARREL;

            addDestroyableObject(objX, objY, type);
          }
        }
        else
        {
          for (int i = 2; i < MAP_WIDTH - 3; i++)
          {
            addDestroyableObject(i, 2, (rand() % 4) % 3 + (rand() % 5 == 0) ? MAPOBJ_BARREL_EXPL : MAPOBJ_BARREL_NO_DROP);
            addDestroyableObject(i, MAP_HEIGHT - 3, (rand() % 4) % 3 + (rand() % 5 == 0) ? MAPOBJ_BARREL_EXPL : MAPOBJ_BARREL_NO_DROP);
          }
          for (int i = 3; i < MAP_HEIGHT - 4; i++)
          {
            addDestroyableObject(2, i, (rand() % 4) % 3 + (rand() % 5 == 0) ? MAPOBJ_BARREL_EXPL : MAPOBJ_BARREL_NO_DROP);
            addDestroyableObject(MAP_WIDTH - 3, i, (rand() % 4) % 3 + (rand() % 5 == 0) ? MAPOBJ_BARREL_EXPL : MAPOBJ_BARREL_NO_DROP);
          }
        }
      }
    }
  }
  if (type == 1) // corner block
  {
    if (rand() % 3 == 0) initPattern(PatternSmallChecker);
    int wallOffset = wallType * 24;

    map[0][0] = MAP_WALL_X;
    map[0][1] = wallOffset + MAP_WALL_7;
    map[1][1] = wallOffset + MAP_WALL_77;
    map[1][0] = wallOffset + MAP_WALL_7;
    logicalMap[1][1] = LogicalWall;

    map[0][MAP_HEIGHT - 1] = MAP_WALL_X;
    map[0][MAP_HEIGHT - 2] = wallOffset + MAP_WALL_7;
    map[1][MAP_HEIGHT - 2] = wallOffset + MAP_WALL_77;
    map[1][MAP_HEIGHT - 1] = wallOffset + MAP_WALL_7;
    logicalMap[1][MAP_HEIGHT -2] = LogicalWall;

    map[MAP_WIDTH - 1][0] = MAP_WALL_X;
    map[MAP_WIDTH - 1][1] = wallOffset + MAP_WALL_7;
    map[MAP_WIDTH - 2][1] = wallOffset + MAP_WALL_77;
    map[MAP_WIDTH - 2][0] = wallOffset + MAP_WALL_7;
    logicalMap[MAP_WIDTH - 2][1] = LogicalWall;

    map[MAP_WIDTH - 1][MAP_HEIGHT -1] = MAP_WALL_X;
    map[MAP_WIDTH - 1][MAP_HEIGHT -2] = wallOffset + MAP_WALL_7;
    map[MAP_WIDTH - 2][MAP_HEIGHT -2] = wallOffset + MAP_WALL_77;
    map[MAP_WIDTH - 2][MAP_HEIGHT -1] = wallOffset + MAP_WALL_7;
    logicalMap[MAP_WIDTH - 2][MAP_HEIGHT -2] = LogicalWall;

    if (rand() % 4 == 0)
    {
      // some barrels in the middle
      addDestroyableObject(MAP_WIDTH / 2 - 1, MAP_HEIGHT, MAPOBJ_BARREL);
      addDestroyableObject(MAP_WIDTH / 2, MAP_HEIGHT, MAPOBJ_BARREL);
      addDestroyableObject(MAP_WIDTH / 2 + 1, MAP_HEIGHT, MAPOBJ_BARREL);
    }
  }
  if (type == 2) // blocks in the middle
  {
    if (rand() % 3 == 0) initPattern(PatternBorder);

    r = 1 + rand() % 3;

    generateTable(x0 - r, y0 - 1, 1 + 2 * r, 3, MAPOBJ_BIG_OBSTACLE);

    if (rand() / 3 == 0)
    {
      r = rand() % 4;
      switch (r)
      {
        case 0: addDestroyableObject(1, MAP_HEIGHT - 2, MAPOBJ_BARREL); break;
        case 1: addDestroyableObject(1, 1, MAPOBJ_BARREL); break;
        case 2: addDestroyableObject(MAP_WIDTH - 2, MAP_HEIGHT - 2, MAPOBJ_BARREL); break;
        case 3: addDestroyableObject(MAP_WIDTH - 2, 1, MAPOBJ_BARREL); break;
      }
    }
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
    if (rand() % 2 == 0)
    {
      // type 1
      if (rand() % 3 == 0) initPattern(PatternSmallChecker);
      bool leftOriented = rand() % 2 == 0;

      int bankType = rand() % 4;
      if (bankType == 2)
      {
        int xPlayer = game().getPlayerPosition().x;
        if (xPlayer > GAME_WIDTH / 2 - TILE_WIDTH && xPlayer < GAME_WIDTH / 2 + TILE_WIDTH)
          bankType = rand() % 2;
      }

      int x0 = leftOriented ? 5 : 3;

      if (leftOriented)
      {
        objectsMap[2][4] = MAPOBJ_CHURCH_FURN_L;
        logicalMap[2][4] = LogicalObstacle;
        if (rand() % 2 == 0) addDestroyableObject(1, 1, MAPOBJ_BARREL);
        else addDestroyableObject(1, MAP_HEIGHT - 2, MAPOBJ_BARREL);
      }
      else
      {
        objectsMap[12][4] = MAPOBJ_CHURCH_FURN_R;
        logicalMap[12][4] = LogicalObstacle;
        if (rand() % 2 == 0) addDestroyableObject(MAP_WIDTH - 2, 1, MAPOBJ_BARREL);
        else addDestroyableObject(MAP_WIDTH - 2, MAP_HEIGHT - 2, MAPOBJ_BARREL);
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

          default:
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
    else
    {
      initPattern(PatternSmallStar);
      generateLongObject(2, 2, 3, MAPOBJ_LONG_LEFT);
      generateLongObject(2, 6, 3, MAPOBJ_LONG_LEFT);
      generateLongObject(10, 2, 3, MAPOBJ_LONG_LEFT);
      generateLongObject(10, 6, 3, MAPOBJ_LONG_LEFT);
      generateLongObject(6, 4, 3, MAPOBJ_LONG_LEFT);
    }
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

    if (game().getLevel() >= 4 || rand() % 4 > 0)
    {
      int nbBarrels = 6 + rand() % 9;
      for (int i = 0; i < nbBarrels; i++)
      {
        int objX = 2 + rand() % (MAP_WIDTH - 4);
        int objY = 2 + rand() % (MAP_HEIGHT - 4);

        int type = (rand() % 4) % 3 + MAPOBJ_BARREL_NO_DROP;
        if (rand() % 4 == 0) type = MAPOBJ_BARREL_EXPL;

        addDestroyableObject(objX, objY, type);
      }
    }
  }

  generateRandomTiles();
}

void DungeonMap::addHole(int x, int y)
{
  int n = MAPOBJ_HOLE;
  if (y > 0 && logicalMap[x][y - 1] == LogicalHole)
    n = MAPOBJ_HOLE_BOTTOM;
  else if (logicalMap[x - 1][y] == LogicalHole && logicalMap[x - 1][y - 1] == LogicalHole)
    n = MAPOBJ_HOLE_LEFT;
  else if (logicalMap[x + 1][y] == LogicalHole && logicalMap[x + 1][y - 1] == LogicalHole)
    n = MAPOBJ_HOLE_RIGHT;
  objectsMap[x][y] = n;
  logicalMap[x][y] = LogicalHole;

  if (logicalMap[x][y + 1] == LogicalHole && objectsMap[x - 1][y + 1] == MAPOBJ_HOLE_TOP)
    objectsMap[x - 1][y + 1] = MAPOBJ_HOLE_LEFT;
  if (logicalMap[x][y + 1] == LogicalHole && objectsMap[x + 1][y + 1] == MAPOBJ_HOLE_TOP)
    objectsMap[x + 1][y + 1] = MAPOBJ_HOLE_RIGHT;
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
    /*else if (rand() % 2 == 0)
    {
      for (int i = 1; i <= MAP_WIDTH - 2; i++)
      {
        if (i != MAP_WIDTH / 2 || !hasNeighbourUp()) addHole(i, 1);
      }
      for (int i = 2; i <= MAP_HEIGHT - 3; i++)
      {
        if (i != MAP_HEIGHT / 2 || !hasNeighbourLeft()) addHole(1, i);
        if (i != MAP_HEIGHT / 2 || !hasNeighbourRight()) addHole(MAP_WIDTH - 2, i);
      }
      addHole(MAP_WIDTH - 2, MAP_HEIGHT - 2);
      for (int i = 1; i <= MAP_WIDTH - 3; i++)
      {
        if (i != MAP_WIDTH / 2 || !hasNeighbourDown()) addHole(i, MAP_HEIGHT - 2);
      }
    }*/
  }
  else if (type == 1)
  {
    // corner hole
    if (rand() % 3 == 0) initPattern(PatternSmallChecker);
    addHole(1, 1);
    addHole(MAP_WIDTH - 2, MAP_HEIGHT -2);
    addHole(1, MAP_HEIGHT -2);
    addHole(MAP_WIDTH - 2, 1);

    if (rand() % 2 == 0)
      addDestroyableObject(MAP_WIDTH - 2 + rand() % 5, MAP_HEIGHT - 2 + rand() % 5, MAPOBJ_BARREL_EXPL);
  }
  else if (type == 2)
  {
    // blocks in the middle
    if (rand() % 3 == 0) initPattern(PatternBorder);

    /*if (false) //irregular // TO CORRECT
    {
      r = 1; //rand() % 2;

      for (i = x0 - r; i <= x0 + r; i++)
      {
        if (rand() % 3 == 0) addHole(i, y0 - 1);
        addHole(i, y0);
        if (rand() % 3 == 0) addHole(i, y0 + 1);
      }

      for (j = y0 - 1; j <= y0 + 1; j++)
      {
        if (rand() % 3 == 0) addHole(x0 - r - 1, j);
        if (rand() % 3 == 0) addHole(x0 + r + 1, j);
      }
    }
    else*/
    {
      r = 1 + rand() % 2;
      for (i = x0 - r; i <= x0 + r; i++)
        for (j = y0 - 1; j <= y0 + 1; j++)
        {
          addHole(i, j);
        }
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

    if (rand() % 3 == 0)
    {
      addDestroyableObject(4, 2, (rand() % 4) % 3 + MAPOBJ_BARREL_NO_DROP);
      addDestroyableObject(4, 3, (rand() % 4) % 3 + MAPOBJ_BARREL_NO_DROP);
      addDestroyableObject(4, MAP_HEIGHT - 4, (rand() % 4) % 3 + MAPOBJ_BARREL_NO_DROP);
      addDestroyableObject(4, MAP_HEIGHT - 3, (rand() % 4) % 3 + MAPOBJ_BARREL_NO_DROP);
      addDestroyableObject(MAP_WIDTH - 5, 2, (rand() % 4) % 3 + MAPOBJ_BARREL_NO_DROP);
      addDestroyableObject(MAP_WIDTH - 5, 3, (rand() % 4) % 3 + MAPOBJ_BARREL_NO_DROP);
      addDestroyableObject(MAP_WIDTH - 5, MAP_HEIGHT - 4, (rand() % 4) % 3 + MAPOBJ_BARREL_NO_DROP);
      addDestroyableObject(MAP_WIDTH - 5, MAP_HEIGHT - 3, (rand() % 4) % 3 + MAPOBJ_BARREL_NO_DROP);
    }
  }
  else if (type == 4)
  {
    if (rand() % 2 == 0)
    {
      // holes and (sometimes) obstacles randomly
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
    else
    {
      // big holes left and right
      bool holesNW = rand() % 2 == 0;
      for (int i = 0; i < 5; i++)
      {
        if (holesNW)
        {
          addHole(1 + i, 1);
          addHole(1 + i, 2);

          addHole(MAP_WIDTH - 2 - i, MAP_HEIGHT - 3);
          addHole(MAP_WIDTH - 2 - i, MAP_HEIGHT - 2);
        }
        else
          {
          addHole(1 + i, MAP_HEIGHT - 3);
          addHole(1 + i, MAP_HEIGHT - 2);

          addHole(MAP_WIDTH - 2 - i, 1);
          addHole(MAP_WIDTH - 2 - i, 2);
        }
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

    if (game().getLevel() >= 4 || rand() % 4 > 0)
    {
      int nbBarrels = 6 + rand() % 9;
      for (int i = 0; i < nbBarrels; i++)
      {
        int objX = 2 + rand() % (MAP_WIDTH - 4);
        int objY = 2 + rand() % (MAP_HEIGHT - 4);

        int type = (rand() % 4) % 3 + MAPOBJ_BARREL_NO_DROP;
        if (rand() % 4 == 0) type = MAPOBJ_BARREL_EXPL;

        addDestroyableObject(objX, objY, type);
      }
    }
  }

  generateRandomTiles();
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
    if (logicalMap[rx][ry] == LogicalFloor
        && objectsMap[rx][ry] == 0)
    {
      objectsMap[rx][ry] = MAPOBJ_GRID;
      counter--;
    }
  }
}

void DungeonMap::generateCorridors()
{
  int xCor = 1 + rand()% 4;
  int yCor = 1 + rand()% 1;

  int wallOffset = wallType * 24;

  if (!hasNeighbourLeft())
  {
    for (int i = 0; i < xCor; i++)
      for (int j = 0; j < MAP_HEIGHT; j++)
      {
        map[i][j] = MAP_WALL_X;
        logicalMap[i][j] = LogicalWall;
      }

  }
  if (!hasNeighbourRight())
  {
    for (int i = MAP_WIDTH - 1; i > MAP_WIDTH - 1 - xCor; i--)
      for (int j = 0; j < MAP_HEIGHT; j++)
      {
        map[i][j] = MAP_WALL_X;
        logicalMap[i][j] = LogicalWall;
      }
  }
  if (!hasNeighbourUp())
  {
    for (int i = 0; i < MAP_WIDTH; i++)
      for (int j = 0; j < yCor; j++)
      {
        map[i][j] = MAP_WALL_X;
        logicalMap[i][j] = LogicalWall;
      }
  }
  if (!hasNeighbourDown())
  {
    for (int i = 0; i < MAP_WIDTH; i++)
      for (int j = MAP_HEIGHT - 1; j > MAP_HEIGHT - 1 - yCor; j--)
      {
        map[i][j] = MAP_WALL_X;
        logicalMap[i][j] = LogicalWall;
      }
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
          if (j == 0 || getTile(i, j - 1) == MAP_WALL_X)
          {
            map[i][j] = wallOffset + MAP_WALL_7;
          }
          else if (j == MAP_HEIGHT - 1 || getTile(i, j + 1) == MAP_WALL_X)
          {
            map[i][j] = wallOffset + MAP_WALL_7;
          }
          else
          {
            if (j < MAP_HEIGHT / 2) map[i][j] = wallOffset + MAP_WALL_87 + rand() % 8;
            else if (j > MAP_HEIGHT / 2) map[i][j] = wallOffset + MAP_WALL_87 + rand() % 8;
            else map[i][j] = wallOffset + MAP_WALL_8;
          }
          logicalMap[i][j] = LogicalWall;
        }
        else if (getTile(i + 1, j) == MAP_WALL_X)
        {
          if (j == 0 || getTile(i, j - 1) == MAP_WALL_X)
          {
            map[i][j] = wallOffset + MAP_WALL_7;
            map[i][j] = wallOffset + MAP_WALL_7;
          }
          else if (j == MAP_HEIGHT - 1 || getTile(i, j + 1) == MAP_WALL_X) map[i][j] = wallOffset + MAP_WALL_7;
          else
          {
            if (j < MAP_HEIGHT / 2) map[i][j] = wallOffset + MAP_WALL_87 + rand() % 8;
            else if (j > MAP_HEIGHT / 2) map[i][j] = wallOffset + MAP_WALL_87 + rand() % 8;
            else map[i][j] = wallOffset + MAP_WALL_8;
          }
          logicalMap[i][j] = LogicalWall;
        }
        else if (getTile(i, j - 1) == MAP_WALL_X)
        {
          if (i == 0) map[i][j] = wallOffset + MAP_WALL_7;
          else if (i == MAP_WIDTH - 1) map[i][j] = wallOffset + MAP_WALL_7;
          else
          {
            if (i < MAP_WIDTH / 2) map[i][j] = wallOffset + MAP_WALL_87 + rand() % 8;
            else if (i > MAP_WIDTH / 2) map[i][j] = wallOffset + MAP_WALL_87 + rand() % 8;
            else map[i][j] = wallOffset + MAP_WALL_8;
          }
          logicalMap[i][j] = LogicalWall;
        }
        else if (getTile(i, j + 1) == MAP_WALL_X)
        {
          if (i == 0) map[i][j] = wallOffset + MAP_WALL_7;
          else if (i == MAP_WIDTH - 1) map[i][j] = wallOffset + MAP_WALL_7;
          else
          {
            if (i < MAP_WIDTH / 2) map[i][j] = wallOffset + MAP_WALL_87 + rand() % 8;
            else if (i > MAP_WIDTH / 2) map[i][j] = wallOffset + MAP_WALL_87 + rand() % 8;
            else map[i][j] = wallOffset + MAP_WALL_8;
          }
          logicalMap[i][j] = LogicalWall;
        }
      }
    }
  }
}

void DungeonMap::generateRandomTiles()
{
  for (int i = 0; i < NB_RANDOM_TILES_IN_ROOM; i++)
    generateRandomTile(i);
}

void DungeonMap::generateRandomTile(int index)
{
  bool ok = false;
  for (int i = 0; !ok && i < NB_RANDOM_TILE_TRY; i++)
  {
    int n = rand() % NB_RANDOM_TILES;

    int xTile, yTile;

    if (randomDungeonTiles[n].canBeOnWall)
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

    // proceed rarity
    if (randomDungeonTiles[n].rarity > 1 && (rand() % randomDungeonTiles[n].rarity) != 0 )
    {
      ok = false;
    }

    // each random sprite type should be present no more that once in a room
    for (int j = 0; j < index; j++)
    {
      if (n == randomTileElements[j].type) ok = false;
    }

    if (!randomDungeonTiles[n].randomPlace || roomType == roomTypeKey)
    {
      for (int ix = x0; ix <= xf; ix++)
        for (int iy = y0; iy <= yf; iy++)
        {
          ok = ok && isWalkable(ix, iy);
        }
    }

    if (ok && !randomDungeonTiles[n].canBeOnWall)
    {
      for (int ix = x0; ix <= xf; ix++)
        for (int iy = y0; iy <= yf; iy++)
        {
          ok = ok && isFlyable(ix, iy);
        }
    }

    if (ok)
    {
      randomTileElements[index].type = n;
      randomTileElements[index].x = xTile;
      randomTileElements[index].y = yTile;
      if (randomDungeonTiles[n].canRotate)
        randomTileElements[index].rotation = rand()% 360;
      else
        randomTileElements[index].rotation = 0;
    }
  }
}

bool DungeonMap::callRevelation()
{
  if (hasNeighbourRight() && !gameFloor->getMap(x + 1, y)->isRevealed())
  {
    map[MAP_WIDTH - 1][MAP_HEIGHT / 2] = floorOffset;
    map[MAP_WIDTH - 1][MAP_HEIGHT / 2 - 1] = floorOffset;
    map[MAP_WIDTH - 1][MAP_HEIGHT / 2 + 1] = floorOffset;

    if (cleared)
      openDoor(MAP_WIDTH - 1, MAP_HEIGHT / 2);
    else
      closeDoor(MAP_WIDTH - 1, MAP_HEIGHT / 2);

    game().setDoorVisible(East);
    gameFloor->getMap(x + 1, y)->setRevealed(true);
    hasChanged = true;

    for (int i = 0; i < 36; i++)
      game().generateStar(
                    (i % 2 == 0) ? sf::Color(50, 50, 255, 255) : sf::Color(200, 200, 255, 255),
                    (MAP_WIDTH - 1) * TILE_WIDTH + rand() % TILE_WIDTH,
                     (MAP_HEIGHT / 2 - 1) * TILE_HEIGHT + rand() % (TILE_HEIGHT * 3) );

    return true;
  }
  else if (hasNeighbourLeft() && !gameFloor->getMap(x - 1, y)->isRevealed())
  {
    map[0][MAP_HEIGHT / 2] = floorOffset;
    map[0][MAP_HEIGHT / 2 - 1] = floorOffset;
    map[0][MAP_HEIGHT / 2 + 1] = floorOffset;

    if (cleared)
      openDoor(0, MAP_HEIGHT / 2);
    else
      closeDoor(0, MAP_HEIGHT / 2);

    game().setDoorVisible(West);
    gameFloor->getMap(x - 1, y)->setRevealed(true);
    hasChanged = true;

    for (int i = 0; i < 36; i++)
      game().generateStar(
                    (i % 2 == 0) ? sf::Color(50, 50, 255, 255) : sf::Color(200, 200, 255, 255),
                    rand() % TILE_WIDTH,
                     (MAP_HEIGHT / 2 - 1) * TILE_HEIGHT + rand() % (TILE_HEIGHT * 3) );

    return true;
  }
  else if (hasNeighbourUp() && !gameFloor->getMap(x, y - 1)->isRevealed())
  {
    map[MAP_WIDTH / 2][0] = floorOffset;
    map[MAP_WIDTH / 2 - 1][0] = floorOffset;
    map[MAP_WIDTH / 2 + 1][0] = floorOffset;

    if (cleared)
      openDoor(MAP_WIDTH / 2, 0);
    else
      closeDoor(MAP_WIDTH / 2, 0);

    game().setDoorVisible(North);
    gameFloor->getMap(x, y - 1)->setRevealed(true);
    hasChanged = true;

    for (int i = 0; i < 36; i++)
      game().generateStar(
                    (i % 2 == 0) ? sf::Color(50, 50, 255, 255) : sf::Color(200, 200, 255, 255),
                    (MAP_WIDTH / 2 - 1) * TILE_WIDTH + rand() % (TILE_WIDTH * 3),
                    rand() % TILE_HEIGHT );

    return true;
  }
  else if (hasNeighbourDown() && !gameFloor->getMap(x, y + 1)->isRevealed())
  {
    map[MAP_WIDTH / 2][MAP_HEIGHT - 1] = floorOffset;
    map[MAP_WIDTH / 2 - 1][MAP_HEIGHT - 1] = floorOffset;
    map[MAP_WIDTH / 2 + 1][MAP_HEIGHT - 1] = floorOffset;

    if (cleared)
      openDoor(MAP_WIDTH / 2, MAP_HEIGHT -1);
    else
      closeDoor(MAP_WIDTH / 2, MAP_HEIGHT -1);

    game().setDoorVisible(South);
    gameFloor->getMap(x, y + 1)->setRevealed(true);
    hasChanged = true;

    for (int i = 0; i < 36; i++)
      game().generateStar(
                    (i % 2 == 0) ? sf::Color(50, 50, 255, 255) : sf::Color(200, 200, 255, 255),
                    (MAP_WIDTH / 2 - 1) * TILE_WIDTH + rand() % (TILE_WIDTH * 3),
                    (MAP_HEIGHT - 1) * TILE_HEIGHT + rand() % TILE_HEIGHT );

    return true;
  }

  return false;
}

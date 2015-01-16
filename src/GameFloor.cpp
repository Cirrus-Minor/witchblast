#include "GameFloor.h"
#include "Items.h"
#include "WitchBlastGame.h"
#include <time.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>

GameFloor::GameFloor()
{
  this->level = 0;
  forceShop = false;
  // Init maps
  for (int i=0; i < FLOOR_WIDTH; i++)
    for (int j=0; j < FLOOR_HEIGHT; j++)
      maps[i][j] = NULL;
}

GameFloor::GameFloor(int level)
{
  forceShop = false;
  // Init maps
  for (int i=0; i < FLOOR_WIDTH; i++)
    for (int j=0; j < FLOOR_HEIGHT; j++)
      maps[i][j] = NULL;

  this->level = level;
}

void GameFloor::setForceShop()
{
  forceShop = true;
}

GameFloor::~GameFloor()
{
  // Free maps
  for (int i=0; i < FLOOR_WIDTH; i++)
    for (int j=0; j < FLOOR_HEIGHT; j++)
      if (maps[i][j] != NULL) delete (maps[i][j]);
}

roomTypeEnum GameFloor::getRoom(int x, int y)
{
  if (x < 0 || y < 0 || x >= FLOOR_WIDTH || y >= FLOOR_HEIGHT) return roomTypeNULL;
  return floor[x][y];
}

void GameFloor::setRoom(int x, int y, roomTypeEnum roomType)
{
  floor[x][y] = roomType;
}

bool GameFloor::hasRoomOfType(roomTypeEnum roomType)
{
  for (int i=0; i < FLOOR_WIDTH; i++)
    for (int j=0; j < FLOOR_HEIGHT; j++)
      if (floor[i][j] == roomType) return true;
  return false;
}

DungeonMap* GameFloor::getMap(int x, int y)
{
  if (x < 0 || y < 0 || x >= FLOOR_WIDTH || y >= FLOOR_HEIGHT) return NULL;
  return maps[x][y];
}

void GameFloor::setMap(int x, int y, DungeonMap* map)
{
  maps[x][y] = map;
}

DungeonMap* GameFloor::getAndVisitMap(int x, int y)
{
  maps[x][y]->setVisited(true);

  if (x > 0 && floor[x-1][y] > 0) maps[x-1][y]->setKnown(true);
  if (x < FLOOR_WIDTH - 1 && floor[x+1][y] > 0) maps[x+1][y]->setKnown(true);;
  if (y > 0 && floor[x][y-1] > 0) maps[x][y-1]->setKnown(true);;
  if (y < FLOOR_HEIGHT - 1 && floor[x][y+1] > 0) maps[x][y+1]->setKnown(true);;

  return maps[x][y];
}

void GameFloor::displayToConsole()
{
  for (int j=0; j < FLOOR_HEIGHT; j++)
  {
    for (int i=0; i < FLOOR_WIDTH; i++)
    {
      switch (floor[i][j])
      {
      case roomTypeNULL:
        printf(".");
        break;
      case roomTypeStandard:
        printf("#");
        break;
      case roomTypeBoss:
        printf("@");
        break;
      case roomTypeMerchant:
        printf("$");
        break;
      case roomTypeKey:
        printf("!");
        break;
      case roomTypeBonus:
        printf("*");
        break;
      case roomTypeExit:
        printf("X");
        break;
      case roomTypeStarting:
        printf("0");
        break;
      case roomTypeChallenge:
        printf("?");
        break;
      case roomTypeTemple:
        printf("+");
        break;
      }
    }
    printf("\n");
  }
}

int GameFloor::neighboorCount(int x, int y)
{
  int count = 0;
  if (x > 0 && floor[x-1][y] > 0) count++;
  if (x < FLOOR_WIDTH - 1 && floor[x+1][y] > 0) count++;
  if (y > 0 && floor[x][y-1] > 0) count++;
  if (y < FLOOR_HEIGHT - 1 && floor[x][y+1] > 0) count++;
  return count;
}

IntCoord GameFloor::getFirstNeighboor(int x, int y)
{
  if (x > 0 && floor[x - 1][y] > 0) return IntCoord(x - 1, y);
  if (x < FLOOR_WIDTH - 1 && floor[x + 1][y] > 0) return IntCoord(x + 1, y);
  if (y > 0 && floor[x][y - 1] > 0) return IntCoord(x, y - 1);
  if (y < FLOOR_HEIGHT - 1 && floor[x][y + 1] > 0) return IntCoord(x, y + 1);
  return IntCoord(-1, -1);
}

std::vector<IntCoord> GameFloor::findSuperIsolated()
{
  std::vector<IntCoord> results;
  for (int i=0; i < FLOOR_WIDTH; i++)
    for (int j=0; j < FLOOR_HEIGHT; j++)
    {
      if (floor[i][j] == roomTypeStandard && neighboorCount(i, j) == 1)
      {
        if (isSuperIsolated(i, j))
        {
          IntCoord result(i, j);
          results.push_back(result);
        }
      }
    }
  return results;
}

bool GameFloor::isSuperIsolated(int x, int y)
{
  if (neighboorCount(x, y) != 1) return false;
  else
  {
    if (x > 0 && floor[x-1][y]==1 && neighboorCount(x-1, y) == 2) return true;
    else if (x < FLOOR_WIDTH - 1 && floor[x+1][y]==1 &&  neighboorCount(x+1, y) == 2) return true;
    else if (y < FLOOR_HEIGHT - 1 && floor[x][y+1]==1 &&  neighboorCount(x, y+1) == 2) return true;
  }
  return false;
}

bool GameFloor::finalize()
{
  // step 1 : Exit and boss rooms
  std::vector<IntCoord> superIsolatedVector = findSuperIsolated();
  if (superIsolatedVector.size() == 0)
    return false;
  else
  {
    int index = rand() % superIsolatedVector.size();
    floor[superIsolatedVector[index].x][superIsolatedVector[index].y] = roomTypeExit;

    IntCoord bossCoord = getFirstNeighboor(superIsolatedVector[index].x, superIsolatedVector[index].y);
    if (bossCoord.x == -1) return false;

    floor[bossCoord.x][bossCoord.y] = roomTypeBoss;
  }

  // step 2 : bonus, key, etc...
  std::vector<IntCoord> isolatedVector;
  for (int i=0; i < FLOOR_WIDTH; i++)
    for (int j=0; j < FLOOR_HEIGHT; j++)
      if (floor[i][j] == 1 && neighboorCount(i, j) == roomTypeStandard)
      {
        IntCoord found(i, j);
        isolatedVector.push_back(found);
      }

  int nbIsolatedRooms = isolatedVector.size();
  if (nbIsolatedRooms < 2) return false;
  int index;

  // bonus
  index = rand() % isolatedVector.size();
  floor[isolatedVector[index].x][isolatedVector[index].y] = roomTypeBonus;
  isolatedVector.erase(isolatedVector.begin() + index);

  // key
  index = rand() % isolatedVector.size();
  floor[isolatedVector[index].x][isolatedVector[index].y] = roomTypeKey;
  isolatedVector.erase(isolatedVector.begin() + index);

  int nbIsolatedRoomsMin = 5;
  if (level == 1) nbIsolatedRoomsMin = 2;
  else if (level == 2) nbIsolatedRoomsMin = 4;
  //if (level > 2 && game().getPlayer()->getActiveSpell().spell == SpellNone) nbIsolatedRoomsMin = 4;

  if (nbIsolatedRooms < nbIsolatedRoomsMin) return false;

  if (nbIsolatedRooms < 3) return true;

  // shop
  index = rand() % isolatedVector.size();
  floor[isolatedVector[index].x][isolatedVector[index].y] = roomTypeMerchant;
  isolatedVector.erase(isolatedVector.begin() + index);

  if (level == 1 || nbIsolatedRooms < 4) return true;

  // temple
  index = rand() % isolatedVector.size();
  floor[isolatedVector[index].x][isolatedVector[index].y] = roomTypeTemple;
  isolatedVector.erase(isolatedVector.begin() + index);

  // challenge
  if (isolatedVector.size() > 0)
  {
    index = rand() % isolatedVector.size();
    floor[isolatedVector[index].x][isolatedVector[index].y] = roomTypeChallenge;
    isolatedVector.erase(isolatedVector.begin() + index);
  }

  return true;
}

void GameFloor::createFloor()
{
  bool ok=false;
  while (!ok)
  {
    int i, j;

    // Free maps
    for (i=0; i < FLOOR_WIDTH; i++)
      for (j=0; j < FLOOR_HEIGHT; j++)
        if (maps[i][j] != NULL)
        {
          delete (maps[i][j]);
          maps[i][j] = NULL;
        }
    generate();
    ok = finalize();

    int x0 = FLOOR_WIDTH / 2;
    int y0 = FLOOR_HEIGHT / 2;

    // Maps
    for (i=0; i < FLOOR_WIDTH; i++)
      for (j=0; j < FLOOR_HEIGHT; j++)
        if (floor[i][j] > 0)
        {
          maps[i][j] = new DungeonMap(this, i, j);
          if (i == x0 && j == y0)
            maps[i][j]->setRoomType(roomTypeStarting);
          else
            maps[i][j]->setRoomType((roomTypeEnum)(floor[i][j]));
        }

    displayToConsole();
  }
}

void GameFloor::generate()
{
  int i, j;
  int nbRooms;
  int requiredRoms = 6 + level * 2;
  if (requiredRoms > 18) requiredRoms = 18;
  // Init
  for (i=0; i < FLOOR_WIDTH; i++)
    for (j=0; j < FLOOR_HEIGHT; j++)
    {
      floor[i][j] = roomTypeNULL;
    }

  // First room
  int x0 = FLOOR_WIDTH / 2;
  int y0 = FLOOR_HEIGHT / 2;
  floor[x0][y0] = roomTypeStarting;
  nbRooms = 1;

  // neighboor
  while (nbRooms == 1)
  {
    if (rand() % 3 == 0)
    {
      floor[x0-1][y0] = roomTypeStandard;
      nbRooms++;
    }
    if (rand() % 3 == 0)
    {
      floor[x0+1][y0] = roomTypeStandard;
      nbRooms++;
    }
    if (rand() % 3 == 0)
    {
      floor[x0][y0-1] = roomTypeStandard;
      nbRooms++;
    }
    if (level == 1)
    {
      if (rand() % 3 == 0)
      {
        floor[x0][y0+1] = roomTypeStandard;
        nbRooms++;
      }
    }
  }

  // others
  while (nbRooms < requiredRoms)
    for (int k = 0; k < 8; k++)
    {
      i = rand() % FLOOR_WIDTH;
      j = rand() % FLOOR_HEIGHT;
      if (floor[i][j] == 0 && ( (level == 1) || (i != x0 || j != y0 + 1) ))
      {
        int n = neighboorCount(i, j);
        switch (n)
        {
        case 1:
        {
          floor[i][j] = roomTypeStandard;
          nbRooms++;
          break;
        }
        case 2:
        {
          if (rand()% 5 == 0)
          {
            floor[i][j] = roomTypeStandard;
            nbRooms++;
          }
          break;
        }
        case 3:
        {
          if (rand()% 20 == 0)
          {
            floor[i][j] = roomTypeStandard;
            nbRooms++;
          }
          break;
        }
        }
      }
    }
}

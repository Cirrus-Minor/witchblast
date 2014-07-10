#include "GameFloor.h"
#include <time.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>

GameFloor::GameFloor()
{
  this->level = 0;
  // Init maps
  for (int i=0; i < FLOOR_WIDTH; i++)
    for (int j=0; j < FLOOR_HEIGHT; j++)
      maps[i][j] = NULL;
}

GameFloor::GameFloor(int level)
{
  // Init maps
  for (int i=0; i < FLOOR_WIDTH; i++)
    for (int j=0; j < FLOOR_HEIGHT; j++)
      maps[i][j] = NULL;

  this->level = level;
}

GameFloor::~GameFloor()
{
  // Free maps
  for (int i=0; i < FLOOR_WIDTH; i++)
    for (int j=0; j < FLOOR_HEIGHT; j++)
      if (maps[i][j] != NULL) delete (maps[i][j]);
}

int GameFloor::getRoom(int x, int y)
{
  if (x < 0 || y < 0 || x >= FLOOR_WIDTH || y >= FLOOR_HEIGHT) return 0;
  return floor[x][y];
}

void GameFloor::setRoom(int x, int y, int roomType)
{
  floor[x][y] = roomType;
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
        case 0: printf("."); break;
        case 1: printf("#"); break;
        case 2: printf("&"); break;
        case 3: printf("$"); break;
        case 4: printf("!"); break;
        case 5: printf("*"); break;
        case 6: printf("X"); break;
        case 7: printf("0"); break;
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

bool GameFloor::isSuperIsolated(int x, int y)
{
  if (neighboorCount(x, y) != 1) return false;
  else
  {
    if (x > 0 && floor[x-1][y]==1 && neighboorCount(x-1, y) == 2)
    {
      floor[x-1][y] = roomTypeBoss;
      floor[x][y] = roomTypeExit;
      return true;
    }
    else if (x < FLOOR_WIDTH - 1 && floor[x+1][y]==1 &&  neighboorCount(x+1, y) == 2)
    {
      floor[x+1][y] = roomTypeBoss;
      floor[x][y] = roomTypeExit;
      return true;
    }
    else if (y < FLOOR_HEIGHT - 1 && floor[x][y+1]==1 &&  neighboorCount(x, y+1) == 2)
    {
      floor[x][y+1] = roomTypeBoss;
      floor[x][y] = roomTypeExit;
      return true;
    }
  }
  return false;
}

bool GameFloor::finalize()
{
  bool bKey = false;
  bool bBonus = false;
  bool bMerchant = false;
  bool bExit = false;

  for (int i=0; i < FLOOR_WIDTH; i++)
    for (int j=0; j < FLOOR_HEIGHT; j++)
    {
      if (floor[i][j] == 1 && neighboorCount(i, j) == roomTypeStandard)
      {
        if (!bExit && isSuperIsolated(i, j))
        {
          bExit = true;
        }
        else
        {
          if (!bKey)
          {
            floor[i][j]= roomTypeKey;
            bKey = true;
          }
          else if (!bBonus)
          {
            floor[i][j]= roomTypeBonus;
            bBonus = true;
          }
          else if (!bMerchant)
          {
            floor[i][j]= roomTypeMerchant;
            bMerchant = true;
          }
        }
      }
    }

  return bExit && bKey && bBonus;
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
      floor[i][j] = 0;
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
      floor[x0-1][y0] = 1;
      nbRooms++;
    }
    if (rand() % 3 == 0)
    {
      floor[x0+1][y0] = 1;
      nbRooms++;
    }
    if (rand() % 3 == 0)
    {
      floor[x0][y0-1] = 1;
      nbRooms++;
    }
    if (level == 1)
    {
      if (rand() % 3 == 0)
      {
        floor[x0][y0+1] = 1;
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
            floor[i][j] = 1;
            nbRooms++;
            break;
          }
          case 2:
          {
            if (rand()% 5 == 0)
            {
              floor[i][j] = 1;
              nbRooms++;
            }
            break;
          }
          case 3:
          {
            if (rand()% 20 == 0)
            {
              floor[i][j] = 1;
              nbRooms++;
            }
            break;
          }
        }
      }
    }


}

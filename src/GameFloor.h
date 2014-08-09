#ifndef GAMEFLOOR_H
#define GAMEFLOOR_H

#include "Constants.h"
#include "DungeonMap.h"
#include <vector>
#include <iostream>

class GameFloor
{
  public:
    GameFloor();
    GameFloor(int level);
    virtual ~GameFloor();
    void createFloor();
    void displayToConsole();
    roomTypeEnum getRoom(int x, int y);
    void setRoom(int x, int y, roomTypeEnum roomType);
    DungeonMap* getMap(int x, int y);
    void setMap(int x, int y, DungeonMap* map);
    DungeonMap* getAndVisitMap(int x, int y);
  protected:
  private:
    int level;
    roomTypeEnum floor[FLOOR_WIDTH][FLOOR_HEIGHT];

    int neighboorCount(int x, int y);
    IntCoord getFirstNeighboor(int x, int y);

    bool isSuperIsolated(int x, int y);
    std::vector<IntCoord> findSuperIsolated();
    void generate();
    bool finalize();

    DungeonMap* maps[FLOOR_WIDTH][FLOOR_HEIGHT];
};

#endif // GAMEFLOOR_H

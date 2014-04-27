#ifndef GAMEFLOOR_H
#define GAMEFLOOR_H

#include "Constants.h"
#include "DungeonMap.h"

class GameFloor
{
  public:
    GameFloor();
    GameFloor(int level);
    virtual ~GameFloor();
    void createFloor();
    void displayToConsole();
    int getRoom(int x, int y);
    void setRoom(int x, int y, int roomType);
    DungeonMap* getMap(int x, int y);
    void setMap(int x, int y, DungeonMap* map);
    DungeonMap* getAndVisitMap(int x, int y);
  protected:
  private:
    int level;
    int floor[FLOOR_WIDTH][FLOOR_HEIGHT];

    int neighboorCount(int x, int y);
    bool isSuperIsolated(int x, int y);
    void generate();
    bool finalize();

    DungeonMap* maps[FLOOR_WIDTH][FLOOR_HEIGHT];
};

#endif // GAMEFLOOR_H

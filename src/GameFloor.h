#ifndef GAMEFLOOR_H
#define GAMEFLOOR_H

#include "Constants.h"
#include "DungeonMap.h"
#include <vector>
#include <iostream>

class GameFloor
{
  public:
    /*!
     *  \brief constructor of GameFloor (for level 0)
     */
    GameFloor();

    /*!
     *  \brief constructor of GameFloor for a given level
     *  \param level : the level of the floor
     */
    GameFloor(int level);
    virtual ~GameFloor();

    void setForceShop();

    void createFloor();
    void displayToConsole();

    /*!
     *  \brief returns the room type of the given position
     *  \param x : x coordinate of the room
     *  \param y : y coordinate of the room
     *  \return : the room type at the given position
     */
    roomTypeEnum getRoom(int x, int y);

    /*!
     *  \brief looks if a room type exists in this floor
     *  \param roomType : the room type we are looking for
     *  \return : true if the room type has been found
     */
    bool hasRoomOfType(roomTypeEnum roomType);
    void setRoom(int x, int y, roomTypeEnum roomType);
    DungeonMap* getMap(int x, int y);
    void setMap(int x, int y, DungeonMap* map);
    DungeonMap* getAndVisitMap(int x, int y);

    int neighboorCount(int x, int y);
  protected:
  private:
    int level;
    roomTypeEnum floor[FLOOR_WIDTH][FLOOR_HEIGHT];

    IntCoord getFirstNeighboor(int x, int y);

    bool isSuperIsolated(int x, int y);
    std::vector<IntCoord> findSuperIsolated();
    void generate();
    bool finalize();

    bool forceShop;

    DungeonMap* maps[FLOOR_WIDTH][FLOOR_HEIGHT];
};

#endif // GAMEFLOOR_H

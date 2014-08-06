#ifndef CHESTENTITY_H
#define CHESTENTITY_H

#include "sfml_game/CollidingSpriteEntity.h"
#include "Constants.h"

class ChestEntity : public CollidingSpriteEntity
{
  public:
    ChestEntity(float x, float y, int chestType, bool isOpen);
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();

    void open();
    bool getOpened();
    int getChestType();

  protected:

    virtual void readCollidingEntity(CollidingSpriteEntity* entity);

  private:
    bool isOpen;
    int chestType;

    float timer;
    void fallRock();
    void initFallingGrid();
    bool fallingGrid[MAP_WIDTH][MAP_HEIGHT];
};

#endif // CHESTENTITY_H

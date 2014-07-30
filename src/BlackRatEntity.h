#ifndef BLACKRATSPRITE_H
#define BLACKRATSPRITE_H

#include "EnemyEntity.h"

class BlackRatEntity : public EnemyEntity
{
  public:
    BlackRatEntity(float x, float y);
    virtual void animate(float delay);
    virtual void calculateBB();
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnnemy(GameEntity* collidingEntity);

  private:
    void findNextGoal();
    int currentDirection;
    IntCoord currentTile;
    IntCoord targetTile;
};

#endif // BLACKRATSPRITE_H

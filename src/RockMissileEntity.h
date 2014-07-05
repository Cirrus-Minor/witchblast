#ifndef ROCKMISSILENTITY_H
#define ROCKMISSILENTITY_H

#include "EnnemyEntity.h"

class RockMissileEntity : public EnnemyEntity
{
  public:
    RockMissileEntity(float x, float y, int rockType);
    virtual void animate(float delay);
    virtual void calculateBB();
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnnemy(GameEntity* collidingEntity);

	  virtual void dying();
  private:
    int collisionDirection;
    int rockType;
    bool hasCollided;

    void collideWall();
};

#endif // ROCKMISSILENTITY_H

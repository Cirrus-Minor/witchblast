#ifndef RATSPRITE_H
#define RATSPRITE_H

#include "EnemyEntity.h"

class RatEntity : public EnemyEntity
{
  public:
    enum ratTypeEnum { RatTypeNormal, RatTypeHelmet };
    RatEntity(float x, float y, ratTypeEnum ratType);
    virtual void animate(float delay);
    virtual void calculateBB();

  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnnemy(GameEntity* collidingEntity);
    virtual void collideWithBolt(BoltEntity* boltEntity);
  private:
    ratTypeEnum ratType;
};

#endif // RATSPRITE_H

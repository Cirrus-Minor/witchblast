#ifndef SPIDERWEBENTITY_H
#define SPIDERWEBENTITY_H

#include "EnemyEntity.h"

class SpiderWebEntity : public EnemyEntity
{
  public:
    SpiderWebEntity(float x, float y);
    virtual void animate(float delay);
    virtual void calculateBB();
    virtual void readCollidingEntity(CollidingSpriteEntity* entity);
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnnemy(GameEntity* collidingEntity);

	  virtual void drop();
  private:
    void collideWall();
};

#endif // SPIDERWEBENTITY_H

#ifndef SPIDERWEBENTITY_H
#define SPIDERWEBENTITY_H

#include "EnemyEntity.h"

class SpiderWebEntity : public EnemyEntity
{
  public:
    SpiderWebEntity(float x, float y, bool isFromPlayer);
    virtual void animate(float delay);
    virtual void calculateBB();
    virtual void readCollidingEntity(CollidingSpriteEntity* entity);
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnemy(EnemyEntity* entity) override;

	  virtual void drop();
  private:
    void collideWall();
    bool isFromPlayer;
};

#endif // SPIDERWEBENTITY_H

#ifndef BATENTITY_H
#define BATENTITY_H

#include "EnemyEntity.h"

class BatEntity : public EnemyEntity
{
  public:
    BatEntity(float x, float y, bool invocated);
    virtual void animate(float delay);
    virtual void calculateBB();
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnnemy(GameEntity* collidingEntity);

	  virtual void dying();
	  virtual void drop();
  private:
    float changingDelay;
    bool invocated;
};

#endif // BATENTITY_H

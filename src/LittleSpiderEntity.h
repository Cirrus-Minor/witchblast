#ifndef LITTLESPIDERENTITY_H
#define LITTLESPIDERENTITY_H

#include "EnnemyEntity.h"
#include "PlayerEntity.h"

class LittleSpiderEntity : public EnnemyEntity
{
  public:
    LittleSpiderEntity(float x, float y);
    virtual void animate(float delay);
    virtual void calculateBB();
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnnemy(GameEntity* collidingEntity);

	  virtual void drop();
  private:
    float timer;
};

#endif // LITTLESPIDERENTITY_H

#ifndef GREENRATSPRITE_H
#define GREENRATSPRITE_H

#include "EnnemyEntity.h"
#include "PlayerEntity.h"

class GreenRatEntity : public EnnemyEntity
{
  public:
    GreenRatEntity(float x, float y);
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
    float timer;
};

#endif // GREENRATSPRITE_H

#ifndef RATSPRITE_H
#define RATSPRITE_H

#include "EnnemyEntity.h"

class RatEntity : public EnnemyEntity
{
  public:
    RatEntity(float x, float y, GameMap* map);
    virtual void animate(float delay);
    virtual void calculateBB();
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

	  //virtual void readCollidingEntity(CollidingSpriteEntity* entity);
	  virtual void dying();
  private:

};

#endif // RATSPRITE_H

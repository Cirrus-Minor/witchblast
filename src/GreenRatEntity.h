#ifndef GREENRATSPRITE_H
#define GREENRATSPRITE_H

#include "EnnemyEntity.h"
#include "PlayerEntity.h"

class GreenRatEntity : public EnnemyEntity
{
  public:
    GreenRatEntity(float x, float y, GameMap* map, PlayerEntity* player);
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
    float timer;
    PlayerEntity* player;
};

#endif // GREENRATSPRITE_H

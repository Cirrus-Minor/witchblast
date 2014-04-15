#ifndef BATSPRITE_H
#define BATSPRITE_H

#include "EnnemyEntity.h"

class BatEntity : public EnnemyEntity
{
  public:
    BatEntity(float x, float y, GameMap* map);
    virtual void animate(float delay);
    virtual void calculateBB();
  protected:
    virtual bool collideWithMap(int direction);
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

	  virtual void dying();
  private:
    float changingDelay;

};

#endif // BATSPRITE_H

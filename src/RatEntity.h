#ifndef RATSPRITE_H
#define RATSPRITE_H

#include "EnnemyEntity.h"

class RatEntity : public EnnemyEntity
{
  public:
    RatEntity(float x, float y, WitchBlastGame* parent);
    virtual void animate(float delay);
    virtual void calculateBB();
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

	  virtual void dying();
  private:

};

#endif // RATSPRITE_H

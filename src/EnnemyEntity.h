#ifndef ENNEMYPRITE_H
#define ENNEMYPRITE_H

#include "BaseCreatureEntity.h"

class EnnemyEntity : public BaseCreatureEntity
{
  public:
    EnnemyEntity(sf::Texture* image, WitchBlastGame* parent, float x, float y);
    virtual void animate(float delay);
    virtual void calculateBB();
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

	  virtual void readCollidingEntity(CollidingSpriteEntity* entity);
	  virtual void dying();
	  virtual void drop();

	  virtual bool canCollide();

    int meleeDamages;
  private:

};

#endif // ENNEMYPRITE_H

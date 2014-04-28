#ifndef EVILFLOWER_H
#define EVILFLOWER_H

#include "EnnemyEntity.h"
#include "PlayerEntity.h"

class EvilFlowerEntity : public EnnemyEntity
{
  public:
    EvilFlowerEntity(float x, float y);
    virtual void animate(float delay);
    virtual void calculateBB();
    virtual void render(sf::RenderWindow* app);
    void fire();
  protected:
	  virtual void dying();
  private:
    float fireDelay;
};

#endif // EVILFLOWER_H

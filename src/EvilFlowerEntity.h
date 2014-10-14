#ifndef EVILFLOWER_H
#define EVILFLOWER_H

#include "EnemyEntity.h"
#include "PlayerEntity.h"

class EvilFlowerEntity : public EnemyEntity
{
  public:
    EvilFlowerEntity(float x, float y);
    virtual void animate(float delay);
    virtual void calculateBB();
    virtual void render(sf::RenderTarget* app);
    void fire();

  private:
    float fireDelay;
};

#endif // EVILFLOWER_H

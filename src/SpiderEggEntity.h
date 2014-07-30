#ifndef SPIDEREGGENTITY_H
#define SPIDEREGGENTITY_H

#include "EnemyEntity.h"

class SpiderEggEntity : public EnemyEntity
{
  public:
    SpiderEggEntity(float x, float y);
    virtual void animate(float delay);
    virtual void calculateBB();
    virtual void render(sf::RenderTarget* app);
    virtual void readCollidingEntity(CollidingSpriteEntity* entity);
    virtual void dyingFromAge();
  protected:
    virtual bool canCollide();

	  virtual void drop() {};
  private:
    bool jumping;
    float hVelocity;
};

#endif // FALLINGROCKENTITY_H

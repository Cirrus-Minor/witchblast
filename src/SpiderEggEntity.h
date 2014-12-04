#ifndef SPIDEREGGENTITY_H
#define SPIDEREGGENTITY_H

#include "EnemyEntity.h"

class SpiderEggEntity : public EnemyEntity
{
  public:
    SpiderEggEntity(float x, float y, bool invocated);
    virtual void animate(float delay);
    virtual void calculateBB();
    virtual void render(sf::RenderTarget* app);
    virtual void readCollidingEntity(CollidingSpriteEntity* entity);
    virtual void dyingFromAge();
    virtual bool canCollide();

  protected:
	  virtual void drop() {};

  private:
    bool jumping;
    bool invocated;
    float hVelocity;
};

#endif // FALLINGROCKENTITY_H

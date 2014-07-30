#ifndef FALLINGROCKENTITY_H
#define FALLINGROCKENTITY_H

#include "EnemyEntity.h"

class FallingRockEntity : public EnemyEntity
{
  public:
    FallingRockEntity(float x, float y, int rockType);
    virtual void animate(float delay);
    virtual void calculateBB();
    virtual void render(sf::RenderTarget* app);
  protected:
    virtual bool canCollide();

	  virtual void dying();
  private:
    int rockType;
    int h;
    bool jumping;
    float hVelocity;
};

#endif // FALLINGROCKENTITY_H

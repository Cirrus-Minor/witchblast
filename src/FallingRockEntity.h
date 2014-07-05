#ifndef FALLINGROCKENTITY_H
#define FALLINGROCKENTITY_H

#include "EnnemyEntity.h"

class FallingRockEntity : public EnnemyEntity
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
};

#endif // FALLINGROCKENTITY_H

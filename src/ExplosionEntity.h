#ifndef EXPLOSIONENTITY_H
#define EXPLOSIONENTITY_H

#include "sfml_game/SpriteEntity.h"

class ExplosionEntity : public SpriteEntity
{
  public:
    ExplosionEntity(float x, float y);

    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void dying();

  protected:
    void testCollisions();
};

#endif // EXPLOSIONENTITY_H

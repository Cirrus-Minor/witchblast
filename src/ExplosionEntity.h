#ifndef EXPLOSIONENTITY_H
#define EXPLOSIONENTITY_H

#include "sfml_game/SpriteEntity.h"

enum explosionTypeEnum { ExplosionTypeStandard, ExplosionTypeViolet };

class ExplosionEntity : public SpriteEntity
{
  public:
    ExplosionEntity(float x, float y, explosionTypeEnum explosionType, int damage);

    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void dying();

  protected:
    void testCollisions();

  private:
    int damage;
    explosionTypeEnum explosionType;
};

#endif // EXPLOSIONENTITY_H

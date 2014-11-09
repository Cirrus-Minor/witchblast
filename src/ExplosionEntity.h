#ifndef EXPLOSIONENTITY_H
#define EXPLOSIONENTITY_H

#include "sfml_game/SpriteEntity.h"
#include "BaseCreatureEntity.h"

enum explosionTypeEnum { ExplosionTypeStandard, ExplosionTypeViolet };

class ExplosionEntity : public SpriteEntity
{
  public:
    ExplosionEntity(float x, float y, explosionTypeEnum explosionType, int damage, enemyTypeEnum enemyType);

    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void dying();

    //enemyTypeEnum getEnemyType();

  protected:
    void testCollisions();

  private:
    int damage;
    explosionTypeEnum explosionType;
    enemyTypeEnum enemyType;
};

#endif // EXPLOSIONENTITY_H

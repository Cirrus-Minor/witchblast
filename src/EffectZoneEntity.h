#ifndef EFFECT_ZONE_ENTITY_H
#define EFFECT_ZONE_ENTITY_H

#include "sfml_game/SpriteEntity.h"
#include "BaseCreatureEntity.h"

enum EffectZoneTypeEnum { EffectZoneTypeIce, EffectZoneTypePoison };

class EffectZoneEntity : public SpriteEntity
{
  public:
    EffectZoneEntity(float x, float y, bool fromPlayer, float duration,
                     EffectZoneTypeEnum effectZoneType);

    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void dying();

  protected:
    void testCollisions();

  private:
    int damage;
    EffectZoneTypeEnum effectZoneType;
    enumShotType damageType;
    bool fromPlayer;
    float randomAngle;

    void collideIce(BaseCreatureEntity* entity);
    void collidePoison(BaseCreatureEntity* entity);
};

#endif // EFFECT_ZONE_ENTITY_H

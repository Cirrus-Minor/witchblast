#ifndef FAIRYENTITY_H
#define FAIRYENTITY_H

#include "sfml_game/SpriteEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/GameMap.h"

class FairyEntity : public SpriteEntity
{
  public:
    FairyEntity(float x, float y, enumFamiliar fairyType);
    virtual void animate(float delay);

    void fire(int dir, GameMap* map);

  protected:

  private:
    PlayerEntity* parentEntity;
    float fireDelay;
    int facingDirection;
    enumFamiliar fairyType;
    enumShotType shotType;
    float fairyFireDelay;
    int fairyDamages;
    unsigned int shotLevel;

    void computeFacingDirection();
    void checkCollisions();
};
#endif // MAGNETENTITY_H

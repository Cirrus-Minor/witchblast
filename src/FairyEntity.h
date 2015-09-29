#ifndef FAIRYENTITY_H
#define FAIRYENTITY_H

#include "sfml_game/SpriteEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/GameMap.h"

class FairyEntity : public SpriteEntity
{
  public:
    FairyEntity(float x, float y, enumFamiliar fairyType, bool isPlayerControlled = false);
    virtual void animate(float delay);

    void fire(int dir, bool bySelf = false);

  protected:

  private:
    PlayerEntity* parentEntity;
    bool isPlayer;
    float fireDelay;
    float teleportDelay;
    int facingDirection;
    enumFamiliar fairyType;
    enumShotType shotType;
    float fairyFireDelay;
    int fairyDamages;
    unsigned int shotLevel;

    void computeFacingDirection();
    void checkCollisions();

    void tryToFire();
};
#endif // MAGNETENTITY_H

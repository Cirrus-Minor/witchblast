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

    bool isPlayerControlled();
    /*!
     *  \brief Moves the fairy in the given direction
     *
     *  Moves the fairy in the given direction.
     *
     *  \param direction : direction of the new move. Numeric pad, diagonals included : 4 = left, 8 = north, 7 = north-west...
     */
    void move(int direction);

    void fire(int dir, bool bySelf = false);

    void gainNewPower();

    enum PowerUp
    {
      PowUpDamage,
      PowUpDamage2,
      PowUpFireRate,
      PowUpFireRate2,
      PowUpSpeed,
      PowUpDouble,
      PowUpCritical,

      PowUpTypeFire,
      PowUpTypeIce,
      PowUpTypePoison,
      PowUpTypeStone,

      LAST_POWER_UP
    };

  protected:

  private:
    PlayerEntity* parentEntity;
    bool isPlayer;
    bool isFiring;
    float fireDelay;
    float teleportDelay;
    int facingDirection;
    enumFamiliar fairyType;
    enumShotType shotType;
    float fairyFireDelay;
    int fairyDamages;
    unsigned int shotLevel;
    float creatureSpeed;

    void computeFacingDirection();
    void checkCollisions();

    void tryToFire();

    // Multiplayer
    bool power[LAST_POWER_UP];

    void compute();
};
#endif // FAIRYENTITY_H

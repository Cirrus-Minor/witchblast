#ifndef BOLTENTITY_H
#define BOLTENTITY_H

#include "sfml_game/CollidingSpriteEntity.h"
#include "Constants.h"

/*! \class BoltEntity
 * \brief bolt thrown by the player
 *
 *  BoltEntity are the missile weapons thrown by the player.
 *  The can collide with an enemy (to hurt him) or with the walls.
 */
class BoltEntity : public CollidingSpriteEntity
{
  public:
    BoltEntity(sf::Texture* image, float x, float y, float boltLifeTime, enumShotType boltType);
    virtual void animate(float delay);
    void collide();
    void generateParticule(Vector2D vel);

    int getDamages();
    void setDamages(int damages);
    enumShotType getBoltType();

  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    int damages;
    float renderScale;
    enumShotType boltType;
  private:
};

#endif // BOLTENTITY_H

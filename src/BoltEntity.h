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
    BoltEntity(float x, float y, float boltLifeTime, enumShotType boltType, int level);
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    void collide();
    void generateParticule(Vector2D vel);

    int getDamages();
    unsigned int getLevel();
    void setDamages(int damages);
    enumShotType getBoltType();
    bool isFlying();
    void setFlying(bool flying);

  protected:
    virtual void calculateBB();
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();
    virtual bool collideWithMap(int direction);

    virtual void onDying();
    virtual void stuck();

    int damages;
    float renderScale;
    enumShotType boltType;
  private:
    bool testWallsCollision;
    bool flying;
    unsigned int level;
};

#endif // BOLTENTITY_H

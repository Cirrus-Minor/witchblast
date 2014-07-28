#ifndef GIANTSPIDERENTITY_H
#define GIANTSPIDERENTITY_H

#include "EnnemyEntity.h"

class GiantSpiderEntity : public EnnemyEntity
{
  public:
    GiantSpiderEntity(float x, float y);
    virtual void animate(float delay);
    virtual void calculateBB();
    virtual void render(sf::RenderTarget* app);
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnnemy(GameEntity* collidingEntity);
    virtual bool canCollide();
    virtual bool hurt(int damages, enumShotType hurtingType, int level);
    virtual void drop();

    int getHealthLevel();
  private:
    int state;
    float timer;
    float fireDelay;

    int hurtLevel;

    void calculateRotation();
    void fire(int type);

    std::string creatureName;
};

#endif // GIANTSPIDERENTITY_H
